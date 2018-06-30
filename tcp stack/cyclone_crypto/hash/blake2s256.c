/**
 * @file blake2s256.c
 * @brief BLAKE2s-256 hash function
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

//Switch to the appropriate trace level
#define TRACE_LEVEL CRYPTO_TRACE_LEVEL

//Dependencies
#include "core/crypto.h"
#include "hash/blake2s256.h"

//Check crypto library configuration
#if (BLAKE2S256_SUPPORT == ENABLED)

//BLAKE2s-256 object identifier (1.3.6.1.4.1.1722.12.2.2.8)
static const uint8_t blake2s256Oid[] = {0x43, 0x06, 0x01, 0x04, 0x01, 0x8D, 0x3A, 0x0C, 0x02, 0x02, 0x08};

//Common interface for hash algorithms
const HashAlgo blake2s256HashAlgo =
{
   "BLAKE2s-256",
   blake2s256Oid,
   sizeof(blake2s256Oid),
   sizeof(Blake2s256Context),
   BLAKE2S256_BLOCK_SIZE,
   BLAKE2S256_DIGEST_SIZE,
   (HashAlgoCompute) blake2s256Compute,
   (HashAlgoInit) blake2s256Init,
   (HashAlgoUpdate) blake2s256Update,
   (HashAlgoFinal) blake2s256Final
};


/**
 * @brief Digest a message using BLAKE2s-256
 * @param[in] data Pointer to the message being hashed
 * @param[in] length Length of the message
 * @param[out] digest Pointer to the calculated digest
 * @return Error code
 **/

error_t blake2s256Compute(const void *data, size_t length, uint8_t *digest)
{
   //Compute the unkeyed hash with BLAKE2s-256
   return blake2sCompute(NULL, 0, data, length, digest, BLAKE2S256_DIGEST_SIZE);
}


/**
 * @brief Initialize BLAKE2s-256 hash computation
 * @param[in] context Pointer to the BLAKE2s context to initialize
 **/

void blake2s256Init(Blake2s256Context *context)
{
   //Initialize the hashing context
   blake2sInit(context, NULL, 0, BLAKE2S256_DIGEST_SIZE);
}


/**
 * @brief Update BLAKE2s-256 hash computation
 * @param[in] context Pointer to the BLAKE2s context
 * @param[in] data Pointer to the buffer being hashed
 * @param[in] length Length of the buffer
 **/

void blake2s256Update(Blake2s256Context *context, const void *data, size_t length)
{
   //Digest the data
   blake2sUpdate(context, data, length);
}


/**
 * @brief Finish BLAKE2s-256 hash computation
 * @param[in] context Pointer to the BLAKE2s context
 * @param[out] digest Calculated digest (optional parameter)
 **/

void blake2s256Final(Blake2s256Context *context, uint8_t *digest)
{
   //Generate the message digest
   blake2sFinal(context, digest);
}

#endif
