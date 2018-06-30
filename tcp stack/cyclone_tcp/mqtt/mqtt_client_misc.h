/**
 * @file mqtt_client_misc.h
 * @brief Helper functions for MQTT client
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

#ifndef _MQTT_CLIENT_MISC_H
#define _MQTT_CLIENT_MISC_H

//Dependencies
#include "core/net.h"
#include "mqtt/mqtt_client.h"

//C++ guard
#ifdef __cplusplus
   extern "C" {
#endif

//MQTT client related functions
void mqttClientChangeState(MqttClientContext *context, MqttClientState newState);

error_t mqttClientCheckKeepAlive(MqttClientContext *context);

error_t mqttSerializeHeader(uint8_t *buffer, size_t *pos, MqttPacketType type,
   bool_t dup, MqttQosLevel qos, bool_t retain, size_t remainingLen);

error_t mqttSerializeByte(uint8_t *buffer, size_t bufferLen,
   size_t *pos, uint8_t value);

error_t mqttSerializeShort(uint8_t *buffer, size_t bufferLen,
   size_t *pos, uint16_t value);

error_t mqttSerializeString(uint8_t *buffer, size_t bufferLen,
   size_t *pos, const void *string, size_t stringLen);

error_t mqttSerializeData(uint8_t *buffer, size_t bufferLen,
   size_t *pos, const void *data, size_t dataLen);

error_t mqttDeserializeHeader(uint8_t *buffer, size_t bufferLen, size_t *pos,
   MqttPacketType *type, bool_t *dup, MqttQosLevel *qos, bool_t *retain, size_t *remainingLen);

error_t mqttDeserializeByte(uint8_t *buffer, size_t bufferLen,
   size_t *pos, uint8_t *value);

error_t mqttDeserializeShort(uint8_t *buffer, size_t bufferLen,
   size_t *pos, uint16_t *value);

error_t mqttDeserializeString(uint8_t *buffer, size_t bufferLen,
   size_t *pos, char_t **string, size_t *stringLen);

//C++ guard
#ifdef __cplusplus
   }
#endif

#endif
