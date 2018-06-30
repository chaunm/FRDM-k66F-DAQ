/**
 * @file blake2b.h
 * @brief BLAKE2 cryptographic hash and MAC (BLAKE2b variant)
 *
 * @section License
 *
 * Copyright (C) 2010-2018 Oryx Embedded SARL. All rights reserved.
 *
 * This file is part of CycloneCrypto Eval.
 *
 * This software is provided in source form for a short-term evaluation only. The
 * evaluation license expires 90 days after the date you first download the software.
 *
 * If you plan to use this software in a commercial product, you are required to
 * purchase a commercial license from Oryx Embedded SARL.
 *
 * After the 90-day evaluation period, you agree to either purchase a commercial
 * license or delete all copies of this software. If you wish to extend the
 * evaluation period, you must contact sales@oryx-embedded.com.
 *
 * This evaluation software is provided "as is" without warranty of any kind.
 * Technical support is available as an option during the evaluation period.
 *
 * @author Oryx Embedded SARL (www.oryx-embedded.com)
 * @version 1.8.2
 **/

#ifndef _BLAKE2B_H
#define _BLAKE2B_H

//Dependencies
#include "core/crypto.h"

//BLAKE2b block size
#define BLAKE2B_BLOCK_SIZE 128

//C++ guard
#ifdef __cplusplus
   extern "C" {
#endif


/**
 * @brief BLAKE2b algorithm context
 **/

typedef struct
{
   union
   {
      uint64_t h[8];
      uint8_t digest[64];
   };
   union
   {
      uint64_t m[16];
      uint8_t buffer[128];
   };
   size_t size;
   uint64_t totalSize[2];
   size_t digestSize;
} Blake2bContext;


//BLAKE2b related functions
error_t blake2bCompute(const void *key, size_t keyLen, const void *data,
   size_t dataLen, uint8_t *digest, size_t digestLen);

error_t blake2bInit(Blake2bContext *context, const void *key,
   size_t keyLen, size_t digestLen);

void blake2bUpdate(Blake2bContext *context, const void *data, size_t length);
void blake2bFinal(Blake2bContext *context, uint8_t *digest);
void blake2bProcessBlock(Blake2bContext *context, bool_t last);

//C++ guard
#ifdef __cplusplus
   }
#endif

#endif
