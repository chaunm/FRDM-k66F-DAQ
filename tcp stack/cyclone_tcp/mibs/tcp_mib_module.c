/**
 * @file tcp_mib_module.c
 * @brief TCP MIB module
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
 * The TCP-MIB describes managed objects used for implementations of
 * the Transmission Control Protocol (TCP) in an IP version independent
 * manner. Refer to the following RFCs for complete details:
 * - RFC 4022: MIB for the Transmission Control Protocol (TCP)
 * - RFC 4001: Textual Conventions for Internet Network Addresses
 *
 * @author Oryx Embedded SARL (www.oryx-embedded.com)
 * @version 1.8.2
 **/

//Switch to the appropriate trace level
#define TRACE_LEVEL SNMP_TRACE_LEVEL

//Dependencies
#include "core/net.h"
#include "mibs/mib_common.h"
#include "mibs/tcp_mib_module.h"
#include "mibs/tcp_mib_impl.h"
#include "core/crypto.h"
#include "encoding/asn1.h"
#include "encoding/oid.h"
#include "debug.h"

//Check TCP/IP stack configuration
#if (TCP_MIB_SUPPORT == ENABLED && TCP_SUPPORT == ENABLED)


/**
 * @brief TCP MIB base
 **/

TcpMibBase tcpMibBase;


/**
 * @brief TCP MIB objects
 **/

const MibObject tcpMibObjects[] =
{
   //tcpRtoAlgorithm object (1.3.6.1.2.1.6.1)
   {
      "tcpRtoAlgorithm",
      {43, 6, 1, 2, 1, 6, 1},
      7,
      ASN1_CLASS_UNIVERSAL,
      ASN1_TYPE_INTEGER,
      MIB_ACCESS_READ_ONLY,
      &tcpMibBase.tcpRtoAlgorithm,
      NULL,
      sizeof(int32_t),
      NULL,
      NULL,
      NULL
   },
   //tcpRtoMin object (1.3.6.1.2.1.6.2)
   {
      "tcpRtoMin",
      {43, 6, 1, 2, 1, 6, 2},
      7,
      ASN1_CLASS_UNIVERSAL,
      ASN1_TYPE_INTEGER,
      MIB_ACCESS_READ_ONLY,
      &tcpMibBase.tcpRtoMin,
      NULL,
      sizeof(int32_t),
      NULL,
      NULL,
      NULL
   },
   //tcpRtoMax object (1.3.6.1.2.1.6.3)
   {
      "tcpRtoMax",
      {43, 6, 1, 2, 1, 6, 3},
      7,
      ASN1_CLASS_UNIVERSAL,
      ASN1_TYPE_INTEGER,
      MIB_ACCESS_READ_ONLY,
      &tcpMibBase.tcpRtoMax,
      NULL,
      sizeof(int32_t),
      NULL,
      NULL,
      NULL
   },
   //tcpMaxConn object (1.3.6.1.2.1.6.4)
   {
      "tcpMaxConn",
      {43, 6, 1, 2, 1, 6, 4},
      7,
      ASN1_CLASS_UNIVERSAL,
      ASN1_TYPE_INTEGER,
      MIB_ACCESS_READ_ONLY,
      &tcpMibBase.tcpMaxConn,
      NULL,
      sizeof(int32_t),
      NULL,
      NULL,
      NULL
   },
   //tcpActiveOpens object (1.3.6.1.2.1.6.5)
   {
      "tcpActiveOpens",
      {43, 6, 1, 2, 1, 6, 5},
      7,
      ASN1_CLASS_APPLICATION,
      MIB_TYPE_COUNTER32,
      MIB_ACCESS_READ_ONLY,
      &tcpMibBase.tcpActiveOpens,
      NULL,
      sizeof(uint32_t),
      NULL,
      NULL,
      NULL
   },
   //tcpPassiveOpens object (1.3.6.1.2.1.6.6)
   {
      "tcpPassiveOpens",
      {43, 6, 1, 2, 1, 6, 6},
      7,
      ASN1_CLASS_APPLICATION,
      MIB_TYPE_COUNTER32,
      MIB_ACCESS_READ_ONLY,
      &tcpMibBase.tcpPassiveOpens,
      NULL,
      sizeof(uint32_t),
      NULL,
      NULL,
      NULL
   },
   //tcpAttemptFails object (1.3.6.1.2.1.6.7)
   {
      "tcpAttemptFails",
      {43, 6, 1, 2, 1, 6, 7},
      7,
      ASN1_CLASS_APPLICATION,
      MIB_TYPE_COUNTER32,
      MIB_ACCESS_READ_ONLY,
      &tcpMibBase.tcpAttemptFails,
      NULL,
      sizeof(uint32_t),
      NULL,
      NULL,
      NULL
   },
   //tcpEstabResets object (1.3.6.1.2.1.6.8)
   {
      "tcpEstabResets",
      {43, 6, 1, 2, 1, 6, 8},
      7,
      ASN1_CLASS_APPLICATION,
      MIB_TYPE_COUNTER32,
      MIB_ACCESS_READ_ONLY,
      &tcpMibBase.tcpEstabResets,
      NULL,
      sizeof(uint32_t),
      NULL,
      NULL,
      NULL
   },
   //tcpCurrEstab object (1.3.6.1.2.1.6.9)
   {
      "tcpCurrEstab",
      {43, 6, 1, 2, 1, 6, 9},
      7,
      ASN1_CLASS_APPLICATION,
      MIB_TYPE_GAUGE32,
      MIB_ACCESS_READ_ONLY,
      NULL,
      NULL,
      sizeof(uint32_t),
      NULL,
      tcpMibGetTcpCurrEstab,
      NULL
   },
   //tcpInSegs object (1.3.6.1.2.1.6.10)
   {
      "tcpInSegs",
      {43, 6, 1, 2, 1, 6, 10},
      7,
      ASN1_CLASS_APPLICATION,
      MIB_TYPE_COUNTER32,
      MIB_ACCESS_READ_ONLY,
      &tcpMibBase.tcpInSegs,
      NULL,
      sizeof(uint32_t),
      NULL,
      NULL,
      NULL
   },
   //tcpOutSegs object (1.3.6.1.2.1.6.11)
   {
      "tcpOutSegs",
      {43, 6, 1, 2, 1, 6, 11},
      7,
      ASN1_CLASS_APPLICATION,
      MIB_TYPE_COUNTER32,
      MIB_ACCESS_READ_ONLY,
      &tcpMibBase.tcpOutSegs,
      NULL,
      sizeof(uint32_t),
      NULL,
      NULL,
      NULL
   },
   //tcpRetransSegs object (1.3.6.1.2.1.6.12)
   {
      "tcpRetransSegs",
      {43, 6, 1, 2, 1, 6, 12},
      7,
      ASN1_CLASS_APPLICATION,
      MIB_TYPE_COUNTER32,
      MIB_ACCESS_READ_ONLY,
      &tcpMibBase.tcpRetransSegs,
      NULL,
      sizeof(uint32_t),
      NULL,
      NULL,
      NULL
   },
   //tcpInErrs object (1.3.6.1.2.1.6.14)
   {
      "tcpInErrs",
      {43, 6, 1, 2, 1, 6, 14},
      7,
      ASN1_CLASS_APPLICATION,
      MIB_TYPE_COUNTER32,
      MIB_ACCESS_READ_ONLY,
      &tcpMibBase.tcpInErrs,
      NULL,
      sizeof(uint32_t),
      NULL,
      NULL,
      NULL
   },
   //tcpOutRsts object (1.3.6.1.2.1.6.15)
   {
      "tcpOutRsts",
      {43, 6, 1, 2, 1, 6, 15},
      7,
      ASN1_CLASS_APPLICATION,
      MIB_TYPE_COUNTER32,
      MIB_ACCESS_READ_ONLY,
      &tcpMibBase.tcpOutRsts,
      NULL,
      sizeof(uint32_t),
      NULL,
      NULL,
      NULL
   },
   //tcpHCInSegs object (1.3.6.1.2.1.6.17)
   {
      "tcpHCInSegs",
      {43, 6, 1, 2, 1, 6, 17},
      7,
      ASN1_CLASS_APPLICATION,
      MIB_TYPE_COUNTER64,
      MIB_ACCESS_READ_ONLY,
      &tcpMibBase.tcpHCInSegs,
      NULL,
      sizeof(uint64_t),
      NULL,
      NULL,
      NULL
   },
   //tcpHCOutSegs object (1.3.6.1.2.1.6.18)
   {
      "tcpHCOutSegs",
      {43, 6, 1, 2, 1, 6, 18},
      7,
      ASN1_CLASS_APPLICATION,
      MIB_TYPE_COUNTER64,
      MIB_ACCESS_READ_ONLY,
      &tcpMibBase.tcpHCOutSegs,
      NULL,
      sizeof(uint64_t),
      NULL,
      NULL,
      NULL
   },
   //tcpConnectionState object (1.3.6.1.2.1.6.19.1.7)
   {
      "tcpConnectionState",
      {43, 6, 1, 2, 1, 6, 19, 1, 7},
      9,
      ASN1_CLASS_UNIVERSAL,
      ASN1_TYPE_INTEGER,
      MIB_ACCESS_READ_WRITE,
      NULL,
      NULL,
      sizeof(int32_t),
      tcpMibSetTcpConnectionEntry,
      tcpMibGetTcpConnectionEntry,
      tcpMibGetNextTcpConnectionEntry
   },
   //tcpConnectionProcess object (1.3.6.1.2.1.6.19.1.8)
   {
      "tcpConnectionProcess",
      {43, 6, 1, 2, 1, 6, 19, 1, 8},
      9,
      ASN1_CLASS_APPLICATION,
      MIB_TYPE_UNSIGNED32,
      MIB_ACCESS_READ_ONLY,
      NULL,
      NULL,
      sizeof(uint32_t),
      NULL,
      tcpMibGetTcpConnectionEntry,
      tcpMibGetNextTcpConnectionEntry
   },
   //tcpListenerProcess object (1.3.6.1.2.1.6.20.1.4)
   {
      "tcpListenerProcess",
      {43, 6, 1, 2, 1, 6, 20, 1, 4},
      9,
      ASN1_CLASS_APPLICATION,
      MIB_TYPE_UNSIGNED32,
      MIB_ACCESS_READ_ONLY,
      NULL,
      NULL,
      sizeof(uint32_t),
      NULL,
      tcpMibGetTcpListenerEntry,
      tcpMibGetNextTcpListenerEntry
   }
};


/**
 * @brief TCP MIB module
 **/

const MibModule tcpMibModule =
{
   "TCP-MIB",
   {43, 6, 1, 2, 1, 49},
   6,
   tcpMibObjects,
   arraysize(tcpMibObjects),
   tcpMibInit,
   NULL,
   NULL,
   NULL,
   NULL
};

#endif
