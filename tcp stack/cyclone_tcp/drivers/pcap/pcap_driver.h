/**
 * @file pcap_driver.h
 * @brief PCAP driver
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

#ifndef _PCAP_DRIVER_H
#define _PCAP_DRIVER_H

//Dependencies
#include "core/nic.h"

//Maximum packet size
#ifndef PCAP_DRIVER_MAX_PACKET_SIZE
   #define PCAP_DRIVER_MAX_PACKET_SIZE 1536
#elif (PCAP_DRIVER_MAX_PACKET_SIZE < 1)
   #error PCAP_DRIVER_MAX_PACKET_SIZE parameter is not valid
#endif

//Maximum number of packets in the receive queue
#ifndef PCAP_DRIVER_QUEUE_SIZE
   #define PCAP_DRIVER_QUEUE_SIZE 64
#elif (PCAP_DRIVER_QUEUE_SIZE < 1)
   #error PCAP_DRIVER_QUEUE_SIZE parameter is not valid
#endif

//Receive timeout in milliseconds
#ifndef PCAP_DRIVER_TIMEOUT
   #define PCAP_DRIVER_TIMEOUT 1
#elif (PCAP_DRIVER_TIMEOUT < 1)
   #error PCAP_DRIVER_TIMEOUT parameter is not valid
#endif

//C++ guard
#ifdef __cplusplus
   extern "C" {
#endif

//PCAP driver
extern const NicDriver pcapDriver;

//PCAP related functions
error_t pcapDriverInit(NetInterface *interface);

void pcapDriverTick(NetInterface *interface);

void pcapDriverEnableIrq(NetInterface *interface);
void pcapDriverDisableIrq(NetInterface *interface);

void pcapDriverEventHandler(NetInterface *interface);

error_t pcapDriverSendPacket(NetInterface *interface,
   const NetBuffer *buffer, size_t offset);

error_t pcapDriverSetMulticastFilter(NetInterface *interface);

void pcapDriverTask(NetInterface *interface);

//C++ guard
#ifdef __cplusplus
   }
#endif

#endif
