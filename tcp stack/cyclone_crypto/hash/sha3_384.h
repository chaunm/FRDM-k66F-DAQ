/**
 * @file sha3_384.h
 * @brief SHA3-384 hash function (SHA-3 with 384-bit output)
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

#ifndef _SHA3_384_H
#define _SHA3_384_H

//Dependencies
#include "core/crypto.h"
#include "xof/keccak.h"

//SHA3-384 block size
#define SHA3_384_BLOCK_SIZE 104
//SHA3-384 digest size
#define SHA3_384_DIGEST_SIZE 48
//Common interface for hash algorithms
#define SHA3_384_HASH_ALGO (&sha3_384HashAlgo)

//C++ guard
#ifdef __cplusplus
   extern "C" {
#endif


/**
 * @brief SHA3-384 algorithm context
 **/

typedef KeccakContext Sha3_384Context;


//SHA3-384 related constants
extern const HashAlgo sha3_384HashAlgo;

//SHA3-384 related functions
error_t sha3_384Compute(const void *data, size_t length, uint8_t *digest);
void sha3_384Init(Sha3_384Context *context);
void sha3_384Update(Sha3_384Context *context, const void *data, size_t length);
void sha3_384Final(Sha3_384Context *context, uint8_t *digest);

//C++ guard
#ifdef __cplusplus
   }
#endif

#endif
