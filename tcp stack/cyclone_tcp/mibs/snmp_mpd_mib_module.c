/**
 * @file snmp_mpd_mib_module.c
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
 * @section Description
 *
 * The SNMP-MPD-MIB defines managed objects for SNMP message processing and
 * dispatching. Refer to RFC 3412 for more details
 *
 * @author Oryx Embedded SARL (www.oryx-embedded.com)
 * @version 1.8.2
 **/

//Switch to the appropriate trace level
#define TRACE_LEVEL SNMP_TRACE_LEVEL

//Dependencies
#include "core/net.h"
#include "mibs/mib_common.h"
#include "mibs/snmp_mpd_mib_module.h"
#include "mibs/snmp_mpd_mib_impl.h"
#include "core/crypto.h"
#include "encoding/asn1.h"
#include "encoding/oid.h"
#include "debug.h"

//Check TCP/IP stack configuration
#if (SNMP_MPD_MIB_SUPPORT == ENABLED)


/**
 * @brief SNMP MPD MIB base
 **/

SnmpMpdMibBase snmpMpdMibBase;


/**
 * @brief SNMP MPD MIB objects
 **/

const MibObject snmpMpdMibObjects[] =
{
   //snmpUnknownSecurityModels object (1.3.6.1.6.3.11.2.1.1)
   {
      "snmpUnknownSecurityModels",
      {43, 6, 1, 6, 3, 11, 2, 1, 1},
      9,
      ASN1_CLASS_APPLICATION,
      MIB_TYPE_COUNTER32,
      MIB_ACCESS_READ_ONLY,
      &snmpMpdMibBase.snmpUnknownSecurityModels,
      NULL,
      sizeof(uint32_t),
      NULL,
      NULL,
      NULL
   },
   //snmpInvalidMsgs object (1.3.6.1.6.3.11.2.1.2)
   {
      "snmpInvalidMsgs",
      {43, 6, 1, 6, 3, 11, 2, 1, 2},
      9,
      ASN1_CLASS_APPLICATION,
      MIB_TYPE_COUNTER32,
      MIB_ACCESS_READ_ONLY,
      &snmpMpdMibBase.snmpInvalidMsgs,
      NULL,
      sizeof(uint32_t),
      NULL,
      NULL,
      NULL
   },
   //snmpUnknownPDUHandlers object (1.3.6.1.6.3.11.2.1.3)
   {
      "snmpUnknownPDUHandlers",
      {43, 6, 1, 6, 3, 11, 2, 1, 3},
      9,
      ASN1_CLASS_APPLICATION,
      MIB_TYPE_COUNTER32,
      MIB_ACCESS_READ_ONLY,
      &snmpMpdMibBase.snmpUnknownPDUHandlers,
      NULL,
      sizeof(uint32_t),
      NULL,
      NULL,
      NULL
   }
};


/**
 * @brief SNMP MPD MIB module
 **/

const MibModule snmpMpdMibModule =
{
   "SNMP-MPD-MIB",
   {43, 6, 1, 6, 3, 11},
   6,
   snmpMpdMibObjects,
   arraysize(snmpMpdMibObjects),
   snmpMpdMibInit,
   snmpMpdMibLoad,
   snmpMpdMibUnload,
   snmpMpdMibLock,
   snmpMpdMibUnlock
};

#endif
