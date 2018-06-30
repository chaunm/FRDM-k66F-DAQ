/**
 * @file udp_mib_module.h
 * @brief UDP MIB module
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

#ifndef _UDP_MIB_MODULE_H
#define _UDP_MIB_MODULE_H

//Dependencies
#include "mibs/mib_common.h"

//UDP MIB module support
#ifndef UDP_MIB_SUPPORT
   #define UDP_MIB_SUPPORT DISABLED
#elif (UDP_MIB_SUPPORT != ENABLED && UDP_MIB_SUPPORT != DISABLED)
   #error UDP_MIB_SUPPORT parameter is not valid
#endif

//Macro definitions
#if (UDP_MIB_SUPPORT == ENABLED)
   #define UDP_MIB_INC_COUNTER32(name, value) udpMibBase.name += value
   #define UDP_MIB_INC_COUNTER64(name, value) udpMibBase.name += value
#else
   #define UDP_MIB_INC_COUNTER32(name, value)
   #define UDP_MIB_INC_COUNTER64(name, value)
#endif

//C++ guard
#ifdef __cplusplus
   extern "C" {
#endif


/**
 * @brief UDP MIB base
 **/

typedef struct
{
   uint32_t udpInDatagrams;
   uint32_t udpNoPorts;
   uint32_t udpInErrors;
   uint32_t udpOutDatagrams;
   uint64_t udpHCInDatagrams;
   uint64_t udpHCOutDatagrams;
} UdpMibBase;


//UDP MIB related constants
extern UdpMibBase udpMibBase;
extern const MibObject udpMibObjects[];
extern const MibModule udpMibModule;

//C++ guard
#ifdef __cplusplus
   }
#endif

#endif
