/**
 * @file esp32_wifi_driver.h
 * @brief ESP32 Wi-Fi controller
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

#ifndef _ESP32_WIFI_DRIVER_H
#define _ESP32_WIFI_DRIVER_H

//Dependencies
#include "core/nic.h"

//C++ guard
#ifdef __cplusplus
   extern "C" {
#endif

//ESP32 Wi-Fi driver (STA mode)
extern const NicDriver esp32WifiStaDriver;
//ESP32 Wi-Fi driver (AP mode)
extern const NicDriver esp32WifiApDriver;

//ESP32 Wi-Fi related functions
error_t esp32WifiInit(NetInterface *interface);

void esp32WifiTick(NetInterface *interface);

void esp32WifiEnableIrq(NetInterface *interface);
void esp32WifiDisableIrq(NetInterface *interface);
void esp32WifiEventHandler(NetInterface *interface);

error_t esp32WifiSendPacket(NetInterface *interface,
   const NetBuffer *buffer, size_t offset);

error_t esp32WifiSetMulticastFilter(NetInterface *interface);

//C++ guard
#ifdef __cplusplus
   }
#endif

#endif
