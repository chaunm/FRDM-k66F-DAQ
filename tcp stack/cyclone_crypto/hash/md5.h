/**
 * @file md5.h
 * @brief MD5 (Message-Digest Algorithm)
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

#ifndef _MD5_H
#define _MD5_H

//Dependencies
#include "core/crypto.h"

//MD5 block size
#define MD5_BLOCK_SIZE 64
//MD5 digest size
#define MD5_DIGEST_SIZE 16
//Common interface for hash algorithms
#define MD5_HASH_ALGO (&md5HashAlgo)

//C++ guard
#ifdef __cplusplus
   extern "C" {
#endif


/**
 * @brief MD5 algorithm context
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
} Md5Context;


//MD5 related constants
extern const HashAlgo md5HashAlgo;

//MD5 related functions
error_t md5Compute(const void *data, size_t length, uint8_t *digest);
void md5Init(Md5Context *context);
void md5Update(Md5Context *context, const void *data, size_t length);
void md5Final(Md5Context *context, uint8_t *digest);
void md5ProcessBlock(Md5Context *context);

//C++ guard
#ifdef __cplusplus
   }
#endif

#endif
