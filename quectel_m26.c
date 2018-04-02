/**
* @file quectel_26.c
* @brief GPRS control routine
*
* @section License
* ^^(^____^)^^
*
**/
#include "quectel_m26.h"

//==============================================
//Global variable
//==============================================
gprs_op_status_t gprsOpState = GPRS_POWER_OFF;
gprs_msg_buf_t gprsRxMsg;
gprs_msg_buf_t gprsTxMsg;
bool is_CR_received = FALSE;
bool is_LF_received = FALSE;

gprs_validate_cmd_t gprsValidateCmd = {
  .retry = 0,
  .expected_cmd = UNEXPECTED_RESPOND,
  .received_cmd = UNKNOWN_RESPOND,
  .valid_expected = false,
  .data_conection_terminated = false
};

//==============================================
//Definition
//==============================================
#define TCP_DATA_MODE 1 //1- Transparent Mode; 0 -Non-Transparent Mode
#define TCP_ADDRESS_MODE 0 // 1 - Domain; 0 - IP address

#define GPRS_CHANGE_STATE(state) gprsOpState = state
#define GPRS_INIT_MSG "AT\r\n"
#define GPRS_QUERY_SIMCARD_MSG "AT+CPIN?\r"
#define GPRS_QUERY_SIGNALSTRENGTH_MSG "AT+CSQ\r"
#define GPRS_QUERY_GSMREGISTRATION_MSG "AT+CREG?\r"
#define GPRS_QUERY_GPRSREGISTRATION_MSG "AT+CGREG?\r"
#define GPRS_SET_APN_MSG "AT+QICSGP=1,\"%s\"\r"
#define GPRS_START_TCPIPSTACK_MSG "AT+QIREGAPP\r"
#define GPRS_CHECK_CURRENTMODE_MSG "AT+QICSGP?\r" // 1: GPRS connecting mode 0: CSD connecting mode
#define GPRS_ACTIVATE_GPRSCONTEXT_MSG "AT+QIACT\r"
#define GPRS_SET_DSTADDRESSMODE_MSG "AT+QIDNSIP=%d\r"
#define GPRS_SET_TCPIPSTACKMODE_MSG "AT+QIMODE=%d\r"
#define GPRS_CONNECT_TCPADDRESS_MSG "AT+QIOPEN=\"TCP\",\"%s\",\"%s\"\r"
#define GPRS_CONECT_TCPDOMAIN_MSG "AT+QIOPEN=\"TCP\",\"%s\",\"%d\"\r"
#define GPRS_CONNECT_UDPADDRESS_MSG "AT+QIOPEN=\"UDP\",\"%s\",\"%d\"\r"
#define GPRS_DEACTIVE_GPRSCONTEXT_MSG "AT+QIDEACT\r"
#define GPRS_TURNOFF_ECHO "ATE1\r"

//Test data
//#define APP_IPV4_SERVER_ADDR "171.224.95.239"
//#define APP_IPV4_SERVER_PORT 8100
// chaunm test
#define APP_IPV4_SERVER_ADDR "117.0.129.178"
#define APP_IPV4_SERVER_PORT 8324

//==============================================
//Function Declaration
//==============================================
void _resetValidateCmdVariable (void)
{
  gprsValidateCmd.received_cmd = UNKNOWN_RESPOND;
  gprsValidateCmd.expected_cmd = UNEXPECTED_RESPOND;
  gprsValidateCmd.valid_expected = false;
  gprsValidateCmd.retry = 0;
  gprsValidateCmd.data_conection_terminated = false;
}

void _gprs_initUART (void)
{
  uart_config_t config;
  UART_GetDefaultConfig(&config);
  config.baudRate_Bps = GPRS_UART_BAUDRATE;
  config.enableTx = true;
  config.enableRx = true;
  UART_Init(GPRS_UART, &config, CLOCK_GetFreq(GPRS_UART_CLKSRC));
  /* Enable RX interrupt. */
  UART_EnableInterrupts(GPRS_UART, kUART_RxDataRegFullInterruptEnable | kUART_RxOverrunInterruptEnable);
  EnableIRQ(GPRS_UART_IRQn);
}

void _gprs_clear_rx_buf(void)
{
  gprsRxMsg.ptr = 0;
  gprsRxMsg.len = 0;
}

void _gprs_process_rx_message(uint8_t *rx_msg, uint16_t rx_len)
{
  if (!memcmp(rx_msg, "OK", strlen("OK"))){
    gprsValidateCmd.received_cmd = OK_RESPOND;
  } else if (!memcmp(rx_msg, "RDY", strlen("RDY"))){
    gprsValidateCmd.received_cmd = RDY_RESPOND;
  } else if (!memcmp(rx_msg, "Call Ready", strlen("Call Ready"))){
    gprsValidateCmd.received_cmd = CALL_READY_RESPOND;
  } else if (!memcmp(rx_msg, "CONNECT", strlen("CONNECT"))){
    gprsValidateCmd.received_cmd = CONNECT_RESPOND;
    //} else if (!memcmp(rx_msg, "+CPIN: READY", strlen("+CPIN: READY"))){
    //gprsValidateCmd.receive_cmd = SIM_READY_RESPOND;
    //khanhtx3: refs #2643 add rx message in GPRS manufacturing mode
  } else if (!memcmp(rx_msg, "+CPIN: NOT INSERTED", strlen("+CPIN: NOT INSERTED"))){
    gprsValidateCmd.received_cmd = SIM_NOT_INSERTED_RESPOND;
  } else if (!memcmp(rx_msg, "+CME ERROR: ", strlen("+CME ERROR: "))){
    uint16_t error_code;
    error_code = rx_msg[strlen("+CME ERROR: ")];
    error_code -= '0';
    if( rx_msg[strlen("+CME ERROR: ")+1] >= '0' )
    {
      error_code *= 10;
      error_code += rx_msg[strlen("+CME ERROR: ")+1] - '0';
    }
    if( (error_code >= 10) && (error_code <= 15) )	//get all SIM error
      gprsValidateCmd.received_cmd = SIM_NOT_INSERTED_RESPOND;
    else
      gprsValidateCmd.received_cmd = ERROR_RESPOND;
  } else if (!memcmp(rx_msg, "ERROR", strlen("ERROR"))){
    gprsValidateCmd.received_cmd = CMD_ERROR_RESPOND;
  } else if (!memcmp(rx_msg, "DEACT OK", strlen("DEACT OK"))){
    gprsValidateCmd.received_cmd = DEACTIVE_OK_RESPOND;
    //khanhtx3: refs #2643 add rx message in GPRS manufacturing mode
    //#ifdef GPRS_TEST_MODE
  } else if (!memcmp(rx_msg, "+CSQ: ", strlen("+CSQ: "))){
    //gprsValidateCmd.receive_cmd = DEACTIVE_OK_RESPOND;
    gprsValidateCmd.signal_quality = rx_msg[6] - '0';	//+CSQ: 31,0
    if(rx_msg[7] != ',')
    {
      gprsValidateCmd.signal_quality *= 10;
      gprsValidateCmd.signal_quality += rx_msg[7]-'0';
    }
  } else if (!memcmp(rx_msg, "+CREG: 0,", strlen("+CREG: 0,"))){
    //gprsValidateCmd.receive_cmd = DEACTIVE_OK_RESPOND;
    gprsValidateCmd.gsm_code = rx_msg[strlen("+CREG: 0,")] - '0';	//+CREG: 0,1
  } else if (!memcmp(rx_msg, "+CGREG: 0,", strlen("+CGREG: 0,"))){
    //gprsValidateCmd.receive_cmd = DEACTIVE_OK_RESPOND;
    gprsValidateCmd.gprs_code = rx_msg[strlen("+CGREG: 0,")] - '0';	//+CREG: 0,1
  } else if (!memcmp(rx_msg, "UNDER_VOLTAGE", strlen("UNDER_VOLTAGE"))){
    //gprsValidateCmd.receive_cmd = CMD_ERROR_RESPOND;	//keep old response
  } else {
    gprsValidateCmd.received_cmd = UNKNOWN_RESPOND;
  }
}

void _gprs_power_turning_on(void)
{  
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  GPRS_PWR_ON();
  vTaskDelay(3000 / portTICK_PERIOD_MS);
  GPRS_EN_OFF();
  vTaskDelay(3000/ portTICK_PERIOD_MS);
  GPRS_EN_ON();
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  
  GPRS_CHANGE_STATE(GPRS_CHECK_POWER_STATUS);
}

void _gprs_check_power_status(TimeOut_t *timeOut, TickType_t *timeToWait)
{
  vTaskDelay(100 / portTICK_PERIOD_MS);
  UART_WriteBlocking(GPRS_UART, GPRS_INIT_MSG, strlen(GPRS_INIT_MSG));
  
  vTaskDelay(500 / portTICK_PERIOD_MS);
  UART_WriteBlocking(GPRS_UART, GPRS_TURNOFF_ECHO, strlen(GPRS_TURNOFF_ECHO));
  
  GPRS_CHANGE_STATE(GPRS_CHECK_POWER_STATUS_RESPOND);
  vTaskSetTimeOutState(timeOut);
  *timeToWait = 1000;
}

void _gprs_check_power_status_respond (TimeOut_t *timeOut, TickType_t *timeToWait)
{
  if (gprsValidateCmd.received_cmd == OK_RESPOND)
  {
    GPRS_CHANGE_STATE(QUERY_SIMCARD);
    _resetValidateCmdVariable(); // reset retry for the new check
  }
  
  if( xTaskCheckForTimeOut(timeOut, timeToWait ) == pdFALSE )
  {
    return;
  } else
  {
    if (gprsValidateCmd.retry++ > 2) 
    {
      gprs_turnoff();
      _resetValidateCmdVariable(); // reset retry for the new check
    } else
    {
      GPRS_CHANGE_STATE(GPRS_CHECK_POWER_STATUS);
     //_resetValidateCmdVariable(); // reset retry for the new check
    }
  }
}

void _gprs_query_simcard (TimeOut_t *timeOut, TickType_t *timeToWait)
{
  vTaskDelay(100 / portTICK_PERIOD_MS);
  UART_WriteBlocking(GPRS_UART, GPRS_QUERY_SIMCARD_MSG, strlen(GPRS_QUERY_SIMCARD_MSG));
  GPRS_CHANGE_STATE(QUERY_SIMCARD_RESPOND);
  vTaskSetTimeOutState(timeOut);
  *timeToWait = 1000;
}

void _gprs_query_simcard_respond (TimeOut_t *timeOut, TickType_t *timeToWait)
{
  if ((gprsValidateCmd.received_cmd == OK_RESPOND) || \
    (gprsValidateCmd.received_cmd == CALL_READY_RESPOND))
  {
    GPRS_CHANGE_STATE(QUERY_GSM_NETWORK);
    _resetValidateCmdVariable(); // reset retry for the new check
  }
  
  if( xTaskCheckForTimeOut(timeOut, timeToWait ) == pdFALSE )
  {
    return;
  } else
  {
    if (gprsValidateCmd.retry++ > 2) 
    {
      gprs_turnoff();
      _resetValidateCmdVariable(); // reset retry for the new check
    } else
    {
      GPRS_CHANGE_STATE(QUERY_SIMCARD);
      //_resetValidateCmdVariable(); // reset retry for the new check
    }
  }
}

void _gprs_query_gsmnetwork (TimeOut_t *timeOut, TickType_t *timeToWait)
{
  vTaskDelay(100 / portTICK_PERIOD_MS);
  UART_WriteBlocking(GPRS_UART, GPRS_QUERY_GSMREGISTRATION_MSG, strlen(GPRS_QUERY_GSMREGISTRATION_MSG));
  GPRS_CHANGE_STATE(QUERY_GSM_NETWORK_RESPOND);
  vTaskSetTimeOutState(timeOut);
  *timeToWait = 1000;
}

void _gprs_query_gsmnetwork_respond (TimeOut_t *timeOut, TickType_t *timeToWait)
{
  if (gprsValidateCmd.received_cmd == OK_RESPOND) 
  {
    GPRS_CHANGE_STATE(QUERY_GPRS_NETWORK);
    _resetValidateCmdVariable(); // reset retry for the new check
  }
  
  if( xTaskCheckForTimeOut(timeOut, timeToWait ) == pdFALSE )
  {
    return;
  } else
  {
    if (gprsValidateCmd.retry++ > 2) 
    {
      gprs_turnoff();
      _resetValidateCmdVariable(); // reset retry for the new check
    } else
    {
      GPRS_CHANGE_STATE(QUERY_GSM_NETWORK);
      //_resetValidateCmdVariable(); // reset retry for the new check
    }
  }
}

void _gprs_query_gprsnetwork (TimeOut_t *timeOut, TickType_t *timeToWait)
{
  vTaskDelay(100 / portTICK_PERIOD_MS);
  UART_WriteBlocking(GPRS_UART, GPRS_QUERY_GPRSREGISTRATION_MSG, strlen(GPRS_QUERY_GPRSREGISTRATION_MSG));
  GPRS_CHANGE_STATE(QUERY_GPRS_NETWORK_RESPOND);
  vTaskSetTimeOutState(timeOut);
  *timeToWait = 1000;
}

void _gprs_query_gprsnetwork_respond (TimeOut_t *timeOut, TickType_t *timeToWait)
{
  if (gprsValidateCmd.received_cmd == OK_RESPOND) 
  {
    GPRS_CHANGE_STATE(SET_TCPIP_STACK_MODE);
    _resetValidateCmdVariable(); // reset retry for the new check
  }
  
  if( xTaskCheckForTimeOut(timeOut, timeToWait ) == pdFALSE )
  {
    return;
  } else
  {
    if (gprsValidateCmd.retry++ > 2) 
    {
      gprs_turnoff();
      _resetValidateCmdVariable(); // reset retry for the new check
    } else
    {
      GPRS_CHANGE_STATE(QUERY_GPRS_NETWORK);
      //_resetValidateCmdVariable(); // reset retry for the new check
    }
  }
}

void _gprs_set_tcpip_stack_mode (TimeOut_t *timeOut, TickType_t *timeToWait)
{
  char str[32];
  
  vTaskDelay(100 / portTICK_PERIOD_MS);
  sprintf(str, GPRS_SET_TCPIPSTACKMODE_MSG, TCP_DATA_MODE);
  UART_WriteBlocking(GPRS_UART, (uint8_t *) str, strlen(str));
  GPRS_CHANGE_STATE(SET_TCPIP_STACK_MODE_RESPOND);
  vTaskSetTimeOutState(timeOut);
  *timeToWait = 10000;
}

void _gprs_set_tcpip_stack_mode_respond (TimeOut_t *timeOut, TickType_t *timeToWait)
{
  if (gprsValidateCmd.received_cmd == OK_RESPOND) 
  {
    GPRS_CHANGE_STATE(SET_GPRS_APN);
    _resetValidateCmdVariable(); // reset retry for the new check
  }
  
  if( xTaskCheckForTimeOut(timeOut, timeToWait ) == pdFALSE )
  {
    return;
  } else
  {
    if (gprsValidateCmd.retry++ > 2) 
    {
      gprs_turnoff();
      _resetValidateCmdVariable(); // reset retry for the new check
    } else
    {
      GPRS_CHANGE_STATE(SET_TCPIP_STACK_MODE);
      //_resetValidateCmdVariable(); // reset retry for the new check
    }
  }
}

void _gprs_set_apn (TimeOut_t *timeOut, TickType_t *timeToWait)
{
  char str[32];
  
  vTaskDelay(100 / portTICK_PERIOD_MS);
  sprintf(str, GPRS_SET_APN_MSG, "");
  UART_WriteBlocking(GPRS_UART, (uint8_t *) str, strlen(str));
  GPRS_CHANGE_STATE(SET_GPRS_APN_RESPOND);
  vTaskSetTimeOutState(timeOut);
  *timeToWait = 10000;
}

void _gprs_set_apn_respond (TimeOut_t *timeOut, TickType_t *timeToWait)
{
  if (gprsValidateCmd.received_cmd == OK_RESPOND) 
  {
    GPRS_CHANGE_STATE(START_TCPIP_TASK);
    _resetValidateCmdVariable(); // reset retry for the new check
  }
  
  if( xTaskCheckForTimeOut(timeOut, timeToWait ) == pdFALSE )
  {
    return;
  } else
  {
    if (gprsValidateCmd.retry++ > 2) 
    {
      gprs_turnoff();
      _resetValidateCmdVariable(); // reset retry for the new check
    } else
    {
      GPRS_CHANGE_STATE(SET_GPRS_APN);
    }
  }
}

void _gprs_start_tcpip_stack (TimeOut_t *timeOut, TickType_t *timeToWait)
{
  vTaskDelay(100 / portTICK_PERIOD_MS);
  UART_WriteBlocking(GPRS_UART, GPRS_START_TCPIPSTACK_MSG, strlen(GPRS_START_TCPIPSTACK_MSG));
  GPRS_CHANGE_STATE(START_TCPIP_TASK_RESPOND);
  vTaskSetTimeOutState(timeOut);
  *timeToWait = 10000;
}

void _gprs_start_tcpip_stack_respond (TimeOut_t *timeOut, TickType_t *timeToWait)
{
  if (gprsValidateCmd.received_cmd == OK_RESPOND) 
  {
    GPRS_CHANGE_STATE(ACTIVE_GPRS_CONTEXT);
    _resetValidateCmdVariable(); // reset retry for the new check
  }
  
  if( xTaskCheckForTimeOut(timeOut, timeToWait ) == pdFALSE )
  {
    return;
  } else
  {
    if (gprsValidateCmd.retry++ > 2) 
    {
      gprs_turnoff();
      _resetValidateCmdVariable(); // reset retry for the new check
    } else
    {
      GPRS_CHANGE_STATE(START_TCPIP_TASK);
    }
  }
}

void _gprs_active_gprs_context (TimeOut_t *timeOut, TickType_t *timeToWait)
{
  vTaskDelay(100 / portTICK_PERIOD_MS);
  UART_WriteBlocking(GPRS_UART, GPRS_ACTIVATE_GPRSCONTEXT_MSG, strlen(GPRS_ACTIVATE_GPRSCONTEXT_MSG));
  GPRS_CHANGE_STATE(ACTIVE_GPRS_CONTEXT_RESPOND);
  vTaskSetTimeOutState(timeOut);
  *timeToWait = 10000;
}

void _gprs_active_gprs_context_respond (TimeOut_t *timeOut, TickType_t *timeToWait)
{
  if (gprsValidateCmd.received_cmd == OK_RESPOND) 
  {
    GPRS_CHANGE_STATE(SET_DEST_ADDRESS_TYPE);
    _resetValidateCmdVariable(); // reset retry for the new check
  }
  
  if( xTaskCheckForTimeOut(timeOut, timeToWait ) == pdFALSE )
  {
    return;
  } else
  {
    if (gprsValidateCmd.retry++ > 2) 
    {
      gprs_turnoff();
      _resetValidateCmdVariable(); // reset retry for the new check
    } else
    {
      GPRS_CHANGE_STATE(ACTIVE_GPRS_CONTEXT);
      //_resetValidateCmdVariable(); // reset retry for the new check
    }
  }
}

void _gprs_set_dstAddr_type (TimeOut_t *timeOut, TickType_t *timeToWait)
{
  char str[32];
  
  vTaskDelay(100 / portTICK_PERIOD_MS);
  sprintf(str, GPRS_SET_DSTADDRESSMODE_MSG, TCP_ADDRESS_MODE);
  UART_WriteBlocking(GPRS_UART, (uint8_t *) str, strlen(str));
  GPRS_CHANGE_STATE(SET_DEST_ADDRESS_TYPE_RESPOND);
  vTaskSetTimeOutState(timeOut);
  *timeToWait = 5000;
}

void _gprs_set_dstAddr_type_respond (TimeOut_t *timeOut, TickType_t *timeToWait)
{
  if (gprsValidateCmd.received_cmd == OK_RESPOND) 
  {
    GPRS_CHANGE_STATE(CONNECT_UDP_SERVER);
    _resetValidateCmdVariable(); // reset retry for the new check
  }
  
  if( xTaskCheckForTimeOut(timeOut, timeToWait ) == pdFALSE )
  {
    return;
  } else
  {
    if (gprsValidateCmd.retry++ > 2) 
    {
      gprs_turnoff();
      _resetValidateCmdVariable(); // reset retry for the new check
    } else
    {
      GPRS_CHANGE_STATE(SET_DEST_ADDRESS_TYPE);
      //_resetValidateCmdVariable(); // reset retry for the new check
    }
  }
}

void _gprs_connect_udpAddr (TimeOut_t *timeOut, TickType_t *timeToWait)
{
  char str[128];
  
  vTaskDelay(100 / portTICK_PERIOD_MS);
  sprintf(str, GPRS_CONNECT_UDPADDRESS_MSG, APP_IPV4_SERVER_ADDR, APP_IPV4_SERVER_PORT);
   UART_WriteBlocking(GPRS_UART, (uint8_t *) str, strlen(str));
  GPRS_CHANGE_STATE(CONNECT_UDP_SERVER_RESPOND);
  vTaskSetTimeOutState(timeOut);
  *timeToWait = 5000;
}

void _gprs_connect_udpAddr_respond (TimeOut_t *timeOut, TickType_t *timeToWait)
{
  if (gprsValidateCmd.received_cmd == CONNECT_RESPOND) 
  {
    GPRS_CHANGE_STATE(TRANSPARENT_DATA_MODE);
    _gprs_clear_rx_buf();
    _resetValidateCmdVariable(); // reset retry for the new check
  }
  
  if( xTaskCheckForTimeOut(timeOut, timeToWait ) == pdFALSE )
  {
    return;
  } else
  {
    if (gprsValidateCmd.retry++ > 2) 
    {
      gprs_turnoff();
      _resetValidateCmdVariable(); // reset retry for the new check
    } else
    {
      GPRS_CHANGE_STATE(CONNECT_UDP_SERVER);
      //_resetValidateCmdVariable(); // reset retry for the new check
    }
  }
}

void _gprs_deactive_gprs_context(TimeOut_t *timeOut, TickType_t *timeToWait)
{
  vTaskDelay(600 / portTICK_PERIOD_MS);
  UART_WriteBlocking(GPRS_UART, "+++", 3);	
  vTaskDelay(600 / portTICK_PERIOD_MS);
  UART_WriteBlocking(GPRS_UART, "AT+QICLOSE\r", strlen("AT+QICLOSE\r"));	
  vTaskDelay(100 / portTICK_PERIOD_MS);
  UART_WriteBlocking(GPRS_UART, GPRS_DEACTIVE_GPRSCONTEXT_MSG, strlen(GPRS_DEACTIVE_GPRSCONTEXT_MSG));
  GPRS_CHANGE_STATE(DEACTIVE_GPRS_CONTEXT_RESPOND);
  vTaskSetTimeOutState(timeOut);
  *timeToWait = 10000;	
}

void _gprs_deactive_gprs_context_respond (TimeOut_t *timeOut, TickType_t *timeToWait)
{
  if (gprsValidateCmd.received_cmd == DEACTIVE_OK_RESPOND) 
  {
    GPRS_CHANGE_STATE(GPRS_DISCONNECTED);
    _gprs_clear_rx_buf();
    _resetValidateCmdVariable(); // reset retry for the new check
  }
  
  if( xTaskCheckForTimeOut(timeOut, timeToWait ) == pdFALSE )
  {
    return;
  } else
  {
    if (gprsValidateCmd.retry++ > 2) 
    {
      gprs_turnoff();
      _resetValidateCmdVariable(); // reset retry for the new check
    } else
    {
      GPRS_CHANGE_STATE(DEACTIVE_GPRS_CONTEXT);
      //s_resetValidateCmdVariable(); // reset retry for the new check
    }
  }
}

void _gprs_check_broken_conn(uint8_t ch)
{
  static uint8_t closed_pos = 0;
  static uint8_t deact_pos = 0;
  
  switch (closed_pos)
  {
  case 0:
    if (ch == 'C') closed_pos ++;
    else closed_pos = 0;
    break;
  case 1:
    if (ch == 'L') closed_pos ++;
    else closed_pos = 0;
    break;
  case 2:
    if (ch == 'O') closed_pos ++;
    else closed_pos = 0;
    break;
  case 3:
    if (ch == 'S') closed_pos ++;
    else closed_pos = 0;
    break;
  case 4:
    if (ch == 'E') closed_pos ++;
    else closed_pos = 0;
    break;
  case 5:
    if (ch == 'D') gprsValidateCmd.data_conection_terminated = true;
    else closed_pos = 0;
    break;
  }
  
  switch (deact_pos)
  {
  case 0:
    if (ch == '+') deact_pos ++;
    else deact_pos = 0;
    break;
  case 1:
    if (ch == 'P') deact_pos ++;
    else deact_pos = 0;
    break;
  case 2:
    if (ch == 'D') deact_pos ++;
    else deact_pos = 0;
    break;
  case 3:
    if (ch == 'P') deact_pos ++;
    else deact_pos = 0;
    break;
  case 4:
    if (ch == ' ') deact_pos ++;
    else deact_pos = 0;
    break;
  case 5:
    if (ch == 'D') deact_pos ++;
    else deact_pos = 0;
    break;
  case 6:
    if (ch == 'E') deact_pos ++;
    else deact_pos = 0;
    break;
  case 7:
    if (ch == 'A') deact_pos ++;
    else deact_pos = 0;
    break;
  case 8:
    if (ch == 'C') deact_pos ++;
    else deact_pos = 0;
    break;
  case 9:
    if (ch == 'T')  gprsValidateCmd.data_conection_terminated = true;
    else deact_pos = 0;
    break;
  }
}

void _gprs_rx_byte(int port, uint8_t ch)
{
  switch (gprsOpState)
  {
  case TRANSPARENT_DATA_MODE:
    _gprs_check_broken_conn(ch);
    
    /* reset receive pointer */
    gprsRxMsg.buf[gprsRxMsg.ptr++] = ch;
    if (gprsRxMsg.ptr == (GPRS_MSG_BUF_SIZE - 1)) {
      gprsRxMsg.ptr = 0;
    }
    gprsRxMsg.len++;
    //gprs_data_rdy = TRUE;
    //tick_data_rx = get_system_tick();
    break;
  default:
    if (ch == 0x0D) {
      is_CR_received = TRUE;
      break;
    } else if ((is_CR_received == TRUE) && (ch == 0x0A)) {
      if (gprsRxMsg.ptr != 0) {
        _gprs_process_rx_message(&gprsRxMsg.buf[0], gprsRxMsg.len);
      }
      _gprs_clear_rx_buf();
      is_CR_received = FALSE;
      break;
    } else if (is_CR_received == TRUE) {
      _gprs_clear_rx_buf();
      is_CR_received = FALSE;
    }
    
    gprsRxMsg.buf[gprsRxMsg.ptr++] = ch;
    gprsRxMsg.len++;
    
    break;
  }
}  
void GPRS_UART_IRQHandler (void)
{
  uint8_t ucChar;
  if ((kUART_RxDataRegFullFlag | kUART_RxOverrunFlag) & UART_GetStatusFlags(GPRS_UART))
  {
    ucChar = UART_ReadByte(GPRS_UART);
    PRINTF("%c", ucChar); // chaunm - print to debug
    _gprs_rx_byte(1,ucChar); 
  }
}

void gprs_init (void)
{
  _gprs_initUART();
  
  GPRS_EN_INIT(0);
  GPRS_PWR_INIT(0);
  
  GPRS_CHANGE_STATE(GPRS_POWER_OFF);
}

void gprs_turnon(void)
{
  //GPRS_PWR_ON();
  GPRS_CHANGE_STATE(GPRS_POWER_TURNING_ON);
}

void gprs_turnoff(void)
{
  GPRS_EN_OFF();
  vTaskDelay(800 / portTICK_PERIOD_MS);
  GPRS_EN_ON();
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  GPRS_PWR_OFF();
  _resetValidateCmdVariable();
  GPRS_CHANGE_STATE(GPRS_POWER_OFF);
}

void gprs_task (void *param)
{
  TimeOut_t gprs_timeout;
  TickType_t gprs_timeToWait;
  TRACE_ERROR("gprs task started\r\n");
  for (;;)  {
    switch (gprsOpState) {
    case GPRS_POWER_OFF:
      break;
    case GPRS_POWER_TURNING_ON:
      _gprs_power_turning_on();
      break;
    case GPRS_CHECK_POWER_STATUS:
      _gprs_check_power_status(&gprs_timeout, &gprs_timeToWait);
      break;
    case GPRS_CHECK_POWER_STATUS_RESPOND:
      _gprs_check_power_status_respond(&gprs_timeout, &gprs_timeToWait);
      break;
    case QUERY_SIMCARD:
      _gprs_query_simcard (&gprs_timeout, &gprs_timeToWait);
      break;
    case QUERY_SIMCARD_RESPOND:
      _gprs_query_simcard_respond(&gprs_timeout, &gprs_timeToWait);
      break;
    case QUERY_GSM_NETWORK:
      _gprs_query_gsmnetwork(&gprs_timeout, &gprs_timeToWait);
      break;
    case QUERY_GSM_NETWORK_RESPOND:
      _gprs_query_gsmnetwork_respond(&gprs_timeout, &gprs_timeToWait);
      break;
    case QUERY_GPRS_NETWORK:
      _gprs_query_gprsnetwork(&gprs_timeout, &gprs_timeToWait);
      break;
    case QUERY_GPRS_NETWORK_RESPOND:
      _gprs_query_gprsnetwork_respond(&gprs_timeout, &gprs_timeToWait);
      break;
    case SET_TCPIP_STACK_MODE:
      _gprs_set_tcpip_stack_mode(&gprs_timeout, &gprs_timeToWait);
      break;
    case SET_TCPIP_STACK_MODE_RESPOND:
      _gprs_set_tcpip_stack_mode_respond(&gprs_timeout, &gprs_timeToWait);
      break;
    case SET_GPRS_APN:
      _gprs_set_apn(&gprs_timeout, &gprs_timeToWait);
      break;
    case SET_GPRS_APN_RESPOND:
      _gprs_set_apn_respond(&gprs_timeout, &gprs_timeToWait);
      break;
    case START_TCPIP_TASK:
      _gprs_start_tcpip_stack(&gprs_timeout, &gprs_timeToWait);
      break;
    case START_TCPIP_TASK_RESPOND:
      _gprs_start_tcpip_stack_respond(&gprs_timeout, &gprs_timeToWait);
      break;
    case ACTIVE_GPRS_CONTEXT:
      _gprs_active_gprs_context(&gprs_timeout, &gprs_timeToWait);
      break;
    case ACTIVE_GPRS_CONTEXT_RESPOND:
      _gprs_active_gprs_context_respond(&gprs_timeout, &gprs_timeToWait);
      break;
    case SET_DEST_ADDRESS_TYPE:
      _gprs_set_dstAddr_type(&gprs_timeout, &gprs_timeToWait);
      break;
    case SET_DEST_ADDRESS_TYPE_RESPOND:
      _gprs_set_dstAddr_type_respond(&gprs_timeout, &gprs_timeToWait);
      break;
    case CONNECT_UDP_SERVER:
      _gprs_connect_udpAddr(&gprs_timeout, &gprs_timeToWait);
      break;
    case CONNECT_UDP_SERVER_RESPOND:
      _gprs_connect_udpAddr_respond(&gprs_timeout, &gprs_timeToWait);
      break;
    case TRANSPARENT_DATA_MODE:
      if (gprsValidateCmd.data_conection_terminated)
        GPRS_CHANGE_STATE(DEACTIVE_GPRS_CONTEXT);
      
      //UART_WriteBlocking(GPRS_UART, "Hello", 5);
      //vTaskDelay(10000 / portTICK_PERIOD_MS);
      break;
    case DEACTIVE_GPRS_CONTEXT:
      _gprs_deactive_gprs_context(&gprs_timeout, &gprs_timeToWait);
      break;
    case DEACTIVE_GPRS_CONTEXT_RESPOND:
      _gprs_deactive_gprs_context_respond(&gprs_timeout, &gprs_timeToWait);
      break;
    case GPRS_DISCONNECTED:
      break;
    default:
      break;
    }
  }
}

gprs_op_status_t gprsCheckStatus(void)
{
  return gprsOpState;
}

error_t gprsSendMsg(const void *data, size_t length)
{
  UART_WriteBlocking(GPRS_UART, data, length);
  return NO_ERROR;
}

void gprs_connect (void)
{
  if (gprsCheckStatus() == GPRS_DISCONNECTED)
    GPRS_CHANGE_STATE(CONNECT_UDP_SERVER);
}

void gprs_disconnect (void)
{
  if (gprsCheckStatus() == TRANSPARENT_DATA_MODE)
    GPRS_CHANGE_STATE(DEACTIVE_GPRS_CONTEXT);
}
