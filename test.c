#include "compiler_port.h"
#include "FreeRTOS.h"
#include "task.h"
#include "os_port_freertos.h"
#include "quectel_m26.h"
#include "test.h"
#include "net_config.h"
#include "variables.h"

void TestM26Gprs(void* param)
{
  static uint8_t dataSendCount = 0;
  TRACE_INFO("GPRS Test Started\r\n");
  while (1)
  {
    if (gprsCheckStatus() == GPRS_POWER_OFF)
    {
      gprs_turnon();
    }
    else if (gprsCheckStatus() == GPRS_DISCONNECTED)
    {
      gprs_connect();
    }
    else if (gprsCheckStatus() == TRANSPARENT_DATA_MODE)
    {
      if (dataSendCount < 3)
      {
        TRACE_INFO("GPRS UDP transparent mode - send data\r\n");
        gprsSendMsg("xin chao\r\n", 10);
        dataSendCount++;
      }
    }
    osDelayTask(2000);
  }
}

void TestOpenDoorUpdate(void* param)
{
  sMenu_Control.accessUID = 0;
  TRACE_INFO("Door Access Test Started\r\n");
  while (1)
  {
    if (sMenu_Control.accessUID < 5)
        sMenu_Control.accessUID++;
    else if (sMenu_Control.accessUID == 5)
        sMenu_Control.accessUID = 0;
    
    if (sMenu_Control.accessUID > 5)
      sMenu_Control.accessUID = 1;
    osDelayTask(60000);
  }
}

void TestGeneration(void* param)
{
  static uint8_t GenStatus = 0;
  while(1)
  {
    if (sATS_Variable.Gen1ControlStatus < 7)
      sATS_Variable.Gen1ControlStatus++;
    else if (sATS_Variable.Gen1ControlStatus == 7)
      sATS_Variable.Gen1ControlStatus = 10;
    else if (sATS_Variable.Gen1ControlStatus == 10)
      sATS_Variable.Gen1ControlStatus = 0;
    switch (GenStatus)
    {
    case 0:
      sATS_Variable.Gen1Fail = 1;
      sATS_Variable.Gen1Voltage = 0;
      GenStatus = 1;
      break;
    case 1:
      sATS_Variable.Gen1Fail = 0;
      sATS_Variable.Gen1Voltage = 0;
      GenStatus = 2;
      break;
    case 2:
      sATS_Variable.Gen1Fail = 0;
      sATS_Variable.Gen1Voltage = 1;
      GenStatus = 0;
      break;
    default: 
      break;
    }    
    osDelayTask(10000);
  }
}