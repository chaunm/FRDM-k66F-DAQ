/**
 * @file lm3s_eth_driver.h
 * @brief Luminary Stellaris LM3S Ethernet controller
 *
 * @section License
 *
 * Copyright (C) 2010-2018 Oryx Embedded SARL. All rights reserved.
 *
 * This file is part of CycloneTCP Eval.
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

#ifndef _LM3S_ETH_DRIVER_H
#define _LM3S_ETH_DRIVER_H

//Dependencies
#include "core/nic.h"

//Interrupt priority grouping
#ifndef LM3S_ETH_IRQ_PRIORITY_GROUPING
   #define LM3S_ETH_IRQ_PRIORITY_GROUPING 3
#elif (LM3S_ETH_IRQ_PRIORITY_GROUPING < 0)
   #error LM3S_ETH_IRQ_PRIORITY_GROUPING parameter is not valid
#endif

//Ethernet interrupt priority
#ifndef LM3S_ETH_IRQ_PRIORITY
   #define LM3S_ETH_IRQ_PRIORITY 192
#elif (LM3S_ETH_IRQ_PRIORITY < 0)
   #error LM3S_ETH_IRQ_PRIORITY parameter is not valid
#endif

//C++ guard
#ifdef __cplusplus
   extern "C" {
#endif

//Stellaris LM3S Ethernet driver
extern const NicDriver lm3sEthDriver;

//Stellaris LM3S Ethernet related functions
error_t lm3sEthInit(NetInterface *interface);
void lm3sEthInitGpio(NetInterface *interface);

void lm3sEthTick(NetInterface *interface);

void lm3sEthEnableIrq(NetInterface *interface);
void lm3sEthDisableIrq(NetInterface *interface);
void lm3sEthEventHandler(NetInterface *interface);

error_t lm3sEthSendPacket(NetInterface *interface,
   const NetBuffer *buffer, size_t offset);

error_t lm3sEthReceivePacket(NetInterface *interface);

error_t lm3sEthSetMulticastFilter(NetInterface *interface);

void lm3sEthWritePhyReg(uint8_t address, uint16_t data);
uint16_t lm3sEthReadPhyReg(uint8_t address);

void lm3sEthDumpPhyReg(void);

//C++ guard
#ifdef __cplusplus
   }
#endif

#endif
