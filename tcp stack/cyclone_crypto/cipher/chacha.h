/**
 * @file chacha.h
 * @brief ChaCha encryption algorithm
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

#ifndef _CHACHA_H
#define _CHACHA_H

//Dependencies
#include "core/crypto.h"

//C++ guard
#ifdef __cplusplus
   extern "C" {
#endif


/**
 * @brief ChaCha algorithm context
 **/

typedef struct
{
   uint_t nr;
   uint32_t state[16];
   uint32_t block[16];
   size_t pos;
} ChachaContext;


//ChaCha related functions
error_t chachaInit(ChachaContext *context, uint_t nr, const uint8_t *key,
   size_t keyLen, const uint8_t *nonce, size_t nonceLen);

void chachaCipher(ChachaContext *context, const uint8_t *input,
   uint8_t *output, size_t length);

void chachaProcessBlock(ChachaContext *context);

//C++ guard
#ifdef __cplusplus
   }
#endif

#endif
