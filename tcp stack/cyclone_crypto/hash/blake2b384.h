/**
 * @file blake2b384.h
 * @brief BLAKE2b-384 hash function
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

#ifndef _BLAKE2B384_H
#define _BLAKE2B384_H

//Dependencies
#include "core/crypto.h"
#include "hash/blake2b.h"

//BLAKE2b-384 block size
#define BLAKE2B384_BLOCK_SIZE 128
//BLAKE2b-384 digest size
#define BLAKE2B384_DIGEST_SIZE 48
//Common interface for hash algorithms
#define BLAKE2B384_HASH_ALGO (&blake2b384HashAlgo)

//C++ guard
#ifdef __cplusplus
   extern "C" {
#endif


/**
 * @brief BLAKE2b-384 algorithm context
 **/

typedef Blake2bContext Blake2b384Context;


//BLAKE2b-384 related constants
extern const HashAlgo blake2b384HashAlgo;

//BLAKE2b-384 related functions
error_t blake2b384Compute(const void *data, size_t length, uint8_t *digest);
void blake2b384Init(Blake2b384Context *context);
void blake2b384Update(Blake2b384Context *context, const void *data, size_t length);
void blake2b384Final(Blake2b384Context *context, uint8_t *digest);

//C++ guard
#ifdef __cplusplus
   }
#endif

#endif
