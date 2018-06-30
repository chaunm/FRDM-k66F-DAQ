/**
 * @file rsa.h
 * @brief RSA public-key cryptography standard
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

#ifndef _RSA_H
#define _RSA_H

//Dependencies
#include "core/crypto.h"
#include "mpi/mpi.h"

//C++ guard
#ifdef __cplusplus
   extern "C" {
#endif


/**
 * @brief RSA public key
 **/

typedef struct
{
   Mpi n; ///<Modulus
   Mpi e; ///<Public exponent
} RsaPublicKey;


/**
 * @brief RSA private key
 **/

typedef struct
{
   Mpi n;    ///<Modulus
   Mpi e;    ///<Public exponent
   Mpi d;    ///<Private exponent
   Mpi p;    ///<First factor
   Mpi q;    ///<Second factor
   Mpi dp;   ///<First factor's CRT exponent
   Mpi dq;   ///<second factor's CRT exponent
   Mpi qinv; ///<CRT coefficient
} RsaPrivateKey;


//RSA related constants
extern const uint8_t PKCS1_OID[8];
extern const uint8_t RSA_ENCRYPTION_OID[9];
extern const uint8_t MD5_WITH_RSA_ENCRYPTION_OID[9];
extern const uint8_t SHA1_WITH_RSA_ENCRYPTION_OID[9];
extern const uint8_t SHA256_WITH_RSA_ENCRYPTION_OID[9];
extern const uint8_t SHA384_WITH_RSA_ENCRYPTION_OID[9];
extern const uint8_t SHA512_WITH_RSA_ENCRYPTION_OID[9];
extern const uint8_t RSASSA_PKCS1_V1_5_WITH_SHA3_224_OID[9];
extern const uint8_t RSASSA_PKCS1_V1_5_WITH_SHA3_256_OID[9];
extern const uint8_t RSASSA_PKCS1_V1_5_WITH_SHA3_384_OID[9];
extern const uint8_t RSASSA_PKCS1_V1_5_WITH_SHA3_512_OID[9];

//RSA related functions
void rsaInitPublicKey(RsaPublicKey *key);
void rsaFreePublicKey(RsaPublicKey *key);
void rsaInitPrivateKey(RsaPrivateKey *key);
void rsaFreePrivateKey(RsaPrivateKey *key);

error_t rsaep(const RsaPublicKey *key, const Mpi *m, Mpi *c);
error_t rsadp(const RsaPrivateKey *key, const Mpi *c, Mpi *m);

error_t rsasp1(const RsaPrivateKey *key, const Mpi *m, Mpi *s);
error_t rsavp1(const RsaPublicKey *key, const Mpi *s, Mpi *m);

error_t rsaesPkcs1v15Encrypt(const PrngAlgo *prngAlgo, void *prngContext, const RsaPublicKey *key,
   const uint8_t *message, size_t messageLen, uint8_t *ciphertext, size_t *ciphertextLen);

error_t rsaesPkcs1v15Decrypt(const RsaPrivateKey *key, const uint8_t *ciphertext,
   size_t ciphertextLen, uint8_t *message, size_t messageSize, size_t *messageLen);

error_t rsassaPkcs1v15Sign(const RsaPrivateKey *key, const HashAlgo *hash,
   const uint8_t *digest, uint8_t *signature, size_t *signatureLen);

error_t rsassaPkcs1v15Verify(const RsaPublicKey *key, const HashAlgo *hash,
   const uint8_t *digest, const uint8_t *signature, size_t signatureLen);

error_t emsaPkcs1v15Encode(const HashAlgo *hash,
   const uint8_t *digest, uint8_t *em, size_t emLen);

error_t emsaPkcs1v15Decode(const uint8_t *em, size_t emLen, const uint8_t **oid,
   size_t *oidLen, const uint8_t **digest, size_t *digestLen);

//C++ guard
#ifdef __cplusplus
   }
#endif

#endif
