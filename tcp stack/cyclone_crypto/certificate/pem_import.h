/**
 * @file pem_import.h
 * @brief PEM file import functions
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

#ifndef _PEM_IMPORT_H
#define _PEM_IMPORT_H

//Dependencies
#include "core/crypto.h"
#include "pkc/dh.h"
#include "pkc/rsa.h"
#include "pkc/dsa.h"
#include "ecc/ec.h"

//C++ guard
#ifdef __cplusplus
   extern "C" {
#endif

//PEM format decoding functions
error_t pemImportDhParameters(const char_t *input, size_t length, DhParameters *params);

error_t pemImportRsaPrivateKey(const char_t *input, size_t length, RsaPrivateKey *key);
error_t pemImportDsaPrivateKey(const char_t *input, size_t length, DsaPrivateKey *key);

error_t pemImportEcParameters(const char_t *input, size_t length, EcDomainParameters *params);
error_t pemImportEcPrivateKey(const char_t *input, size_t length, Mpi *key);

error_t pemImportCertificate(const char_t **input, size_t *inputLen,
   uint8_t **output, size_t *outputSize, size_t *outputLen);

int_t pemSearchTag(const char_t *s, size_t sLen, const char_t *tag, size_t tagLen);

//C++ guard
#ifdef __cplusplus
   }
#endif

#endif
