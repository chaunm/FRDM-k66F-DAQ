#ifndef __ACCESS_CONTROL_H__
#define __ACCESS_CONTROL_H__
#include <stdint.h>

void ACS_SaveUserID (uint16_t EEPROM_Addr, uint8_t* UserID);
void ACS_DeleteUserID (uint16_t EEPROM_Addr, uint8_t* UserID);
int8_t ACS_FindUserID (uint8_t *userID);
void ACS_AccessCheck(void);
#endif