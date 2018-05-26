/**
 * @file snmp_vacm_mib_module.c
 * @brief SNMP VACM MIB module
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
 * @section Description
 *
 * The SNMP-VACM-MIB describes managed objects for remotely managing the
 * configuration parameters for the View-based Access Control Model. Refer
 * to RFC 3415 for more
 * details
 *
 * @author Oryx Embedded SARL (www.oryx-embedded.com)
 * @version 1.8.2
 **/

//Switch to the appropriate trace level
#define TRACE_LEVEL SNMP_TRACE_LEVEL

//Dependencies
#include "core/net.h"
#include "mibs/mib_common.h"
#include "mibs/snmp_vacm_mib_module.h"
#include "mibs/snmp_vacm_mib_impl.h"
#include "core/crypto.h"
#include "encoding/asn1.h"
#include "encoding/oid.h"
#include "debug.h"

//Check TCP/IP stack configuration
#if (SNMP_VACM_MIB_SUPPORT == ENABLED)


/**
 * @brief SNMP VACM MIB base
 **/

SnmpVacmMibBase snmpVacmMibBase;


/**
 * @brief SNMP VACM MIB objects
 **/

const MibObject snmpVacmMibObjects[] =
{
   //vacmContextName object (1.3.6.1.6.3.16.1.1.1.1)
   {
      "vacmContextName",
      {43, 6, 1, 6, 3, 16, 1, 1, 1, 1},
      10,
      ASN1_CLASS_UNIVERSAL,
      ASN1_TYPE_OCTET_STRING,
      MIB_ACCESS_READ_ONLY,
      NULL,
      NULL,
      0,
      NULL,
      snmpVacmMibGetContextEntry,
      snmpVacmMibGetNextContextEntry
   },
   //vacmGroupName object (1.3.6.1.6.3.16.1.2.1.3)
   {
      "vacmGroupName",
      {43, 6, 1, 6, 3, 16, 1, 2, 1, 3},
      10,
      ASN1_CLASS_UNIVERSAL,
      ASN1_TYPE_OCTET_STRING,
      MIB_ACCESS_READ_CREATE,
      NULL,
      NULL,
      0,
      snmpVacmMibSetSecurityToGroupEntry,
      snmpVacmMibGetSecurityToGroupEntry,
      snmpVacmMibGetNextSecurityToGroupEntry
   },
   //vacmSecurityToGroupStorageType object (1.3.6.1.6.3.16.1.2.1.4)
   {
      "vacmSecurityToGroupStorageType",
      {43, 6, 1, 6, 3, 16, 1, 2, 1, 4},
      10,
      ASN1_CLASS_UNIVERSAL,
      ASN1_TYPE_INTEGER,
      MIB_ACCESS_READ_CREATE,
      NULL,
      NULL,
      sizeof(int32_t),
      snmpVacmMibSetSecurityToGroupEntry,
      snmpVacmMibGetSecurityToGroupEntry,
      snmpVacmMibGetNextSecurityToGroupEntry
   },
   //vacmSecurityToGroupStatus object (1.3.6.1.6.3.16.1.2.1.5)
   {
      "vacmSecurityToGroupStatus",
      {43, 6, 1, 6, 3, 16, 1, 2, 1, 5},
      10,
      ASN1_CLASS_UNIVERSAL,
      ASN1_TYPE_INTEGER,
      MIB_ACCESS_READ_CREATE,
      NULL,
      NULL,
      sizeof(int32_t),
      snmpVacmMibSetSecurityToGroupEntry,
      snmpVacmMibGetSecurityToGroupEntry,
      snmpVacmMibGetNextSecurityToGroupEntry
   },
   //vacmAccessContextMatch object (1.3.6.1.6.3.16.1.4.1.4)
   {
      "vacmAccessContextMatch",
      {43, 6, 1, 6, 3, 16, 1, 4, 1, 4},
      10,
      ASN1_CLASS_UNIVERSAL,
      ASN1_TYPE_INTEGER,
      MIB_ACCESS_READ_CREATE,
      NULL,
      NULL,
      sizeof(int32_t),
      snmpVacmMibSetAccessEntry,
      snmpVacmMibGetAccessEntry,
      snmpVacmMibGetNextAccessEntry
   },
   //vacmAccessReadViewName object (1.3.6.1.6.3.16.1.4.1.5)
   {
      "vacmAccessReadViewName",
      {43, 6, 1, 6, 3, 16, 1, 4, 1, 5},
      10,
      ASN1_CLASS_UNIVERSAL,
      ASN1_TYPE_OCTET_STRING,
      MIB_ACCESS_READ_CREATE,
      NULL,
      NULL,
      0,
      snmpVacmMibSetAccessEntry,
      snmpVacmMibGetAccessEntry,
      snmpVacmMibGetNextAccessEntry
   },
   //vacmAccessWriteViewName object (1.3.6.1.6.3.16.1.4.1.6)
   {
      "vacmAccessWriteViewName",
      {43, 6, 1, 6, 3, 16, 1, 4, 1, 6},
      10,
      ASN1_CLASS_UNIVERSAL,
      ASN1_TYPE_OCTET_STRING,
      MIB_ACCESS_READ_CREATE,
      NULL,
      NULL,
      0,
      snmpVacmMibSetAccessEntry,
      snmpVacmMibGetAccessEntry,
      snmpVacmMibGetNextAccessEntry
   },
   //vacmAccessNotifyViewName object (1.3.6.1.6.3.16.1.4.1.7)
   {
      "vacmAccessNotifyViewName",
      {43, 6, 1, 6, 3, 16, 1, 4, 1, 7},
      10,
      ASN1_CLASS_UNIVERSAL,
      ASN1_TYPE_OCTET_STRING,
      MIB_ACCESS_READ_CREATE,
      NULL,
      NULL,
      0,
      snmpVacmMibSetAccessEntry,
      snmpVacmMibGetAccessEntry,
      snmpVacmMibGetNextAccessEntry
   },
   //vacmAccessStorageType object (1.3.6.1.6.3.16.1.4.1.8)
   {
      "vacmAccessStorageType",
      {43, 6, 1, 6, 3, 16, 1, 4, 1, 8},
      10,
      ASN1_CLASS_UNIVERSAL,
      ASN1_TYPE_INTEGER,
      MIB_ACCESS_READ_CREATE,
      NULL,
      NULL,
      sizeof(int32_t),
      snmpVacmMibSetAccessEntry,
      snmpVacmMibGetAccessEntry,
      snmpVacmMibGetNextAccessEntry
   },
   //vacmAccessStatus object (1.3.6.1.6.3.16.1.4.1.9)
   {
      "vacmAccessStatus",
      {43, 6, 1, 6, 3, 16, 1, 4, 1, 9},
      10,
      ASN1_CLASS_UNIVERSAL,
      ASN1_TYPE_INTEGER,
      MIB_ACCESS_READ_CREATE,
      NULL,
      NULL,
      sizeof(int32_t),
      snmpVacmMibSetAccessEntry,
      snmpVacmMibGetAccessEntry,
      snmpVacmMibGetNextAccessEntry
   },
   //vacmViewSpinLock object (1.3.6.1.6.3.16.1.5.1)
   {
      "vacmViewSpinLock",
      {43, 6, 1, 6, 3, 16, 1, 5, 1},
      9,
      ASN1_CLASS_UNIVERSAL,
      ASN1_TYPE_INTEGER,
      MIB_ACCESS_READ_WRITE,
      NULL,
      NULL,
      sizeof(int32_t),
      snmpVacmMibSetViewSpinLock,
      snmpVacmMibGetViewSpinLock,
      NULL
   },
   //vacmViewTreeFamilyMask object (1.3.6.1.6.3.16.1.5.2.1.3)
   {
      "vacmViewTreeFamilyMask",
      {43, 6, 1, 6, 3, 16, 1, 5, 2, 1, 3},
      11,
      ASN1_CLASS_UNIVERSAL,
      ASN1_TYPE_OCTET_STRING,
      MIB_ACCESS_READ_CREATE,
      NULL,
      NULL,
      0,
      snmpVacmMibSetViewTreeFamilyEntry,
      snmpVacmMibGetViewTreeFamilyEntry,
      snmpVacmMibGetNextViewTreeFamilyEntry
   },
   //vacmViewTreeFamilyType object (1.3.6.1.6.3.16.1.5.2.1.4)
   {
      "vacmViewTreeFamilyType",
      {43, 6, 1, 6, 3, 16, 1, 5, 2, 1, 4},
      11,
      ASN1_CLASS_UNIVERSAL,
      ASN1_TYPE_INTEGER,
      MIB_ACCESS_READ_CREATE,
      NULL,
      NULL,
      sizeof(int32_t),
      snmpVacmMibSetViewTreeFamilyEntry,
      snmpVacmMibGetViewTreeFamilyEntry,
      snmpVacmMibGetNextViewTreeFamilyEntry
   },
   //vacmViewTreeFamilyStorageType object (1.3.6.1.6.3.16.1.5.2.1.5)
   {
      "vacmViewTreeFamilyStorageType",
      {43, 6, 1, 6, 3, 16, 1, 5, 2, 1, 5},
      11,
      ASN1_CLASS_UNIVERSAL,
      ASN1_TYPE_INTEGER,
      MIB_ACCESS_READ_CREATE,
      NULL,
      NULL,
      sizeof(int32_t),
      snmpVacmMibSetViewTreeFamilyEntry,
      snmpVacmMibGetViewTreeFamilyEntry,
      snmpVacmMibGetNextViewTreeFamilyEntry
   },
   //vacmViewTreeFamilyStatus object (1.3.6.1.6.3.16.1.5.2.1.6)
   {
      "vacmViewTreeFamilyStatus",
      {43, 6, 1, 6, 3, 16, 1, 5, 2, 1, 6},
      11,
      ASN1_CLASS_UNIVERSAL,
      ASN1_TYPE_INTEGER,
      MIB_ACCESS_READ_CREATE,
      NULL,
      NULL,
      sizeof(int32_t),
      snmpVacmMibSetViewTreeFamilyEntry,
      snmpVacmMibGetViewTreeFamilyEntry,
      snmpVacmMibGetNextViewTreeFamilyEntry
   }
};


/**
 * @brief SNMP VACM MIB module
 **/

const MibModule snmpVacmMibModule =
{
   "SNMP-VACM-MIB",
   {43, 6, 1, 6, 3, 16},
   6,
   snmpVacmMibObjects,
   arraysize(snmpVacmMibObjects),
   snmpVacmMibInit,
   snmpVacmMibLoad,
   snmpVacmMibUnload,
   snmpVacmMibLock,
   snmpVacmMibUnlock
};

#endif
