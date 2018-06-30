/**
 * @file mpi_arm_v7a_gcc.S
 * @brief ARMv7-A (Cortex-A5/A8/A9) assembly routines for GCC compiler
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

/*
 * Macros
 */

.macro         MUL_ACC_CORE
               mov   r5, #0
               ldr   r6, [r1], #4
               umlal r4, r5, r6, r3
               ldr   r6, [r0]
               adds  r6, r6, r4
               adc   r4, r5, #0
               str   r6, [r0], #4
.endm

/*
 * Exports
 */

.global mpiMulAccCore

.syntax unified
.cpu cortex-a9
.arm
.text

/*
 * Multiply-accumulate operation
 */

mpiMulAccCore:
               push  {r4-r6}
               mov   r4, #0
               cmp   r2, #16
               blo   next1
loop1:
               MUL_ACC_CORE
               MUL_ACC_CORE
               MUL_ACC_CORE
               MUL_ACC_CORE
               MUL_ACC_CORE
               MUL_ACC_CORE
               MUL_ACC_CORE
               MUL_ACC_CORE
               MUL_ACC_CORE
               MUL_ACC_CORE
               MUL_ACC_CORE
               MUL_ACC_CORE
               MUL_ACC_CORE
               MUL_ACC_CORE
               MUL_ACC_CORE
               MUL_ACC_CORE
               sub   r2, r2, #16
               cmp   r2, #16
               bhs   loop1
next1:
               cmp   r2, #8
               blo   next2
               MUL_ACC_CORE
               MUL_ACC_CORE
               MUL_ACC_CORE
               MUL_ACC_CORE
               MUL_ACC_CORE
               MUL_ACC_CORE
               MUL_ACC_CORE
               MUL_ACC_CORE
               sub   r2, r2, #8
next2:
               cmp   r2, #4
               blo   next3
               MUL_ACC_CORE
               MUL_ACC_CORE
               MUL_ACC_CORE
               MUL_ACC_CORE
               sub   r2, r2, #4
next3:
               cmp   r2, #2
               blo   next4
               MUL_ACC_CORE
               MUL_ACC_CORE
               sub   r2, r2, #2
next4:
               cmp   r2, #1
               blo   next5
               MUL_ACC_CORE
next5:
               cmp   r4, #0
               beq   next6
loop2:
               ldr   r6, [r0]
               adds  r6, r6, r4
               str   r6, [r0], #4
               mov   r4, #0
               adcs  r4, r4, #0
               bne   loop2
next6:
               pop   {r4-r6}
               bx    r14

.end
