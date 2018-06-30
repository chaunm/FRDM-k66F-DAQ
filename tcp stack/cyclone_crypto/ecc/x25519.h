/**
 * @file x25519.h
 * @brief X25519 function implementation
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

#ifndef _X25519_H
#define _X25519_H

//Dependencies
#include "core/crypto.h"
#include "ecc/curve25519.h"

//C++ guard
#ifdef __cplusplus
   extern "C" {
#endif


/**
 * @brief X25519 working context
 **/

typedef struct
{
   uint32_t k[8];
   uint32_t u[8];
   uint32_t x1[8];
   uint32_t z1[8];
   uint32_t x2[8];
   uint32_t z2[8];
   uint32_t t1[8];
   uint32_t t2[8];
} X25519Context;


//X25519 related functions
error_t x25519(uint8_t *r, const uint8_t *k, const uint8_t *u);

//C++ guard
#ifdef __cplusplus
   }
#endif

#endif
