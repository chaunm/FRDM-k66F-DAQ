/**
 * @file base64.h
 * @brief Base64 encoding scheme
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

#ifndef _BASE64_H
#define _BASE64_H

//Dependencies
#include "core/crypto.h"

//C++ guard
#ifdef __cplusplus
   extern "C" {
#endif

//Base64 encoding related functions
void base64Encode(const void *input, size_t inputLen,
   char_t *output, size_t *outputLen);

error_t base64Decode(const char_t *input, size_t inputLen,
   void *output, size_t *outputLen);

//C++ guard
#ifdef __cplusplus
   }
#endif

#endif
