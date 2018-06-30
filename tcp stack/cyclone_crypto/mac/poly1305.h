/**
 * @file poly1305.h
 * @brief Poly1305 message-authentication code
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

#ifndef _POLY1305_H
#define _POLY1305_H

//Dependencies
#include "core/crypto.h"

//C++ guard
#ifdef __cplusplus
   extern "C" {
#endif


/**
 * @brief Poly1305 context
 **/

typedef struct
{
   uint32_t r[4];
   uint32_t s[4];
   uint64_t a[8];
   uint8_t buffer[17];
   size_t size;
} Poly1305Context;


//Poly1305 related functions
void poly1305Init(Poly1305Context *context, const uint8_t *key);
void poly1305Update(Poly1305Context *context, const void *data, size_t length);
void poly1305Final(Poly1305Context *context, uint8_t *tag);

void poly1305ProcessBlock(Poly1305Context *context);

//C++ guard
#ifdef __cplusplus
   }
#endif

#endif
