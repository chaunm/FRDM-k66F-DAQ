/**
 * @file ripemd128.h
 * @brief RIPEMD-128 hash function
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

#ifndef _RIPEMD128_H
#define _RIPEMD128_H

//Dependencies
#include "core/crypto.h"

//RIPEMD-128 block size
#define RIPEMD128_BLOCK_SIZE 64
//RIPEMD-128 digest size
#define RIPEMD128_DIGEST_SIZE 16
//Common interface for hash algorithms
#define RIPEMD128_HASH_ALGO (&ripemd128HashAlgo)

//C++ guard
#ifdef __cplusplus
   extern "C" {
#endif


/**
 * @brief RIPEMD-128 algorithm context
 **/

typedef struct
{
   union
   {
      uint32_t h[4];
      uint8_t digest[16];
   };
   union
   {
      uint32_t x[16];
      uint8_t buffer[64];
   };
   size_t size;
   uint64_t totalSize;
} Ripemd128Context;


//RIPEMD-128 related constants
extern const HashAlgo ripemd128HashAlgo;

//RIPEMD-128 related functions
error_t ripemd128Compute(const void *data, size_t length, uint8_t *digest);
void ripemd128Init(Ripemd128Context *context);
void ripemd128Update(Ripemd128Context *context, const void *data, size_t length);
void ripemd128Final(Ripemd128Context *context, uint8_t *digest);
void ripemd128ProcessBlock(Ripemd128Context *context);

//C++ guard
#ifdef __cplusplus
   }
#endif

#endif
