/*
 * mqtt_json_make.h
 *
 *  Created on: Jul 25, 2018
 *      Author: chaunm
 */

#ifndef MQTT_JSON_MAKE_H_
#define MQTT_JSON_MAKE_H_
#include <stdint.h>
#include "mqtt_json_type.h"

char* mqtt_json_make_online_message(char* boxID);
char* mqtt_json_make_response(char* boxID, unsigned int messageID, mqtt_json_result_t errorCode);
char* mqtt_json_make_fw_update_result(char* boxID, char* serverIP, char* fileName, int32_t result);
#endif /* MQTT_JSON_MAKE_H_ */
