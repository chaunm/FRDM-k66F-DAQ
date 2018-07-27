/*
* mqtt_json_parse.c
*
*  Created on: Jul 23, 2018
*      Author: chaunm
*/
#include <string.h>
#include <stdlib.h>
#include "debug.h"
#include "mqtt_json_parse.h"
#include "mqtt_json_make.h"
#include "mqtt_json_type.h"
#include "cJSON.h"
#include "variables.h"
#include "i2c_lock.h"
#include "task.h"
#include "access_control.h"
#include "core/net.h"

/***********************************************************************************************************
*                                        CONFIGURE MESSAGE PARSING                                        *
***********************************************************************************************************/
/* parse temperature threshold */
static mqtt_json_result_t mqtt_json_parse_conigure_temp_threshold(cJSON* jsonMessage)
{
	cJSON* tempThresJson;
	cJSON* tempJson;
	cJSON* indexJson;
	uint8_t tempIndex;
	tempThresJson = cJSON_GetObjectItem(jsonMessage, "data");
	if (!cJSON_IsObject(tempThresJson))
		return MQTT_PARSE_DATA_ERROR;
	indexJson = cJSON_GetObjectItem(tempThresJson,  "index");
	if (!cJSON_IsNumber(indexJson))
		return MQTT_PARSE_DATA_ERROR;
	if ((indexJson->valueint > 4) || (indexJson->valueint < 1))
		return MQTT_PARSE_DATA_ERROR;
	tempJson = cJSON_GetObjectItem(tempThresJson,  "temperature");
	if (!cJSON_IsNumber(tempJson))
		return MQTT_PARSE_DATA_ERROR;
	TRACE_INFO("Temperature threshold index: %d, value %d\r\n", indexJson->valueint, tempJson->valueint);
	//write value to eeprom
	tempIndex = indexJson->valueint - 1;
	sMenu_Variable.u16ThresTemp[tempIndex] = tempJson->valueint; 
	I2C_Get_Lock();
    vTaskSuspendAll();
    WriteEEPROM_Word(sSetting_Values[_TEMP1 + tempIndex].addrEEPROM,sMenu_Variable.u16ThresTemp[tempIndex]); 
	xTaskResumeAll();
	I2C_Release_Lock();
	return MQTT_PARSE_SUCCESS;
}
/* Parse card ID configuration message */
static mqtt_json_result_t mqtt_json_parse_configure_card_id(cJSON* jsonMessage)
{
	cJSON* cardIdJson;
	cJSON* indexJson;
	cJSON* cardJson;
	cardIdJson = cJSON_GetObjectItem(jsonMessage, "data");
	if (!cJSON_IsObject(cardIdJson))
		return MQTT_PARSE_DATA_ERROR;
	indexJson = cJSON_GetObjectItem(cardIdJson, "index");
	if (!cJSON_IsNumber(indexJson))
		return MQTT_PARSE_DATA_ERROR;
	if ((indexJson->valueint > 5) || (indexJson->valueint < 1))
		return MQTT_PARSE_DATA_ERROR;
	cardJson = cJSON_GetObjectItem(cardIdJson, "card_id");
	if ((!cJSON_IsString(cardJson)) || (cardJson->valuestring == NULL))
		return MQTT_PARSE_DATA_ERROR;
	if (strlen(cardJson->valuestring) != 8)
		return MQTT_PARSE_DATA_ERROR;
	TRACE_INFO("Card Id index: %d, ID: %s\r\n", indexJson->valueint, cardJson->valuestring);
	//write to eeprom
	ACS_SaveUserID(sMenu_Variable.u8UserIDAddr[indexJson->valueint - 1],(uint8_t*)cardJson->valuestring);
	return MQTT_PARSE_SUCCESS;
}

/* Parse battery threshold configuration message */
static mqtt_json_result_t mqtt_json_parse_configure_battery_threshold(cJSON* jsonMessage)
{
	cJSON* batteryThresJson;
	cJSON* indexJson;
	cJSON* batteryVoltJson;
	batteryThresJson = cJSON_GetObjectItem(jsonMessage, "data");
	if (!(cJSON_IsObject(batteryThresJson)))
		return MQTT_PARSE_DATA_ERROR;
	indexJson = cJSON_GetObjectItem(batteryThresJson, "index");
	if (!cJSON_IsNumber(indexJson))
		return MQTT_PARSE_DATA_ERROR;
	if ((indexJson->valueint > 2) || (indexJson->valueint < 1))
		return MQTT_PARSE_DATA_ERROR;
	batteryVoltJson = cJSON_GetObjectItem(batteryThresJson, "voltage");
	if (!cJSON_IsNumber(batteryVoltJson))
		return MQTT_PARSE_DATA_ERROR;
	TRACE_INFO("Battery threshold index: %d, threshold: %d\r\n", indexJson->valueint, batteryVoltJson->valueint);
	// write to eeprom
	sMenu_Variable.u16BattThresVolt[indexJson->valueint - 1] = batteryVoltJson->valueint; 
	I2C_Get_Lock();
    vTaskSuspendAll();
    WriteEEPROM_Word(sSetting_Values[_DC_LOW].addrEEPROM,sMenu_Variable.u16BattThresVolt[indexJson->valueint]);
	xTaskResumeAll();
	I2C_Release_Lock();
	return MQTT_PARSE_SUCCESS;
}

/* Parse aircon set temperature configuration message */
static mqtt_json_result_t mqtt_json_parse_configure_aircon_temperature(cJSON* jsonMessage)
{
	cJSON* acTempJson;
	cJSON* indexJson;
	cJSON* tempJson;
	uint8_t tempIndex;
	acTempJson = cJSON_GetObjectItem(jsonMessage, "data");
	if (!(cJSON_IsObject(acTempJson)))
		return MQTT_PARSE_DATA_ERROR;
	indexJson = cJSON_GetObjectItem(acTempJson, "index");
	if (!cJSON_IsNumber(indexJson))
		return MQTT_PARSE_DATA_ERROR;
	if ((indexJson->valueint > 4) || (indexJson->valueint < 1))
		return MQTT_PARSE_DATA_ERROR;
	tempJson = cJSON_GetObjectItem(acTempJson, "temperature");
	if (!cJSON_IsNumber(tempJson))
		return MQTT_PARSE_DATA_ERROR;
	TRACE_INFO("aircon set temp index: %d, value: %d\r\n", indexJson->valueint, tempJson->valueint);
	// write to eeprom and update display
	tempIndex = indexJson->valueint - 1;
	sMenu_Variable.changedVal = _AIRCON_TEMP1 + tempIndex; 
    sMenu_Variable.u16AirConTemp[tempIndex] = tempJson->valueint; 
    WriteEEPROM_Word(sSetting_Values[_AIRCON_TEMP1 + tempIndex].addrEEPROM,sMenu_Variable.u16AirConTemp[tempIndex]);
	return MQTT_PARSE_SUCCESS;
}

/* Parse configuration message */
static mqtt_json_result_t mqtt_json_parse_configure_message(cJSON* jsonMessage)
{
	cJSON *jsonParameter;
	cJSON *jsonMsgData;
	IpAddr ipAddr;
	jsonParameter = cJSON_GetObjectItem(jsonMessage, "parameter");
	if (cJSON_IsString(jsonParameter) && (jsonParameter->valuestring != NULL))
	{
		if (!strcmp(jsonParameter->valuestring, "id"))
		{
			jsonMsgData = cJSON_GetObjectItem(jsonMessage, "data");
			if (cJSON_IsString(jsonMsgData) && (jsonMsgData->valuestring != NULL))
			{
				TRACE_INFO("Configure param: %s, value: %s\r\n", jsonParameter->valuestring, jsonMsgData->valuestring);
			}
			else
				return MQTT_PARSE_DATA_ERROR;
		}
		else if (!strcmp(jsonParameter->valuestring, "temperature_threshold"))
		{
			return mqtt_json_parse_conigure_temp_threshold(jsonMessage);
		}
		else if (!strcmp(jsonParameter->valuestring, "card_id"))
		{
			return mqtt_json_parse_configure_card_id(jsonMessage);
		}
		else if (!strcmp(jsonParameter->valuestring, "phase_threshold_voltage"))
		{
			jsonMsgData = cJSON_GetObjectItem(jsonMessage, "data");
			if (cJSON_IsNumber(jsonMsgData))
			{
				TRACE_INFO("Configure param: %s, value: %d\r\n", jsonParameter->valuestring, jsonMsgData->valueint);
				// write to EEPROM
				sMenu_Variable.u16AcThresVolt[0] = jsonMsgData->valueint; 
				sMenu_Variable.u16AcThresVolt[1] = jsonMsgData->valueint; 
				sMenu_Variable.u16AcThresVolt[2] = jsonMsgData->valueint; 
				I2C_Get_Lock();
				vTaskSuspendAll();
				WriteEEPROM_Word(sSetting_Values[_AC_LOW].addrEEPROM,sMenu_Variable.u16AcThresVolt[0]); 
				xTaskResumeAll();
				I2C_Release_Lock();
			}
			else
				return MQTT_PARSE_DATA_ERROR;
		}
		else if (!strcmp(jsonParameter->valuestring, "battery_threshold"))
		{
			return mqtt_json_parse_configure_battery_threshold(jsonMessage);
		}
		else if (!strcmp(jsonParameter->valuestring, "aircon_temp"))
		{
			return mqtt_json_parse_configure_aircon_temperature(jsonMessage);
		}
		else if (!strcmp(jsonParameter->valuestring, "device_ip"))
		{
			jsonMsgData = cJSON_GetObjectItem(jsonMessage, "data");
			if (cJSON_IsString(jsonMsgData) && (jsonMsgData->valuestring != NULL))
			{
				TRACE_INFO("Configure param: %s, value: %s\r\n", jsonParameter->valuestring, jsonMsgData->valuestring);
				if (ipStringToAddr(jsonMsgData->valuestring, &ipAddr))
					return MQTT_PARSE_DATA_ERROR;
				// save ip_addr to eeprom
				I2C_Get_Lock();
				vTaskSuspendAll();
				WriteEEPROM_long(_DEV_IP1, ipAddr.ipv4Addr);
				xTaskResumeAll();
				I2C_Release_Lock();
			}
			else 
				return MQTT_PARSE_DATA_ERROR;
		}
		else if (!strcmp(jsonParameter->valuestring, "subnet_mask"))
		{
			jsonMsgData = cJSON_GetObjectItem(jsonMessage, "data");
			if (cJSON_IsString(jsonMsgData) && (jsonMsgData->valuestring != NULL))
			{
				TRACE_INFO("Configure param: %s, value: %s\r\n", jsonParameter->valuestring, jsonMsgData->valuestring);
				if (ipStringToAddr(jsonMsgData->valuestring, &ipAddr))
					return MQTT_PARSE_DATA_ERROR;
				// save ip_addr to eeprom
				I2C_Get_Lock();
				vTaskSuspendAll();
				WriteEEPROM_long(_DEV_SUBNET1, ipAddr.ipv4Addr);
				xTaskResumeAll();
				I2C_Release_Lock();
			}
			else
				return MQTT_PARSE_DATA_ERROR;
		}
		else if (!strcmp(jsonParameter->valuestring, "gateway"))
		{
			jsonMsgData = cJSON_GetObjectItem(jsonMessage, "data");
			if (cJSON_IsString(jsonMsgData) && (jsonMsgData->valuestring != NULL))
			{
				TRACE_INFO("Configure param: %s, value: %s\r\n", jsonParameter->valuestring, jsonMsgData->valuestring);
				if (ipStringToAddr(jsonMsgData->valuestring, &ipAddr))
					return MQTT_PARSE_DATA_ERROR;
				// save ip_addr to eeprom
				I2C_Get_Lock();
				vTaskSuspendAll();
				WriteEEPROM_long(_DEV_GATEW1, ipAddr.ipv4Addr);
				xTaskResumeAll();
				I2C_Release_Lock();
			}
			else 
				return MQTT_PARSE_DATA_ERROR;
		}
		else if (!strcmp(jsonParameter->valuestring, "server_ip"))
		{
			jsonMsgData = cJSON_GetObjectItem(jsonMessage, "data");
			if (cJSON_IsString(jsonMsgData) && (jsonMsgData->valuestring != NULL))
			{
				TRACE_INFO("Configure param: %s, value: %s\r\n", jsonParameter->valuestring, jsonMsgData->valuestring);
				if (ipStringToAddr(jsonMsgData->valuestring, &ipAddr))
					return MQTT_PARSE_DATA_ERROR;
				// save ip_addr to eeprom
				I2C_Get_Lock();
				vTaskSuspendAll();
				WriteEEPROM_long(_SERVER_IP1, ipAddr.ipv4Addr);
				xTaskResumeAll();
				I2C_Release_Lock();
			}
			else
				return MQTT_PARSE_DATA_ERROR;
		}
		else
			return MQTT_PARSE_PARAM_ERROR;
	}
	else
		return MQTT_PARSE_PARAM_ERROR;
	return MQTT_PARSE_SUCCESS;
}

/***********************************************************************************************************
*                                        CONTROL MESSAGE PARSING                                          *
***********************************************************************************************************/
/* Parse control door data */
static mqtt_json_result_t mqtt_json_parse_control_door(char* data)
{
	if (!strcmp(data, "open"))
	{
		TRACE_INFO("door open command succeed\r\n");
	}
	else if (!strcmp(data, "close"))
	{
		TRACE_INFO("door close command succeed\r\n");
	}
	else
		return MQTT_PARSE_DATA_ERROR;
	return MQTT_PARSE_SUCCESS;
}

/* Parse control alarm data */
static mqtt_json_result_t mqtt_json_parse_control_alarm(char* data)
{
	if (!strcmp(data, "on"))
	{
		TRACE_INFO("alarm on command succeed\r\n");
	}
	else if (!strcmp(data, "off"))
	{
		TRACE_INFO("alarm off command succeed\r\n");
	}
	else
		return MQTT_PARSE_DATA_ERROR;
	return MQTT_PARSE_SUCCESS;
}

/* Parse control fan data */
static mqtt_json_result_t mqtt_json_parse_control_fan(char* parameter, char* data)
{
	if (!strcmp(data, "on"))
	{
		TRACE_INFO("fan %s on command succeed\r\n", parameter);
	}
	else if (!strcmp(data, "off"))
	{
		TRACE_INFO("fan %s off command succeed\r\n", parameter);
	}
	else
		return MQTT_PARSE_DATA_ERROR;
	return MQTT_PARSE_SUCCESS;
}

/* Parse aircon control message */
static mqtt_json_result_t mqtt_json_parse_control_aircon(cJSON* jsonMessage)
{
	cJSON* airconCtrlJson;
	cJSON* powerJson;
	cJSON* temperatureJson;
	cJSON* indexJson;
	airconCtrlJson = cJSON_GetObjectItem(jsonMessage, "data");
	if (!(cJSON_IsObject(airconCtrlJson)))
		return MQTT_PARSE_DATA_ERROR;
	indexJson = cJSON_GetObjectItem(airconCtrlJson, "index");
	if (!cJSON_IsNumber(indexJson))
		return MQTT_PARSE_DATA_ERROR;
	if ((indexJson->valueint > 2) || (indexJson->valueint < 1))
		return MQTT_PARSE_DATA_ERROR;
	powerJson = cJSON_GetObjectItem(airconCtrlJson, "power");
	if (!cJSON_IsString(powerJson))
		return MQTT_PARSE_DATA_ERROR;
	if ((strcmp(powerJson->valuestring, "on") && strcmp(powerJson->valuestring, "off")))
		return MQTT_PARSE_DATA_ERROR;
	temperatureJson = cJSON_GetObjectItem(airconCtrlJson, "temperature");
	if (!cJSON_IsNumber(temperatureJson))
		return MQTT_PARSE_DATA_ERROR;
	TRACE_INFO("Aircon control index: %d\r\n", indexJson->valueint);
	TRACE_INFO("Power: %s, temperature: %d\r\n", powerJson->valuestring, temperatureJson->valueint);
	return MQTT_PARSE_SUCCESS;
}

/* parse control message */
static mqtt_json_result_t mqtt_json_parse_control_message(cJSON* jsonMessage)
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
				TRACE_INFO("Control param: %s, value: %s\r\n", jsonParameter->valuestring, jsonMsgData->valuestring);
				return mqtt_json_parse_control_door(jsonMsgData->valuestring);
			}
			else
				return MQTT_PARSE_DATA_ERROR;
		}
		else if (!strcmp(jsonParameter->valuestring, "alarm"))
		{
			jsonMsgData = cJSON_GetObjectItem(jsonMessage, "data");
			if (cJSON_IsString(jsonMsgData) && (jsonMsgData->valuestring != NULL))
			{
				TRACE_INFO("Control param: %s, value: %s\r\n", jsonParameter->valuestring, jsonMsgData->valuestring);
				return mqtt_json_parse_control_alarm(jsonMsgData->valuestring);
			}
			else
				return MQTT_PARSE_DATA_ERROR;
		}
		else if (!strcmp(jsonParameter->valuestring, "fan_1"))
		{
			jsonMsgData = cJSON_GetObjectItem(jsonMessage, "data");
			if (cJSON_IsString(jsonMsgData) && (jsonMsgData->valuestring != NULL))
			{
				TRACE_INFO("Control param: %s, value: %s\r\n", jsonParameter->valuestring, jsonMsgData->valuestring);
				return mqtt_json_parse_control_fan(jsonParameter->valuestring, jsonMsgData->valuestring);
			}
			else
				return MQTT_PARSE_DATA_ERROR;
		}
		else if (!strcmp(jsonParameter->valuestring, "fan_2"))
		{
			jsonMsgData = cJSON_GetObjectItem(jsonMessage, "data");
			if (cJSON_IsString(jsonMsgData) && (jsonMsgData->valuestring != NULL))
			{
				TRACE_INFO("Control param: %s, value: %s\r\n", jsonParameter->valuestring, jsonMsgData->valuestring);
				return mqtt_json_parse_control_fan(jsonParameter->valuestring, jsonMsgData->valuestring);
			}
			else
				return MQTT_PARSE_DATA_ERROR;
		}
		else if (!strcmp(jsonParameter->valuestring, "aircon"))
		{
			return mqtt_json_parse_control_aircon(jsonMessage);
		}
		else
			return MQTT_PARSE_PARAM_ERROR;
	}
	else
		return MQTT_PARSE_PARAM_ERROR;
}

/* Parse message receive from MQTT input topic */
void mqtt_json_parse_message(char* message, unsigned int length)
{
	cJSON *jsonMessage;
	cJSON *jsonBoxId;
	cJSON *jsonMsgId;
	cJSON *jsonMsgType;
	mqtt_json_result_t result = MQTT_PARSE_SUCCESS;
	TRACE_INFO("Parse message with length: %d\r\n", length);
	
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
		TRACE_INFO("BOX ID: %s\r\n", jsonBoxId->valuestring);
	}
	else
	{
		result = MQTT_PARSE_BOXID_ERROR;
		goto END_PARSE;
	}
	jsonMsgId = cJSON_GetObjectItem(jsonMessage, "message_id");
	if(cJSON_IsNumber(jsonMsgId))
	{
		TRACE_INFO("Message ID: %d\r\n", jsonMsgId->valueint);
	}
	else
	{
		result = MQTT_PARSE_MSGID_ERROR;
		goto END_PARSE;
	}
	jsonMsgType = cJSON_GetObjectItem(jsonMessage, "type");
	if (cJSON_IsString(jsonMsgType) && (jsonMsgType->valuestring != NULL))
	{
		TRACE_INFO("Message Type: %s\r\n", jsonMsgType->valuestring);
	}
	else
	{
		result = MQTT_PARSE_TYPE_ERROR;
		goto END_PARSE;
	}
	if (!strcmp(jsonMsgType->valuestring, "config"))
	{
		TRACE_INFO("Parse configuration message\r\n");
		result = mqtt_json_parse_configure_message(jsonMessage);
	}
	else if (!strcmp(jsonMsgType->valuestring, "control"))
	{
		TRACE_INFO("Parse control message \r\n");
		result = mqtt_json_parse_control_message(jsonMessage);
	}
	else if (!strcmp(jsonMsgType->valuestring, "reset"))
	{
		TRACE_INFO("Resetting ...\r\n");
	}
	else
		result = MQTT_PARSE_TYPE_ERROR;
	
END_PARSE:
	if ((result != MQTT_PARSE_MESSAGE_ERROR) && (result != MQTT_PARSE_BOXID_ERROR) && (result != MQTT_PARSE_MSGID_ERROR))
	{
		if (result == MQTT_PARSE_SUCCESS)
			responseMessage = mqtt_json_make_response(jsonBoxId->valuestring, jsonMsgId->valueint, result);
		else
			responseMessage = mqtt_json_make_response(jsonBoxId->valuestring, jsonMsgId->valueint, result);
		if (responseMessage != NULL)
		{
			TRACE_INFO ("response:\n%s\r\n", responseMessage);
			free(responseMessage);
		}
	}
	cJSON_Delete(jsonMessage);
	free(procBuffer);
}
