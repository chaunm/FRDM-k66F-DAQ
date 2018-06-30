/**
 * @file snmp_framework_mib_module.h
 * @brief SNMP FRAMEWORK MIB module
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

#ifndef _SNMP_FRAMEWORK_MIB_MODULE_H
#define _SNMP_FRAMEWORK_MIB_MODULE_H

//Dependencies
#include "mibs/mib_common.h"
#include "snmp/snmp_agent.h"

//SNMP FRAMEWORK MIB module support
#ifndef SNMP_FRAMEWORK_MIB_SUPPORT
   #define SNMP_FRAMEWORK_MIB_SUPPORT DISABLED
#elif (SNMP_FRAMEWORK_MIB_SUPPORT != ENABLED && SNMP_FRAMEWORK_MIB_SUPPORT != DISABLED)
   #error SNMP_FRAMEWORK_MIB_SUPPORT parameter is not valid
#endif

//C++ guard
#ifdef __cplusplus
   extern "C" {
#endif


/**
 * @brief SNMP FRAMEWORK MIB base
 **/

typedef struct
{
   SnmpAgentContext *context;
} SnmpFrameworkMibBase;


//SNMP FRAMEWORK MIB related constants
extern SnmpFrameworkMibBase snmpFrameworkMibBase;
extern const MibObject snmpFrameworkMibObjects[];
extern const MibModule snmpFrameworkMibModule;

//C++ guard
#ifdef __cplusplus
   }
#endif

#endif
