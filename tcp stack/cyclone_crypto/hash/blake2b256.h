/**
 * @file blake2b256.h
 * @brief BLAKE2b-256 hash function
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

#ifndef _BLAKE2B256_H
#define _BLAKE2B256_H

//Dependencies
#include "core/crypto.h"
#include "hash/blake2b.h"

//BLAKE2b-256 block size
#define BLAKE2B256_BLOCK_SIZE 128
//BLAKE2b-256 digest size
#define BLAKE2B256_DIGEST_SIZE 32
//Common interface for hash algorithms
#define BLAKE2B256_HASH_ALGO (&blake2b256HashAlgo)

//C++ guard
#ifdef __cplusplus
   extern "C" {
#endif


/**
 * @brief BLAKE2b-256 algorithm context
 **/

typedef Blake2bContext Blake2b256Context;


//BLAKE2b-256 related constants
extern const HashAlgo blake2b256HashAlgo;

//BLAKE2b-256 related functions
error_t blake2b256Compute(const void *data, size_t length, uint8_t *digest);
void blake2b256Init(Blake2b256Context *context);
void blake2b256Update(Blake2b256Context *context, const void *data, size_t length);
void blake2b256Final(Blake2b256Context *context, uint8_t *digest);

//C++ guard
#ifdef __cplusplus
   }
#endif

#endif
