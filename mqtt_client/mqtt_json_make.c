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
#include "debug.h"
#include "variables.h"

/* Make online message */
char* mqtt_json_make_online_message(char* boxID)
{
	cJSON *jsonEvent;
	char* eventMsg;
	jsonEvent = cJSON_CreateObject();
	if (jsonEvent == NULL)
	{
		TRACE_INFO("Not enough memory to create json response message\r\n");
		return NULL;
	}
	cJSON_AddStringToObject(jsonEvent, "id", boxID);
    cJSON_AddStringToObject(jsonEvent, "type", "event");
	cJSON_AddStringToObject(jsonEvent, "firmware", __FIRMWARE_VERSION);
	cJSON_AddStringToObject(jsonEvent, "build date", __DATE__);
	cJSON_AddStringToObject(jsonEvent, "build time", __TIME__);
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
		TRACE_INFO("Not enough memory to create json response message\r\n");
		return NULL;
	}
	cJSON_AddStringToObject(jsonResponse, "id", boxID);
	cJSON_AddNumberToObject(jsonResponse, "message_id", messageID);
	cJSON_AddNumberToObject(jsonResponse, "error_code", errorCode);
	responseMessage = cJSON_Print(jsonResponse);
	cJSON_Delete(jsonResponse);
	return responseMessage;
}

/* Make firmware update report data */
char* mqtt_json_make_fw_update_result(char* boxID, char* serverIP, char* fileName, int32_t result)
{
	cJSON* jsonMessage;
	char* responseMessage;
	jsonMessage = cJSON_CreateObject();
	if (jsonMessage == NULL)
	{
		TRACE_INFO("Not enough memory to create json message\r\n");
		return NULL;
	}
	cJSON_AddStringToObject(jsonMessage, "id", boxID);
	cJSON_AddStringToObject(jsonMessage, "type", "firmware_update");
	cJSON_AddStringToObject(jsonMessage, "server", serverIP);
	cJSON_AddStringToObject(jsonMessage, "file", fileName);			
	cJSON_AddNumberToObject(jsonMessage, "error_code", result);
	responseMessage = cJSON_Print(jsonMessage);
	cJSON_Delete(jsonMessage);
	return responseMessage;
		
}