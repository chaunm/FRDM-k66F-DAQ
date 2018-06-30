/**
 * @file cmac.h
 * @brief CMAC (Cipher-based Message Authentication Code)
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

#ifndef _CMAC_H
#define _CMAC_H

//Dependencies
#include "core/crypto.h"

//RC2 support?
#if (RC2_SUPPORT == ENABLED)
   #include "cipher/rc2.h"
#endif

//RC6 support?
#if (RC6_SUPPORT == ENABLED)
   #include "cipher/rc6.h"
#endif

//IDEA support?
#if (IDEA_SUPPORT == ENABLED)
   #include "cipher/idea.h"
#endif

//DES support?
#if (DES_SUPPORT == ENABLED)
   #include "cipher/des.h"
#endif

//Triple DES support?
#if (DES3_SUPPORT == ENABLED)
   #include "cipher/des3.h"
#endif

//AES support?
#if (AES_SUPPORT == ENABLED)
   #include "cipher/aes.h"
#endif

//Camellia support?
#if (CAMELLIA_SUPPORT == ENABLED)
   #include "cipher/camellia.h"
#endif

//SEED support?
#if (SEED_SUPPORT == ENABLED)
   #include "cipher/seed.h"
#endif

//ARIA support?
#if (ARIA_SUPPORT == ENABLED)
   #include "cipher/aria.h"
#endif

//PRESENT support?
#if (PRESENT_SUPPORT == ENABLED)
   #include "cipher/present.h"
#endif

//C++ guard
#ifdef __cplusplus
   extern "C" {
#endif


/**
 * @brief CMAC algorithm context
 **/

typedef struct
{
   const CipherAlgo *cipher;
   uint8_t cipherContext[MAX_CIPHER_CONTEXT_SIZE];
   uint8_t k1[MAX_CIPHER_BLOCK_SIZE];
   uint8_t k2[MAX_CIPHER_BLOCK_SIZE];
   uint8_t buffer[MAX_CIPHER_BLOCK_SIZE];
   size_t bufferLength;
   uint8_t mac[MAX_CIPHER_BLOCK_SIZE];
} CmacContext;


//CMAC related functions
error_t cmacCompute(const CipherAlgo *cipher, const void *key, size_t keyLen,
   const void *data, size_t dataLen, uint8_t *mac, size_t macLen);

error_t cmacInit(CmacContext *context, const CipherAlgo *cipher,
   const void *key, size_t keyLen);

void cmacReset(CmacContext *context);
void cmacUpdate(CmacContext *context, const void *data, size_t dataLen);
error_t cmacFinal(CmacContext *context, uint8_t *mac, size_t macLen);

void cmacMul(uint8_t *x, const uint8_t *a, size_t n, uint8_t rb);
void cmacXorBlock(uint8_t *x, const uint8_t *a, const uint8_t *b, size_t n);

//C++ guard
#ifdef __cplusplus
   }
#endif

#endif
