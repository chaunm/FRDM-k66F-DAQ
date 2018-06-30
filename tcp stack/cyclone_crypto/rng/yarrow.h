/**
 * @file yarrow.h
 * @brief Yarrow PRNG
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

#ifndef _YARROW_H
#define _YARROW_H

//Dependencies
#include "core/crypto.h"
#include "cipher/aes.h"
#include "hash/sha256.h"

//Common interface for PRNG algorithms
#define YARROW_PRNG_ALGO (&yarrowPrngAlgo)

//Pool identifiers
#define YARROW_FAST_POOL_ID 0
#define YARROW_SLOW_POOL_ID 1

//Yarrow PRNG parameters
#define YARROW_N 3
#define YARROW_K 2
#define YARROW_PG 10
#define YARROW_FAST_THRESHOLD 100
#define YARROW_SLOW_THRESHOLD 160

//C++ guard
#ifdef __cplusplus
   extern "C" {
#endif


/**
 * @brief Yarrow PRNG context
 **/

typedef struct
{
   OsMutex mutex;                    //Mutex to prevent simultaneous access to the PRNG state
   bool_t ready;                     //This flag tells whether the PRNG has been properly seeded
   uint_t currentPool[YARROW_N];     //Current pool identifier
   Sha256Context fastPool;           //Fast pool
   size_t fastPoolEntropy[YARROW_N]; //Entropy estimation (fast pool)
   Sha256Context slowPool;           //Slow pool
   size_t slowPoolEntropy[YARROW_N]; //Entropy estimation (slow pool)
   AesContext cipherContext;         //Cipher context
   uint8_t key[32];                  //Current key
   uint8_t counter[16];              //Counter block
   size_t blockCount;                //Number of blocks that have been generated
} YarrowContext;


//Yarrow related constants
extern const PrngAlgo yarrowPrngAlgo;

//Yarrow related functions
error_t yarrowInit(YarrowContext *context);
void yarrowRelease(YarrowContext *context);

error_t yarrowSeed(YarrowContext *context, const uint8_t *input, size_t length);

error_t yarrowAddEntropy(YarrowContext *context, uint_t source,
   const uint8_t *input, size_t length, size_t entropy);

error_t yarrowRead(YarrowContext *context, uint8_t *output, size_t length);

void yarrowGenerateBlock(YarrowContext *context, uint8_t *output);
void yarrowFastReseed(YarrowContext *context);
void yarrowSlowReseed(YarrowContext *context);

//C++ guard
#ifdef __cplusplus
   }
#endif

#endif
