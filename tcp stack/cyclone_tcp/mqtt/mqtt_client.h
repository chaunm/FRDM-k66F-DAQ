/**
 * @file mqtt_client.h
 * @brief MQTT client
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

#ifndef _MQTT_CLIENT_H
#define _MQTT_CLIENT_H

//Dependencies
#include "core/net.h"
#include "mqtt/mqtt_common.h"

//MQTT client support
#ifndef MQTT_CLIENT_SUPPORT
   #define MQTT_CLIENT_SUPPORT ENABLED
#elif (MQTT_CLIENT_SUPPORT != ENABLED && MQTT_CLIENT_SUPPORT != DISABLED)
   #error MQTT_CLIENT_SUPPORT parameter is not valid
#endif

//MQTT over SSL/TLS
#ifndef MQTT_CLIENT_TLS_SUPPORT
   #define MQTT_CLIENT_TLS_SUPPORT DISABLED
#elif (MQTT_CLIENT_TLS_SUPPORT != ENABLED && MQTT_CLIENT_TLS_SUPPORT != DISABLED)
   #error MQTT_CLIENT_TLS_SUPPORT parameter is not valid
#endif

//MQTT over WebSocket
#ifndef MQTT_CLIENT_WS_SUPPORT
   #define MQTT_CLIENT_WS_SUPPORT DISABLED
#elif (MQTT_CLIENT_WS_SUPPORT != ENABLED && MQTT_CLIENT_WS_SUPPORT != DISABLED)
   #error MQTT_CLIENT_WS_SUPPORT parameter is not valid
#endif

//Default keep-alive time interval, in seconds
#ifndef MQTT_CLIENT_DEFAULT_KEEP_ALIVE
   #define MQTT_CLIENT_DEFAULT_KEEP_ALIVE 0
#elif (MQTT_CLIENT_DEFAULT_KEEP_ALIVE < 0)
   #error MQTT_CLIENT_DEFAULT_KEEP_ALIVE parameter is not valid
#endif

//Default communication timeout, in milliseconds
#ifndef MQTT_CLIENT_DEFAULT_TIMEOUT
   #define MQTT_CLIENT_DEFAULT_TIMEOUT 20000
#elif (MQTT_CLIENT_DEFAULT_TIMEOUT < 0)
   #error MQTT_CLIENT_DEFAULT_TIMEOUT parameter is not valid
#endif

//Maximum length of the hostname
#ifndef MQTT_CLIENT_MAX_HOST_LEN
   #define MQTT_CLIENT_MAX_HOST_LEN 32
#elif (MQTT_CLIENT_MAX_HOST_LEN < 1)
   #error MQTT_CLIENT_MAX_HOST_LEN parameter is not valid
#endif

//Maximum length of the resource name
#ifndef MQTT_CLIENT_MAX_URI_LEN
   #define MQTT_CLIENT_MAX_URI_LEN 16
#elif (MQTT_CLIENT_MAX_URI_LEN < 1)
   #error MQTT_CLIENT_MAX_URI_LEN parameter is not valid
#endif

//Maximum length of the client identifier
#ifndef MQTT_CLIENT_MAX_ID_LEN
   #define MQTT_CLIENT_MAX_ID_LEN 16
#elif (MQTT_CLIENT_MAX_ID_LEN < 0)
   #error MQTT_CLIENT_MAX_ID_LEN parameter is not valid
#endif

//Maximum length of the user name
#ifndef MQTT_CLIENT_MAX_USERNAME_LEN
   #define MQTT_CLIENT_MAX_USERNAME_LEN 16
#elif (MQTT_CLIENT_MAX_USERNAME_LEN < 0)
   #error MQTT_CLIENT_MAX_USERNAME_LEN parameter is not valid
#endif

//Maximum length of the password
#ifndef MQTT_CLIENT_MAX_PASSWORD_LEN
   #define MQTT_CLIENT_MAX_PASSWORD_LEN 16
#elif (MQTT_CLIENT_MAX_PASSWORD_LEN < 0)
   #error MQTT_CLIENT_MAX_PASSWORD_LEN parameter is not valid
#endif

//Maximum length of the will topic
#ifndef MQTT_CLIENT_MAX_WILL_TOPIC_LEN
   #define MQTT_CLIENT_MAX_WILL_TOPIC_LEN 16
#elif (MQTT_CLIENT_MAX_WILL_TOPIC_LEN < 0)
   #error MQTT_CLIENT_MAX_WILL_TOPIC_LEN parameter is not valid
#endif

//Maximum length of the will message payload
#ifndef MQTT_CLIENT_MAX_WILL_PAYLOAD_LEN
   #define MQTT_CLIENT_MAX_WILL_PAYLOAD_LEN 16
#elif (MQTT_CLIENT_MAX_WILL_PAYLOAD_LEN < 0)
   #error MQTT_CLIENT_MAX_WILL_PAYLOAD_LEN parameter is not valid
#endif

//Size of the MQTT client buffer
#ifndef MQTT_CLIENT_BUFFER_SIZE
   #define MQTT_CLIENT_BUFFER_SIZE 1024
#elif (MQTT_CLIENT_BUFFER_SIZE < 1)
   #error MQTT_CLIENT_BUFFER_SIZE parameter is not valid
#endif

//SSL/TLS supported?
#if (MQTT_CLIENT_TLS_SUPPORT == ENABLED)
   #include "core/crypto.h"
   #include "tls.h"
#endif

//WebSocket supported?
#if (MQTT_CLIENT_WS_SUPPORT == ENABLED)
   #include "web_socket/web_socket.h"
#endif

//Forward declaration of MqttClientContext structure
struct _MqttClientContext;
#define MqttClientContext struct _MqttClientContext

//C++ guard
#ifdef __cplusplus
   extern "C" {
#endif


/**
 * @brief MQTT client states
 **/

typedef enum
{
   MQTT_CLIENT_STATE_CLOSED              = 0,
   MQTT_CLIENT_STATE_CONNECTING          = 1,
   MQTT_CLIENT_STATE_CONNECTED           = 2,
   MQTT_CLIENT_STATE_IDLE                = 3,
   MQTT_CLIENT_STATE_SENDING_PACKET      = 4,
   MQTT_CLIENT_STATE_PACKET_SENT         = 5,
   MQTT_CLIENT_STATE_WAITING_PACKET      = 6,
   MQTT_CLIENT_STATE_RECEIVING_PACKET    = 7,
   MQTT_CLIENT_STATE_PACKET_RECEIVED     = 8,
   MQTT_CLIENT_STATE_DISCONNECTING       = 9,
   MQTT_CLIENT_STATE_DISCONNECTED        = 10
} MqttClientState;


/**
 * @brief CONNACK message received callback
 **/

typedef void (*MqttClientConnAckCallback)(MqttClientContext *context,
   uint8_t connectAckFlags, uint8_t connectReturnCode);


/**
 * @brief PUBLISH message received callback
 **/

typedef void (*MqttClientPublishCallback)(MqttClientContext *context,
   const char_t *topic, const uint8_t *message, size_t length,
   bool_t dup, MqttQosLevel qos, bool_t retain, uint16_t packetId);


/**
 * @brief PUBACK message received callback
 **/

typedef void (*MqttClientPubAckCallback)(MqttClientContext *context,
   uint16_t packetId);


/**
 * @brief PUBREC message received callback
 **/

typedef void (*MqttClientPubRecCallback)(MqttClientContext *context,
   uint16_t packetId);


/**
 * @brief PUBREL message received callback
 **/

typedef void (*MqttClientPubRelCallback)(MqttClientContext *context,
   uint16_t packetId);


/**
 * @brief PUBCOMP message received callback
 **/

typedef void (*MqttClientPubCompCallback)(MqttClientContext *context,
   uint16_t packetId);


/**
 * @brief SUBACK message received callback
 **/

typedef void (*MqttClientSubAckCallback)(MqttClientContext *context,
   uint16_t packetId);


/**
 * @brief UNSUBACK message received callback
 **/

typedef void (*MqttClientUnsubAckCallback)(MqttClientContext *context,
   uint16_t packetId);


/**
 * @brief PINGRESP message received callback
 **/

typedef void (*MqttClientPingRespCallback)(MqttClientContext *context);


//SSL/TLS supported?
#if (MQTT_CLIENT_TLS_SUPPORT == ENABLED)

/**
 * @brief SSL initialization callback
 **/

typedef error_t (*MqttClientTlsInitCallback)(MqttClientContext *context,
   TlsContext *tlsContext);

#endif


/**
 * @brief Will message
 **/

typedef struct
{
   char_t topic[MQTT_CLIENT_MAX_WILL_TOPIC_LEN + 1];  ///<Will topic name
   uint8_t payload[MQTT_CLIENT_MAX_WILL_PAYLOAD_LEN]; ///<Will message payload
   size_t length;                                     ///<Length of the Will message payload
   MqttQosLevel qos;                                  ///<QoS level to be used when publishing the Will message
   bool_t retain;                                     ///<Specifies if the Will message is to be retained
} MqttClientWillMessage;


/**
 * @brief MQTT client callback functions
 **/

typedef struct
{
   MqttClientConnAckCallback connAckCallback;   ///<CONNACK message received callback
   MqttClientPublishCallback publishCallback;   ///<PUBLISH message received callback
   MqttClientPubAckCallback pubAckCallback;     ///<PUBACK message received callback
   MqttClientPubAckCallback pubRecCallback;     ///<PUBREC message received callback
   MqttClientPubAckCallback pubRelCallback;     ///<PUBREL message received callback
   MqttClientPubAckCallback pubCompCallback;    ///<PUBCOMP message received callback
   MqttClientPubAckCallback subAckCallback;     ///<SUBACK message received callback
   MqttClientPubAckCallback unsubAckCallback;   ///<UNSUBACK message received callback
   MqttClientPingRespCallback pingRespCallback; ///<PINGRESP message received callback
#if (MQTT_CLIENT_TLS_SUPPORT == ENABLED)
   MqttClientTlsInitCallback tlsInitCallback;   ///<SSL initialization callback
#endif
} MqttClientCallbacks;


/**
 * @brief MQTT client settings
 **/

typedef struct
{
   MqttProtocolLevel protocolLevel;                   ///<MQTT protocol version
   MqttTransportProtocol transportProtocol;           ///<Transport protocol
   uint16_t keepAlive;                                ///<Keep-alive time interval
   systime_t timeout;                                 ///<Communication timeout
#if (MQTT_CLIENT_WS_SUPPORT == ENABLED)
   char_t host[MQTT_CLIENT_MAX_HOST_LEN + 1];         ///<Hostname of the resource being requested
   char_t uri[MQTT_CLIENT_MAX_URI_LEN + 1];           ///<Resource name
#endif
   char_t clientId[MQTT_CLIENT_MAX_ID_LEN + 1];       ///<Client identifier
   char_t username[MQTT_CLIENT_MAX_USERNAME_LEN + 1]; ///<User name
   char_t password[MQTT_CLIENT_MAX_PASSWORD_LEN + 1]; ///<Password
   MqttClientWillMessage willMessage;                 ///<Will message
} MqttClientSettings;


/**
 * @brief MQTT client context
 **/

struct _MqttClientContext
{
   MqttClientSettings settings;             ///<MQTT client settings
   MqttClientCallbacks callbacks;           ///<MQTT client callback functions
   MqttClientState state;                   ///<MQTT client state
   systime_t keepAliveTimestamp;            ///<Timestamp used to manage keep-alive
   systime_t pingTimestamp;                 ///<Timestamp used to measure round-trip time
   NetInterface *interface;                 ///<Underlying network interface
   Socket *socket;                          ///<Underlying TCP socket
#if (MQTT_CLIENT_TLS_SUPPORT == ENABLED)
   TlsContext *tlsContext;                  ///<SSL context
   TlsSession tlsSession;                   ///<SSL session
#endif
#if (MQTT_CLIENT_WS_SUPPORT == ENABLED)
   WebSocket *webSocket;                    ///<Underlying WebSocket
#endif
   uint8_t buffer[MQTT_CLIENT_BUFFER_SIZE]; ///<Internal buffer
   uint8_t *packet;                         ///<Pointer to the incoming/outgoing MQTT packet
   size_t packetPos;                        ///<Current position
   size_t packetLen;                        ///<Length of the entire MQTT packet
   MqttPacketType packetType;               ///<Control packet type
   uint16_t packetId;                       ///<Packet identifier
   size_t remainingLen;                     ///<Length of the variable header and payload
};


//MQTT client related functions
error_t mqttClientInit(MqttClientContext *context);
void mqttClientInitCallbacks(MqttClientCallbacks *callbacks);

error_t mqttClientRegisterCallbacks(MqttClientContext *context,
   const MqttClientCallbacks *callbacks);

error_t mqttClientSetProtocolLevel(MqttClientContext *context,
   MqttProtocolLevel protocolLevel);

error_t mqttClientSetTransportProtocol(MqttClientContext *context,
   MqttTransportProtocol transportProtocol);

error_t mqttClientSetKeepAlive(MqttClientContext *context, uint16_t keepAlive);
error_t mqttClientSetTimeout(MqttClientContext *context, uint16_t timeout);

error_t mqttClientSetHost(MqttClientContext *context, const char_t *host);
error_t mqttClientSetUri(MqttClientContext *context, const char_t *uri);

error_t mqttClientSetIdentifier(MqttClientContext *context,
   const char_t *clientId);

error_t mqttClientSetAuthInfo(MqttClientContext *context,
   const char_t *username, const char_t *password);

error_t mqttClientSetWillMessage(MqttClientContext *context, const char_t *topic,
   const void *message, size_t length, MqttQosLevel qos, bool_t retain);

error_t mqttClientBindToInterface(MqttClientContext *context,
   NetInterface *interface);

error_t mqttClientConnect(MqttClientContext *context,
   const IpAddr *serverIpAddr, uint16_t serverPort, bool_t cleanSession);

error_t mqttClientPublish(MqttClientContext *context,
   const char_t *topic, const void *message, size_t length,
   MqttQosLevel qos, bool_t retain, uint16_t *packetId);

error_t mqttClientSubscribe(MqttClientContext *context,
   const char_t *topic, MqttQosLevel qos, uint16_t *packetId);

error_t mqttClientUnsubscribe(MqttClientContext *context,
   const char_t *topic, uint16_t *packetId);

error_t mqttClientPing(MqttClientContext *context, systime_t *rtt);

error_t mqttClientDisconnect(MqttClientContext *context);
error_t mqttClientClose(MqttClientContext *context);

error_t mqttClientProcessEvents(MqttClientContext *context, systime_t timeout);

//C++ guard
#ifdef __cplusplus
   }
#endif

#endif
