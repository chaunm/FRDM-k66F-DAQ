/*
 * mqtt_json_make.c
 *
 *  Created on: Jul 25, 2018
 *      Author: chaunm
 */
#include <stdio.h>
#include <stdlib.h>
#include "mqtt_json_make.h"
#include "mqtt_json_type.h"
#include "cJSON.h"

/* Make online message */
char* mqtt_json_make_online_message(char* boxID)
{
	cJSON *jsonEvent;
	char* eventMsg;
	jsonEvent = cJSON_CreateObject();
	if (jsonEvent == NULL)
	{
		printf("Not enough memory to create json response message\r\n");
		return NULL;
	}
	cJSON_AddStringToObject(jsonEvent, "id", boxID);
    cJSON_AddStringToObject(jsonEvent, "type", "event");
	cJSON_AddStringToObject(jsonEvent, "status", "online");
	eventMsg = cJSON_Print(jsonEvent);
	cJSON_Delete(jsonEvent);
	return eventMsg;
}

/* Make response data */
char* mqtt_json_make_response(char* boxID, unsigned int messageID, mqtt_json_result_t errorCode)
{
	cJSON *jsonResponse;
	char* responseMessage;
	jsonResponse = cJSON_CreateObject();
	if (jsonResponse == NULL)
	{
		printf("Not enough memory to create json response message\r\n");
		return NULL;
	}
	cJSON_AddStringToObject(jsonResponse, "id", boxID);
	cJSON_AddNumberToObject(jsonResponse, "message_id", messageID);
	cJSON_AddNumberToObject(jsonResponse, "error_code", errorCode);
	responseMessage = cJSON_Print(jsonResponse);
	cJSON_Delete(jsonResponse);
	return responseMessage;
}
