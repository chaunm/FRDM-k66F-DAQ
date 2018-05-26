/**
 * @file whirlpool.h
 * @brief Whirlpool hash function
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

#ifndef _WHIRLPOOL_H
#define _WHIRLPOOL_H

//Dependencies
#include "core/crypto.h"

//Whirlpool block size
#define WHIRLPOOL_BLOCK_SIZE 64
//Whirlpool digest size
#define WHIRLPOOL_DIGEST_SIZE 64
//Common interface for hash algorithms
#define WHIRLPOOL_HASH_ALGO (&whirlpoolHashAlgo)

//C++ guard
#ifdef __cplusplus
   extern "C" {
#endif


/**
 * @brief Whirlpool algorithm context
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
      uint64_t x[8];
      uint8_t buffer[64];
   };

   uint64_t k[8];
   uint64_t l[8];
   uint64_t state[8];

   size_t size;
   uint64_t totalSize;
} WhirlpoolContext;


//Whirlpool related constants
extern const HashAlgo whirlpoolHashAlgo;

//Whirlpool related functions
error_t whirlpoolCompute(const void *data, size_t length, uint8_t *digest);
void whirlpoolInit(WhirlpoolContext *context);
void whirlpoolUpdate(WhirlpoolContext *context, const void *data, size_t length);
void whirlpoolFinal(WhirlpoolContext *context, uint8_t *digest);
void whirlpoolProcessBlock(WhirlpoolContext *context);

//C++ guard
#ifdef __cplusplus
   }
#endif

#endif
