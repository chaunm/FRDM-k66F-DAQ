/**
 * @file curve448.h
 * @brief Curve448 elliptic curve
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

#ifndef _CURVE448_H
#define _CURVE448_H

//Dependencies
#include "core/crypto.h"

//Length of the elliptic curve
#define CURVE448_BIT_LEN 448
#define CURVE448_BYTE_LEN 56
#define CURVE448_WORD_LEN 14

//A24 constant
#define CURVE448_A24 39082

//C++ guard
#ifdef __cplusplus
   extern "C" {
#endif

//Curve448 related functions
void curve448SetInt(uint32_t *a, uint32_t b);
void curve448Add(uint32_t *r, const uint32_t *a, const uint32_t *b);
void curve448AddInt(uint32_t *r, const uint32_t *a, uint32_t b);
void curve448Sub(uint32_t *r, const uint32_t *a, const uint32_t *b);
void curve448SubInt(uint32_t *r, const uint32_t *a, uint32_t b);
void curve448Mul(uint32_t *r, const uint32_t *a, const uint32_t *b);
void curve448MulInt(uint32_t *r, const uint32_t *a, uint32_t b);
void curve448Red(uint32_t *r, const uint32_t *a, uint32_t h);
void curve448Sqr(uint32_t *r, const uint32_t *a);
void curve448Inv(uint32_t *r, const uint32_t *a);

uint32_t curve448Sqrt(uint32_t *r, const uint32_t *a, const uint32_t *b);

void curve448Copy(uint32_t *a, const uint32_t *b);
void curve448Swap(uint32_t *a, uint32_t *b, uint32_t c);

void curve448Select(uint32_t *r, const uint32_t *a, const uint32_t *b,
   uint32_t c);

uint32_t curve448Comp(const uint32_t *a, const uint32_t *b);

void curve448Import(uint32_t *a, const uint8_t *data);
void curve448Export(uint32_t *a, uint8_t *data);

//C++ guard
#ifdef __cplusplus
   }
#endif

#endif
