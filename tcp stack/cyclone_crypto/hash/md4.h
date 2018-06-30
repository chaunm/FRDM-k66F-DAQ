/**
 * @file md4.h
 * @brief MD4 (Message-Digest Algorithm)
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

#ifndef _MD4_H
#define _MD4_H

//Dependencies
#include "core/crypto.h"

//MD4 block size
#define MD4_BLOCK_SIZE 64
//MD4 digest size
#define MD4_DIGEST_SIZE 16
//Common interface for hash algorithms
#define MD4_HASH_ALGO (&md4HashAlgo)

//C++ guard
#ifdef __cplusplus
   extern "C" {
#endif


/**
 * @brief MD4 algorithm context
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
} Md4Context;


//MD4 related constants
extern const HashAlgo md4HashAlgo;

//MD4 related functions
error_t md4Compute(const void *data, size_t length, uint8_t *digest);
void md4Init(Md4Context *context);
void md4Update(Md4Context *context, const void *data, size_t length);
void md4Final(Md4Context *context, uint8_t *digest);
void md4ProcessBlock(Md4Context *context);

//C++ guard
#ifdef __cplusplus
   }
#endif

#endif
