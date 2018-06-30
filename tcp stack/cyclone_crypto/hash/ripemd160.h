/**
 * @file ripemd160.h
 * @brief RIPEMD-160 hash function
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

#ifndef _RIPEMD160_H
#define _RIPEMD160_H

//Dependencies
#include "core/crypto.h"

//RIPEMD-160 block size
#define RIPEMD160_BLOCK_SIZE 64
//RIPEMD-160 digest size
#define RIPEMD160_DIGEST_SIZE 20
//Common interface for hash algorithms
#define RIPEMD160_HASH_ALGO (&ripemd160HashAlgo)

//C++ guard
#ifdef __cplusplus
   extern "C" {
#endif


/**
 * @brief RIPEMD-160 algorithm context
 **/

typedef struct
{
   union
   {
      uint32_t h[5];
      uint8_t digest[20];
   };
   union
   {
      uint32_t x[16];
      uint8_t buffer[64];
   };
   size_t size;
   uint64_t totalSize;
} Ripemd160Context;


//RIPEMD-160 related constants
extern const HashAlgo ripemd160HashAlgo;

//RIPEMD-160 related functions
error_t ripemd160Compute(const void *data, size_t length, uint8_t *digest);
void ripemd160Init(Ripemd160Context *context);
void ripemd160Update(Ripemd160Context *context, const void *data, size_t length);
void ripemd160Final(Ripemd160Context *context, uint8_t *digest);
void ripemd160ProcessBlock(Ripemd160Context *context);

//C++ guard
#ifdef __cplusplus
   }
#endif

#endif
