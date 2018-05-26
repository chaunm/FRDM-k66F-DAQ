/**
 * @file dsa.h
 * @brief DSA (Digital Signature Algorithm)
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

#ifndef _DSA_H
#define _DSA_H

//Dependencies
#include "core/crypto.h"
#include "mpi/mpi.h"

//C++ guard
#ifdef __cplusplus
   extern "C" {
#endif


/**
 * @brief DSA public key
 **/

typedef struct
{
   Mpi p; ///<Prime modulus
   Mpi q; ///<Prime divisor
   Mpi g; ///<Generator of the subgroup
   Mpi y; ///<Public key
} DsaPublicKey;


/**
 * @brief DSA private key
 **/

typedef struct
{
   Mpi p; ///<Prime modulus
   Mpi q; ///<<Prime divisor
   Mpi g; ///<Generator of the subgroup
   Mpi x; ///<Private key
} DsaPrivateKey;


/**
 * @brief DSA signature
 **/

typedef struct
{
   Mpi r;
   Mpi s;
} DsaSignature;


//DSA related constants
extern const uint8_t DSA_OID[7];
extern const uint8_t DSA_WITH_SHA1_OID[7];
extern const uint8_t DSA_WITH_SHA224_OID[9];
extern const uint8_t DSA_WITH_SHA256_OID[9];
extern const uint8_t DSA_WITH_SHA384_OID[9];
extern const uint8_t DSA_WITH_SHA512_OID[9];
extern const uint8_t DSA_WITH_SHA3_224_OID[9];
extern const uint8_t DSA_WITH_SHA3_256_OID[9];
extern const uint8_t DSA_WITH_SHA3_384_OID[9];
extern const uint8_t DSA_WITH_SHA3_512_OID[9];

//DSA related functions
void dsaInitPublicKey(DsaPublicKey *key);
void dsaFreePublicKey(DsaPublicKey *key);

void dsaInitPrivateKey(DsaPrivateKey *key);
void dsaFreePrivateKey(DsaPrivateKey *key);

void dsaInitSignature(DsaSignature *signature);
void dsaFreeSignature(DsaSignature *signature);

error_t dsaWriteSignature(const DsaSignature *signature, uint8_t *data, size_t *length);
error_t dsaReadSignature(const uint8_t *data, size_t length, DsaSignature *signature);

error_t dsaGenerateSignature(const PrngAlgo *prngAlgo, void *prngContext,
   const DsaPrivateKey *key, const uint8_t *digest, size_t digestLen,
   DsaSignature *signature);

error_t dsaVerifySignature(const DsaPublicKey *key,
   const uint8_t *digest, size_t digestLen, const DsaSignature *signature);

//C++ guard
#ifdef __cplusplus
   }
#endif

#endif
