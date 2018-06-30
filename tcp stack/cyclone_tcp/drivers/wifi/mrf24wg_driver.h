/**
 * @file mrf24wg_driver.h
 * @brief MRF24WG Wi-Fi controller
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

#ifndef _MRF24WG_DRIVER_H
#define _MRF24WG_DRIVER_H

//Dependencies
#include "core/nic.h"

//TX buffer size
#ifndef MRF24WG_TX_BUFFER_SIZE
   #define MRF24WG_TX_BUFFER_SIZE 1536
#elif (MRF24WG_TX_BUFFER_SIZE != 1536)
   #error MRF24WG_TX_BUFFER_SIZE parameter is not valid
#endif

//RX buffer size
#ifndef MRF24WG_RX_BUFFER_SIZE
   #define MRF24WG_RX_BUFFER_SIZE 1536
#elif (MRF24WG_RX_BUFFER_SIZE != 1536)
   #error MRF24WG_RX_BUFFER_SIZE parameter is not valid
#endif

//C++ guard
#ifdef __cplusplus
   extern "C" {
#endif


/**
 * @brief TX buffer
 **/

typedef struct
{
   bool_t used;
   size_t length;
   uint8_t data[MRF24WG_TX_BUFFER_SIZE];
} Mrf24wgBuffer;


//MRF24WG driver
extern const NicDriver mrf24wgDriver;

//MRF24WG related functions
error_t mrf24wgInit(NetInterface *interface);

void mrf24wgTick(NetInterface *interface);

void mrf24wgEnableIrq(NetInterface *interface);
void mrf24wgDisableIrq(NetInterface *interface);
void mrf24wgEventHandler(NetInterface *interface);

error_t mrf24wgSendPacket(NetInterface *interface,
   const NetBuffer *buffer, size_t offset);

error_t mrf24wgSetMulticastFilter(NetInterface *interface);

void mrf24wgAppWifiEvent(uint8_t msgType, void *msg);
void mrf24wgAppEthEvent(uint8_t msgType, void *msg, void *ctrlBuf);

//C++ guard
#ifdef __cplusplus
   }
#endif

#endif
