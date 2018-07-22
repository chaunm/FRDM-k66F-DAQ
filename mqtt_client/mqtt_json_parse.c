/*
 * mqtt_json_parse.c
 *
 *  Created on: Jul 23, 2018
 *      Author: chaunm
 */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "mqtt_json_parse.h"

/* Make response data */
static char* mqtt_json_make_response(char* boxID, unsigned int messageID, char* result, mqtt_json_parse_result_t errorCode)
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
	cJSON_AddStringToObject(jsonResponse, "result", result);
	cJSON_AddNumberToObject(jsonResponse, "error_code", errorCode);
	responseMessage = cJSON_Print(jsonResponse);
	cJSON_Delete(jsonResponse);
	return responseMessage;
}

/* Parse configuration message */
static mqtt_json_parse_result_t mqtt_json_parse_configure_message(cJSON* jsonMessage)
{
	cJSON *jsonParameter;
	cJSON *jsonMsgData;
	jsonParameter = cJSON_GetObjectItem(jsonMessage, "parameter");
	if (cJSON_IsString(jsonParameter) && (jsonParameter->valuestring != NULL))
	{
		if (!strcmp(jsonParameter->valuestring, "id"))
		{
			jsonMsgData = cJSON_GetObjectItem(jsonMessage, "data");
			if (cJSON_IsString(jsonMsgData) && (jsonMsgData->valuestring != NULL))
			{
				printf("Configure param: %s, value: %s\r\n", jsonParameter->valuestring, jsonMsgData->valuestring);
			}
		}
		else if (!strcmp(jsonParameter->valuestring, "device_ip"))
		{
			jsonMsgData = cJSON_GetObjectItem(jsonMessage, "data");
			if (cJSON_IsString(jsonMsgData) && (jsonMsgData->valuestring != NULL))
			{
				printf("Configure param: %s, value: %s\r\n", jsonParameter->valuestring, jsonMsgData->valuestring);
			}
		}
		else if (!strcmp(jsonParameter->valuestring, "subnet_mask"))
		{
			jsonMsgData = cJSON_GetObjectItem(jsonMessage, "data");
			if (cJSON_IsString(jsonMsgData) && (jsonMsgData->valuestring != NULL))
			{
				printf("Configure param: %s, value: %s\r\n", jsonParameter->valuestring, jsonMsgData->valuestring);
			}
		}
		else if (!strcmp(jsonParameter->valuestring, "gateway"))
		{
			jsonMsgData = cJSON_GetObjectItem(jsonMessage, "data");
			if (cJSON_IsString(jsonMsgData) && (jsonMsgData->valuestring != NULL))
			{
				printf("Configure param: %s, value: %s\r\n", jsonParameter->valuestring, jsonMsgData->valuestring);
			}
		}
		else if (!strcmp(jsonParameter->valuestring, "server_ip"))
		{
			jsonMsgData = cJSON_GetObjectItem(jsonMessage, "data");
			if (cJSON_IsString(jsonMsgData) && (jsonMsgData->valuestring != NULL))
			{
				printf("Configure param: %s, value: %s\r\n", jsonParameter->valuestring, jsonMsgData->valuestring);
			}
		}
		else
			return MQTT_PARSE_PARAM_ERROR;
	}
	else
		return MQTT_PARSE_PARAM_ERROR;
	return MQTT_PARSE_SUCCESS;
}

/* parse control message */
static mqtt_json_parse_result_t mqtt_json_parse_control_message(cJSON* jsonMessage)
{
	cJSON *jsonParameter;
	cJSON *jsonMsgData;
	jsonParameter = cJSON_GetObjectItem(jsonMessage, "parameter");
	if (cJSON_IsString(jsonParameter) && (jsonParameter->valuestring != NULL))
	{
		if (!strcmp(jsonParameter->valuestring, "door"))
		{
			jsonMsgData = cJSON_GetObjectItem(jsonMessage, "data");
			if (cJSON_IsString(jsonMsgData) && (jsonMsgData->valuestring != NULL))
			{
				printf("Configure param: %s, value: %s\r\n", jsonParameter->valuestring, jsonMsgData->valuestring);
			}
		}
		else if (!strcmp(jsonParameter->valuestring, "alarm"))
		{
			jsonMsgData = cJSON_GetObjectItem(jsonMessage, "data");
			if (cJSON_IsString(jsonMsgData) && (jsonMsgData->valuestring != NULL))
			{
				printf("Configure param: %s, value: %s\r\n", jsonParameter->valuestring, jsonMsgData->valuestring);
			}
		}
		else
			return MQTT_PARSE_PARAM_ERROR;
	}
	else
		return MQTT_PARSE_PARAM_ERROR;
	return MQTT_PARSE_SUCCESS;
}

/* Parse message receive from MQTT input topic */
void mqtt_json_parse_message(char* message, unsigned int length)
{
	cJSON *jsonMessage;
	cJSON *jsonBoxId;
	cJSON *jsonMsgId;
	cJSON *jsonMsgType;
	mqtt_json_parse_result_t result = MQTT_PARSE_SUCCESS;

	char* responseMessage;
	if ((message == NULL) || (length == 0))
		return;
	char* procBuffer = (char*)malloc(length + 1);
	memcpy(procBuffer, message, length);
	procBuffer[length] = 0;
	jsonMessage = cJSON_Parse(procBuffer);
	if (jsonMessage == NULL)
	{
		result = MQTT_PARSE_MESSAGE_ERROR;
		goto END_PARSE;
	}
	jsonBoxId = cJSON_GetObjectItem(jsonMessage, "id");
	if(cJSON_IsString(jsonBoxId) && (jsonBoxId->valuestring != NULL))
	{
		printf("BOX ID: %s\r\n", jsonBoxId->valuestring);
	}
	else
	{
		result = MQTT_PARSE_BOXID_ERROR;
		goto END_PARSE;
	}
	jsonMsgId = cJSON_GetObjectItem(jsonMessage, "message_id");
	if(cJSON_IsNumber(jsonMsgId))
	{
		printf("Message ID: %d\r\n", jsonMsgId->valueint);
	}
	else
	{
		result = MQTT_PARSE_MSGID_ERROR;
		goto END_PARSE;
	}
	jsonMsgType = cJSON_GetObjectItem(jsonMessage, "type");
	if (cJSON_IsString(jsonMsgType) && (jsonMsgType->valuestring != NULL))
	{
		printf("Message Type: %s\r\n", jsonMsgType->valuestring);
	}
	else
	{
		result = MQTT_PARSE_TYPE_ERROR;
		goto END_PARSE;
	}
	if (!strcmp(jsonMsgType->valuestring, "config"))
	{
		printf("Parse configuration message\r\n");
		result = mqtt_json_parse_configure_message(jsonMessage);
	}
	else if (!strcmp(jsonMsgType->valuestring, "control"))
	{
		printf("Parse control message \r\n");
		result = mqtt_json_parse_control_message(jsonMessage);
	}
	else
		result = MQTT_PARSE_TYPE_ERROR;

	END_PARSE:
	if ((result != MQTT_PARSE_MESSAGE_ERROR) && (result != MQTT_PARSE_BOXID_ERROR) && (result != MQTT_PARSE_MSGID_ERROR))
	{
		if (result == MQTT_PARSE_SUCCESS)
			responseMessage = mqtt_json_make_response(jsonBoxId->valuestring, jsonMsgId->valueint, "succeed", result);
		else
			responseMessage = mqtt_json_make_response(jsonBoxId->valuestring, jsonMsgId->valueint, "failure", result);
		if (responseMessage != NULL)
		{
			printf ("response:\n%s\r\n", responseMessage);
			free(responseMessage);
		}
	}
	cJSON_Delete(jsonMessage);
	free(procBuffer);
}
