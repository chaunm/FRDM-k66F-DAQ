/**
 * @file rc4.h
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

#ifndef _RC4_H
#define _RC4_H

//Dependencies
#include "core/crypto.h"

//Common interface for encryption algorithms
#define RC4_CIPHER_ALGO (&rc4CipherAlgo)

//C++ guard
#ifdef __cplusplus
   extern "C" {
#endif


/**
 * @brief RC4 algorithm context
 **/

typedef struct
{
   uint_t i;
   uint_t j;
   uint8_t s[256];
} Rc4Context;


//RC4 related constants
extern const CipherAlgo rc4CipherAlgo;

//RC4 related functions
error_t rc4Init(Rc4Context *context, const uint8_t *key, size_t length);
void rc4Cipher(Rc4Context *context, const uint8_t *input, uint8_t *output, size_t length);

//C++ guard
#ifdef __cplusplus
   }
#endif

#endif
