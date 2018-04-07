#include "access_control.h"
#include "eeprom_rtc.h"
#include "rs485.h"
#include "variables.h"
#include "private_mib_module.h"

uint8_t TempUserID[5][9];
uint8_t newCardDetect;
// Save an UserID to EEPROM
void ACS_SaveUserID(uint16_t EEPROM_Addr, uint8_t* UserID)
{
  uint8_t i=0;
  for(i=0;i<8;i++)
    WriteEEPROM_Byte(EEPROM_Addr+i,*(UserID+i));
}

// Delete an UserID from EEPROM
void ACS_DeleteUserID(uint16_t EEPROM_Addr, uint8_t* UserID)
{
  uint8_t i=0;
  for(i=0;i<8;i++)
    WriteEEPROM_Byte(EEPROM_Addr+i,0xFF);
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
      // update acess ID - chaunm 07-04
      privateMibBase.siteInfoGroup.siteInfoAccessIdLen = 8;
      memcpy(privateMibBase.siteInfoGroup.siteInfoAccessId, (const char*)AccessIdTemp, 8);
    }
    DoorAccess.u8MosbusEn = 0;
    DoorAccess.u8ByteCount = 0;
  }
}