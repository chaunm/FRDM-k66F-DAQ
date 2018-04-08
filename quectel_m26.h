#ifndef __QUECTEL_M26_H
#define __QUECTEL_M26_H

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "board.h"
#include "fsl_gpio.h"
#include "fsl_uart.h"
#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "portmacro.h"
#include "task.h"
#include "debug.h"
#include "error.h"

// Definition
#define GPRS_UART                                         UART1
#define GPRS_UART_CLKSRC                          SYS_CLK
#define GPRS_UART_IRQn                              UART1_RX_TX_IRQn
#define GPRS_UART_IRQHandler                    UART1_RX_TX_IRQHandler

#define GPRS_UART_BAUDRATE			172827//115200//518400// => 115200bps

typedef enum gprs_op_status_e
{
	GPRS_CHECK_POWER_STATUS,
	GPRS_CHECK_POWER_STATUS_RESPOND,
	GPRS_POWER_OFF,
	GPRS_POWER_ON,
	GPRS_POWER_TURNING_ON, //wait for power stable
	TURNOFF_ECHO,
	QUERY_SIMCARD,
	QUERY_SIMCARD_RESPOND,
	QUERY_GSM_NETWORK,
	QUERY_GSM_NETWORK_RESPOND,
	QUERY_GPRS_NETWORK,
	QUERY_GPRS_NETWORK_RESPOND,
	SET_TCPIP_STACK_MODE,
	SET_TCPIP_STACK_MODE_RESPOND,
	SET_GPRS_APN,
	SET_GPRS_APN_RESPOND,
	START_TCPIP_TASK,
	START_TCPIP_TASK_RESPOND,
	ACTIVE_GPRS_CONTEXT,
	ACTIVE_GPRS_CONTEXT_RESPOND,
	SET_DEST_ADDRESS_TYPE,
	SET_DEST_ADDRESS_TYPE_RESPOND,
        GET_GPRS_LOCAL_IP, 
        GET_GPRS_LOCAL_IP_RESPOND,
        PARSE_GPRS_IP_ADDR,
	CONNECT_TCP_SERVER,
	CONNECT_TCP_SERVER_RESPOND,
	CONNECT_UDP_SERVER,
	CONNECT_UDP_SERVER_RESPOND,
	TRANSPARENT_DATA_MODE,
	GPRS_COMMAND_MODE,
	GPRS_DATA_MODE,
	DEACTIVE_GPRS_CONTEXT,
	DEACTIVE_GPRS_CONTEXT_RESPOND,
	GPRS_DISCONNECTED
}gprs_op_status_t;

#define GPRS_MSG_BUF_SIZE 512
typedef struct
{
	uint8_t buf[GPRS_MSG_BUF_SIZE];
	uint16_t ptr;
	uint16_t len;
	uint32_t inter_char_timeout;
} gprs_msg_buf_t;

typedef enum gprs_cmd_respond_e
{
	UNKNOWN_RESPOND = 0x0001,
	OK_RESPOND = 0x0002,
	ERROR_RESPOND = 0x0004,
	SIM_READY_RESPOND = 0x0008,
	SIM_NOT_INSERTED_RESPOND = 0x0010,
	CONNECT_RESPOND = 0x0020,
	RDY_RESPOND = 0x0040,
	CALL_READY_RESPOND = 0x0080,
	CMD_ERROR_RESPOND = 0x0100,
	AT_RESPOND = 0x0200,
	DEACTIVE_OK_RESPOND = 0x0400,
	QNTP_RESPOND = 0x0800,
	CCLK_RESPOND = 0x1000,
	UNEXPECTED_RESPOND = 0x8000
} gprs_cmd_respond_t;

typedef struct gprs_validate_cmd_t
{
	uint16_t status;
	uint16_t signal_quality;	//AT+CSQ
	uint16_t gsm_code;	// AT+CREG?
	uint16_t gprs_code;	// AT+CGREG?
	uint16_t retry;
	gprs_cmd_respond_t expected_cmd;
	gprs_cmd_respond_t received_cmd;
	bool valid_expected;
	bool data_conection_terminated;
} gprs_validate_cmd_t;

typedef struct gprs_ip_address_t {
  unsigned char ipAddress[18];
  uint32_t ipAddr;
  uint8_t len;
} gprs_ip_address_t;

// Function declaration
void gprs_init(void);
void gprs_turnon(void);
void gprs_turnoff(void);
void gprs_task (void *param);
gprs_op_status_t gprsCheckStatus(void);
void gprs_connect (void);
void gprs_disconnect (void);
error_t gprsSendMsg(const void *data, size_t length);
 	
#endif

