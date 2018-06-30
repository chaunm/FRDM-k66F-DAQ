/**
 * @file snmp_mpd_mib_module.h
 * @brief SNMP MPD MIB module
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

#ifndef _SNMP_MPD_MIB_MODULE_H
#define _SNMP_MPD_MIB_MODULE_H

//Dependencies
#include "mibs/mib_common.h"

//SNMP MPD MIB module support
#ifndef SNMP_MPD_MIB_SUPPORT
   #define SNMP_MPD_MIB_SUPPORT DISABLED
#elif (SNMP_MPD_MIB_SUPPORT != ENABLED && SNMP_MPD_MIB_SUPPORT != DISABLED)
   #error SNMP_MPD_MIB_SUPPORT parameter is not valid
#endif

//Macro definitions
#if (SNMP_MPD_MIB_SUPPORT == ENABLED)
   #define SNMP_MPD_MIB_INC_COUNTER32(name, value) snmpMpdMibBase.name += value
#else
   #define SNMP_MPD_MIB_INC_COUNTER32(name, value)
#endif

//C++ guard
#ifdef __cplusplus
   extern "C" {
#endif


/**
 * @brief SNMP MPD MIB base
 **/

typedef struct
{
   uint32_t snmpUnknownSecurityModels;
   uint32_t snmpInvalidMsgs;
   uint32_t snmpUnknownPDUHandlers;
} SnmpMpdMibBase;


//SNMP MPD MIB related constants
extern SnmpMpdMibBase snmpMpdMibBase;
extern const MibObject snmpMpdMibObjects[];
extern const MibModule snmpMpdMibModule;

//C++ guard
#ifdef __cplusplus
   }
#endif

#endif
