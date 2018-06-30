/**
 * @file rc2.h
 * @brief RC2 block cipher
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

#ifndef _RC2_H
#define _RC2_H

//Dependencies
#include "core/crypto.h"

//RC2 block size
#define RC2_BLOCK_SIZE 8
//Common interface for encryption algorithms
#define RC2_CIPHER_ALGO (&rc2CipherAlgo)

//C++ guard
#ifdef __cplusplus
   extern "C" {
#endif


/**
 * @brief RC2 algorithm context
 **/

typedef struct
{
   union
   {
      uint16_t k[64];
      uint8_t l[128];
   };
} Rc2Context;


//RC2 related constants
extern const CipherAlgo rc2CipherAlgo;

//RC2 related functions
error_t rc2Init(Rc2Context *context, const uint8_t *key, size_t keyLen);

error_t rc2InitEx(Rc2Context *context, const uint8_t *key, size_t keyLen,
   uint_t effectiveKeyLen);

void rc2EncryptBlock(Rc2Context *context, const uint8_t *input, uint8_t *output);
void rc2DecryptBlock(Rc2Context *context, const uint8_t *input, uint8_t *output);

//C++ guard
#ifdef __cplusplus
   }
#endif

#endif
