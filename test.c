#include "compiler_port.h"
#include "FreeRTOS.h"
#include "task.h"
#include "os_port_freertos.h"
#include "test.h"
#include "net_config.h"
#include "variables.h"
#include "debug.h"

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