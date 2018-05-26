/**
 * @file bcm43362_driver.h
 * @brief BCM43362 Wi-Fi controller
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

#ifndef _BCM43362_DRIVER_H
#define _BCM43362_DRIVER_H

//Dependencies
#include "core/nic.h"

//C++ guard
#ifdef __cplusplus
   extern "C" {
#endif

//BCM43362 driver (STA mode)
extern const NicDriver bcm43362StaDriver;
//BCM43362 driver (AP mode)
extern const NicDriver bcm43362ApDriver;

//BCM43362 related functions
error_t bcm43362Init(NetInterface *interface);

void bcm43362Tick(NetInterface *interface);

void bcm43362EnableIrq(NetInterface *interface);
void bcm43362DisableIrq(NetInterface *interface);
bool_t bcm43362IrqHandler(void);
void bcm43362EventHandler(NetInterface *interface);

error_t bcm43362SendPacket(NetInterface *interface,
   const NetBuffer *buffer, size_t offset);

error_t bcm43362SetMulticastFilter(NetInterface *interface);

void bcm43362AppWifiEvent(uint8_t msgType, void *msg);
void bcm43362AppEthEvent(uint8_t msgType, void *msg, void *ctrlBuf);

//C++ guard
#ifdef __cplusplus
   }
#endif

#endif
