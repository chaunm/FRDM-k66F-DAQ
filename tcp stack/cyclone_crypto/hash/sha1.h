/**
 * @file sha1.h
 * @brief SHA-1 (Secure Hash Algorithm 1)
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

#ifndef _SHA1_H
#define _SHA1_H

//Dependencies
#include "core/crypto.h"

//SHA-1 block size
#define SHA1_BLOCK_SIZE 64
//SHA-1 digest size
#define SHA1_DIGEST_SIZE 20
//Common interface for hash algorithms
#define SHA1_HASH_ALGO (&sha1HashAlgo)

//C++ guard
#ifdef __cplusplus
   extern "C" {
#endif


/**
 * @brief SHA-1 algorithm context
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
      uint32_t w[16];
      uint8_t buffer[64];
   };
   size_t size;
   uint64_t totalSize;
} Sha1Context;


//SHA-1 related constants
extern const HashAlgo sha1HashAlgo;

//SHA-1 related functions
error_t sha1Compute(const void *data, size_t length, uint8_t *digest);
void sha1Init(Sha1Context *context);
void sha1Update(Sha1Context *context, const void *data, size_t length);
void sha1Final(Sha1Context *context, uint8_t *digest);
void sha1ProcessBlock(Sha1Context *context);

//C++ guard
#ifdef __cplusplus
   }
#endif

#endif
