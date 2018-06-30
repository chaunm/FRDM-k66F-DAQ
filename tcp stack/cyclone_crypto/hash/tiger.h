/**
 * @file tiger.h
 * @brief Tiger hash function
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

#ifndef _TIGER_H
#define _TIGER_H

//Dependencies
#include "core/crypto.h"

//Tiger block size
#define TIGER_BLOCK_SIZE 64
//Tiger digest size
#define TIGER_DIGEST_SIZE 24
//Common interface for hash algorithms
#define TIGER_HASH_ALGO (&tigerHashAlgo)

//C++ guard
#ifdef __cplusplus
   extern "C" {
#endif


/**
 * @brief Tiger algorithm context
 **/

typedef struct
{
   union
   {
      uint64_t h[3];
      uint8_t digest[24];
   };
   union
   {
      uint64_t x[8];
      uint8_t buffer[64];
   };
   size_t size;
   uint64_t totalSize;
} TigerContext;


//Tiger related constants
extern const HashAlgo tigerHashAlgo;

//Tiger related functions
error_t tigerCompute(const void *data, size_t length, uint8_t *digest);
void tigerInit(TigerContext *context);
void tigerUpdate(TigerContext *context, const void *data, size_t length);
void tigerFinal(TigerContext *context, uint8_t *digest);
void tigerProcessBlock(TigerContext *context);

//C++ guard
#ifdef __cplusplus
   }
#endif

#endif
