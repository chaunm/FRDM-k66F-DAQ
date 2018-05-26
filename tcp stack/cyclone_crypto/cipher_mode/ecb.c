/**
 * @file ecb.c
 * @brief Electronic Codebook (ECB) mode
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
 * The Electronic Codebook (ECB) mode is a confidentiality mode that features,
 * for a given key, the assignment of a fixed ciphertext block to each
 * plaintext block, analogous to the assignment of code words in a codebook.
 * Refer to SP 800-38A for more details
 *
 * @author Oryx Embedded SARL (www.oryx-embedded.com)
 * @version 1.8.2
 **/

//Switch to the appropriate trace level
#define TRACE_LEVEL CRYPTO_TRACE_LEVEL

//Dependencies
#include "core/crypto.h"
#include "cipher_mode/ecb.h"
#include "debug.h"

//Check crypto library configuration
#if (ECB_SUPPORT == ENABLED)


/**
 * @brief ECB encryption
 * @param[in] cipher Cipher algorithm
 * @param[in] context Cipher algorithm context
 * @param[in] p Plaintext to be encrypted
 * @param[out] c Ciphertext resulting from the encryption
 * @param[in] length Total number of data bytes to be encrypted
 * @return Error code
 **/

error_t ecbEncrypt(const CipherAlgo *cipher, void *context,
   const uint8_t *p, uint8_t *c, size_t length)
{
   //ECB mode operates in a block-by-block fashion
   while(length >= cipher->blockSize)
   {
      //Encrypt current block
      cipher->encryptBlock(context, p, c);

      //Next block
      p += cipher->blockSize;
      c += cipher->blockSize;
      length -= cipher->blockSize;
   }

   //The plaintext must be a multiple of the block size
   if(length != 0)
      return ERROR_INVALID_LENGTH;

   //Successful encryption
   return NO_ERROR;
}


/**
 * @brief ECB decryption
 * @param[in] cipher Cipher algorithm
 * @param[in] context Cipher algorithm context
 * @param[in] c Ciphertext to be decrypted
 * @param[out] p Plaintext resulting from the decryption
 * @param[in] length Total number of data bytes to be decrypted
 * @return Error code
 **/

error_t ecbDecrypt(const CipherAlgo *cipher, void *context,
   const uint8_t *c, uint8_t *p, size_t length)
{
   //ECB mode operates in a block-by-block fashion
   while(length >= cipher->blockSize)
   {
      //Decrypt current block
      cipher->decryptBlock(context, c, p);

      //Next block
      c += cipher->blockSize;
      p += cipher->blockSize;
      length -= cipher->blockSize;
   }

   //The ciphertext must be a multiple of the block size
   if(length != 0)
      return ERROR_INVALID_LENGTH;

   //Successful encryption
   return NO_ERROR;
}

#endif
