#include "access_control.h"
#include "eeprom_rtc.h"
#include "rs485.h"
#include "variables.h"
#include "i2c_lock.h"
#include "freeRTOS.h"
#include "task.h"

uint8_t TempUserID[5][9];
uint8_t newCardDetect;
uint8_t doorOpenTimeCount = 0;
// Save an UserID to EEPROM
void ACS_SaveUserID(uint16_t EEPROM_Addr, uint8_t* UserID)
{
  uint8_t i=0;
  for(i=0;i<8;i++)
  {
    I2C_Get_Lock();
    vTaskSuspendAll();
    WriteEEPROM_Byte(EEPROM_Addr+i,*(UserID+i));
    xTaskResumeAll();
    I2C_Release_Lock();
  }
}

// Delete an UserID from EEPROM
void ACS_DeleteUserID(uint16_t EEPROM_Addr, uint8_t* UserID)
{
  uint8_t i=0;
  for(i=0;i<8;i++)
  {
    I2C_Get_Lock();
    vTaskSuspendAll();
    WriteEEPROM_Byte(EEPROM_Addr+i,0xFF);    
    xTaskResumeAll();
    I2C_Release_Lock();
  }
}

// Check if an user ID card in list
int8_t ACS_FindUserID (uint8_t *userID)
{
  uint8_t i = 0,j = 0, k = 0;
  for(i=0; i < 5; i++)
  {
    k = 0;
    for(j = 0; j < 8; j++)
      if(*(userID + j) != sMenu_Variable.u8UserID[i][j])
      {
        k++;
        break;
      }
    if(k == 0)
    {
      return (i+1);
    }
  }
  return -1;
}

void ACS_AccessCheck(void)
{
  uint8_t   i=0;
  //  uint8_t   mTempBuff[8];
  if(DoorAccess.u8MosbusEn == 2)
  {
    if(sMenu_Control.learnUID)
    {
      //Save user ID
      for(i=0;i<8;i++)
        TempUserID[sMenu_Control.index][i] = DoorAccess.u8BuffRead[i];
      sMenu_Control.refesh = 1;
    }
    else
    {
      //Check user in system memory -> open door
      for(i=0;i<8;i++)
      {
        //        mTempBuff[i] = DoorAccess.u8BuffRead[i];
        AccessIdTemp[i] = DoorAccess.u8BuffRead[i];
        //        privateMibBase.siteInfoGroup.siteInfoAccessId[i] = mTempBuff[i];
        //        privateMibBase.siteInfoGroup.siteInfoAccessIdLen = 8;
      }
      //      sMenu_Control.accessUID = Find_UserID(mTempBuff);
      sMenu_Control.accessUID = ACS_FindUserID(AccessIdTemp);
      newCardDetect = 1;
    }
    DoorAccess.u8MosbusEn = 0;
    DoorAccess.u8ByteCount = 0;
  }
}

