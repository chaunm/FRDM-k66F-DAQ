#include "compiler_port.h"
#include "FreeRTOS.h"
#include "task.h"
#include "os_port_freertos.h"
#include "quectel_m26.h"
#include "test.h"

void TestM26Gprs(void* param)
{
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
      gprsSendMsg("xin chao", 8);
    osDelayTask(10000);
  }
}