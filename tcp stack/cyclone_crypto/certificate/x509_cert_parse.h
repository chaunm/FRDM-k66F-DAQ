/**
 * @file x509_cert_parse.h
 * @brief X.509 certificate parsing
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

#ifndef _X509_CERT_PARSE_H
#define _X509_CERT_PARSE_H

//Dependencies
#include "core/crypto.h"
#include "certificate/x509_common.h"

//C++ guard
#ifdef __cplusplus
   extern "C" {
#endif

//X.509 related functions
error_t x509ParseCertificate(const uint8_t *data, size_t length,
   X509CertificateInfo *certInfo);

error_t x509ParseTbsCertificate(const uint8_t *data, size_t length,
   size_t *totalLength, X509CertificateInfo *certInfo);

error_t x509ParseVersion(const uint8_t *data, size_t length,
   size_t *totalLength, X509Version *version);

error_t x509ParseSerialNumber(const uint8_t *data, size_t length,
   size_t *totalLength, X509SerialNumber *serialNumber);

error_t x509ParseSignature(const uint8_t *data, size_t length,
   size_t *totalLength, X509SignatureId *signature);

error_t x509ParseName(const uint8_t *data, size_t length,
   size_t *totalLength, X509Name *name);

error_t x509ParseNameAttribute(const uint8_t *data, size_t length,
   size_t *totalLength, X509NameAttribute *nameAttribute);

error_t x509ParseValidity(const uint8_t *data, size_t length,
   size_t *totalLength, X509Validity *validity);

error_t x509ParseTime(const uint8_t *data, size_t length,
   size_t *totalLength, DateTime *dateTime);

error_t x509ParseSubjectPublicKeyInfo(const uint8_t *data, size_t length,
   size_t *totalLength, X509SubjectPublicKeyInfo *subjectPublicKeyInfo);

error_t x509ParseAlgorithmIdentifier(const uint8_t *data, size_t length,
   size_t *totalLength, X509SubjectPublicKeyInfo *subjectPublicKeyInfo);

error_t x509ParseRsaPublicKey(const uint8_t *data, size_t length,
   X509RsaPublicKey *rsaPublicKey);

error_t x509ParseDsaParameters(const uint8_t *data, size_t length,
   X509DsaParameters *dsaParams);

error_t x509ParseDsaPublicKey(const uint8_t *data, size_t length,
   X509DsaPublicKey *dsaPublicKey);

error_t x509ParseEcParameters(const uint8_t *data, size_t length,
   X509EcParameters *ecParams);

error_t x509ParseEcPublicKey(const uint8_t *data, size_t length,
   X509EcPublicKey *ecPublicKey);

error_t x509ParseIssuerUniqueId(const uint8_t *data, size_t length,
   size_t *totalLength);

error_t x509ParseSubjectUniqueId(const uint8_t *data, size_t length,
   size_t *totalLength);

error_t x509ParseExtensions(const uint8_t *data, size_t length,
   size_t *totalLength, X509Extensions *extensions);

error_t x509ParseBasicConstraints(const uint8_t *data, size_t length,
   X509BasicConstraints *basicConstraints);

error_t x509ParseNameConstraints(const uint8_t *data, size_t length,
   X509NameConstraints *nameConstraints);

error_t x509ParsePolicyConstraints(const uint8_t *data, size_t length);
error_t x509ParsePolicyMappings(const uint8_t *data, size_t length);
error_t x509ParseInhibitAnyPolicy(const uint8_t *data, size_t length);

error_t x509ParseKeyUsage(const uint8_t *data, size_t length,
   uint16_t *keyUsage);

error_t x509ParseExtendedKeyUsage(const uint8_t *data, size_t length,
   uint8_t *extKeyUsage);

error_t x509ParseSubjectAltName(const uint8_t *data, size_t length,
   X509SubjectAltName *subjectAltName);

error_t x509ParseGeneralSubtrees(const uint8_t *data, size_t length);

error_t x509ParseGeneralSubtree(const uint8_t *data, size_t length,
   size_t *totalLength, X509GeneralName *generalName);

error_t x509ParseGeneralName(const uint8_t *data, size_t length,
   size_t *totalLength, X509GeneralName *generalName);

error_t x509ParseSubjectKeyId(const uint8_t *data, size_t length,
   X509SubjectKeyId *subjectKeyId);

error_t x509ParseAuthorityKeyId(const uint8_t *data, size_t length,
   X509AuthorityKeyId *authorityKeyId);

error_t x509ParseNsCertType(const uint8_t *data, size_t length,
   uint8_t *nsCertType);

error_t x509ParseSignatureAlgo(const uint8_t *data, size_t length,
   size_t *totalLength, X509SignatureId *signatureAlgo);

error_t x509ParseSignatureValue(const uint8_t *data, size_t length,
   size_t *totalLength, X509SignatureValue *signatureValue);

//C++ guard
#ifdef __cplusplus
   }
#endif

#endif
