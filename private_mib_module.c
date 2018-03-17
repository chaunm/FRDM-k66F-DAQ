/**
 * @file private_mib_module.c
 * @brief Private MIB module
 *
 * @section License
 *
 * ^^(^____^)^^
 **/

//Switch to the appropriate trace level
#define TRACE_LEVEL SNMP_TRACE_LEVEL

//Dependencies
#include "core/net.h"
#include "mibs/mib_common.h"
#include "private_mib_module.h"
#include "private_mib_impl.h"
#include "crypto.h"
#include "asn1.h"
#include "oid.h"
#include "debug.h"


/**
 * @brief Private MIB base
 **/

PrivateMibBase privateMibBase;


/**
 * @brief Private MIB objects
 **/

const MibObject privateMibObjects[] =
{
		//testString object (1.3.6.1.4.1.8072.9999.9999.1.1)
	{
		"siteInfoBTSCode",
		{43, 6, 1, 4, 1, 130, 229, 100, 1, 1, 1},
		11,
		ASN1_CLASS_UNIVERSAL,
		ASN1_TYPE_OCTET_STRING,
		MIB_ACCESS_READ_WRITE,
		NULL,//&privateMibBase.siteInfoGroup.siteInfoBTSCode,
		NULL,//&privateMibBase.siteInfoGroup.siteInfoBTSCodeLen,
		30,
		privateMibSetSiteInfoGroup,//NULL,//		
		privateMibGetSiteInfoGroup,//NULL,//		
		NULL
	},
	{
		"siteInfoThresTemp1",
		{43, 6, 1, 4, 1, 130, 229, 100, 1, 1, 2},
		11,
		ASN1_CLASS_UNIVERSAL,
		ASN1_TYPE_INTEGER,
		MIB_ACCESS_READ_WRITE,
		NULL,
		NULL,
		sizeof(uint32_t),
		privateMibSetSiteInfoGroup,
		privateMibGetSiteInfoGroup,
		NULL
	},
	{
		"siteInfoThresTemp2",
		{43, 6, 1, 4, 1, 130, 229, 100, 1, 1, 3},
		11,
		ASN1_CLASS_UNIVERSAL,
		ASN1_TYPE_INTEGER,
		MIB_ACCESS_READ_WRITE,
		NULL,
		NULL,
		sizeof(uint32_t),
		privateMibSetSiteInfoGroup,
		privateMibGetSiteInfoGroup,
		NULL
	},
	{
		"siteInfoThresTemp3",
		{43, 6, 1, 4, 1, 130, 229, 100, 1, 1, 4},
		11,
		ASN1_CLASS_UNIVERSAL,
		ASN1_TYPE_INTEGER,
		MIB_ACCESS_READ_WRITE,
		NULL,
		NULL,
		sizeof(uint32_t),
		privateMibSetSiteInfoGroup,
		privateMibGetSiteInfoGroup,
		NULL
	},
	{
		"siteInfoThresTemp4",
		{43, 6, 1, 4, 1, 130, 229, 100, 1, 1, 5},
		11,
		ASN1_CLASS_UNIVERSAL,
		ASN1_TYPE_INTEGER,
		MIB_ACCESS_READ_WRITE,
		NULL,
		NULL,
		sizeof(uint32_t),
		privateMibSetSiteInfoGroup,
		privateMibGetSiteInfoGroup,
		NULL
	},
	{
		"siteInfoMeasuredTemp",
		{43, 6, 1, 4, 1, 130, 229, 100, 1, 1, 6},
		11,
		ASN1_CLASS_UNIVERSAL,
		ASN1_TYPE_INTEGER,
		MIB_ACCESS_READ_ONLY,
		NULL,
		NULL,
		sizeof(uint32_t),
		NULL,
		privateMibGetSiteInfoGroup,
		NULL
	},
	{
		"siteInfoMeasuredHumid",
		{43, 6, 1, 4, 1, 130, 229, 100, 1, 1, 7},
		11,
		ASN1_CLASS_UNIVERSAL,
		ASN1_TYPE_INTEGER,
		MIB_ACCESS_READ_ONLY,
		NULL,
		NULL,
		sizeof(uint32_t),
		NULL,
		privateMibGetSiteInfoGroup,
		NULL
	},
	{
		"siteInfoAccessId",
		{43, 6, 1, 4, 1, 130, 229, 100, 1, 1, 8},
		11,
		ASN1_CLASS_UNIVERSAL,
		ASN1_TYPE_OCTET_STRING,
		MIB_ACCESS_READ_WRITE,
		NULL,
		NULL,
		MIB2_IF_DESCR_SIZE,
		NULL,
		privateMibGetSiteInfoGroup,
		NULL
	},
	{
		"siteInfoTrapCounter",
		{43, 6, 1, 4, 1, 130, 229, 100, 1, 1, 9},
		11,
		ASN1_CLASS_APPLICATION,
		MIB_TYPE_COUNTER32,
		MIB_ACCESS_READ_ONLY,
		NULL,
		NULL,
		sizeof(uint32_t),
		NULL,
		privateMibGetSiteInfoGroup,
		NULL
	},

	//AcInfo group
	{
		"acPhaseNumber",
		{43, 6, 1, 4, 1, 130, 229, 100, 1, 2, 1},//43, 6, 1, 4, 1, 0x82, 0xE5, 0x64, 1, 2, 1
		11,
		ASN1_CLASS_UNIVERSAL,
		ASN1_TYPE_INTEGER,
		MIB_ACCESS_READ_ONLY,
		&privateMibBase.acPhaseGroup.acPhaseNumber,
		NULL,
		sizeof(int32_t),
		NULL,
		NULL,
		NULL
	},
	//AcInfo table
	{
		"acPhaseIndex",
		{43, 6, 1, 4, 1, 130, 229, 100, 1, 2, 2, 1, 1},
		13,
		ASN1_CLASS_UNIVERSAL,
		ASN1_TYPE_INTEGER,
		MIB_ACCESS_READ_ONLY,
		NULL,
		NULL,
		sizeof(int32_t),
		NULL,
		privateMibGetAcPhaseEntry,
		privateMibGetNextAcPhaseEntry
	},
	{
		"acPhaseVolt",
		{43, 6, 1, 4, 1, 130, 229, 100, 1, 2, 2, 1, 2},
		13,
		ASN1_CLASS_UNIVERSAL,
		ASN1_TYPE_INTEGER,
		MIB_ACCESS_READ_ONLY,
		NULL,
		NULL,
		sizeof(uint32_t),
		NULL,
		privateMibGetAcPhaseEntry,
		privateMibGetNextAcPhaseEntry
	},
	{
		"acPhaseAlarmStatus",
		{43, 6, 1, 4, 1, 130, 229, 100, 1, 2, 2, 1, 3},
		13,
		ASN1_CLASS_UNIVERSAL,
		ASN1_TYPE_INTEGER,
		MIB_ACCESS_READ_ONLY,
		NULL,
		NULL,
		sizeof(uint32_t),
		NULL,
		privateMibGetAcPhaseEntry,
		privateMibGetNextAcPhaseEntry
	},
	{
		"acPhaseCurrent",
		{43, 6, 1, 4, 1, 130, 229, 100, 1, 2, 2, 1, 4},
		13,
		ASN1_CLASS_UNIVERSAL,
		ASN1_TYPE_INTEGER,
		MIB_ACCESS_READ_ONLY,
		NULL,
		NULL,
		sizeof(uint32_t),
		NULL,
		privateMibGetAcPhaseEntry,
		privateMibGetNextAcPhaseEntry
	},
	{
		"acPhasePower",
		{43, 6, 1, 4, 1, 130, 229, 100, 1, 2, 2, 1, 5},
		13,
		ASN1_CLASS_UNIVERSAL,
		ASN1_TYPE_INTEGER,
		MIB_ACCESS_READ_ONLY,
		NULL,
		NULL,
		sizeof(uint32_t),
		NULL,
		privateMibGetAcPhaseEntry,
		privateMibGetNextAcPhaseEntry
	},
	{
		"acPhaseFrequency",
		{43, 6, 1, 4, 1, 130, 229, 100, 1, 2, 2, 1, 6},
		13,
		ASN1_CLASS_UNIVERSAL,
		ASN1_TYPE_INTEGER,
		MIB_ACCESS_READ_ONLY,
		NULL,
		NULL,
		sizeof(uint32_t),
		NULL,
		privateMibGetAcPhaseEntry,
		privateMibGetNextAcPhaseEntry
	},
	{
		"acPhaseThresVolt",
		{43, 6, 1, 4, 1, 130, 229, 100, 1, 2, 2, 1, 7},
		13,
		ASN1_CLASS_UNIVERSAL,
		ASN1_TYPE_INTEGER,
		MIB_ACCESS_READ_WRITE,
		NULL,
		NULL,
		sizeof(uint32_t),
		privateMibSetAcPhaseEntry,
		privateMibGetAcPhaseEntry,
		privateMibGetNextAcPhaseEntry
	},
	{
		"battery1Voltage",
		{43, 6, 1, 4, 1, 130, 229, 100, 1, 3, 1},
		11,
		ASN1_CLASS_UNIVERSAL,
		ASN1_TYPE_INTEGER,
		MIB_ACCESS_READ_ONLY,
		NULL,
		NULL,
		sizeof(uint32_t),
		NULL,
		privateMibGetBatteryGroup,
		NULL
	},
	{
		"battery2Voltage",
		{43, 6, 1, 4, 1, 130, 229, 100, 1, 3, 2},
		11,
		ASN1_CLASS_UNIVERSAL,
		ASN1_TYPE_INTEGER,
		MIB_ACCESS_READ_ONLY,
		NULL,
		NULL,
		sizeof(uint32_t),
		NULL,
		privateMibGetBatteryGroup,
		NULL
	},
	{
		"battery1AlarmStatus",
		{43, 6, 1, 4, 1, 130, 229, 100, 1, 3, 3},
		11,
		ASN1_CLASS_UNIVERSAL,
		ASN1_TYPE_INTEGER,
		MIB_ACCESS_READ_ONLY,
		NULL,
		NULL,
		sizeof(uint32_t),
		NULL,
		privateMibGetBatteryGroup,
		NULL
	},
	{
		"battery2AlarmStatus",
		{43, 6, 1, 4, 1, 130, 229, 100, 1, 3, 4},
		11,
		ASN1_CLASS_UNIVERSAL,
		ASN1_TYPE_INTEGER,
		MIB_ACCESS_READ_ONLY,
		NULL,
		NULL,
		sizeof(uint32_t),
		NULL,
		privateMibGetBatteryGroup,
		NULL
	},
	{
		"battery1ThresVolt",
		{43, 6, 1, 4, 1, 130, 229, 100, 1, 3, 5},
		11,
		ASN1_CLASS_UNIVERSAL,
		ASN1_TYPE_INTEGER,
		MIB_ACCESS_READ_WRITE,
		NULL,
		NULL,
		sizeof(uint32_t),
		privateMibSetBatteryGroup,
		privateMibGetBatteryGroup,
		NULL
	},
	{
		"battery2ThresVolt",
		{43, 6, 1, 4, 1, 130, 229, 100, 1, 3, 6},
		11,
		ASN1_CLASS_UNIVERSAL,
		ASN1_TYPE_INTEGER,
		MIB_ACCESS_READ_WRITE,
		NULL,
		NULL,
		sizeof(uint32_t),
		privateMibSetBatteryGroup,
		privateMibGetBatteryGroup,
		NULL
	},
	{
		"airCon1Status",
		{43, 6, 1, 4, 1, 130, 229, 100, 1, 4, 1},
		11,
		ASN1_CLASS_UNIVERSAL,
		ASN1_TYPE_INTEGER,
		MIB_ACCESS_READ_WRITE,
		NULL,
		NULL,
		sizeof(uint32_t),
		NULL,
		privateMibGetAccessoriesGroup,
		NULL
	},
	{
		"airCon2Status",
		{43, 6, 1, 4, 1, 130, 229, 100, 1, 4, 2},
		11,
		ASN1_CLASS_UNIVERSAL,
		ASN1_TYPE_INTEGER,
		MIB_ACCESS_READ_WRITE,
		NULL,
		NULL,
		sizeof(uint32_t),
		NULL,
		privateMibGetAccessoriesGroup,
		NULL
	},
	{
		"fan1Status",
		{43, 6, 1, 4, 1, 130, 229, 100, 1, 4, 3},
		11,
		ASN1_CLASS_UNIVERSAL,
		ASN1_TYPE_INTEGER,
		MIB_ACCESS_READ_WRITE,
		NULL,
		NULL,
		sizeof(uint32_t),
		NULL,
		privateMibGetAccessoriesGroup,
		NULL
	},
	{
		"fan2Status",
		{43, 6, 1, 4, 1, 130, 229, 100, 1, 4, 4},
		11,
		ASN1_CLASS_UNIVERSAL,
		ASN1_TYPE_INTEGER,
		MIB_ACCESS_READ_WRITE,
		NULL,
		NULL,
		sizeof(uint32_t),
		NULL,
		privateMibGetAccessoriesGroup,
		NULL
	},
	{
		"doorStatus",
		{43, 6, 1, 4, 1, 130, 229, 100, 1, 4, 5},
		11,
		ASN1_CLASS_UNIVERSAL,
		ASN1_TYPE_INTEGER,
		MIB_ACCESS_READ_WRITE,
		NULL,
		NULL,
		sizeof(uint32_t),
		privateMibSetAccessoriesGroup,
		privateMibGetAccessoriesGroup,
		NULL
	},
	{
		"airConSetTemp1",
		{43, 6, 1, 4, 1, 130, 229, 100, 1, 4, 6},
		11,
		ASN1_CLASS_UNIVERSAL,
		ASN1_TYPE_INTEGER,
		MIB_ACCESS_READ_WRITE,
		NULL,
		NULL,
		sizeof(uint32_t),
		privateMibSetAccessoriesGroup,
		privateMibGetAccessoriesGroup,
		NULL
	},
	{
		"airConSetTemp2",
		{43, 6, 1, 4, 1, 130, 229, 100, 1, 4, 7},
		11,
		ASN1_CLASS_UNIVERSAL,
		ASN1_TYPE_INTEGER,
		MIB_ACCESS_READ_WRITE,
		NULL,
		NULL,
		sizeof(uint32_t),
		privateMibSetAccessoriesGroup,
		privateMibGetAccessoriesGroup,
		NULL
	},
	{
		"airConSetTemp3",
		{43, 6, 1, 4, 1, 130, 229, 100, 1, 4, 8},
		11,
		ASN1_CLASS_UNIVERSAL,
		ASN1_TYPE_INTEGER,
		MIB_ACCESS_READ_WRITE,
		NULL,
		NULL,
		sizeof(uint32_t),
		privateMibSetAccessoriesGroup,
		privateMibGetAccessoriesGroup,
		NULL
	},
	{
		"airConSetTemp4",
		{43, 6, 1, 4, 1, 130, 229, 100, 1, 4, 9},
		11,
		ASN1_CLASS_UNIVERSAL,
		ASN1_TYPE_INTEGER,
		MIB_ACCESS_READ_WRITE,
		NULL,
		NULL,
		sizeof(uint32_t),
		privateMibSetAccessoriesGroup,
		privateMibGetAccessoriesGroup,
		NULL
	},
	{
		"ledControlStatus",
		{43, 6, 1, 4, 1, 130, 229, 100, 1, 4, 10},
		11,
		ASN1_CLASS_UNIVERSAL,
		ASN1_TYPE_INTEGER,
		MIB_ACCESS_READ_WRITE,
		NULL,
		NULL,
		sizeof(uint32_t),
		privateMibSetAccessoriesGroup,
		privateMibGetAccessoriesGroup,
		NULL
	},
	{
		"speakerControlStatus",
		{43, 6, 1, 4, 1, 130, 229, 100, 1, 4, 11},
		11,
		ASN1_CLASS_UNIVERSAL,
		ASN1_TYPE_INTEGER,
		MIB_ACCESS_READ_WRITE,
		NULL,
		NULL,
		sizeof(uint32_t),
		privateMibSetAccessoriesGroup,
		privateMibGetAccessoriesGroup,
		NULL
	},
	//Config table
	{
		"configDevIPAddr",
		{43, 6, 1, 4, 1, 130, 229, 100, 1, 14, 1},
		11,
		ASN1_CLASS_APPLICATION,
		MIB_TYPE_IP_ADDRESS,
		MIB_ACCESS_READ_WRITE,
		NULL,
		NULL,
		MIB2_IP_ADDRESS_SIZE,
		NULL,
		privateMibGetConfigInfoGroup,
		NULL
	},
	{
		"configDevGWAddr",
		{43, 6, 1, 4, 1, 130, 229, 100, 1, 14, 2},
		11,
		ASN1_CLASS_APPLICATION,
		MIB_TYPE_IP_ADDRESS,
		MIB_ACCESS_READ_WRITE,
		NULL,
		NULL,
		MIB2_IP_ADDRESS_SIZE,
		NULL,
		privateMibGetConfigInfoGroup,
		NULL
	},
	{
		"configDevSNAddr",
		{43, 6, 1, 4, 1, 130, 229, 100, 1, 14, 3},
		11,
		ASN1_CLASS_APPLICATION,
		MIB_TYPE_IP_ADDRESS,
		MIB_ACCESS_READ_WRITE,
		NULL,
		NULL,
		MIB2_IP_ADDRESS_SIZE,
		NULL,
		privateMibGetConfigInfoGroup,
		NULL
	},
	{
		"configDevPort",
		{43, 6, 1, 4, 1, 130, 229, 100, 1, 14, 4},//43, 6, 1, 4, 1, 0x82, 0xE5, 0x64, 1, 2, 1
		11,
		ASN1_CLASS_UNIVERSAL,
		ASN1_TYPE_INTEGER,
		MIB_ACCESS_READ_ONLY,
		&privateMibBase.configGroup.configDevPort,
		NULL,
		sizeof(int32_t),
		NULL,
		privateMibGetConfigInfoGroup,
		NULL
	},
	{
		"configServerIPAddr",
		{43, 6, 1, 4, 1, 130, 229, 100, 1, 14, 5},
		11,
		ASN1_CLASS_APPLICATION,
		MIB_TYPE_IP_ADDRESS,
		MIB_ACCESS_READ_WRITE,
		NULL,
		NULL,
		MIB2_IP_ADDRESS_SIZE,
		NULL,
		privateMibGetConfigInfoGroup,
		NULL
	},
	{
		"configServerPort",
		{43, 6, 1, 4, 1, 130, 229, 100, 1, 14, 6},//43, 6, 1, 4, 1, 0x82, 0xE5, 0x64, 1, 2, 1
		11,
		ASN1_CLASS_UNIVERSAL,
		ASN1_TYPE_INTEGER,
		MIB_ACCESS_READ_ONLY,
		&privateMibBase.configGroup.configServerPort,
		NULL,
		sizeof(int32_t),
		NULL,
		privateMibGetConfigInfoGroup,
		NULL
	},
	{
		"configRefreshInterval",
		{43, 6, 1, 4, 1, 130, 229, 100, 1, 14, 7},//43, 6, 1, 4, 1, 0x82, 0xE5, 0x64, 1, 2, 1
		11,
		ASN1_CLASS_UNIVERSAL,
		ASN1_TYPE_INTEGER,
		MIB_ACCESS_READ_ONLY,
		&privateMibBase.configGroup.configRefreshInterval,
		NULL,
		sizeof(int32_t),
		NULL,
		privateMibGetConfigInfoGroup,
		NULL
	},

	{
		"configUserNumber",
		{43, 6, 1, 4, 1, 130, 229, 100, 1, 14, 8},//43, 6, 1, 4, 1, 0x82, 0xE5, 0x64, 1, 2, 1
		11,
		ASN1_CLASS_UNIVERSAL,
		ASN1_TYPE_INTEGER,
		MIB_ACCESS_READ_ONLY,
		&privateMibBase.configGroup.configRefreshInterval,
		NULL,
		sizeof(int32_t),
		NULL,
		privateMibGetConfigInfoGroup,
		NULL
	},

	{
		"configAccNumber",
		{43, 6, 1, 4, 1, 130, 229, 100, 1, 14, 9},//43, 6, 1, 4, 1, 0x82, 0xE5, 0x64, 1, 2, 1
		11,
		ASN1_CLASS_UNIVERSAL,
		ASN1_TYPE_INTEGER,
		MIB_ACCESS_READ_ONLY,
		&privateMibBase.configGroup.configAccNumber,
		NULL,
		sizeof(int32_t),
		NULL,
		NULL,
		NULL
	},
	{
		"configAccIndex",
		{43, 6, 1, 4, 1, 130, 229, 100, 1, 14, 10, 1, 1},//43, 6, 1, 4, 1, 0x82, 0xE5, 0x64, 1, 2, 1
		13,
		ASN1_CLASS_UNIVERSAL,
		ASN1_TYPE_INTEGER,
		MIB_ACCESS_READ_ONLY,
		NULL,
		NULL,
		sizeof(int32_t),
		NULL,
		privateMibGetConfigAccEntry,
		privateMibGetNextConfigAccEntry
	},
	{
		"configAccName",
		{43, 6, 1, 4, 1, 130, 229, 100, 1, 14, 10, 1, 2},
		13,
		ASN1_CLASS_UNIVERSAL,
		ASN1_TYPE_OCTET_STRING,
		MIB_ACCESS_READ_ONLY,
		NULL,
		NULL,
		MIB2_IF_DESCR_SIZE,
		NULL,
		privateMibGetConfigAccEntry,
		privateMibGetNextConfigAccEntry
	},
	{
		"configAccAuthkey",
		{43, 6, 1, 4, 1, 130, 229, 100, 1, 14, 10, 1, 3},
		13,
		ASN1_CLASS_UNIVERSAL,
		ASN1_TYPE_OCTET_STRING,
		MIB_ACCESS_READ_ONLY,
		NULL,
		NULL,
		MIB2_IF_DESCR_SIZE,
		NULL,
		privateMibGetConfigAccEntry,
		privateMibGetNextConfigAccEntry
	},
	{
		"configAccPrivkey",
		{43, 6, 1, 4, 1, 130, 229, 100, 1, 14, 10, 1, 4},
		13,
		ASN1_CLASS_UNIVERSAL,
		ASN1_TYPE_OCTET_STRING,
		MIB_ACCESS_READ_ONLY,
		NULL,
		NULL,
		MIB2_IF_DESCR_SIZE,
		NULL,
		privateMibGetConfigAccEntry,
		privateMibGetNextConfigAccEntry
	},
	{
		"configAccAuthType",
		{43, 6, 1, 4, 1, 130, 229, 100, 1, 14, 10, 1, 5},
		13,
		ASN1_CLASS_UNIVERSAL,
		ASN1_TYPE_INTEGER,
		MIB_ACCESS_READ_ONLY,
		NULL,
		NULL,
		sizeof(uint32_t),
		NULL,
		privateMibGetConfigAccEntry,
		privateMibGetNextConfigAccEntry
	},
	{
		"configAccPrivType",
		{43, 6, 1, 4, 1, 130, 229, 100, 1, 14, 10, 1, 6},
		13,
		ASN1_CLASS_UNIVERSAL,
		ASN1_TYPE_INTEGER,
		MIB_ACCESS_READ_ONLY,
		NULL,
		NULL,
		sizeof(uint32_t),
		NULL,
		privateMibGetConfigAccEntry,
		privateMibGetNextConfigAccEntry
	},
	{
		"configAccessIdNumber",
		{43, 6, 1, 4, 1, 130, 229, 100, 1, 14, 11},//43, 6, 1, 4, 1, 0x82, 0xE5, 0x64, 1, 2, 1
		11,
		ASN1_CLASS_UNIVERSAL,
		ASN1_TYPE_INTEGER,
		MIB_ACCESS_READ_ONLY,
		&privateMibBase.configGroup.configAccessIdNumber,
		NULL,
		sizeof(int32_t),
		NULL,
		NULL,
		NULL
	},
	{
		"configAccessIdIndex",
		{43, 6, 1, 4, 1, 130, 229, 100, 1, 14, 12, 1, 1},//43, 6, 1, 4, 1, 0x82, 0xE5, 0x64, 1, 2, 1
		13,
		ASN1_CLASS_UNIVERSAL,
		ASN1_TYPE_INTEGER,
		MIB_ACCESS_READ_ONLY,
		NULL,
		NULL,
		sizeof(int32_t),
		NULL,
		privateMibGetConfigAccessIdEntry,
		privateMibGetNextConfigAccessIdEntry
	},
	{
		"configAccessIdCard",
		{43, 6, 1, 4, 1, 130, 229, 100, 1, 14, 12, 1, 2},
		13,
		ASN1_CLASS_UNIVERSAL,
		ASN1_TYPE_OCTET_STRING,
		MIB_ACCESS_READ_WRITE,
		NULL,
		NULL,
		MIB2_IF_DESCR_SIZE,
		privateMibSetConfigAccessIdEntry,
		privateMibGetConfigAccessIdEntry,
		privateMibGetNextConfigAccessIdEntry
	},
	{
		"alarmFireAlarms",
		{43, 6, 1, 4, 1, 130, 229, 100, 1, 15, 1},
		11,
		ASN1_CLASS_UNIVERSAL,
		ASN1_TYPE_INTEGER,
		MIB_ACCESS_READ_ONLY,
		NULL,
		NULL,
		sizeof(uint32_t),
		NULL,
		privateMibGetAlarmGroup,
		NULL
	},

	{
		"alarmSmokeAlarms",
		{43, 6, 1, 4, 1, 130, 229, 100, 1, 15, 2},
		11,
		ASN1_CLASS_UNIVERSAL,
		ASN1_TYPE_INTEGER,
		MIB_ACCESS_READ_ONLY,
		NULL,
		NULL,
		sizeof(uint32_t),
		NULL,
		privateMibGetAlarmGroup,
		NULL
	},
	{
		"alarmMotionDetectAlarms",
		{43, 6, 1, 4, 1, 130, 229, 100, 1, 15, 3},
		11,
		ASN1_CLASS_UNIVERSAL,
		ASN1_TYPE_INTEGER,
		MIB_ACCESS_READ_ONLY,
		NULL,
		NULL,
		sizeof(uint32_t),
		NULL,
		privateMibGetAlarmGroup,
		NULL
	},
	{
		"alarmFloodDetectAlarms",
		{43, 6, 1, 4, 1, 130, 229, 100, 1, 15, 4},
		11,
		ASN1_CLASS_UNIVERSAL,
		ASN1_TYPE_INTEGER,
		MIB_ACCESS_READ_ONLY,
		NULL,
		NULL,
		sizeof(uint32_t),
		NULL,
		privateMibGetAlarmGroup,
		NULL
	},
	{
		"alarmDoorOpenAlarms",
		{43, 6, 1, 4, 1, 130, 229, 100, 1, 15, 5},
		11,
		ASN1_CLASS_UNIVERSAL,
		ASN1_TYPE_INTEGER,
		MIB_ACCESS_READ_ONLY,
		NULL,
		NULL,
		sizeof(uint32_t),
		NULL,
		privateMibGetAlarmGroup,
		NULL
	},
	{
		"alarmGenFailureAlarms",
		{43, 6, 1, 4, 1, 130, 229, 100, 1, 15, 6},
		11,
		ASN1_CLASS_UNIVERSAL,
		ASN1_TYPE_INTEGER,
		MIB_ACCESS_READ_ONLY,
		NULL,
		NULL,
		sizeof(uint32_t),
		NULL,
		privateMibGetAlarmGroup,
		NULL
	},
	{
		"alarmDcThresAlarms",
		{43, 6, 1, 4, 1, 130, 229, 100, 1, 15, 7},
		11,
		ASN1_CLASS_UNIVERSAL,
		ASN1_TYPE_INTEGER,
		MIB_ACCESS_READ_ONLY,
		NULL,
		NULL,
		sizeof(uint32_t),
		NULL,
		privateMibGetAlarmGroup,
		NULL
	},
	{
		"alarmMachineStopAlarms",
		{43, 6, 1, 4, 1, 130, 229, 100, 1, 15, 8},
		11,
		ASN1_CLASS_UNIVERSAL,
		ASN1_TYPE_INTEGER,
		MIB_ACCESS_READ_ONLY,
		NULL,
		NULL,
		sizeof(uint32_t),
		NULL,
		privateMibGetAlarmGroup,
		NULL
	},
	{
		"alarmAccessAlarms",
		{43, 6, 1, 4, 1, 130, 229, 100, 1, 15, 9},
		11,
		ASN1_CLASS_UNIVERSAL,
		ASN1_TYPE_INTEGER,
		MIB_ACCESS_READ_ONLY,
		NULL,
		NULL,
		sizeof(uint32_t),
		NULL,
		privateMibGetAlarmGroup,
		NULL
	},
	{
		"alarmAcThresAlarms",
		{43, 6, 1, 4, 1, 130, 229, 100, 1, 15, 10},
		11,
		ASN1_CLASS_UNIVERSAL,
		ASN1_TYPE_INTEGER,
		MIB_ACCESS_READ_ONLY,
		NULL,
		NULL,
		sizeof(uint32_t),
		NULL,
		privateMibGetAlarmGroup,
		NULL
	},
	//testString object (1.3.6.1.4.1.8072.9999.9999.1.1)
	{
		"testString",
		{43, 6, 1, 4, 1, 191, 8, 206, 15, 206, 15, 1, 1},
		13,
		ASN1_CLASS_UNIVERSAL,
		ASN1_TYPE_OCTET_STRING,
		MIB_ACCESS_READ_WRITE,
		&privateMibBase.testString,
		&privateMibBase.testStringLen,
		PRIVATE_MIB_TEST_STRING_SIZE,
		privateMibSetStringEntry,
		NULL,
		NULL
	},
	//testInteger object (1.3.6.1.4.1.8072.9999.9999.1.2)
	{
		"testInteger",
		{43, 6, 1, 4, 1, 191, 8, 206, 15, 206, 15, 1, 2},
		13,
		ASN1_CLASS_UNIVERSAL,
		ASN1_TYPE_INTEGER,
		MIB_ACCESS_READ_WRITE,
		&privateMibBase.testInteger,
		NULL,
		sizeof(int32_t),
		NULL,
		NULL,
		NULL
	},
	//currentTime object (1.3.6.1.4.1.8072.9999.9999.1.3)
	{
		"currentTime",
		{43, 6, 1, 4, 1, 191, 8, 206, 15, 206, 15, 1, 3},
		13,
		ASN1_CLASS_APPLICATION,
		MIB_TYPE_TIME_TICKS,
		MIB_ACCESS_READ_ONLY,
		NULL,
		NULL,
		sizeof(uint32_t),
		NULL,
		privateMibGetCurrentTime,
		NULL
	},
	//ledCount object (1.3.6.1.4.1.8072.9999.9999.2.1)
	{
		"ledCount",
		{43, 6, 1, 4, 1, 191, 8, 206, 15, 206, 15, 2, 1},
		13,
		ASN1_CLASS_UNIVERSAL,
		ASN1_TYPE_INTEGER,
		MIB_ACCESS_READ_ONLY,
		&privateMibBase.ledCount,
		NULL,
		sizeof(int32_t),
		NULL,
		NULL,
		NULL
	},
	//ledColor object (1.3.6.1.4.1.8072.9999.9999.2.2.1)
	{
		"ledColor",
		{43, 6, 1, 4, 1, 191, 8, 206, 15, 206, 15, 2, 2, 1},
		14,
		ASN1_CLASS_UNIVERSAL,
		ASN1_TYPE_OCTET_STRING,
		MIB_ACCESS_READ_ONLY,
		NULL,
		NULL,
		PRIVATE_MIB_LED_COLOR_SIZE,
		NULL,
		privateMibGetLedEntry,
		privateMibGetNextLedEntry
	},
	//ledState object (1.3.6.1.4.1.8072.9999.9999.2.2.2)
	{
		"ledState",
		{43, 6, 1, 4, 1, 191, 8, 206, 15, 206, 15, 2, 2, 2},
		14,
		ASN1_CLASS_UNIVERSAL,
		ASN1_TYPE_INTEGER,
		MIB_ACCESS_READ_WRITE,
		NULL,
		NULL,
		sizeof(int32_t),
		privateMibSetLedEntry,
		privateMibGetLedEntry,
		privateMibGetNextLedEntry
	}
};


/**
 * @brief Private MIB module
 **/

const MibModule privateMibModule =
{
   privateMibObjects,
   arraysize(privateMibObjects),
   privateMibInit,
   privateMibLock,
   privateMibUnlock
};
