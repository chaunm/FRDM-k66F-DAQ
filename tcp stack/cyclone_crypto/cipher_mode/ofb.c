/**
 * @file ofb.c
 * @brief Output Feedback (OFB) mode
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
 * The Output Feedback (OFB) mode is a confidentiality mode that features the
 * iteration of the forward cipher on an IV to generate a sequence of output
 * blocks that are exclusive-ORed with the plaintext to produce the ciphertext,
 * and vice versa. The OFB mode requires that the IV is a nonce, i.e., the IV
 * must be unique for each execution of the mode under the given key.
 * Refer to SP 800-38A for more details
 *
 * @author Oryx Embedded SARL (www.oryx-embedded.com)
 * @version 1.8.2
 **/

//Switch to the appropriate trace level
#define TRACE_LEVEL CRYPTO_TRACE_LEVEL

//Dependencies
#include "core/crypto.h"
#include "cipher_mode/ofb.h"
#include "debug.h"

//Check crypto library configuration
#if (OFB_SUPPORT == ENABLED)


/**
 * @brief OFB encryption
 * @param[in] cipher Cipher algorithm
 * @param[in] context Cipher algorithm context
 * @param[in] s Size of the plaintext and ciphertext segments
 * @param[in,out] iv Initialization vector
 * @param[in] p Plaintext to be encrypted
 * @param[out] c Ciphertext resulting from the encryption
 * @param[in] length Total number of data bytes to be encrypted
 * @return Error code
 **/

error_t ofbEncrypt(const CipherAlgo *cipher, void *context, uint_t s,
   uint8_t *iv, const uint8_t *p, uint8_t *c, size_t length)
{
   size_t i;
   uint8_t o[16];

   //The parameter must be a multiple of 8
   if((s % 8) != 0)
      return ERROR_INVALID_PARAMETER;

   //Determine the size, in bytes, of the plaintext and ciphertext segments
   s = s / 8;

   //Check the resulting value
   if(s < 1 || s > cipher->blockSize)
      return ERROR_INVALID_PARAMETER;

   //Process each plaintext segment
   while(length >= s)
   {
      //Compute O(j) = CIPH(I(j))
      cipher->encryptBlock(context, iv, o);

      //Compute C(j) = P(j) XOR MSB(O(j))
      for(i = 0; i < s; i++)
         c[i] = p[i] ^ o[i];

      //Compute I(j+1) = LSB(I(j)) | O(j)
      cryptoMemmove(iv, iv + s, cipher->blockSize - s);
      cryptoMemcpy(iv + cipher->blockSize - s, o, s);

      //Next block
      p += s;
      c += s;
      length -= s;
   }

   //The plaintext must be a multiple of the segment size
   if(length != 0)
      return ERROR_INVALID_LENGTH;

   //Successful encryption
   return NO_ERROR;
}


/**
 * @brief OFB decryption
 * @param[in] cipher Cipher algorithm
 * @param[in] context Cipher algorithm context
 * @param[in] s Size of the plaintext and ciphertext segments
 * @param[in,out] iv Initialization vector
 * @param[in] c Ciphertext to be decrypted
 * @param[out] p Plaintext resulting from the decryption
 * @param[in] length Total number of data bytes to be decrypted
 * @return Error code
 **/

error_t ofbDecrypt(const CipherAlgo *cipher, void *context, uint_t s,
   uint8_t *iv, const uint8_t *c, uint8_t *p, size_t length)
{
   size_t i;
   uint8_t o[16];

   //The parameter must be a multiple of 8
   if((s % 8) != 0)
      return ERROR_INVALID_PARAMETER;

   //Determine the size, in bytes, of the plaintext and ciphertext segments
   s = s / 8;

   //Check the resulting value
   if(s < 1 || s > cipher->blockSize)
      return ERROR_INVALID_PARAMETER;

   //Process each ciphertext segment
   while(length >= s)
   {
      //Compute O(j) = CIPH(I(j))
      cipher->encryptBlock(context, iv, o);

      //Compute P(j) = C(j) XOR MSB(O(j))
      for(i = 0; i < s; i++)
         p[i] = c[i] ^ o[i];

      //Compute I(j+1) = LSB(I(j)) | O(j)
      cryptoMemmove(iv, iv + s, cipher->blockSize - s);
      cryptoMemcpy(iv + cipher->blockSize - s, o, s);

      //Next block
      c += s;
      p += s;
      length -= s;
   }

   //The plaintext must be a multiple of the segment size
   if(length != 0)
      return ERROR_INVALID_LENGTH;

   //Successful encryption
   return NO_ERROR;
}

#endif
