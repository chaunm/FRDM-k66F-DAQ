/**
 * @file dh.h
 * @brief Diffie-Hellman key exchange
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

#ifndef _DH_H
#define _DH_H

//Dependencies
#include "core/crypto.h"
#include "mpi/mpi.h"

//C++ guard
#ifdef __cplusplus
   extern "C" {
#endif


/**
 * @brief Diffie-Hellman parameters
 **/

typedef struct
{
   Mpi p; ///<Prime modulus
   Mpi g; ///<Generator
} DhParameters;


/**
 * @brief Diffie-Hellman context
 **/

typedef struct
{
   DhParameters params; //Diffie-Hellman parameters
   Mpi xa;              ///<One's own private value
   Mpi ya;              ///<One's own public value
   Mpi yb;              ///<Peer's public value
} DhContext;


//Diffie-Hellman related functions
void dhInit(DhContext *context);
void dhFree(DhContext *context);

error_t dhGenerateKeyPair(DhContext *context,
   const PrngAlgo *prngAlgo, void *prngContext);

error_t dhCheckPublicKey(DhParameters *params, const Mpi *publicKey);

error_t dhComputeSharedSecret(DhContext *context,
   uint8_t *output, size_t outputSize, size_t *outputLen);

//C++ guard
#ifdef __cplusplus
   }
#endif

#endif
