/**
 * @file cbc.c
 * @brief Cipher Block Chaining (CBC) mode
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
 * The Cipher Block Chaining (CBC) mode is a confidentiality mode whose
 * encryption process features the combining of the plaintext blocks with
 * the previous ciphertext blocks. The CBC mode requires an IV to combine
 * with the first plaintext block. Refer to SP 800-38A for more details
 *
 * @author Oryx Embedded SARL (www.oryx-embedded.com)
 * @version 1.8.2
 **/

//Switch to the appropriate trace level
#define TRACE_LEVEL CRYPTO_TRACE_LEVEL

//Dependencies
#include "core/crypto.h"
#include "cipher_mode/cbc.h"
#include "debug.h"

//Check crypto library configuration
#if (CBC_SUPPORT == ENABLED)


/**
 * @brief CBC encryption
 * @param[in] cipher Cipher algorithm
 * @param[in] context Cipher algorithm context
 * @param[in,out] iv Initialization vector
 * @param[in] p Plaintext to be encrypted
 * @param[out] c Ciphertext resulting from the encryption
 * @param[in] length Total number of data bytes to be encrypted
 * @return Error code
 **/

error_t cbcEncrypt(const CipherAlgo *cipher, void *context,
   uint8_t *iv, const uint8_t *p, uint8_t *c, size_t length)
{
   size_t i;

   //CBC mode operates in a block-by-block fashion
   while(length >= cipher->blockSize)
   {
      //XOR input block with IV contents
      for(i = 0; i < cipher->blockSize; i++)
         c[i] = p[i] ^ iv[i];

      //Encrypt the current block based upon the output
      //of the previous encryption
      cipher->encryptBlock(context, c, c);

      //Update IV with output block contents
      cryptoMemcpy(iv, c, cipher->blockSize);

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
 * @brief CBC decryption
 * @param[in] cipher Cipher algorithm
 * @param[in] context Cipher algorithm context
 * @param[in,out] iv Initialization vector
 * @param[in] c Ciphertext to be decrypted
 * @param[out] p Plaintext resulting from the decryption
 * @param[in] length Total number of data bytes to be decrypted
 * @return Error code
 **/

error_t cbcDecrypt(const CipherAlgo *cipher, void *context,
   uint8_t *iv, const uint8_t *c, uint8_t *p, size_t length)
{
   size_t i;
   uint8_t t[16];

   //CBC mode operates in a block-by-block fashion
   while(length >= cipher->blockSize)
   {
      //Save input block
      cryptoMemcpy(t, c, cipher->blockSize);

      //Decrypt the current block
      cipher->decryptBlock(context, c, p);

      //XOR output block with IV contents
      for(i = 0; i < cipher->blockSize; i++)
         p[i] ^= iv[i];

      //Update IV with input block contents
      cryptoMemcpy(iv, t, cipher->blockSize);

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
