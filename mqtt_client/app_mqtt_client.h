#ifndef __APP_MQTT_CLIENT_H__
#define __APP_MQTT_CLIENT_H__

#include "core/net.h"
#include "error.h"

#define MQTT_SEND_TEST_MSG  DISABLED

#define JSON_LIB        CJSON
//MQTT server name
#define APP_SERVER_NAME "iot.eclipse.org"

//MQTT server port
#define APP_SERVER_PORT 1883   //MQTT over TCP

#define MQTT_EVENT_TOPIC                "DAQ/event"
#define MQTT_RESPONSE_TOPIC             "DAQ/response"

#define MQTT_CLIENT_QUEUE_SIZE          8
#define MQTT_CLIENT_TOPIC_MAX_SIZE      50
#define MQTT_CLIENT_MSG_MAX_SIZE        512
#define MQTT_RECV_TASK_STACK_SIZE       1024
#define MQTT_CLIENT_TASK_STACK_SIZE     512

typedef struct {
    char message[MQTT_CLIENT_MSG_MAX_SIZE + 1];
    uint16_t size;
} mqtt_recv_buffer_t;

typedef struct {
    char message[MQTT_CLIENT_MSG_MAX_SIZE + 1];
    char topic[MQTT_CLIENT_TOPIC_MAX_SIZE + 1];
} mqtt_pub_buffer_t;

error_t mqttConnect(NetInterface *interface);
void mqttPublishMsg(char* topic, char* message, uint16_t msgSize);
void mqttClientTask (void *param);
#endif