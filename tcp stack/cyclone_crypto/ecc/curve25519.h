/**
 * @file curve25519.h
 * @brief Curve25519 elliptic curve
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

#ifndef _CURVE25519_H
#define _CURVE25519_H

//Dependencies
#include "core/crypto.h"

//Length of the elliptic curve
#define CURVE25519_BIT_LEN 255
#define CURVE25519_BYTE_LEN 32
#define CURVE25519_WORD_LEN 8

//A24 constant
#define CURVE25519_A24 121666

//C++ guard
#ifdef __cplusplus
   extern "C" {
#endif

//Curve25519 related functions
void curve25519SetInt(uint32_t *a, uint32_t b);
void curve25519Add(uint32_t *r, const uint32_t *a, const uint32_t *b);
void curve25519AddInt(uint32_t *r, const uint32_t *a, uint32_t b);
void curve25519Sub(uint32_t *r, const uint32_t *a, const uint32_t *b);
void curve25519SubInt(uint32_t *r, const uint32_t *a, uint32_t b);
void curve25519Mul(uint32_t *r, const uint32_t *a, const uint32_t *b);
void curve25519MulInt(uint32_t *r, const uint32_t *a, uint32_t b);
void curve25519Red(uint32_t *r, const uint32_t *a);
void curve25519Sqr(uint32_t *r, const uint32_t *a);
void curve25519Inv(uint32_t *r, const uint32_t *a);

uint32_t curve25519Sqrt(uint32_t *r, const uint32_t *a, const uint32_t *b);

void curve25519Copy(uint32_t *a, const uint32_t *b);
void curve25519Swap(uint32_t *a, uint32_t *b, uint32_t c);

void curve25519Select(uint32_t *r, const uint32_t *a, const uint32_t *b,
   uint32_t c);

uint32_t curve25519Comp(const uint32_t *a, const uint32_t *b);

void curve25519Import(uint32_t *a, const uint8_t *data);
void curve25519Export(uint32_t *a, uint8_t *data);

//C++ guard
#ifdef __cplusplus
   }
#endif

#endif
