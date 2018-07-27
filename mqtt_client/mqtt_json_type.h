/*
 * mqtt_json_type.h
 *
 *  Created on: Jul 25, 2018
 *      Author: chaunm
 */

#ifndef MQTT_JSON_TYPE_H_
#define MQTT_JSON_TYPE_H_

typedef enum
{
	MQTT_PARSE_SUCCESS	= 0,
	MQTT_PARSE_MESSAGE_ERROR,
	MQTT_PARSE_BOXID_ERROR,
	MQTT_PARSE_MSGID_ERROR,
	MQTT_PARSE_TYPE_ERROR,
	MQTT_PARSE_PARAM_ERROR,
	MQTT_PARSE_DATA_ERROR
} mqtt_json_result_t;

#endif /* MQTT_JSON_TYPE_H_ */
