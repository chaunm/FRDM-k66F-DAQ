/*
 * mqtt_json_make.c
 *
 *  Created on: Jul 25, 2018
 *      Author: chaunm
 */
#include <stdio.h>
#include <stdlib.h>
#include "core/net.h"
#include "mqtt_json_make.h"
#include "mqtt_json_type.h"
#include "cJSON.h"
#include "debug.h"
#include "variables.h"
#include "snmpConnect_manager.h"
#include "private_mib_module.h"

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
	cJSON_AddStringToObject(jsonEvent, "MAC", macIdString);
	cJSON_AddStringToObject(jsonEvent, "status", "online");		
    if (interfaceManagerGetActiveInterface() == ETH_INTERFACE)
        cJSON_AddStringToObject(jsonEvent, "interface", "ethernet");
    else if (interfaceManagerGetActiveInterface() == GPRS_INTERFACE)
        cJSON_AddStringToObject(jsonEvent, "interface", "gprs");
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

/* make periodically report data */
char* mqtt_json_make_device_info(char* boxID, PrivateMibBase *deviceData)
{
	cJSON* jsonMessage;
	char* message;
	jsonMessage = cJSON_CreateObject();
	if (jsonMessage == NULL)
	{
		TRACE_INFO("Not enough memory to create json message\r\n");
		return NULL;
	}
	cJSON_AddStringToObject(jsonMessage, "id", boxID);
    cJSON_AddStringToObject(jsonMessage, "type", "device data");
	cJSON_AddNumberToObject(jsonMessage, "temp threshold 1", deviceData->siteInfoGroup.siteInfoThresTemp1);
	cJSON_AddNumberToObject(jsonMessage, "temp threshold 2", deviceData->siteInfoGroup.siteInfoThresTemp2);
	cJSON_AddNumberToObject(jsonMessage, "temp threshold 3", deviceData->siteInfoGroup.siteInfoThresTemp3);
	cJSON_AddNumberToObject(jsonMessage, "temp threshold 4", deviceData->siteInfoGroup.siteInfoThresTemp4);
	cJSON_AddNumberToObject(jsonMessage, "measured temp", deviceData->siteInfoGroup.siteInfoMeasuredTemp);
	cJSON_AddNumberToObject(jsonMessage, "measured humi", deviceData->siteInfoGroup.siteInfoMeasuredHumid);
	message = cJSON_Print(jsonMessage);
	cJSON_Delete(jsonMessage);
	return message;;	
}

/* make the device ac voltage package */
char* mqtt_json_make_ac_phase_info(char* boxID, PrivateMibBase *deviceData)
{
	cJSON* jsonMessage;
	cJSON* phaseJson;
	cJSON* phaseJsonArray;
	char* message;
	uint8_t i;
	jsonMessage = cJSON_CreateObject();
	if (jsonMessage == NULL)
	{
		TRACE_INFO("Not enough memory to create json message\r\n");
		return NULL;
	}
	cJSON_AddStringToObject(jsonMessage, "id", boxID);
	cJSON_AddStringToObject(jsonMessage, "type", "AC");
	cJSON_AddNumberToObject(jsonMessage, "phase count", deviceData->acPhaseGroup.acPhaseNumber);
	phaseJsonArray = cJSON_CreateArray();
	for (i = 0; i < deviceData->acPhaseGroup.acPhaseNumber; i++)
	{
		phaseJson = cJSON_CreateObject();
		cJSON_AddNumberToObject(phaseJson, "index", i + 1);
		cJSON_AddNumberToObject(phaseJson, "voltage", deviceData->acPhaseGroup.acPhaseTable[i].acPhaseVolt);
		cJSON_AddNumberToObject(phaseJson, "frequency", deviceData->acPhaseGroup.acPhaseTable[i].acPhaseFrequency);
		cJSON_AddNumberToObject(phaseJson, "voltage_threshold", deviceData->acPhaseGroup.acPhaseTable[i].acPhaseThresVolt);
		cJSON_AddNumberToObject(phaseJson, "alarm", deviceData->acPhaseGroup.acPhaseTable[i].acPhaseAlarmStatus);
		cJSON_AddNumberToObject(phaseJson, "current", deviceData->acPhaseGroup.acPhaseTable[i].acPhaseCurrent);
		cJSON_AddNumberToObject(phaseJson, "power", deviceData->acPhaseGroup.acPhaseTable[i].acPhasePower);
		cJSON_AddItemToArray(phaseJsonArray, phaseJson);
	}
	cJSON_AddItemToObject(jsonMessage, "phase", phaseJsonArray);
	message = cJSON_Print(jsonMessage);
	cJSON_Delete(jsonMessage);
	return message;
}

/* create battery update message */
char* mqtt_json_make_battery_message(char* boxID, PrivateMibBase *deviceData)
{
	cJSON* jsonMessage;
	cJSON* jsonBatterry;
	cJSON* batteryArray;
	char* message;
	jsonMessage = cJSON_CreateObject();
	if (jsonMessage == NULL)
	{
		TRACE_INFO("Not enough memory to create json message\r\n");
		return NULL;
	}
	batteryArray = cJSON_CreateArray();
	cJSON_AddStringToObject(jsonMessage, "id", boxID);
	cJSON_AddStringToObject(jsonMessage, "type", "battery");
	cJSON_AddNumberToObject(jsonMessage, "battery count", 2);
	jsonBatterry = cJSON_CreateObject();
	cJSON_AddNumberToObject(jsonBatterry, "index", 1);
	cJSON_AddNumberToObject(jsonBatterry, "voltage", deviceData->batteryGroup.battery1Voltage);
	cJSON_AddNumberToObject(jsonBatterry, "alarm", deviceData->batteryGroup.battery1AlarmStatus);
	cJSON_AddNumberToObject(jsonBatterry, "threshold", deviceData->batteryGroup.battery1ThresVolt);
	cJSON_AddItemToArray(batteryArray, jsonBatterry);
	jsonBatterry = cJSON_CreateObject();
	cJSON_AddNumberToObject(jsonBatterry, "index", 2);
	cJSON_AddNumberToObject(jsonBatterry, "voltage", deviceData->batteryGroup.battery2Voltage);
	cJSON_AddNumberToObject(jsonBatterry, "alarm", deviceData->batteryGroup.battery2AlarmStatus);
	cJSON_AddNumberToObject(jsonBatterry, "threshold", deviceData->batteryGroup.battery2ThresVolt);
	cJSON_AddItemToArray(batteryArray, jsonBatterry);
	cJSON_AddItemToObject(jsonMessage, "batteries", batteryArray);
	message = cJSON_Print(jsonMessage);
	cJSON_Delete(jsonMessage);
	return message;
}

/* create alarm update message */
char* mqtt_json_make_alarm_message(char* boxID, PrivateMibBase *deviceData)
{
	cJSON* jsonMessage;
	char* message;
	jsonMessage = cJSON_CreateObject();
	if (jsonMessage == NULL)
	{
		TRACE_INFO("Can not create JSON object\r\n");
		return NULL;
	}
	cJSON_AddStringToObject(jsonMessage, "id", boxID);
	cJSON_AddStringToObject(jsonMessage, "type", "alarm");
	cJSON_AddNumberToObject(jsonMessage, "fire", deviceData->alarmGroup.alarmFireAlarms);
	cJSON_AddNumberToObject(jsonMessage, "smoke", deviceData->alarmGroup.alarmSmokeAlarms);
	cJSON_AddNumberToObject(jsonMessage, "motion", deviceData->alarmGroup.alarmMotionDetectAlarms);
	cJSON_AddNumberToObject(jsonMessage, "flood", deviceData->alarmGroup.alarmFloodDetectAlarms);
	cJSON_AddNumberToObject(jsonMessage, "door", deviceData->alarmGroup.alarmDoorOpenAlarms);
	cJSON_AddNumberToObject(jsonMessage, "Generator", deviceData->alarmGroup.alarmGenFailureAlarms);
	cJSON_AddNumberToObject(jsonMessage, "machine stop", deviceData->alarmGroup.alarmMachineStopAlarms);
	cJSON_AddNumberToObject(jsonMessage, "ac threshold", deviceData->alarmGroup.alarmAcThresAlarms);
	cJSON_AddNumberToObject(jsonMessage, "dc threshold", deviceData->alarmGroup.alarmDcThresAlarms);
	cJSON_AddNumberToObject(jsonMessage, "access", deviceData->alarmGroup.alarmAccessAlarms);
	message = cJSON_Print(jsonMessage);
	cJSON_Delete(jsonMessage);
	return message;
}

/* create accessory message */
char* mqtt_json_make_accessory_message(char* boxID, PrivateMibBase *deviceData)
{
	cJSON* jsonMessage;
	char* message;
	jsonMessage = cJSON_CreateObject();
	if (jsonMessage == NULL)
	{
		TRACE_INFO("Can not create JSON object\r\n");
		return NULL;
	}
	cJSON_AddStringToObject(jsonMessage, "id", boxID);
	cJSON_AddStringToObject(jsonMessage, "type", "accessories");
	cJSON_AddNumberToObject(jsonMessage, "fan 1", deviceData->accessoriesGroup.fan1Status);
	cJSON_AddNumberToObject(jsonMessage, "fan 2", deviceData->accessoriesGroup.fan2Status);
	cJSON_AddNumberToObject(jsonMessage, "door", deviceData->accessoriesGroup.doorStatus);
	cJSON_AddNumberToObject(jsonMessage, "aircon 1 on", deviceData->accessoriesGroup.airCon1Status);
	cJSON_AddNumberToObject(jsonMessage, "aircon 2 on", deviceData->accessoriesGroup.airCon2Status);
	cJSON_AddNumberToObject(jsonMessage, "aircon set temp 1", deviceData->accessoriesGroup.airConSetTemp1);
	cJSON_AddNumberToObject(jsonMessage, "aircon set temp 2", deviceData->accessoriesGroup.airConSetTemp2);
	cJSON_AddNumberToObject(jsonMessage, "aircon set temp 3", deviceData->accessoriesGroup.airConSetTemp3);
	cJSON_AddNumberToObject(jsonMessage, "aircon set temp 4", deviceData->accessoriesGroup.airConSetTemp4);
	cJSON_AddNumberToObject(jsonMessage, "aircon runtime 1", deviceData->accessoriesGroup.airconRuntime1);
	cJSON_AddNumberToObject(jsonMessage, "aircon runtime 2", deviceData->accessoriesGroup.airconRuntime2);
	cJSON_AddNumberToObject(jsonMessage, "indoor temp", deviceData->accessoriesGroup.siteIndoorTemp);
	cJSON_AddNumberToObject(jsonMessage, "outdoor temp", deviceData->accessoriesGroup.siteOutdoorTemp);
	cJSON_AddNumberToObject(jsonMessage, "led", deviceData->accessoriesGroup.ledControlStatus);
	cJSON_AddNumberToObject(jsonMessage, "speaker", deviceData->accessoriesGroup.speakerControlStatus);
	message = cJSON_Print(jsonMessage);
	cJSON_Delete(jsonMessage);
	return message;
}

/* create configuration message */
char* mqtt_json_make_configuration_message(char* boxID, PrivateMibBase *deviceData)
{
	cJSON* jsonMessage;
	char* message = NULL;
	jsonMessage = cJSON_CreateObject();
	if (jsonMessage == NULL)
	{
		TRACE_INFO("Can not create JSON object\r\n");
		return NULL;
	}
	cJSON_AddStringToObject(jsonMessage, "id", boxID);
	cJSON_AddStringToObject(jsonMessage, "type", "configuration");
	cJSON_AddStringToObject(jsonMessage, "IP", (char*)sMenu_Variable.ucIP);
	cJSON_AddStringToObject(jsonMessage, "subnet", (char*)sMenu_Variable.ucSN);
	cJSON_AddStringToObject(jsonMessage, "gateway", (char*)sMenu_Variable.ucGW);
	cJSON_AddStringToObject(jsonMessage, "server", (char*)sMenu_Variable.ucSIP);
	message = cJSON_Print(jsonMessage);
	return message;
}
				   
		
								
