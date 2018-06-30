/**
 * @file shake128.h
 * @brief SHAKE128 extendable-output function (XOF)
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

#ifndef _SHAKE128_H
#define _SHAKE128_H

//Dependencies
#include "core/crypto.h"
#include "xof/keccak.h"

//C++ guard
#ifdef __cplusplus
   extern "C" {
#endif


/**
 * @brief SHAKE128 algorithm context
 **/

typedef KeccakContext Shake128Context;


//SHAKE128 related constants
extern const uint8_t shake128Oid[9];

//SHAKE128 related functions
error_t shake128Compute(const void *input, size_t inputLen,
   uint8_t *output, size_t outputLen);

void shake128Init(Shake128Context *context);
void shake128Absorb(Shake128Context *context, const void *input, size_t length);
void shake128Final(Shake128Context *context);
void shake128Squeeze(Shake128Context *context, uint8_t *output, size_t length);

//C++ guard
#ifdef __cplusplus
   }
#endif

#endif
