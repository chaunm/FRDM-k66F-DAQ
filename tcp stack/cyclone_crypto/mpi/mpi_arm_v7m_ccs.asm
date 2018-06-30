; @file mpi_arm_v7m_ccs.s
; @brief ARMv7-M (Cortex-M3/M4/M7) assembly routines for TI ARM compiler
;
; @section License
;
; Copyright (C) 2010-2018 Oryx Embedded SARL. All rights reserved.
;
; This file is part of CycloneCrypto Eval.
;
; This software is provided in source form for a short-term evaluation only. The
; evaluation license expires 90 days after the date you first download the software.
;
; If you plan to use this software in a commercial product, you are required to
; purchase a commercial license from Oryx Embedded SARL.
;
; After the 90-day evaluation period, you agree to either purchase a commercial
; license or delete all copies of this software. If you wish to extend the
; evaluation period, you must contact sales@oryx-embedded.com.
;
; This evaluation software is provided "as is" without warranty of any kind.
; Technical support is available as an option during the evaluation period.
;
; @author Oryx Embedded SARL (www.oryx-embedded.com)
; @version 1.8.2

;**********
;* Macros *
;**********

MUL_ACC_CORE   .macro
               mov   r5, #0
               ldr   r6, [r1], #4
               umlal r4, r5, r6, r3
               ldr   r6, [r0]
               adds  r6, r6, r4
               adc   r4, r5, #0
               str   r6, [r0], #4
               .endm

;***********
;* Exports *
;***********

               .global mpiMulAccCore

               .thumb
               .text

;*********************************
;* Multiply-accumulate operation *
;*********************************

mpiMulAccCore:
               push  {r4-r6}
               mov   r4, #0
               cmp   r2, #8
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
               sub   r2, r2, #8
               cmp   r2, #8
               bhs   loop1
next1:
               cmp   r2, #1
               blo   next2
loop2:
               MUL_ACC_CORE
               subs  r2, r2, #1
               bne   loop2
next2:
               cbz   r4, next3
loop3:
               ldr   r6, [r0]
               adds  r6, r6, r4
               str   r6, [r0], #4
               mov   r4, #0
               adcs  r4, r4, #0
               bne   loop3
next3:
               pop   {r4-r6}
               bx    r14

               .end
