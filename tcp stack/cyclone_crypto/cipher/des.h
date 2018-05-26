/**
 * @file des.h
 * @brief DES (Data Encryption Standard)
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

#ifndef _DES_H
#define _DES_H

//Dependencies
#include "core/crypto.h"

//DES block size
#define DES_BLOCK_SIZE 8
//Common interface for encryption algorithms
#define DES_CIPHER_ALGO (&desCipherAlgo)

//C++ guard
#ifdef __cplusplus
   extern "C" {
#endif


/**
 * @brief DES algorithm context
 **/

typedef struct
{
   uint32_t ks[32];
} DesContext;


//DES related constants
extern const CipherAlgo desCipherAlgo;

//DES related functions
error_t desInit(DesContext *context, const uint8_t *key, size_t keyLen);
void desEncryptBlock(DesContext *context, const uint8_t *input, uint8_t *output);
void desDecryptBlock(DesContext *context, const uint8_t *input, uint8_t *output);

//C++ guard
#ifdef __cplusplus
   }
#endif

#endif
