/**
 * @file snmp_mpd_mib_impl.c
 * @brief SNMP MPD MIB module implementation
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
 * @brief SNMP MPD MIB module initialization
 * @return Error code
 **/

error_t snmpMpdMibInit(void)
{
   //Debug message
   TRACE_INFO("Initializing SNMP MPD MIB base...\r\n");

   //Clear SNMP MPD MIB base
   memset(&snmpMpdMibBase, 0, sizeof(snmpMpdMibBase));

   //Successful processing
   return NO_ERROR;
}


/**
 * @brief Load SNMP MPD MIB module
 * @param[in] context Pointer to the SNMP agent context
 * @return Error code
 **/

error_t snmpMpdMibLoad(void *context)
{
   //Successful processing
   return NO_ERROR;
}


/**
 * @brief Unload SNMP MPD MIB module
 * @param[in] context Pointer to the SNMP agent context
 **/

void snmpMpdMibUnload(void *context)
{
}


/**
 * @brief Lock SNMP MPD MIB base
 **/

void snmpMpdMibLock(void)
{
}


/**
 * @brief Unlock SNMP MPD MIB base
 **/

void snmpMpdMibUnlock(void)
{
}

#endif
