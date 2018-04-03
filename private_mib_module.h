/**
* @file private_mib_module.h
* @brief Private MIB module
*
* @section License
*
* ^^(^____^)^^
**/

#ifndef _PRIVATE_MIB_MODULE_H
#define _PRIVATE_MIB_MODULE_H

//Dependencies
#include "mibs/mib_common.h"

//Size of testString object
#define PRIVATE_MIB_TEST_STRING_SIZE 32
//Number of LEDs
#define PRIVATE_MIB_LED_COUNT 3
//Size of ledColor object
#define PRIVATE_MIB_LED_COLOR_SIZE 8


/**
* @brief LED table entry
**/

typedef struct
{
	char_t ledColor[PRIVATE_MIB_LED_COLOR_SIZE];
	size_t ledColorLen;
	int32_t ledState;
} PrivateMibLedEntry;

/**
* @brief AcInfo group
**/

typedef struct
{
	char_t siteInfoBTSCode[MIB2_IF_DESCR_SIZE];
	size_t siteInfoBTSCodeLen;
	uint32_t siteInfoThresTemp1;
	uint32_t siteInfoThresTemp2;
	uint32_t siteInfoThresTemp3;
	uint32_t siteInfoThresTemp4;
	uint32_t siteInfoMeasuredTemp;
	uint32_t siteInfoMeasuredHumid;
	char_t siteInfoAccessId[MIB2_IF_DESCR_SIZE];
	size_t siteInfoAccessIdLen;        
	uint32_t siteInfoTrapCounter;
} PrivateMibSiteInfoGroup;

/**
* @brief acPhase table entry
**/

typedef struct
{
	int32_t acPhaseIndex;
	uint32_t acPhaseVolt;
	uint8_t acPhaseAlarmStatus;
	uint32_t acPhaseCurrent;
	uint32_t acPhasePower;
	uint32_t acPhaseFrequency;
	uint32_t acPhaseThresVolt;
} PrivateMibAcPhaseEntry;

/**
* @brief AcInfo group
**/

typedef struct
{
	int32_t acPhaseNumber;
	PrivateMibAcPhaseEntry acPhaseTable[3];
} PrivateMibAcPhaseGroup;

/**
* @brief BatteryInfo group
**/

typedef struct
{
	uint32_t battery1Voltage;
	uint32_t battery2Voltage;
	uint8_t battery1AlarmStatus;
	uint8_t battery2AlarmStatus;
	uint32_t battery1ThresVolt;
	uint32_t battery2ThresVolt;
} PrivateMibBatteryGroup;
/**
* @brief AccessoriesInfo group
**/

typedef struct
{
	uint8_t airCon1Status;
	uint8_t airCon2Status;
	uint8_t fan1Status;
	uint8_t fan2Status;
	uint8_t doorStatus;
	uint32_t airConSetTemp1;
	uint32_t airConSetTemp2;
	uint32_t airConSetTemp3;
	uint32_t airConSetTemp4;
	uint8_t ledControlStatus;
	uint8_t speakerControlStatus;
	
} PrivateMibAccessoriesGroup;
/**
* @brief configAcc table entry
**/

typedef struct
{
	int32_t configAccIndex;
	char_t configAccName[MIB2_IF_DESCR_SIZE];
	size_t configAccNameLen;
	char_t configAccAuthkey[MIB2_IF_DESCR_SIZE];
	size_t configAccAuthkeyLen;
	char_t configAccPrivkey[MIB2_IF_DESCR_SIZE];
	size_t configAccPrivkeyLen;
	uint8_t configAccAuthType;
	uint8_t configAccPrivType;
} PrivateMibConfigAccEntry;

/**
* @brief configAccessId table entry
**/

typedef struct
{
	int32_t configAccessIdIndex;
	char_t configAccessIdCard[MIB2_IF_DESCR_SIZE];
	size_t configAccessIdCardLen;
} PrivateMibConfigAccessIdEntry;

/**
* @brief configAcc group
**/

typedef struct
{
	Ipv4Addr configDevIPAddr;
	Ipv4Addr configDevGWAddr;
	Ipv4Addr configDevSNAddr;
	uint32_t configDevPort;
	Ipv4Addr configServerIPAddr;
	uint32_t configServerPort;
	uint32_t configRefreshInterval;
	int32_t configAccNumber;
	int32_t configAccessIdNumber;
	PrivateMibConfigAccEntry configAccTable[5];
	PrivateMibConfigAccessIdEntry configAccessIdTable[5];
} PrivateMibConfigGroup;

/**
* @brief AlarmInfo group
**/

typedef struct
{
	uint32_t alarmFireAlarms;
	uint32_t alarmSmokeAlarms;
	uint32_t alarmMotionDetectAlarms;
	uint32_t alarmFloodDetectAlarms;
	uint32_t alarmDoorOpenAlarms;
	uint32_t alarmGenFailureAlarms;
	uint32_t alarmDcThresAlarms;
	uint32_t alarmMachineStopAlarms;
	uint32_t alarmAccessAlarms;
	uint32_t alarmAcThresAlarms;
	uint32_t alarmFireAlarms_old;
	uint32_t alarmSmokeAlarms_old;
	uint32_t alarmMotionDetectAlarms_old;
	uint32_t alarmFloodDetectAlarms_old;
	uint32_t alarmDoorOpenAlarms_old;
	uint32_t alarmGenFailureAlarms_old;
	uint32_t alarmDcThresAlarms_old;
	uint32_t alarmMachineStopAlarms_old;
	uint32_t alarmAccessAlarms_old;
	uint32_t alarmAcThresAlarms_old;
} PrivateMibAlarmGroup;
/**
* @brief Private MIB base
**/

typedef struct
{
	char_t testString[PRIVATE_MIB_TEST_STRING_SIZE];
	size_t testStringLen;
	int32_t testInteger;
	uint32_t currentTime;
	int32_t ledCount;
	PrivateMibLedEntry ledTable[PRIVATE_MIB_LED_COUNT];
	PrivateMibSiteInfoGroup siteInfoGroup;
	PrivateMibAcPhaseGroup acPhaseGroup;
	PrivateMibAccessoriesGroup accessoriesGroup;
	PrivateMibConfigGroup configGroup;
	PrivateMibAlarmGroup alarmGroup;
	PrivateMibBatteryGroup batteryGroup;
} PrivateMibBase;


//Private MIB related constants
extern PrivateMibBase privateMibBase;
extern const MibObject privateMibObjects[];
extern const MibModule privateMibModule;

#endif
