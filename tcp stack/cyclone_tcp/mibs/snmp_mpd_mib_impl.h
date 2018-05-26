/**
 * @file snmp_mpd_mib_impl.h
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

#ifndef _SNMP_MPD_MIB_IMPL_H
#define _SNMP_MPD_MIB_IMPL_H

//Dependencies
#include "mibs/mib_common.h"

//C++ guard
#ifdef __cplusplus
   extern "C" {
#endif

//SNMP MPD MIB related functions
error_t snmpMpdMibInit(void);
error_t snmpMpdMibLoad(void *context);
void snmpMpdMibUnload(void *context);
void snmpMpdMibLock(void);
void snmpMpdMibUnlock(void);

//C++ guard
#ifdef __cplusplus
   }
#endif

#endif
