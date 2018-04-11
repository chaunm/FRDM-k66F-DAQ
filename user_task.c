#if (USERDEF_CLIENT_SNMP == ENABLED)
#include "snmp/snmp_agent.h"
#include "mibs/mib2_module.h"
#include "mibs/mib2_impl.h"
#include "oid.h"
#include "private_mib_module.h"
#include "private_mib_impl.h"
#endif

#include "user_task.h"
#include "net_config.h"
#include "board.h"
#include "lm2068.h"
#include "menu.h"
#include "access_control.h"
#include "os_port.h"
#include "rs485.h"
#include "snmpConnect_manager.h"
#include "test.h"

#if (USERDEF_ADC_TASK == ENABLED)
#include "fsl_adc16.h"
#include "adc_config.h"
#endif

#if (USERDEF_CLIENT_FTP == ENABLED)
#include "ftp.h"
#endif

/******************* DEFINITIONS *************************/

/******************* LOCAL VARIABLES *************************/
#if (USERDEF_USER_INTERFACE == ENABLED)
uint32_t sysCountTest = 0;
uint8_t relay_control;
uint8_t relay_number;
uint8_t trap_flag[20]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
#endif

#if (USERDEF_ADC_TASK == ENABLED)
uint32_t adcValue[10];
#endif

#if (USERDEF_SW_TIMER == ENABLED)
/* The software timer period. */
#define SW_TIMER_PERIOD_MS (1000 / portTICK_PERIOD_MS)
TimerHandle_t SwTimerHandle = NULL;
#endif

extern NetInterface *interface;
/**
* @brief LED blinking task
**/

void blinkTask(void *param)
{
  TRACE_ERROR("Blink Task started\r\n");
  //Endless loop
  while(1)
  {
    LED_STATUS_OFF();
    osDelayTask(500);
    LED_STATUS_ON();
    osDelayTask(500);
  }
}

#if (USERDEF_ADC_TASK == ENABLED)
/**
* @brief ADC task
**/
void adcTask(void *param)
{
  static adc16_config_t adc16ConfigStruct;
  static adc16_channel_config_t adc16ChannelConfigStruct;
  TRACE_ERROR("ADC Task started\r\n");
    //Endless loop
    while(1)
    {
      //      //Perform A/D conversion
      //      adcValue = adcGetValue(18) / 16;
      adc16ChannelConfigStruct.channelNumber = BOARD_ADC16_0_USER_CHANNEL1;
      ADC16_SetChannelConfig(BOARD_ADC16_0_BASE, BOARD_ADC16_0_CHANNEL_GROUP, &adc16ChannelConfigStruct);
      while (0U == (kADC16_ChannelConversionDoneFlag &
                    ADC16_GetChannelStatusFlags(BOARD_ADC16_0_BASE, BOARD_ADC16_0_CHANNEL_GROUP)))
      {
      }
      adcValue[0] = ADC16_GetChannelConversionValue(BOARD_ADC16_0_BASE, BOARD_ADC16_0_CHANNEL_GROUP);
      
      adc16ChannelConfigStruct.channelNumber = BOARD_ADC16_0_USER_CHANNEL2;
      ADC16_SetChannelConfig(BOARD_ADC16_0_BASE, BOARD_ADC16_0_CHANNEL_GROUP, &adc16ChannelConfigStruct);
      while (0U == (kADC16_ChannelConversionDoneFlag &
                    ADC16_GetChannelStatusFlags(BOARD_ADC16_0_BASE, BOARD_ADC16_0_CHANNEL_GROUP)))
      {
      }
      adcValue[1] = ADC16_GetChannelConversionValue(BOARD_ADC16_0_BASE, BOARD_ADC16_0_CHANNEL_GROUP);
      
      adc16ChannelConfigStruct.channelNumber = BOARD_ADC16_0_USER_CHANNEL3;
      ADC16_SetChannelConfig(BOARD_ADC16_0_BASE, BOARD_ADC16_0_CHANNEL_GROUP, &adc16ChannelConfigStruct);
      while (0U == (kADC16_ChannelConversionDoneFlag &
                    ADC16_GetChannelStatusFlags(BOARD_ADC16_0_BASE, BOARD_ADC16_0_CHANNEL_GROUP)))
      {
      }
      adcValue[2] = ADC16_GetChannelConversionValue(BOARD_ADC16_0_BASE, BOARD_ADC16_0_CHANNEL_GROUP);
      
      adc16ChannelConfigStruct.channelNumber = BOARD_ADC16_1_USER_CHANNEL1;
      ADC16_SetChannelConfig(BOARD_ADC16_1_BASE, BOARD_ADC16_0_CHANNEL_GROUP, &adc16ChannelConfigStruct);
      while (0U == (kADC16_ChannelConversionDoneFlag &
                    ADC16_GetChannelStatusFlags(BOARD_ADC16_1_BASE, BOARD_ADC16_0_CHANNEL_GROUP)))
      {
      }
      adcValue[3] = ADC16_GetChannelConversionValue(BOARD_ADC16_1_BASE, BOARD_ADC16_1_CHANNEL_GROUP);
      
      adc16ChannelConfigStruct.channelNumber = BOARD_ADC16_0_USER_CHANNEL2;
      ADC16_SetChannelConfig(BOARD_ADC16_1_BASE, BOARD_ADC16_1_CHANNEL_GROUP, &adc16ChannelConfigStruct);
      while (0U == (kADC16_ChannelConversionDoneFlag &
                    ADC16_GetChannelStatusFlags(BOARD_ADC16_1_BASE, BOARD_ADC16_1_CHANNEL_GROUP)))
      {
      }
      adcValue[4] = ADC16_GetChannelConversionValue(BOARD_ADC16_1_BASE, BOARD_ADC16_1_CHANNEL_GROUP);
      
      PRINTF("ADC Value1: %d\r\n",adcValue[0]);
      PRINTF("ADC Value2: %d\r\n",adcValue[1]);
      PRINTF("ADC Value3: %d\r\n",adcValue[2]);
      PRINTF("ADC Value4: %d\r\n",adcValue[3]);
      PRINTF("ADC Value5: %d\r\n",adcValue[4]);
      //Loop delay
      osDelayTask(1000);
    }
}
#endif

#if (USERDEF_CLIENT_FTP == ENABLED)
void ftpTranferTask(void *param)
{
  TRACE_ERROR("FTP task stared\r\n");
  //	osDelayTask(5000);
  //Endless loop
  while(1)
  {
    //SW2 button pressed?
    if(!BUTTON_ENTER_READ())
    {
      //FTP client test routine
      ftpClientTest();
      
      //Wait for the SW2 button to be released
      while(!BUTTON_ENTER_READ());
    }
    
    //      Loop delay
    osDelayTask(100);
  }
}
#endif

#if (USERDEF_USER_INTERFACE == ENABLED)

/*!
* @brief Task responsible for printing of "Hello world." message.
*/
static void hello_task(void *pvParameters) {
  TRACE_ERROR("User interface task stared\r\n");
  glcd_init();
  //  vTaskDelay(100);
  //  Init_I2CE();
  //  vTaskDelay(100);
  //  Init_All_Variable();
  //  vTaskDelay(100);
  /*//SIM
  GPIO_SetPinsOutput(GPIOA,1<<27u);
  GPIO_ClearPinsOutput(GPIOA,1<<26u);
  vTaskDelay(3000);
  GPIO_SetPinsOutput(GPIOA,1<<26u);
  vTaskDelay(3000);
  GPIO_ClearPinsOutput(GPIOA,1<<26u);*/
  //GLCD
  glcd_writeString("Hello World !",1,3);
  sMenu_Control.init = 1;
  for (;;) {
    vTaskDelay(10);
    Key_Scane(); 
    Menu_Scane();
    Active_Alarm_Scane();
    ACS_AccessCheck();
    UpdateInfo ();
  }
}

/* process RS-485 state machine */
static void rs485_task(void *pvParameters) {
  int8_t	reVal = 0;
  TRACE_ERROR("RS485 task stared\r\n");
  vTaskDelay(3000);
  for (;;) {
    vTaskDelay(200);
    switch(sMenu_Variable.changedVal)
    {
    case _GEN_MAX_RUNTIME:
      Write_Single_Reg(0x01,1,sMenu_Variable.u16GENMaxRuntime);
      Modbus.runningStep = _WAIT_WRITE_SINGLE_REG;
      break;
    case _GEN_UNDER_VOLT:
      Write_Single_Reg(0x01,6,sMenu_Variable.u16GENUnderVolt);
      Modbus.runningStep = _WAIT_WRITE_SINGLE_REG;
      break;
    case _GEN_ERROR_RESET_EN:
      Write_Single_Reg(0x01,15,sMenu_Variable.u16GENErrorResetEnable);
      Modbus.runningStep = _WAIT_WRITE_SINGLE_REG;
      break;
    case _GEN_ERROR_RESET_MIN:
      Write_Single_Reg(0x01,14,sMenu_Variable.u16GENErrorResetTime);
      Modbus.runningStep = _WAIT_WRITE_SINGLE_REG;
      break;
    case _GEN_WARM_UP_TIME:
      Write_Single_Reg(0x01,16,sMenu_Variable.u16GENWarmUpTime);
      Modbus.runningStep = _WAIT_WRITE_SINGLE_REG;
      break;
    case _GEN_COOL_DOWN_TIME:
      Write_Single_Reg(0x01,17,sMenu_Variable.u16GENCoolDownTime);
      Modbus.runningStep = _WAIT_WRITE_SINGLE_REG;
      break;
    case _GEN_NIGHT_EN:
      Write_Single_Reg(0x01,2,sMenu_Variable.u16GENNightEnable);
      Modbus.runningStep = _WAIT_WRITE_SINGLE_REG;
      break;
    case _GEN_NIGHT_BEGIN:
      Write_Single_Reg(0x01,3,sMenu_Variable.u16GENNightStart);
      Modbus.runningStep = _WAIT_WRITE_SINGLE_REG;
      break;
    case _GEN_NIGHT_END:
      Write_Single_Reg(0x01,4,sMenu_Variable.u16GENNightEnd);
      Modbus.runningStep = _WAIT_WRITE_SINGLE_REG;
      break;
    case _DC_LOW_INPUT:
      Write_Single_Reg(0x01,13,sMenu_Variable.u16GENDCLowInput);
      Modbus.runningStep = _WAIT_WRITE_SINGLE_REG;
      break;
    case _DC_LOW_VOLT:
      Write_Single_Reg(0x01,12,sMenu_Variable.u16GENDCLowVolt*10);
      Modbus.runningStep = _WAIT_WRITE_SINGLE_REG;
      break;
    case _AIRCON_TEMP1:
      Write_Single_Reg(0x02,1,sMenu_Variable.u16AirConTemp[0]);
      Modbus.runningStep = _WAIT_WRITE_SINGLE_REG;
      break;
    case _AIRCON_TEMP2:
      Write_Single_Reg(0x02,2,sMenu_Variable.u16AirConTemp[1]);
      Modbus.runningStep = _WAIT_WRITE_SINGLE_REG;
      break;
    case _AIRCON_TEMP3:
      Write_Single_Reg(0x02,3,sMenu_Variable.u16AirConTemp[2]);
      Modbus.runningStep = _WAIT_WRITE_SINGLE_REG;
      break;
    case _AIRCON_TEMP4:
      Write_Single_Reg(0x02,4,sMenu_Variable.u16AirConTemp[3]);
      Modbus.runningStep = _WAIT_WRITE_SINGLE_REG;
      break;
    case _AIRCON_TIME1:
      Write_Single_Reg(0x02,5,sMenu_Variable.u16AirConTime1);
      Modbus.runningStep = _WAIT_WRITE_SINGLE_REG;
      break;
    case _AIRCON_TIME2:
      Write_Single_Reg(0x02,6,sMenu_Variable.u16AirConTime2);
      Modbus.runningStep = _WAIT_WRITE_SINGLE_REG;
      break;
    case _HOUR:
      Write_Time_Reg(0x01,1,0);
      Modbus.runningStep = _WAIT_WRITE_TIME_REG_ATS;
      break;
    default:
      break;
    }
    switch(Modbus.runningStep)
    {
    case _WAIT_MODE:
      vTaskDelay(1000);
      Modbus.runningStep = _READ_ATS_STATUS;
      break;
    case _READ_ATS_STATUS:
      Modbus.u8DataPointer = 0;
      Read_Holding_Regs_Query(0x01,0x00,33);
      Modbus.runningStep = _WAIT_ATS_RESPOND;
      break;
    case _READ_DOOR:
      Modbus.u8DataPointer = 0;
      Read_Holding_Regs_Query(0x03,0x00,5);
      Modbus.runningStep = _WAIT_DOOR_RESPOND;
      break;
    case _READ_AIR_COND:
      Modbus.u8DataPointer = 0;
      Read_Holding_Regs_Query(0x02,0x00,12);
      Modbus.runningStep = _WAIT_AIR_COND_RESPOND;
      break;
    case _READ_ATS_ACTIVE_ALARM:
      break;
    case _READ_ATS_HISTORY:
      break;
    case _WAIT_ATS_RESPOND:
      reVal = RS4851_Check_Respond_Data();
      if(reVal != 1)
      {
        Modbus.atsNorespond ++;
        if(Modbus.atsNorespond >= 5)
        {
          Modbus.atsError = 1;
          sActive_Alarm[15].status = 1;
        }
        Modbus.runningStep = _READ_DOOR;
      }
      else
      {
        Modbus.atsError = 0;
        sActive_Alarm[15].status = 0;
        Modbus.atsNorespond = 0;
        Modbus.runningStep = _READ_DOOR;
      }
      break;
    case _WAIT_DOOR_RESPOND:
      reVal = -1;
      if(reVal != 1)
      {
        Modbus.doorNorespond ++;
        if(Modbus.doorNorespond >= 5)
          Modbus.doorError = 1;
        Modbus.runningStep = _READ_AIR_COND;
      }
      else
      {
        Modbus.doorError = 0;
        Modbus.doorNorespond = 0;
        Modbus.runningStep = _READ_AIR_COND;
      }
      break;
    case _WAIT_AIR_COND_RESPOND:
      reVal = RS4851_Check_Respond_Data();
      if(reVal != 1)
      {
        Modbus.airConNorespond ++;
        if(Modbus.airConNorespond >= 5)
        {
          Modbus.airConError = 1;
          sActive_Alarm[16].status = 1;
        }
        Modbus.runningStep = _READ_ATS_STATUS;
      }
      else
      {
        Modbus.airConError = 0;
        sActive_Alarm[16].status = 0;
        Modbus.airConNorespond = 0;
        Modbus.runningStep = _READ_ATS_STATUS;
      }
      break;
    case _WAIT_WRITE_SINGLE_REG:
      vTaskDelay(200);
      reVal = RS4851_Check_Respond_Data();
      if(reVal == 1)
        sMenu_Variable.changedVal = 0;
      Modbus.runningStep = _READ_ATS_STATUS;
      break;
    case _WAIT_WRITE_TIME_REG_ATS:
      vTaskDelay(200);
      //Write time Aircon module
      Write_Time_Reg(0x02,1,0);
      Modbus.runningStep = _WAIT_WRITE_TIME_REG_AIRCON;
      sMenu_Variable.changedVal = 0;
      break;
    case _WAIT_WRITE_TIME_REG_AIRCON:
      vTaskDelay(200);
      Modbus.runningStep = _READ_ATS_STATUS;
      break;
    default:
      break;
    }
  }
}

/*!
* @brief Idle hook.
*/
void vApplicationIdleHook(void)
{
  static uint32_t ulCount = 0;
  ulCount++;
}

/*!
* @brief tick hook is executed every tick.
*/
void vApplicationTickHook(void)
{
  sysCountTest++;
  sMenu_Control.refreshMainPageCount++;
  if(sysCountTest >= 300)
  {
    GPIO_TogglePinsOutput(GPIOC,1<<4u);
    LED_RUN_TOGGLE();
    if (snmpConnectCheckStatus() ==  GPRS_CONNECTED)
      LED_CON_GPRS_ON();
    else
      LED_CON_GPRS_OFF();
    if (interface !=NULL)
    {
      if (interface->linkState)
        LED_CON_ETH_ON();
      else
        LED_CON_ETH_OFF();
    }
    else
    {
      LED_CON_ETH_OFF();
    }      
    if (IsAnyAlarm() > 0)
      LED_ALARM_ON();
    else
      LED_ALARM_OFF();
    sysCountTest = 0;
    GTime = GetTime();
  }
}
#endif // USERDEF_USER_INTERFACE == ENABLED

#if (USERDEF_IO_INTERFACE == ENABLED)
void IOsTask(void *param)
{
  TRACE_ERROR("IO task stared\r\n");
  //Endless loop
  while(1)
  {		
    IO_MUX_S0_OFF();
    IO_MUX_S1_OFF();
    IO_MUX_S2_OFF();
    IO_MUX_S3_OFF();
    
    osDelayTask(10);
    
    if ((IO_DI1_READ() == 0) && (IO_DI2_READ() == 0)) { DigitalInput[0] = 0;} // Close
    else if ((IO_DI1_READ() == 1) && (IO_DI2_READ() == 0)) { DigitalInput[0] = 1;} // Open
    else if ((IO_DI1_READ() == 1) && (IO_DI2_READ() == 1)) { DigitalInput[0] = 2;} // Cutoff
    
    IO_MUX_S0_ON();
    IO_MUX_S1_OFF();
    IO_MUX_S2_OFF();
    IO_MUX_S3_OFF();
    
    osDelayTask(10);
    
    if ((IO_DI1_READ() == 0) && (IO_DI2_READ() == 0)) { DigitalInput[1] = 0;} // Close
    else if ((IO_DI1_READ() == 1) && (IO_DI2_READ() == 0)) { DigitalInput[1] = 1;} // Open
    else if ((IO_DI1_READ() == 1) && (IO_DI2_READ() == 1)) { DigitalInput[1] = 2;} // Cutoff
    
    IO_MUX_S0_OFF();
    IO_MUX_S1_ON();
    IO_MUX_S2_OFF();
    IO_MUX_S3_OFF();
    
    osDelayTask(10);
    
    if ((IO_DI1_READ() == 0) && (IO_DI2_READ() == 0)) { DigitalInput[2] = 0;} // Close
    else if ((IO_DI1_READ() == 1) && (IO_DI2_READ() == 0)) { DigitalInput[2] = 1;} // Open
    else if ((IO_DI1_READ() == 1) && (IO_DI2_READ() == 1)) { DigitalInput[2] = 2;} // Cutoff
    
    IO_MUX_S0_ON();
    IO_MUX_S1_ON();
    IO_MUX_S2_OFF();
    IO_MUX_S3_OFF();
    
    osDelayTask(10);
    
    if ((IO_DI1_READ() == 0) && (IO_DI2_READ() == 0)) { DigitalInput[3] = 0;} // Close
    else if ((IO_DI1_READ() == 1) && (IO_DI2_READ() == 0)) { DigitalInput[3] = 1;} // Open
    else if ((IO_DI1_READ() == 1) && (IO_DI2_READ() == 1)) { DigitalInput[3] = 2;} // Cutoff
    
    IO_MUX_S0_OFF();
    IO_MUX_S1_OFF();
    IO_MUX_S2_ON();
    IO_MUX_S3_OFF();
    
    osDelayTask(10);
    
    if ((IO_DI1_READ() == 0) && (IO_DI2_READ() == 0)) { DigitalInput[4] = 0;} // Close
    else if ((IO_DI1_READ() == 1) && (IO_DI2_READ() == 0)) { DigitalInput[4] = 1;} // Open
    else if ((IO_DI1_READ() == 1) && (IO_DI2_READ() == 1)) { DigitalInput[4] = 2;} // Cutoff
    
    IO_MUX_S0_ON();
    IO_MUX_S1_OFF();
    IO_MUX_S2_ON();
    IO_MUX_S3_OFF();
    
    osDelayTask(10);
    
    if ((IO_DI1_READ() == 0) && (IO_DI2_READ() == 0)) { DigitalInput[5] = 0;} // Close
    else if ((IO_DI1_READ() == 1) && (IO_DI2_READ() == 0)) { DigitalInput[5] = 1;} // Open
    else if ((IO_DI1_READ() == 1) && (IO_DI2_READ() == 1)) { DigitalInput[5] = 2;} // Cutoff
    
    IO_MUX_S0_OFF();
    IO_MUX_S1_ON();
    IO_MUX_S2_ON();
    IO_MUX_S3_OFF();
    
    osDelayTask(10);
    
    if ((IO_DI1_READ() == 0) && (IO_DI2_READ() == 0)) { DigitalInput[6] = 0;} // Close
    else if ((IO_DI1_READ() == 1) && (IO_DI2_READ() == 0)) { DigitalInput[6] = 1;} // Open
    else if ((IO_DI1_READ() == 1) && (IO_DI2_READ() == 1)) { DigitalInput[6] = 2;} // Cutoff
    
    IO_MUX_S0_ON();
    IO_MUX_S1_ON();
    IO_MUX_S2_ON();
    IO_MUX_S3_OFF();
    
    osDelayTask(10);
    
    if ((IO_DI1_READ() == 0) && (IO_DI2_READ() == 0)) { DigitalInput[7] = 0;} // Close
    else if ((IO_DI1_READ() == 1) && (IO_DI2_READ() == 0)) { DigitalInput[7] = 1;} // Open
    else if ((IO_DI1_READ() == 1) && (IO_DI2_READ() == 1)) { DigitalInput[7] = 2;} // Cutoff
    
    IO_MUX_S0_OFF();
    IO_MUX_S1_OFF();
    IO_MUX_S2_OFF();
    IO_MUX_S3_ON();
    
    osDelayTask(10);
    
    if ((IO_DI1_READ() == 0) && (IO_DI2_READ() == 0)) { DigitalInput[8] = 0;} // Close
    else if ((IO_DI1_READ() == 1) && (IO_DI2_READ() == 0)) { DigitalInput[8] = 1;} // Open
    else if ((IO_DI1_READ() == 1) && (IO_DI2_READ() == 1)) { DigitalInput[8] = 2;} // Cutoff
    
    IO_MUX_S0_ON();
    IO_MUX_S1_OFF();
    IO_MUX_S2_OFF();
    IO_MUX_S3_ON();
    
    osDelayTask(10);
    
    if ((IO_DI1_READ() == 0) && (IO_DI2_READ() == 0)) { DigitalInput[9] = 0;} // Close
    else if ((IO_DI1_READ() == 1) && (IO_DI2_READ() == 0)) { DigitalInput[9] = 1;} // Open
    else if ((IO_DI1_READ() == 1) && (IO_DI2_READ() == 1)) { DigitalInput[9] = 2;} // Cutoff
    //Loop delay
    osDelayTask(1000);
  }
}
#endif // USERDEF_IO_INTERFACE == ENABLED

#if (USERDEF_SW_TIMER == ENABLED)
/*!
* @brief Software timer callback.
*/
static void SwTimerCallback(TimerHandle_t xTimer)
{
  trapStatus_TimePeriod++;  
  // check if door is not locked then lock the door after delay time - chaunm
  if (privateMibBase.accessoriesGroup.doorStatus == 0)
  {
    if (doorOpenTimeCount > 0)
      doorOpenTimeCount--;
    else
    {
      privateMibBase.accessoriesGroup.doorStatus = 1;
      IO_OPENDOOR_MCU_ON();
    }
  }
#if (USERDEF_SNMPCONNECT_MANAGER == ENABLED)
  snmpConnectIncreaseTick();
#endif
}
#endif // USERDEF_SW_TIMER == ENABLED

void UserTaskInit()
{
  OsTask *task;
#if (USERDEF_SW_TIMER == ENABLED)
  /* Create the software timer. */
  SwTimerHandle = xTimerCreate("SwTimer",          /* Text name. */
                               SW_TIMER_PERIOD_MS, /* Timer period. */
                               pdTRUE,             /* Enable auto reload. */
                               0,                  /* ID is not used. */
                               SwTimerCallback);   /* The callback function. */
  /* Start timer. */
  xTimerStart(SwTimerHandle, 0);
#endif // USERDEF_SW_TIMER == ENABLED
  
#if (USERDEF_ADC_TASK == ENABLED)
  //Create user task
  task = osCreateTask("ADC Task", adcTask, NULL, 400, OS_TASK_PRIORITY_NORMAL);
  //Failed to create the task?
  if(task == OS_INVALID_HANDLE)
  {
    //Debug message
    TRACE_ERROR("Failed to create  task!\r\n");
  }
#endif // USERDEF_ADC_TASK == ENABLED
  
#if (USERDEF_CLIENT_FTP == ENABLED)
  //Create user task
  task = osCreateTask("FTP Transfer Task", ftpTranferTask, NULL, 400, OS_TASK_PRIORITY_NORMAL);
  //Failed to create the task?
  if(task == OS_INVALID_HANDLE)
  {
    //Debug message
    TRACE_ERROR("Failed to create task!\r\n");
  }
#endif //USERDEF_CLIENT_FTP == ENABLED
  
#if (USERDEF_USER_INTERFACE == ENABLED)
  //	 /* Create RTOS task */
  //Create a task to active user interface
  task = osCreateTask("Hello_task", hello_task, NULL, 600, 4);
  //Failed to create the task?
  if(task == OS_INVALID_HANDLE)
  {
    //Debug message
    TRACE_ERROR("Failed to create Hello task!\r\n");
  } else
  {
    //Debug message
    TRACE_ERROR("Successfully create LCD task!\r\n");
  }
  //Create a task to active RS485 communication
  task = osCreateTask("RS485_task", rs485_task, NULL, 150, OS_TASK_PRIORITY_NORMAL);
  //Failed to create the task?
  if(task == OS_INVALID_HANDLE)
  {
    //Debug message
    TRACE_ERROR("Failed to create RS485 task!\r\n");
  }
#endif // USERDEF_USER_INTERFACE == ENABLED
  
#if (USERDEF_IO_INTERFACE == ENABLED)
  //Create a task to blink the LED
  task = osCreateTask("IOs", IOsTask, NULL, 200, OS_TASK_PRIORITY_NORMAL);
  //Failed to create the task?
  if(task == OS_INVALID_HANDLE)
  {
    //Debug message
    TRACE_ERROR("Failed to create task!\r\n");
  }
#endif // USERDEF_IO_INTERFACE == ENABLED
  
#if (USERDEF_SNMPCONNECT_MANAGER== ENABLED)
  //Create a task to handle the GPRS operation
  task = osCreateTask("ConnectManager", snmpConnectManagerTask, NULL, 500, OS_TASK_PRIORITY_NORMAL);
  //Failed to create the task?
  if(task == OS_INVALID_HANDLE)
  {
    //Debug message
    TRACE_ERROR("Failed to create SNMP Connect Manager task!\r\n");
  }
#endif	//USERDEF_SNMPCONNECT_MANAGER== ENABLED
  
#if (USERDEF_GPRS == ENABLED)
  // Test code
  //gprs_turnon();
  GPRS_PWR_ON();
  //Create a task to handle the GPRS operation
  task = osCreateTask("GPRS", gprs_task, NULL, 1000, OS_TASK_PRIORITY_NORMAL);
  //Failed to create the task?
  if(task == OS_INVALID_HANDLE)
  {
    //Debug message
    TRACE_ERROR("Failed to create GPRS task!\r\n");
  }
#endif	// USERDEF_GPRS== ENABLED
  
#if (USERDEF_CHAUNM_TEST_GPRS == ENABLED)
  task = osCreateTask("GPRS TEST", TestM26Gprs, NULL, 200, OS_TASK_PRIORITY_NORMAL);
  //Failed to create the task?
  if(task == OS_INVALID_HANDLE)
  {
    //Debug message
    TRACE_ERROR("Failed to create GPRS test task!\r\n");
  }
#endif // USERDEF_CHAUNM_TEST_GPRS
  
#if (USERDEF_CHAUNM_TEST_DOOR == ENABLED)
  task = osCreateTask("DOOR TEST", TestOpenDoorUpdate, NULL, 100, OS_TASK_PRIORITY_NORMAL);
  //Failed to create the task?
  if(task == OS_INVALID_HANDLE)
  {
    //Debug message
    TRACE_ERROR("Failed to create Door test task!\r\n");
  }
#endif // USERDEF_CHAUNM_TEST_DOOR
  
  #if (USERDEF_CHAUNM_TEST_GEN == ENABLED)
  task = osCreateTask("GEN TEST", TestGeneration, NULL, 100, OS_TASK_PRIORITY_NORMAL);
  //Failed to create the task?
  if(task == OS_INVALID_HANDLE)
  {
    //Debug message
    TRACE_ERROR("Failed to create Generator test task!\r\n");
  }
#endif // USERDEF_CHAUNM_TEST_GEN
}