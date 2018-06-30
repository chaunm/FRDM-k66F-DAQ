/**
 * @file md2.h
 * @brief MD2 (Message-Digest Algorithm)
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

#ifndef _MD2_H
#define _MD2_H

//Dependencies
#include "core/crypto.h"

//MD2 block size
#define MD2_BLOCK_SIZE 16
//MD2 digest size
#define MD2_DIGEST_SIZE 16
//Common interface for hash algorithms
#define MD2_HASH_ALGO (&md2HashAlgo)

//C++ guard
#ifdef __cplusplus
   extern "C" {
#endif


/**
 * @brief MD2 algorithm context
 **/

typedef struct
{

   union
   {
      uint8_t x[48];
      uint8_t digest[16];
   };
   uint8_t m[16];
   uint8_t c[16];
   size_t size;
} Md2Context;


//MD2 related constants
extern const HashAlgo md2HashAlgo;

//MD2 related functions
error_t md2Compute(const void *data, size_t length, uint8_t *digest);
void md2Init(Md2Context *context);
void md2Update(Md2Context *context, const void *data, size_t length);
void md2Final(Md2Context *context, uint8_t *digest);
void md2ProcessBlock(const uint8_t *m, uint8_t *x, uint8_t *c);

//C++ guard
#ifdef __cplusplus
   }
#endif

#endif
