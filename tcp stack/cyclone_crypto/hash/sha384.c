/**
 * @file sha384.c
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
 * @section Description
 *
 * SHA-384 is a secure hash algorithm for computing a condensed representation
 * of an electronic message. Refer to FIPS 180-4 for more details
 *
 * @author Oryx Embedded SARL (www.oryx-embedded.com)
 * @version 1.8.2
 **/

//Switch to the appropriate trace level
#define TRACE_LEVEL CRYPTO_TRACE_LEVEL

//Dependencies
#include "core/crypto.h"
#include "hash/sha384.h"

//Check crypto library configuration
#if (SHA384_SUPPORT == ENABLED)

//SHA-384 object identifier (2.16.840.1.101.3.4.2.2)
static const uint8_t sha384Oid[] = {0x60, 0x86, 0x48, 0x01, 0x65, 0x03, 0x04, 0x02, 0x02};

//Common interface for hash algorithms
const HashAlgo sha384HashAlgo =
{
   "SHA-384",
   sha384Oid,
   sizeof(sha384Oid),
   sizeof(Sha384Context),
   SHA384_BLOCK_SIZE,
   SHA384_DIGEST_SIZE,
   (HashAlgoCompute) sha384Compute,
   (HashAlgoInit) sha384Init,
   (HashAlgoUpdate) sha384Update,
   (HashAlgoFinal) sha384Final
};


/**
 * @brief Digest a message using SHA-384
 * @param[in] data Pointer to the message being hashed
 * @param[in] length Length of the message
 * @param[out] digest Pointer to the calculated digest
 * @return Error code
 **/

error_t sha384Compute(const void *data, size_t length, uint8_t *digest)
{
   //Allocate a memory buffer to hold the SHA-384 context
   Sha384Context *context = cryptoAllocMem(sizeof(Sha384Context));
   //Failed to allocate memory?
   if(context == NULL)
      return ERROR_OUT_OF_MEMORY;

   //Initialize the SHA-384 context
   sha384Init(context);
   //Digest the message
   sha384Update(context, data, length);
   //Finalize the SHA-384 message digest
   sha384Final(context, digest);

   //Free previously allocated memory
   cryptoFreeMem(context);
   //Successful processing
   return NO_ERROR;
}


/**
 * @brief Initialize SHA-384 message digest context
 * @param[in] context Pointer to the SHA-384 context to initialize
 **/

void sha384Init(Sha384Context *context)
{
   //Set initial hash value
   context->h[0] = 0xCBBB9D5DC1059ED8;
   context->h[1] = 0x629A292A367CD507;
   context->h[2] = 0x9159015A3070DD17;
   context->h[3] = 0x152FECD8F70E5939;
   context->h[4] = 0x67332667FFC00B31;
   context->h[5] = 0x8EB44A8768581511;
   context->h[6] = 0xDB0C2E0D64F98FA7;
   context->h[7] = 0x47B5481DBEFA4FA4;

   //Number of bytes in the buffer
   context->size = 0;
   //Total length of the message
   context->totalSize = 0;
}


/**
 * @brief Update the SHA-384 context with a portion of the message being hashed
 * @param[in] context Pointer to the SHA-384 context
 * @param[in] data Pointer to the buffer being hashed
 * @param[in] length Length of the buffer
 **/

void sha384Update(Sha384Context *context, const void *data, size_t length)
{
   //The function is defined in the exact same manner as SHA-512
   sha512Update(context, data, length);
}


/**
 * @brief Finish the SHA-384 message digest
 * @param[in] context Pointer to the SHA-384 context
 * @param[out] digest Calculated digest (optional parameter)
 **/

void sha384Final(Sha384Context *context, uint8_t *digest)
{
   //The function is defined in the exact same manner as SHA-512
   sha512Final(context, NULL);

   //Copy the resulting digest
   if(digest != NULL)
      cryptoMemcpy(digest, context->digest, SHA384_DIGEST_SIZE);
}

#endif
