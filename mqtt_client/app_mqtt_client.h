#ifndef __APP_MQTT_CLIENT_H__
#define __APP_MQTT_CLIENT_H__

#include "core/net.h"
#include "error.h"

#define JSON_LIB        CJSON
//MQTT server name
#define APP_SERVER_NAME "iot.eclipse.org"

//MQTT server port
#define APP_SERVER_PORT 1883   //MQTT over TCP

error_t mqttConnect(NetInterface *interface);
void mqttTestTask (void *param);
#endif