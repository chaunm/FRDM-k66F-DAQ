/**
 * @file blake2s224.h
 * @brief BLAKE2s-224 hash function
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

#ifndef _BLAKE2S224_H
#define _BLAKE2S224_H

//Dependencies
#include "core/crypto.h"
#include "hash/blake2s.h"

//BLAKE2s-224 block size
#define BLAKE2S224_BLOCK_SIZE 64
//BLAKE2s-224 digest size
#define BLAKE2S224_DIGEST_SIZE 28
//Common interface for hash algorithms
#define BLAKE2S224_HASH_ALGO (&blake2s224HashAlgo)

//C++ guard
#ifdef __cplusplus
   extern "C" {
#endif


/**
 * @brief BLAKE2s-224 algorithm context
 **/

typedef Blake2sContext Blake2s224Context;


//BLAKE2s-224 related constants
extern const HashAlgo blake2s224HashAlgo;

//BLAKE2s-224 related functions
error_t blake2s224Compute(const void *data, size_t length, uint8_t *digest);
void blake2s224Init(Blake2s224Context *context);
void blake2s224Update(Blake2s224Context *context, const void *data, size_t length);
void blake2s224Final(Blake2s224Context *context, uint8_t *digest);

//C++ guard
#ifdef __cplusplus
   }
#endif

#endif
