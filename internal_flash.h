#ifndef __INTERNAL_FLASH_H__
#define __INTERNAL_FLASH_H__

#include "fsl_flash.h"
#include "partition.h"

void IFLASH_Init();
status_t IFLASH_EraseSector(uint32_t startSector, uint32_t endSector);
status_t IFLASH_Erase(uint32_t startAddr, uint32_t length);
status_t IFLASH_CheckBlank(uint32_t startAddr, uint32_t length);
status_t IFLASH_CopyRamToFlash(uint32_t dstAddr, uint32_t *srcAddr, uint32_t numOfBytes);
void IFLASH_CopyFlashToRam(uint32_t srcAddr, uint8_t* dstAddr, uint32_t numOfBytes);
bool IFLASH_IsUnsecure();
#endif