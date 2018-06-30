/**
 * @file wilc1000_driver.h
 * @brief WILC1000 Wi-Fi controller
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

#ifndef _WILC1000_DRIVER_H
#define _WILC1000_DRIVER_H

//Dependencies
#include "core/nic.h"

//TX buffer size
#ifndef WILC1000_TX_BUFFER_SIZE
   #define WILC1000_TX_BUFFER_SIZE 1600
#elif (WILC1000_TX_BUFFER_SIZE != 1600)
   #error WILC1000_TX_BUFFER_SIZE parameter is not valid
#endif

//RX buffer size
#ifndef WILC1000_RX_BUFFER_SIZE
   #define WILC1000_RX_BUFFER_SIZE 1600
#elif (WILC1000_RX_BUFFER_SIZE != 1600)
   #error WILC1000_RX_BUFFER_SIZE parameter is not valid
#endif

//C++ guard
#ifdef __cplusplus
   extern "C" {
#endif

//WILC1000 driver (STA mode)
extern const NicDriver wilc1000StaDriver;
//WILC1000 driver (AP mode)
extern const NicDriver wilc1000ApDriver;

//WILC1000 related functions
error_t wilc1000Init(NetInterface *interface);

void wilc1000Tick(NetInterface *interface);

void wilc1000EnableIrq(NetInterface *interface);
void wilc1000DisableIrq(NetInterface *interface);
bool_t wilc1000IrqHandler(void);
void wilc1000EventHandler(NetInterface *interface);

error_t wilc1000SendPacket(NetInterface *interface,
   const NetBuffer *buffer, size_t offset);

error_t wilc1000SetMulticastFilter(NetInterface *interface);
bool_t wilc1000GetAddrRefCount(NetInterface *interface, const MacAddr *macAddr);

void wilc1000AppWifiEvent(uint8_t msgType, void *msg);
void wilc1000AppEthEvent(uint8_t msgType, void *msg, void *ctrlBuf);

//C++ guard
#ifdef __cplusplus
   }
#endif

#endif
