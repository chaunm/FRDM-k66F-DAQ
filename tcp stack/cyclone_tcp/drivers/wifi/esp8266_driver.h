/**
 * @file esp8266_driver.h
 * @brief ESP8266 Wi-Fi controller
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

#ifndef _ESP8266_WIFI_DRIVER_H
#define _ESP8266_WIFI_DRIVER_H

//Dependencies
#include "core/nic.h"

//C++ guard
#ifdef __cplusplus
   extern "C" {
#endif

//ESP8266 Wi-Fi driver (STA mode)
extern const NicDriver esp8266WifiStaDriver;
//ESP8266 Wi-Fi driver (AP mode)
extern const NicDriver esp8266WifiApDriver;

//ESP8266 Wi-Fi related functions
error_t esp8266WifiInit(NetInterface *interface);

void esp8266WifiTick(NetInterface *interface);

void esp8266WifiEnableIrq(NetInterface *interface);
void esp8266WifiDisableIrq(NetInterface *interface);
void esp8266WifiEventHandler(NetInterface *interface);

error_t esp8266WifiSendPacket(NetInterface *interface,
   const NetBuffer *buffer, size_t offset);

error_t esp8266WifiSetMulticastFilter(NetInterface *interface);

//C++ guard
#ifdef __cplusplus
   }
#endif

#endif
