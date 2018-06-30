/**
 * @file blake2s.h
 * @brief BLAKE2 cryptographic hash and MAC (BLAKE2s variant)
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

#ifndef _BLAKE2S_H
#define _BLAKE2S_H

//Dependencies
#include "core/crypto.h"

//BLAKE2s block size
#define BLAKE2S_BLOCK_SIZE 64

//C++ guard
#ifdef __cplusplus
   extern "C" {
#endif


/**
 * @brief BLAKE2s algorithm context
 **/

typedef struct
{
   union
   {
      uint32_t h[8];
      uint8_t digest[32];
   };
   union
   {
      uint32_t m[16];
      uint8_t buffer[64];
   };
   size_t size;
   uint32_t totalSize[2];
   size_t digestSize;
} Blake2sContext;


//BLAKE2s related functions
error_t blake2sCompute(const void *key, size_t keyLen, const void *data,
   size_t dataLen, uint8_t *digest, size_t digestLen);

error_t blake2sInit(Blake2sContext *context, const void *key,
   size_t keyLen, size_t digestLen);

void blake2sUpdate(Blake2sContext *context, const void *data, size_t length);
void blake2sFinal(Blake2sContext *context, uint8_t *digest);
void blake2sProcessBlock(Blake2sContext *context, bool_t last);

//C++ guard
#ifdef __cplusplus
   }
#endif

#endif
