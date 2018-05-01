#include "i2c_lock.h"
#include "FreeRTOS.h"
#include "semphr.h"

SemaphoreHandle_t i2cLock;

void I2C_Lock_Init()
{
  i2cLock = xSemaphoreCreateBinary();
  xSemaphoreGive(i2cLock);
}

void I2C_Get_Lock()
{
  xSemaphoreTake(i2cLock, portMAX_DELAY);
}

void I2C_Release_Lock()
{
  xSemaphoreGive(i2cLock);
}