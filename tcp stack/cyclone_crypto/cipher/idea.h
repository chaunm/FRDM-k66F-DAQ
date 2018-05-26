/**
 * @file idea.h
 * @brief IDEA encryption algorithm
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

#ifndef _IDEA_H
#define _IDEA_H

//Dependencies
#include "core/crypto.h"

//IDEA block size
#define IDEA_BLOCK_SIZE 8
//Common interface for encryption algorithms
#define IDEA_CIPHER_ALGO (&ideaCipherAlgo)

//C++ guard
#ifdef __cplusplus
   extern "C" {
#endif


/**
 * @brief IDEA algorithm context
 **/

typedef struct
{
   uint16_t ek[52];
   uint16_t dk[52];
} IdeaContext;


//IDEA related constants
extern const CipherAlgo ideaCipherAlgo;

//IDEA related functions
error_t ideaInit(IdeaContext *context, const uint8_t *key, size_t keyLen);
void ideaEncryptBlock(IdeaContext *context, const uint8_t *input, uint8_t *output);
void ideaDecryptBlock(IdeaContext *context, const uint8_t *input, uint8_t *output);

//C++ guard
#ifdef __cplusplus
   }
#endif

#endif
