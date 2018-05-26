/**
 * @file snmp_framework_mib_impl.h
 * @brief SNMP FRAMEWORK MIB module implementation
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

#ifndef _SNMP_FRAMEWORK_MIB_IMPL_H
#define _SNMP_FRAMEWORK_MIB_IMPL_H

//Dependencies
#include "mibs/mib_common.h"

//C++ guard
#ifdef __cplusplus
   extern "C" {
#endif

//SNMP FRAMEWORK MIB related functions
error_t snmpFrameworkMibInit(void);
error_t snmpFrameworkMibLoad(void *context);
void snmpFrameworkMibUnload(void *context);
void snmpFrameworkMibLock(void);
void snmpFrameworkMibUnlock(void);

error_t snmpFrameworkMibGetSnmpEngineID(const MibObject *object,
   const uint8_t *oid, size_t oidLen, MibVariant *value, size_t *valueLen);

error_t snmpFrameworkMibGetSnmpEngineBoots(const MibObject *object,
   const uint8_t *oid, size_t oidLen, MibVariant *value, size_t *valueLen);

error_t snmpFrameworkMibGetSnmpEngineTime(const MibObject *object,
   const uint8_t *oid, size_t oidLen, MibVariant *value, size_t *valueLen);

error_t snmpFrameworkMibGetSnmpEngineMaxMessageSize(const MibObject *object,
   const uint8_t *oid, size_t oidLen, MibVariant *value, size_t *valueLen);

//C++ guard
#ifdef __cplusplus
   }
#endif

#endif
