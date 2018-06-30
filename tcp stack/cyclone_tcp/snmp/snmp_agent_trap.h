/**
 * @file snmp_agent_trap.h
 * @brief SNMP trap notifications
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

#ifndef _SNMP_AGENT_TRAP_H
#define _SNMP_AGENT_TRAP_H

//Dependencies
#include "core/net.h"
#include "snmp/snmp_agent.h"

//Trap notification support
#ifndef SNMP_AGENT_TRAP_SUPPORT
   #define SNMP_AGENT_TRAP_SUPPORT ENABLED
#elif (SNMP_AGENT_TRAP_SUPPORT != ENABLED && SNMP_AGENT_TRAP_SUPPORT != DISABLED)
   #error SNMP_AGENT_TRAP_SUPPORT parameter is not valid
#endif

//C++ guard
#ifdef __cplusplus
   extern "C" {
#endif


/**
 * @brief Object descriptor for trap notifications
 **/

typedef struct
{
   uint8_t oid[SNMP_MAX_OID_SIZE];
   size_t oidLen;
} SnmpTrapObject;


//SNMP trap related functions
error_t snmpFormatTrapMessage(SnmpAgentContext *context, SnmpVersion version,
   const char_t *userName, uint_t genericTrapType, uint_t specificTrapCode,
   const SnmpTrapObject *objectList, uint_t objectListSize);

error_t snmpFormatTrapPdu(SnmpAgentContext *context, SnmpVersion version,
   const char_t *userName, uint_t genericTrapType, uint_t specificTrapCode,
   const SnmpTrapObject *objectList, uint_t objectListSize);

//C++ guard
#ifdef __cplusplus
   }
#endif

#endif
