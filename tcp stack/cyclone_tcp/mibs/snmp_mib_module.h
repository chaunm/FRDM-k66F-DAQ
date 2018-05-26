/**
 * @file snmp_mib_module.h
 * @brief SNMP MIB module
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

#ifndef _SNMP_MIB_MODULE_H
#define _SNMP_MIB_MODULE_H

//Dependencies
#include "mibs/mib_common.h"
#include "snmp/snmp_agent.h"

//SNMP MIB module support
#ifndef SNMP_MIB_SUPPORT
   #define SNMP_MIB_SUPPORT DISABLED
#elif (SNMP_MIB_SUPPORT != ENABLED && SNMP_MIB_SUPPORT != DISABLED)
   #error SNMP_MIB_SUPPORT parameter is not valid
#endif

//Size of sysDescr object
#ifndef SNMP_MIB_SYS_DESCR_SIZE
   #define SNMP_MIB_SYS_DESCR_SIZE 16
#elif (SNMP_MIB_SYS_DESCR_SIZE < 0)
   #error SNMP_MIB_SYS_DESCR_SIZE parameter is not valid
#endif

//Size of sysObjectID object
#ifndef SNMP_MIB_SYS_OBJECT_ID_SIZE
   #define SNMP_MIB_SYS_OBJECT_ID_SIZE 16
#elif (SNMP_MIB_SYS_OBJECT_ID_SIZE < 0)
   #error SNMP_MIB_SYS_OBJECT_ID_SIZE parameter is not valid
#endif

//Size of sysContact object
#ifndef SNMP_MIB_SYS_CONTACT_SIZE
   #define SNMP_MIB_SYS_CONTACT_SIZE 16
#elif (SNMP_MIB_SYS_CONTACT_SIZE < 0)
   #error SNMP_MIB_SYS_CONTACT_SIZE parameter is not valid
#endif

//Size of sysName object
#ifndef SNMP_MIB_SYS_NAME_SIZE
   #define SNMP_MIB_SYS_NAME_SIZE 16
#elif (SNMP_MIB_SYS_NAME_SIZE < 0)
   #error SNMP_MIB_SYS_NAME_SIZE parameter is not valid
#endif

//Size of sysLocation object
#ifndef SNMP_MIB_SYS_LOCATION_SIZE
   #define SNMP_MIB_SYS_LOCATION_SIZE 16
#elif (SNMP_MIB_SYS_LOCATION_SIZE < 0)
   #error SNMP_MIB_SYS_LOCATION_SIZE parameter is not valid
#endif

//Macro definitions
#if (SNMP_MIB_SUPPORT == ENABLED)
   #define SNMP_MIB_INC_COUNTER32(name, value) snmpMibBase.name += value
#else
   #define SNMP_MIB_INC_COUNTER32(name, value)
#endif

//C++ guard
#ifdef __cplusplus
   extern "C" {
#endif


/**
 * @brief System services
 **/

typedef enum
{
   SNMP_MIB_SYS_SERVICE_PHYSICAL     = 0x01,
   SNMP_MIB_SYS_SERVICE_DATALINK     = 0x02,
   SNMP_MIB_SYS_SERVICE_INTERNET     = 0x04,
   SNMP_MIB_SYS_SERVICE_END_TO_END   = 0x08,
   SNMP_MIB_SYS_SERVICE_APPLICATIONS = 0x40
} SnmpMibSysService;


/**
 * @brief System group
 **/

typedef struct
{
#if (SNMP_MIB_SYS_DESCR_SIZE > 0)
   char_t sysDescr[SNMP_MIB_SYS_DESCR_SIZE];
   size_t sysDescrLen;
#endif
#if (SNMP_MIB_SYS_OBJECT_ID_SIZE > 0)
   uint8_t sysObjectID[SNMP_MIB_SYS_OBJECT_ID_SIZE];
   size_t sysObjectIDLen;
#endif
   uint32_t sysUpTime;
#if (SNMP_MIB_SYS_CONTACT_SIZE > 0)
   char_t sysContact[SNMP_MIB_SYS_CONTACT_SIZE];
   size_t sysContactLen;
#endif
#if (SNMP_MIB_SYS_NAME_SIZE > 0)
   char_t sysName[SNMP_MIB_SYS_NAME_SIZE];
   size_t sysNameLen;
#endif
#if (SNMP_MIB_SYS_LOCATION_SIZE > 0)
   char_t sysLocation[SNMP_MIB_SYS_LOCATION_SIZE];
   size_t sysLocationLen;
#endif
   int32_t sysServices;
   uint32_t sysORLastChange;
} SnmpMibSysGroup;


/**
 * @brief SNMP group
 **/

typedef struct
{
   uint32_t snmpInPkts;
   uint32_t snmpInBadVersions;
   uint32_t snmpInBadCommunityNames;
   uint32_t snmpInBadCommunityUses;
   uint32_t snmpInASNParseErrs;
   int32_t snmpEnableAuthenTraps;
   uint32_t snmpSilentDrops;
   uint32_t snmpProxyDrops;
} SnmpMibSnmpGroup;


/**
 * @brief Set group
 **/

typedef struct
{
   int32_t snmpSetSerialNo;
} SnmpMibSetGroup;


/**
 * @brief SNMP MIB base
 **/

typedef struct
{
   SnmpAgentContext *context;
   SnmpMibSysGroup sysGroup;
   SnmpMibSnmpGroup snmpGroup;
   SnmpMibSetGroup setGroup;
} SnmpMibBase;


//SNMP MIB related constants
extern SnmpMibBase snmpMibBase;
extern const MibObject snmpMibObjects[];
extern const MibModule snmpMibModule;

//C++ guard
#ifdef __cplusplus
   }
#endif

#endif
