#include "variables.h"
#include "app_mqtt_client.h"
#include "os_port_config.h"
#include "core/net.h"
#include "mqtt/mqtt_client.h"
#include "yarrow.h"
#include "debug.h"
#include "snmpConnect_manager.h"
#include "cJSON.h"
#include <string.h>

//Connection states
#define APP_STATE_NOT_CONNECTED 0
#define APP_STATE_CONNECTING    1
#define APP_STATE_CONNECTED     2

// Global variables
MqttClientContext mqttClientContext;
uint8_t mqttConnectionState = APP_STATE_NOT_CONNECTED;

cJSON* jsonMessage = NULL;
cJSON* jsonStatus = NULL;
cJSON* jsonName = NULL;
char* publishMessage;

#if APP_SERVER_PORT == 8883
/**
* @brief SSL/TLS initialization callback
* @param[in] context Pointer to the MQTT client context
* @param[in] tlsContext Pointer to the SSL/TLS context
* @return Error code
**/
error_t mqttTlsInitCallback(MqttClientContext *context,
                            TlsContext *tlsContext)
{
  error_t error;
  
  //Debug message
  TRACE_INFO("MQTT: TLS initialization callback\r\n");
  
  //Set the PRNG algorithm to be used
  error = tlsSetPrng(tlsContext, YARROW_PRNG_ALGO, &yarrowContext);
  //Any error to report?
  if(error)
    return error;
  
  //Set the fully qualified domain name of the server
  error = tlsSetServerName(tlsContext, APP_SERVER_NAME);
  //Any error to report?
  if(error)
    return error;
  
  //Import the list of trusted CA certificates
  error = tlsSetTrustedCaList(tlsContext, trustedCaList, strlen(trustedCaList));
  //Any error to report?
  if(error)
    return error;
  
  //Successful processing
  return NO_ERROR;
}

#endif //(APP_SERVER_PORT == 8883)
/**
* @brief Publish callback function
* @param[in] context Pointer to the MQTT client context
* @param[in] topic Topic name
* @param[in] message Message payload
* @param[in] length Length of the message payload
* @param[in] dup Duplicate delivery of the PUBLISH packet
* @param[in] qos QoS level used to publish the message
* @param[in] retain This flag specifies if the message is to be retained
* @param[in] packetId Packet identifier
**/

void mqttPublishCallback(MqttClientContext *context,
                         const char_t *topic, const uint8_t *message, size_t length,
                         bool_t dup, MqttQosLevel qos, bool_t retain, uint16_t packetId)
{
  //Debug message
  TRACE_INFO("PUBLISH packet received...\r\n");
  TRACE_INFO("  Dup: %u\r\n", dup);
  TRACE_INFO("  QoS: %u\r\n", qos);
  TRACE_INFO("  Retain: %u\r\n", retain);
  TRACE_INFO("  Packet Identifier: %u\r\n", packetId);
  TRACE_INFO("  Topic: %s\r\n", topic);
  TRACE_INFO("  Message (%" PRIuSIZE " bytes):\r\n", length);
  for (int i = 0; i < length; i ++)
    TRACE_INFO("%c", message[i]);
  TRACE_INFO("\r\n");  
  //Check topic name
  
}


/**
* @brief Establish MQTT connection
**/
error_t mqttConnect(NetInterface *interface)
{
  error_t error;
  IpAddr ipAddr;
  MqttClientCallbacks mqttClientCallbacks;
  
  //Debug message
  TRACE_INFO("\r\n\r\nResolving server name...\r\n");
  
  //Resolve MQTT server name
  error = getHostByName(interface, APP_SERVER_NAME, &ipAddr, 0);
  //Any error to report?
  if(error)
    return error;
  
#if (APP_SERVER_PORT == 80 || APP_SERVER_PORT == 443)
  //Register RNG callback
  webSocketRegisterRandCallback(webSocketRngCallback);
#endif
  
  //Initialize MQTT client context
  mqttClientInit(&mqttClientContext);
  // set interface to connect with MQTT
  mqttClientContext.interface = interface;
  //Initialize MQTT client callbacks
  mqttClientInitCallbacks(&mqttClientCallbacks);
  
  //Attach application-specific callback functions
  mqttClientCallbacks.publishCallback = mqttPublishCallback;
#if (APP_SERVER_PORT == 8883 || APP_SERVER_PORT == 443)
  mqttClientCallbacks.tlsInitCallback = mqttTlsInitCallback;
#endif
  
  //Register MQTT client callbacks
  mqttClientRegisterCallbacks(&mqttClientContext, &mqttClientCallbacks);
  
  //Set the MQTT version to be used
  mqttClientSetProtocolLevel(&mqttClientContext,
                             MQTT_PROTOCOL_LEVEL_3_1_1);
  
#if (APP_SERVER_PORT == 1883)
  //MQTT over TCP
  mqttClientSetTransportProtocol(&mqttClientContext, MQTT_TRANSPORT_PROTOCOL_TCP);
#elif (APP_SERVER_PORT == 8883)
  //MQTT over SSL/TLS
  mqttClientSetTransportProtocol(&mqttClientContext, MQTT_TRANSPORT_PROTOCOL_TLS);
#elif (APP_SERVER_PORT == 80)
  //MQTT over WebSocket
  mqttClientSetTransportProtocol(&mqttClientContext, MQTT_TRANSPORT_PROTOCOL_WS);
#elif (APP_SERVER_PORT == 443)
  //MQTT over secure WebSocket
  mqttClientSetTransportProtocol(&mqttClientContext, MQTT_TRANSPORT_PROTOCOL_WSS);
#endif
  
  //Set keep-alive value
  mqttClientSetKeepAlive(&mqttClientContext, 3600);
  
#if (APP_SERVER_PORT == 80 || APP_SERVER_PORT == 443)
  //Set the hostname of the resource being requested
  mqttClientSetHost(&mqttClientContext, APP_SERVER_NAME);
  //Set the name of the resource being requested
  mqttClientSetUri(&mqttClientContext, APP_SERVER_URI);
#endif
  //Set client identifier
  mqttClientSetIdentifier(&mqttClientContext, deviceName);
  
  //Set user name and password
  //mqttClientSetAuthInfo(&mqttClientContext, "username", "password");
  
  //Set Will message
  mqttClientSetWillMessage(&mqttClientContext, "board/status",
                           "offline", 7, MQTT_QOS_LEVEL_0, FALSE);
  
  //Debug message
  TRACE_INFO("Connecting to MQTT server %s...\r\n", ipAddrToString(&ipAddr, NULL));
  
  //Start of exception handling block
  do
  {
    //Establish connection with the MQTT server
    error = mqttClientConnect(&mqttClientContext,
                              &ipAddr, APP_SERVER_PORT, TRUE);
    //Any error to report?
    if(error)
      break;
    
    //Subscribe to the desired topics
    error = mqttClientSubscribe(&mqttClientContext,
                                "board/status", MQTT_QOS_LEVEL_1, NULL);
    //Any error to report?
    if(error)
      break;
    
    //Send PUBLISH packet
    error = mqttClientPublish(&mqttClientContext, "board/status",
                              "online", 6, MQTT_QOS_LEVEL_1, TRUE, NULL);
    //Any error to report?
    if(error)
      break;
    
    //End of exception handling block
  } while(0);
  
  //Check status code
  if(error)
  {
    //Close connection
    mqttClientClose(&mqttClientContext);
  }
  
  //Return status code
  return error;
}

void mqttTestTask (void *param)
{
  error_t error;   
  //Endless loop
  while(1)
  {
    //Check connection state
    if(mqttConnectionState == APP_STATE_NOT_CONNECTED)
    {
      //Update connection state
      mqttConnectionState = APP_STATE_CONNECTING;
      
      //Try to connect to the MQTT server using ppp interface
      if (interfaceManagerGetActiveInterface() != NULL)
        error = mqttConnect(interfaceManagerGetActiveInterface());
      else
        error = ERROR_FAILURE;
      
      //Failed to connect to the MQTT server?
      if(error)
      {
        //Update connection state
        mqttConnectionState = APP_STATE_NOT_CONNECTED;
        //Recovery delay
        osDelayTask(5000);
      }
      else
      {
        //Update connection state
        mqttConnectionState = APP_STATE_CONNECTED;
      }
    }
    else
    {
      //Process incoming events
      error = mqttClientProcessEvents(&mqttClientContext, 100);
      
      //Connection to MQTT server lost?
      if(error != NO_ERROR && error != ERROR_TIMEOUT)
      {
        //Close connection
        mqttClientClose(&mqttClientContext);
        //Update connection state
        mqttConnectionState = APP_STATE_NOT_CONNECTED;
        //Recovery delay
        osDelayTask(2000);
      }
      else
      {
        //Initialize status code
        error = NO_ERROR;           
        //Send PUBLISH packet    
        jsonMessage = cJSON_CreateObject();
        jsonName = cJSON_CreateString(deviceName);
        cJSON_AddItemToObject(jsonMessage, "name", jsonName);
        jsonStatus = cJSON_CreateString("active");
        cJSON_AddItemToObject(jsonMessage, "status", jsonStatus);
        publishMessage = cJSON_Print(jsonMessage);
        cJSON_Delete(jsonMessage);

        error = mqttClientPublish(&mqttClientContext, "board/status",
                                  publishMessage, strlen(publishMessage), MQTT_QOS_LEVEL_1, TRUE, NULL);
        free(publishMessage);
        //Failed to publish data?
        if(error)
        {
          //Close connection
          mqttClientClose(&mqttClientContext);
          //Update connection state
          mqttConnectionState = APP_STATE_NOT_CONNECTED;
          //Recovery delay
          osDelayTask(10000);
        }
        osDelayTask(3000);
      }
    }
  }
}