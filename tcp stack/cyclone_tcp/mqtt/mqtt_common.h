/**
 * @file mqtt_common.h
 * @brief Definitions common to MQTT client and server
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

#ifndef _MQTT_COMMON_H
#define _MQTT_COMMON_H

//Dependencies
#include "core/net.h"

//MQTT port number
#define MQTT_PORT 1883
//MQTT over SSL/TLS port number
#define MQTT_TLS_PORT 8883

//MQTT 3.1 protocol name
#define MQTT_PROTOCOL_NAME_3_1 "MQIsdp"
//MQTT 3.1.1 protocol name
#define MQTT_PROTOCOL_NAME_3_1_1 "MQTT"

//Minimum size of MQTT header
#define MQTT_MIN_HEADER_SIZE 2
//Maximum size of MQTT header
#define MQTT_MAX_HEADER_SIZE 5

//C++ guard
#ifdef __cplusplus
   extern "C" {
#endif


/**
 * @brief MQTT protocol level
 */

typedef enum
{
   MQTT_PROTOCOL_LEVEL_3_1   = 3, ///<MQTT version 3.1
   MQTT_PROTOCOL_LEVEL_3_1_1 = 4  ///<MQTT version 3.1.1
} MqttProtocolLevel;


/**
 * @brief Transport protocol
 **/

typedef enum
{
   MQTT_TRANSPORT_PROTOCOL_TCP = 1, ///TCP protocol
   MQTT_TRANSPORT_PROTOCOL_TLS = 2, ///SSL protocol
   MQTT_TRANSPORT_PROTOCOL_WS  = 3, ///WebSocket protocol
   MQTT_TRANSPORT_PROTOCOL_WSS = 4, ///Secure WebSocket protocol
} MqttTransportProtocol;


/**
 * @brief Quality of service level
 **/

typedef enum
{
   MQTT_QOS_LEVEL_0 = 0, ///<At most once delivery
   MQTT_QOS_LEVEL_1 = 1, ///<At least once delivery
   MQTT_QOS_LEVEL_2 = 2  ///<Exactly once delivery
} MqttQosLevel;


/**
 * @brief MQTT control packet type
 **/

typedef enum
{
   MQTT_PACKET_TYPE_INVALID     = 0,  ///<Invalid packet
   MQTT_PACKET_TYPE_CONNECT     = 1,  ///<Client request to connect to server
   MQTT_PACKET_TYPE_CONNACK     = 2,  ///<Connect acknowledgment
   MQTT_PACKET_TYPE_PUBLISH     = 3,  ///<Publish message
   MQTT_PACKET_TYPE_PUBACK      = 4,  ///<Publish acknowledgment
   MQTT_PACKET_TYPE_PUBREC      = 5,  ///<Publish received (assured delivery part 1)
   MQTT_PACKET_TYPE_PUBREL      = 6,  ///<Publish release (assured delivery part 2)
   MQTT_PACKET_TYPE_PUBCOMP     = 7,  ///<Publish complete (assured delivery part 3)
   MQTT_PACKET_TYPE_SUBSCRIBE   = 8,  ///<Client subscribe request
   MQTT_PACKET_TYPE_SUBACK      = 9,  ///<Subscribe acknowledgment
   MQTT_PACKET_TYPE_UNSUBSCRIBE = 10, ///<Unsubscribe request
   MQTT_PACKET_TYPE_UNSUBACK    = 11, ///<Unsubscribe acknowledgment
   MQTT_PACKET_TYPE_PINGREQ     = 12, ///<Ping request
   MQTT_PACKET_TYPE_PINGRESP    = 13, ///<Ping response
   MQTT_PACKET_TYPE_DISCONNECT  = 14  ///<Client is disconnecting
} MqttPacketType;


/**
 * @brief Connect flags
 **/

typedef enum
{
   MQTT_CONNECT_FLAG_CLEAN_SESSION = 0x02,
   MQTT_CONNECT_FLAG_WILL          = 0x04,
   MQTT_CONNECT_FLAG_WILL_QOS_0    = 0x00,
   MQTT_CONNECT_FLAG_WILL_QOS_1    = 0x08,
   MQTT_CONNECT_FLAG_WILL_QOS_2    = 0x10,
   MQTT_CONNECT_FLAG_WILL_RETAIN   = 0x20,
   MQTT_CONNECT_FLAG_PASSWORD      = 0x40,
   MQTT_CONNECT_FLAG_USERNAME      = 0x80
} MqttConnectFlags;


/**
 * @brief Connect Acknowledge flags
 **/

typedef enum
{
   MQTT_CONNECT_ACK_FLAG_SESSION_PRESENT = 0x01
} MqttConnectAckFlags;


/**
 * @brief Connect return codes
 **/

typedef enum
{
   MQTT_CONNECT_RET_CODE_ACCEPTED             = 0,
   MQTT_CONNECT_RET_CODE_UNACCEPTABLE_VERSION = 1,
   MQTT_CONNECT_RET_CODE_ID_REJECTED          = 2,
   MQTT_CONNECT_RET_CODE_SERVER_UNAVAILABLE   = 3,
   MQTT_CONNECT_RET_CODE_BAD_USER_NAME        = 4,
   MQTT_CONNECT_RET_CODE_NOT_AUTHORIZED       = 5
} MqttConnectRetCode;


//CodeWarrior or Win32 compiler?
#if defined(__CWCC__) || defined(_WIN32)
   #pragma pack(push, 1)
#endif


/**
 * @brief Fixed header
 **/

typedef __start_packed struct
{
#ifdef _CPU_BIG_ENDIAN
   uint8_t type : 4;   //0
   uint8_t dup : 1;
   uint8_t qos : 2;
   uint8_t retain : 1;
#else
   uint8_t retain : 1; //0
   uint8_t qos : 2;
   uint8_t dup : 1;
   uint8_t type : 4;
#endif
   uint8_t length[];   //1
} __end_packed MqttPacketHeader;


/**
 * @brief UTF-8 encoded string
 **/

typedef __start_packed struct
{
   uint16_t length; //0-1
   uint8_t data[];  //2
} __end_packed MqttString;


//CodeWarrior or Win32 compiler?
#if defined(__CWCC__) || defined(_WIN32)
   #pragma pack(pop)
#endif

//C++ guard
#ifdef __cplusplus
   }
#endif

#endif
