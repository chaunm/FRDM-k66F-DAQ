/**
 * @file sha384.h
 * @brief SHA-384 (Secure Hash Algorithm 384)
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

#ifndef _SHA384_H
#define _SHA384_H

//Dependencies
#include "core/crypto.h"
#include "hash/sha512.h"

//SHA-384 block size
#define SHA384_BLOCK_SIZE 128
//SHA-384 digest size
#define SHA384_DIGEST_SIZE 48
//Common interface for hash algorithms
#define SHA384_HASH_ALGO (&sha384HashAlgo)

//C++ guard
#ifdef __cplusplus
   extern "C" {
#endif


/**
 * @brief SHA-384 algorithm context
 **/

typedef Sha512Context Sha384Context;


//SHA-384 related constants
extern const HashAlgo sha384HashAlgo;

//SHA-384 related functions
error_t sha384Compute(const void *data, size_t length, uint8_t *digest);
void sha384Init(Sha384Context *context);
void sha384Update(Sha384Context *context, const void *data, size_t length);
void sha384Final(Sha384Context *context, uint8_t *digest);

//C++ guard
#ifdef __cplusplus
   }
#endif

#endif
