/**
 * @file sha512_256.h
 * @brief SHA-512/256 (Secure Hash Algorithm)
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

#ifndef _SHA512_256_H
#define _SHA512_256_H

//Dependencies
#include "core/crypto.h"
#include "hash/sha512.h"

//SHA-512/256 block size
#define SHA512_256_BLOCK_SIZE 128
//SHA-512/256 digest size
#define SHA512_256_DIGEST_SIZE 32
//Common interface for hash algorithms
#define SHA512_256_HASH_ALGO (&sha512_256HashAlgo)

//C++ guard
#ifdef __cplusplus
   extern "C" {
#endif


/**
 * @brief SHA-512/256 algorithm context
 **/

typedef Sha512Context Sha512_256Context;


//SHA-512/256 related constants
extern const HashAlgo sha512_256HashAlgo;

//SHA-512/256 related functions
error_t sha512_256Compute(const void *data, size_t length, uint8_t *digest);
void sha512_256Init(Sha512_256Context *context);
void sha512_256Update(Sha512_256Context *context, const void *data, size_t length);
void sha512_256Final(Sha512_256Context *context, uint8_t *digest);

//C++ guard
#ifdef __cplusplus
   }
#endif

#endif
