/**
 * @file snmp_community_mib_module.c
 * @brief SNMP COMMUNITY MIB module
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
 * The SNMP-COMMUNITY-MIB describes managed objects for mapping between
 * community strings and version-independent SNMP message parameters. Refer
 * to RFC 3584 for more details
 *
 * @author Oryx Embedded SARL (www.oryx-embedded.com)
 * @version 1.8.2
 **/

//Switch to the appropriate trace level
#define TRACE_LEVEL SNMP_TRACE_LEVEL

//Dependencies
#include "core/net.h"
#include "mibs/mib_common.h"
#include "mibs/snmp_community_mib_module.h"
#include "mibs/snmp_community_mib_impl.h"
#include "core/crypto.h"
#include "encoding/asn1.h"
#include "encoding/oid.h"
#include "debug.h"

//Check TCP/IP stack configuration
#if (SNMP_COMMUNITY_MIB_SUPPORT == ENABLED)


/**
 * @brief SNMP COMMUNITY MIB base
 **/

SnmpCommunityMibBase snmpCommunityMibBase;


/**
 * @brief SNMP COMMUNITY MIB objects
 **/

const MibObject snmpCommunityMibObjects[] =
{
   //snmpCommunityName object (1.3.6.1.6.3.18.1.1.1.2)
   {
      "snmpCommunityName",
      {43, 6, 1, 6, 3, 18, 1, 1, 1, 2},
      10,
      ASN1_CLASS_UNIVERSAL,
      ASN1_TYPE_OCTET_STRING,
      MIB_ACCESS_READ_CREATE,
      NULL,
      NULL,
      0,
      snmpCommunityMibSetCommunityEntry,
      snmpCommunityMibGetCommunityEntry,
      snmpCommunityMibGetNextCommunityEntry
   },
   //snmpCommunitySecurityName object (1.3.6.1.6.3.18.1.1.1.3)
   {
      "snmpCommunitySecurityName",
      {43, 6, 1, 6, 3, 18, 1, 1, 1, 3},
      10,
      ASN1_CLASS_UNIVERSAL,
      ASN1_TYPE_OCTET_STRING,
      MIB_ACCESS_READ_CREATE,
      NULL,
      NULL,
      0,
      snmpCommunityMibSetCommunityEntry,
      snmpCommunityMibGetCommunityEntry,
      snmpCommunityMibGetNextCommunityEntry
   },
   //snmpCommunityContextEngineID object (1.3.6.1.6.3.18.1.1.1.4)
   {
      "snmpCommunityContextEngineID",
      {43, 6, 1, 6, 3, 18, 1, 1, 1, 4},
      10,
      ASN1_CLASS_UNIVERSAL,
      ASN1_TYPE_OCTET_STRING,
      MIB_ACCESS_READ_CREATE,
      NULL,
      NULL,
      0,
      snmpCommunityMibSetCommunityEntry,
      snmpCommunityMibGetCommunityEntry,
      snmpCommunityMibGetNextCommunityEntry
   },
   //snmpCommunityContextName object (1.3.6.1.6.3.18.1.1.1.5)
   {
      "snmpCommunityContextName",
      {43, 6, 1, 6, 3, 18, 1, 1, 1, 5},
      10,
      ASN1_CLASS_UNIVERSAL,
      ASN1_TYPE_OCTET_STRING,
      MIB_ACCESS_READ_CREATE,
      NULL,
      NULL,
      0,
      snmpCommunityMibSetCommunityEntry,
      snmpCommunityMibGetCommunityEntry,
      snmpCommunityMibGetNextCommunityEntry
   },
   //snmpCommunityTransportTag object (1.3.6.1.6.3.18.1.1.1.6)
   {
      "snmpCommunityTransportTag",
      {43, 6, 1, 6, 3, 18, 1, 1, 1, 6},
      10,
      ASN1_CLASS_UNIVERSAL,
      ASN1_TYPE_OCTET_STRING,
      MIB_ACCESS_READ_CREATE,
      NULL,
      NULL,
      0,
      snmpCommunityMibSetCommunityEntry,
      snmpCommunityMibGetCommunityEntry,
      snmpCommunityMibGetNextCommunityEntry
   },
   //snmpCommunityStorageType object (1.3.6.1.6.3.18.1.1.1.7)
   {
      "snmpCommunityStorageType",
      {43, 6, 1, 6, 3, 18, 1, 1, 1, 7},
      10,
      ASN1_CLASS_UNIVERSAL,
      ASN1_TYPE_INTEGER,
      MIB_ACCESS_READ_CREATE,
      NULL,
      NULL,
      sizeof(int32_t),
      snmpCommunityMibSetCommunityEntry,
      snmpCommunityMibGetCommunityEntry,
      snmpCommunityMibGetNextCommunityEntry
   },
   //snmpCommunityStatus object (1.3.6.1.6.3.18.1.1.1.8)
   {
      "snmpCommunityStatus",
      {43, 6, 1, 6, 3, 18, 1, 1, 1, 8},
      10,
      ASN1_CLASS_UNIVERSAL,
      ASN1_TYPE_INTEGER,
      MIB_ACCESS_READ_CREATE,
      NULL,
      NULL,
      sizeof(int32_t),
      snmpCommunityMibSetCommunityEntry,
      snmpCommunityMibGetCommunityEntry,
      snmpCommunityMibGetNextCommunityEntry
   },
};


/**
 * @brief SNMP COMMUNITY MIB module
 **/

const MibModule snmpCommunityMibModule =
{
   "SNMP-COMMUNITY-MIB",
   {43, 6, 1, 6, 3, 18},
   6,
   snmpCommunityMibObjects,
   arraysize(snmpCommunityMibObjects),
   snmpCommunityMibInit,
   snmpCommunityMibLoad,
   snmpCommunityMibUnload,
   snmpCommunityMibLock,
   snmpCommunityMibUnlock
};

#endif
