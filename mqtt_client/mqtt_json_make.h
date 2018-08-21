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
//include device database
#include "snmp/snmp_agent.h"
#include "mibs/mib2_module.h"
#include "mibs/mib2_impl.h"
#include "oid.h"
#include "private_mib_module.h"
#include "private_mib_impl.h"

char* mqtt_json_make_online_message(char* boxID);
char* mqtt_json_make_response(char* boxID, unsigned int messageID, mqtt_json_result_t errorCode);
char* mqtt_json_make_fw_update_result(char* boxID, char* serverIP, char* fileName, int32_t result);
char* mqtt_json_make_device_info(char* boxID, PrivateMibBase *deviceData);
char* mqtt_json_make_ac_phase_info(char* boxID, PrivateMibBase *deviceData);
char* mqtt_json_make_battery_message(char* boxID, PrivateMibBase *deviceData);
char* mqtt_json_make_alarm_message(char* boxID, PrivateMibBase *deviceData);
char* mqtt_json_make_accessory_message(char* boxID, PrivateMibBase *deviceData);
char* mqtt_json_make_configuration_message(char* boxID, PrivateMibBase *deviceData);
#endif /* MQTT_JSON_MAKE_H_ */
