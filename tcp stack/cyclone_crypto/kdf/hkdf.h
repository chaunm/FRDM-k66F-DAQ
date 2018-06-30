/**
 * @file hkdf.h
 * @brief HKDF (HMAC-based Key Derivation Function)
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

#ifndef _HKDF_H
#define _HKDF_H

//Dependencies
#include "core/crypto.h"

//C++ guard
#ifdef __cplusplus
   extern "C" {
#endif

//HKDF related functions
error_t hkdf(const HashAlgo *hash, const uint8_t *ikm, size_t ikmLen,
   const uint8_t *salt, size_t saltLen, const uint8_t *info, size_t infoLen,
   uint8_t *okm, size_t okmLen);

error_t hkdfExtract(const HashAlgo *hash, const uint8_t *ikm, size_t ikmLen,
   const uint8_t *salt, size_t saltLen, uint8_t *prk);

error_t hkdfExpand(const HashAlgo *hash, const uint8_t *prk, size_t prkLen,
   const uint8_t *info, size_t infoLen, uint8_t *okm, size_t okmLen);

//C++ guard
#ifdef __cplusplus
   }
#endif

#endif
