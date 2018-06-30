/**
 * @file chacha20_poly1305.h
 * @brief ChaCha20Poly1305 AEAD
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

#ifndef _CHACHA20_POLY1305_H
#define _CHACHA20_POLY1305_H

//Dependencies
#include "core/crypto.h"

//C++ guard
#ifdef __cplusplus
   extern "C" {
#endif

//ChaCha20Poly1305 related functions
error_t chacha20Poly1305Encrypt(const uint8_t *k, size_t kLen,
   const uint8_t *n, size_t nLen, const uint8_t *a, size_t aLen,
   const uint8_t *p, uint8_t *c, size_t length, uint8_t *t, size_t tLen);

error_t chacha20Poly1305Decrypt(const uint8_t *k, size_t kLen,
   const uint8_t *n, size_t nLen, const uint8_t *a, size_t aLen,
   const uint8_t *c, uint8_t *p, size_t length, const uint8_t *t, size_t tLen);

//C++ guard
#ifdef __cplusplus
   }
#endif

#endif
