/**
 * @file winc1500_driver.h
 * @brief WINC1500 Wi-Fi controller
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

#ifndef _WINC1500_DRIVER_H
#define _WINC1500_DRIVER_H

//Dependencies
#include "core/nic.h"

//TX buffer size
#ifndef WINC1500_TX_BUFFER_SIZE
   #define WINC1500_TX_BUFFER_SIZE 1600
#elif (WINC1500_TX_BUFFER_SIZE != 1600)
   #error WINC1500_TX_BUFFER_SIZE parameter is not valid
#endif

//RX buffer size
#ifndef WINC1500_RX_BUFFER_SIZE
   #define WINC1500_RX_BUFFER_SIZE 1600
#elif (WINC1500_RX_BUFFER_SIZE != 1600)
   #error WINC1500_RX_BUFFER_SIZE parameter is not valid
#endif

//C++ guard
#ifdef __cplusplus
   extern "C" {
#endif

//WINC1500 driver
extern const NicDriver winc1500Driver;

//WINC1500 related functions
error_t winc1500Init(NetInterface *interface);

void winc1500Tick(NetInterface *interface);

void winc1500EnableIrq(NetInterface *interface);
void winc1500DisableIrq(NetInterface *interface);
bool_t winc1500IrqHandler(void);
void winc1500EventHandler(NetInterface *interface);

error_t winc1500SendPacket(NetInterface *interface,
   const NetBuffer *buffer, size_t offset);

error_t winc1500SetMulticastFilter(NetInterface *interface);

void winc1500AppWifiEvent(uint8_t msgType, void *msg);
void winc1500AppEthEvent(uint8_t msgType, void *msg, void *ctrlBuf);

//C++ guard
#ifdef __cplusplus
   }
#endif

#endif
