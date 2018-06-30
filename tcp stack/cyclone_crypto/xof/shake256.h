/**
 * @file shake256.h
 * @brief SHAKE256 extendable-output function (XOF)
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

#ifndef _SHAKE256_H
#define _SHAKE256_H

//Dependencies
#include "core/crypto.h"
#include "xof/keccak.h"

//C++ guard
#ifdef __cplusplus
   extern "C" {
#endif


/**
 * @brief SHAKE256 algorithm context
 **/

typedef KeccakContext Shake256Context;


//SHAKE256 related constants
extern const uint8_t shake256Oid[9];

//SHAKE256 related functions
error_t shake256Compute(const void *input, size_t inputLen,
   uint8_t *output, size_t outputLen);

void shake256Init(Shake256Context *context);
void shake256Absorb(Shake256Context *context, const void *input, size_t length);
void shake256Final(Shake256Context *context);
void shake256Squeeze(Shake256Context *context, uint8_t *output, size_t length);

//C++ guard
#ifdef __cplusplus
   }
#endif

#endif
