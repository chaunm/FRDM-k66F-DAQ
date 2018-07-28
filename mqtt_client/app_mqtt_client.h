#ifndef __APP_MQTT_CLIENT_H__
#define __APP_MQTT_CLIENT_H__

#include "core/net.h"
#include "error.h"


#define JSON_LIB        CJSON
//MQTT server name
#define APP_SERVER_NAME "iot.eclipse.org"

//MQTT server port
#define APP_SERVER_PORT 1883   //MQTT over TCP

#define MQTT_EVENT_TOPIC                "DAQ/event"
#define MQTT_RESPONSE_TOPIC             "DAQ/response"

#define MQTT_CLIENT_QUEUE_SIZE          8
#define MQTT_CLIENT_MSG_MAX_SIZE        512
#define MQTT_RECV_TASK_STACK_SIZE       512
#define MQTT_CLIENT_TASK_STACK_SIZE     512

typedef struct {
    char message[MQTT_CLIENT_MSG_MAX_SIZE + 1];
    uint16_t size;
} mqtt_buffer_t;

error_t mqttConnect(NetInterface *interface);
void mqttClientTask (void *param);
#endif