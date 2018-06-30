/**
 * @file rc4.c
 * @brief RC4 encryption algorithm
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
#include "cipher/rc4.h"

//Check crypto library configuration
#if (RC4_SUPPORT == ENABLED)

//Common interface for encryption algorithms
const CipherAlgo rc4CipherAlgo =
{
   "RC4",
   sizeof(Rc4Context),
   CIPHER_ALGO_TYPE_STREAM,
   0,
   (CipherAlgoInit) rc4Init,
   (CipherAlgoEncryptStream) rc4Cipher,
   (CipherAlgoDecryptStream) rc4Cipher,
   NULL,
   NULL
};


/**
 * @brief Initialize an RC4 context using the supplied key
 * @param[in] context Pointer to the RC4 context to initialize
 * @param[in] key Pointer to the key
 * @param[in] length Length of the key
 * @return Error code
 **/

error_t rc4Init(Rc4Context *context, const uint8_t *key, size_t length)
{
   uint_t i;
   uint_t j;
   uint8_t temp;

   //Clear context
   context->i = 0;
   context->j = 0;

   //Initialize the S array with identity permutation
   for(i = 0; i < 256; i++)
      context->s[i] = i;

   //S is then processed for 256 iterations
   for(i = 0, j = 0; i < 256; i++)
   {
      //Randomize the permutations using the supplied key
      j = (j + context->s[i] + key[i % length]) % 256;

      //Swap the values of S[i] and S[j]
      temp = context->s[i];
      context->s[i] = context->s[j];
      context->s[j] = temp;
   }

   //RC4 context successfully initialized
   return NO_ERROR;
}


/**
 * @brief Encrypt/decrypt data with the RC4 algorithm
 * @param[in] context Pointer to the RC4 context
 * @param[in] input Pointer to the data to encrypt/decrypt
 * @param[in] output Pointer to the resulting data
 * @param[in] length Length of the input data
 **/

void rc4Cipher(Rc4Context *context, const uint8_t *input, uint8_t *output, size_t length)
{
   uint8_t temp;

   //Restore context
   uint_t i = context->i;
   uint_t j = context->j;
   uint8_t *s = context->s;

   //Encryption loop
   while(length > 0)
   {
      //Adjust indices
      i = (i + 1) % 256;
      j = (j + s[i]) % 256;

      //Swap the values of S[i] and S[j]
      temp = s[i];
      s[i] = s[j];
      s[j] = temp;

      //XOR the input data with the RC4 stream
      *output = *input ^ s[(s[i] + s[j]) % 256];

      //Increment data pointers
      input++;
      output++;

      //Remaining bytes to process
      length--;
   }

   //Save context
   context->i = i;
   context->j = j;
}

#endif
