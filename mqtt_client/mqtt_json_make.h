/*
 * mqtt_json_make.h
 *
 *  Created on: Jul 25, 2018
 *      Author: chaunm
 */

#ifndef MQTT_JSON_MAKE_H_
#define MQTT_JSON_MAKE_H_
#include "mqtt_json_type.h"

char* mqtt_json_make_response(char* boxID, unsigned int messageID, mqtt_json_result_t errorCode);
#endif /* MQTT_JSON_MAKE_H_ */
