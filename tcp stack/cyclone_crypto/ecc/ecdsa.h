/**
 * @file ecdsa.h
 * @brief ECDSA (Elliptic Curve Digital Signature Algorithm)
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

#ifndef _ECDSA_H
#define _ECDSA_H

//Dependencies
#include "core/crypto.h"
#include "ecc/ec.h"

//C++ guard
#ifdef __cplusplus
   extern "C" {
#endif


/**
 * @brief ECDSA signature
 **/

typedef struct
{
   Mpi r;
   Mpi s;
} EcdsaSignature;


//ECDSA related constants
extern const uint8_t ECDSA_WITH_SHA1_OID[7];
extern const uint8_t ECDSA_WITH_SHA224_OID[8];
extern const uint8_t ECDSA_WITH_SHA256_OID[8];
extern const uint8_t ECDSA_WITH_SHA384_OID[8];
extern const uint8_t ECDSA_WITH_SHA512_OID[8];
extern const uint8_t ECDSA_WITH_SHA3_224_OID[9];
extern const uint8_t ECDSA_WITH_SHA3_256_OID[9];
extern const uint8_t ECDSA_WITH_SHA3_384_OID[9];
extern const uint8_t ECDSA_WITH_SHA3_512_OID[9];

//ECDSA related functions
void ecdsaInitSignature(EcdsaSignature *signature);
void ecdsaFreeSignature(EcdsaSignature *signature);

error_t ecdsaWriteSignature(const EcdsaSignature *signature, uint8_t *data,
   size_t *length);

error_t ecdsaReadSignature(const uint8_t *data, size_t length,
   EcdsaSignature *signature);

error_t ecdsaGenerateKeyPair(const EcDomainParameters *params,
   const PrngAlgo *prngAlgo, void *prngContext, Mpi *privateKey,
   EcPoint *publicKey);

error_t ecdsaGenerateSignature(const EcDomainParameters *params,
   const PrngAlgo *prngAlgo, void *prngContext, const Mpi *privateKey,
   const uint8_t *digest, size_t digestLen, EcdsaSignature *signature);

error_t ecdsaVerifySignature(const EcDomainParameters *params,
   const EcPoint *publicKey, const uint8_t *digest, size_t digestLen,
   const EcdsaSignature *signature);

//C++ guard
#ifdef __cplusplus
   }
#endif

#endif
