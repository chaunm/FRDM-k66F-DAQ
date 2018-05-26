/**
 * @file des3.h
 * @brief Triple DES (Triple Data Encryption Algorithm)
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

#ifndef _DES3_H
#define _DES3_H

//Dependencies
#include "core/crypto.h"
#include "cipher/des.h"

//Triple DES block size
#define DES3_BLOCK_SIZE 8
//Common interface for encryption algorithms
#define DES3_CIPHER_ALGO (&des3CipherAlgo)

//C++ guard
#ifdef __cplusplus
   extern "C" {
#endif


/**
 * @brief Triple DES algorithm context
 **/

typedef struct
{
   DesContext k1;
   DesContext k2;
   DesContext k3;
} Des3Context;


//Triple DES related constants
extern const CipherAlgo des3CipherAlgo;

//Triple DES related functions
error_t des3Init(Des3Context *context, const uint8_t *key, size_t keyLen);
void des3EncryptBlock(Des3Context *context, const uint8_t *input, uint8_t *output);
void des3DecryptBlock(Des3Context *context, const uint8_t *input, uint8_t *output);

//C++ guard
#ifdef __cplusplus
   }
#endif

#endif
