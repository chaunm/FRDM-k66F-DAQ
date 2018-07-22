/*
 * mqtt_json_parse.h
 *
 *  Created on: Jul 23, 2018
 *      Author: chaunm
 */

#ifndef MQTT_JSON_PARSE_H_
#define MQTT_JSON_PARSE_H_
#include "cJSON.h"

typedef enum
{
	MQTT_PARSE_SUCCESS	= 0,
	MQTT_PARSE_MESSAGE_ERROR,
	MQTT_PARSE_BOXID_ERROR,
	MQTT_PARSE_MSGID_ERROR,
	MQTT_PARSE_TYPE_ERROR,
	MQTT_PARSE_PARAM_ERROR,
	MQTT_PARSE_DATA_ERROR
}mqtt_json_parse_result_t;

void mqtt_json_parse_message(char* message, unsigned int length);


#endif /* MQTT_JSON_PARSE_H_ */
