/*
* variables.h
*
*  Created on: 14-12-2016
*      Author: Administrator
*/

#ifndef SOURCE_VARIABLES_H_
#define SOURCE_VARIABLES_H_

//Dependencies
#include <stdint.h>
#include "eeprom_rtc.h"
#include "menu.h"

#define __FIRMWARE_VERSION        "v1.0.6"

#define ACTIVE_ALARM_NUMBER         18
#define DEVICE_NAME_EEPROM_ADDR     86
#define DEVICE_NAME_MAX_LENGTH      16

typedef struct TimeFormat
{
    unsigned char  hour;
    unsigned char  min;
    unsigned char  sec;
    unsigned char  day;
    unsigned char  date;
    unsigned char  month;
    unsigned char  year;
}TimeFormat;

typedef enum
{
    _DONT_USE,
    _ESC_KEY,
    _UP_KEY,
    _DOWN_KEY,
    _ENTER_KEY
}key_names;

typedef struct
{
    uint8_t   pressedKey;
    uint8_t   key1Pressed;   //1-pressed 0-released
    uint8_t   key2Pressed;
    uint8_t   key3Pressed;
    uint8_t   key4Pressed;
    uint16_t  key1Count;
    uint16_t  key2Count;
    uint16_t  key3Count;
    uint16_t  key4Count;
}sKey_Control_Struct;


typedef enum
{
    _MAIN_MENU,
    _ACTIVE_ALARM, _SYS_MONITOR, _SYS_LOG, _SYS_SETTING,
    _OPERATION_LOGS, _ATS_LOGS,
    _ALARM_SETTING, _ATS_SETTING, _ACCES_SETTING, _NETWORK_CONFIG, _DATETIME_CONFIG, _DEVICE_INFO,
    _DEVICE_ADDRESS, _SERVER_ADDRESS, _USER_INFO,
    _USER_ID, _AIRCON, _SETTING_CONFIRM
}menu_names;

typedef struct
{
    uint8_t	menuIndex;
    char	menuName[22];
    uint8_t	parentIndex;
    uint8_t	numberSubMenu;
    uint8_t	subMenuIndex;
}sMenu_Object_Struct;

typedef struct
{
    uint8_t	mode;						//0-Normal		1-User
    uint8_t     learnUID;
    int8_t      accessUID;
    int8_t      mainPageIndex;
    int8_t	alarmPageIndex;
    uint8_t     menu;
    int8_t	index;
    uint8_t	previousIndex;
    uint8_t	refesh;
    uint8_t	init;
    uint8_t	ajustValue;
    uint16_t	refreshMainPageCount;
    uint8_t     totalActiveAlarm;
}sMenu_Control_Struct;


typedef enum
{
    _AC_LOW, _DC_LOW, _TEMP1, _TEMP2, _TEMP3, _TEMP4,
    _DEV_IP1, _DEV_IP2, _DEV_IP3, _DEV_IP4, _DEV_SUBNET1, _DEV_SUBNET2, _DEV_SUBNET3, _DEV_SUBNET4, _DEV_GATEW1, _DEV_GATEW2, _DEV_GATEW3, _DEV_GATEW4, _DEV_PORT,
    _SERVER_IP1, _SERVER_IP2, _SERVER_IP3, _SERVER_IP4, _SERVER_PORT,
    _GEN_MAX_RUNTIME, _GEN_UNDER_VOLT, _GEN_ERROR_RESET_EN, _GEN_ERROR_RESET_MIN, _GEN_WARM_UP_TIME, _GEN_COOL_DOWN_TIME,
    _GEN_NIGHT_EN, _GEN_NIGHT_BEGIN, _GEN_NIGHT_END, _DC_LOW_INPUT, _DC_LOW_VOLT,
    _AIRCON_TEMP1,_AIRCON_TEMP2,_AIRCON_TEMP3,_AIRCON_TEMP4,_AIRCON_TIME1,_AIRCON_TIME2,
    _HOUR, _MINS, _SECS, _DATE, _MONTH, _YEAR, _DEFAULT_WRITE, _DEV_NAME_LENGTH
}setting_values;

typedef struct
{
    uint8_t   step;
    uint16_t  lowerVal;
    uint16_t  upperVal;
    uint16_t  addrEEPROM;
    int16_t   realVal;
    int16_t   defaultVal;
    uint8_t   point;
}sSetting_Values_Struct;


typedef struct
{
    uint8_t   status;
    uint8_t   checked;
    uint16_t  index;
    char      alarmName[21];
}sActive_Alarm_Struct;


typedef struct
{
    uint16_t	u16DevIP[4];
    uint16_t	u16DevGateway[4];
    uint16_t	u16DevSubnet[4];
    uint16_t	u16DevPort;
    //	uint32_t	u32IP;
    //	uint32_t	u32SN;
    //	uint32_t	u32GW;
    //	uint32_t	u32SIP;
    //	uint16_t	u16ServerIP[4];
    //	uint16_t	u16ServerPort;
}sEthernet_Setting_Struct;

typedef struct
{
    uint16_t	u16AcThresVolt[3];
    uint16_t	u16BattThresVolt[2];
    uint16_t	u16ThresTemp[4];
    //	uint32_t	u32IP;
    //	uint32_t	u32SN;
    //	uint32_t	u32GW;
    //	uint32_t	u32SIP;
    uint8_t		ucIP[16];
    uint8_t		ucSN[16];
    uint8_t		ucGW[16];
    uint8_t		ucSIP[16];
    uint16_t	u16ServerIP[4];
    uint16_t	u16ServerPort;
    sEthernet_Setting_Struct sEthernetSetting;
    sEthernet_Setting_Struct sEthernetSetting_temp;
    
    uint8_t	u8userName[12];
    uint8_t	u8AuthPass[12];
    uint8_t	u8PrivPass[12];
    uint8_t	u8Key[12];
    uint16_t	u16RefreshInterval;
    uint8_t	u8UserID[5][8];
    uint16_t	u8UserIDAddr[5];
    uint16_t	u16AirConTemp[4];
    uint16_t	u16AirConTime1;
    uint16_t	u16AirConTime2;
    
    uint16_t	u16GENMaxRuntime;
    uint16_t	u16GENUnderVolt;
    uint16_t	u16GENErrorResetEnable;
    uint16_t	u16GENErrorResetTime;
    uint16_t	u16GENWarmUpTime;
    uint16_t	u16GENCoolDownTime;
    uint16_t	u16GENNightEnable;
    uint16_t	u16GENNightStart;
    uint16_t	u16GENNightEnd;
    uint16_t	u16GENDCLowInput;
    uint16_t	u16GENDCLowVolt;
    
    uint16_t	changedVal;
}sMenu_Variable_Struct;


typedef struct
{
    uint16_t battVolt;
    uint16_t	grid_Volt;
    uint16_t	genVolt;
    uint8_t	grid_VoltCheckCnt;
    uint8_t	grid_Status;
    uint8_t	activePower;
    uint8_t	batt_DCLow;
    uint8_t	switch_Grid_ON;
    uint8_t	switch_GEN_ON;
    uint8_t	Gen1Voltage;
    uint8_t	GenEnable;
    uint8_t	GenStart;
    uint8_t	Gen1Fail;
    uint8_t	Gen1ControlStatus;
    uint8_t	StartStep;
    uint8_t	StartStep_cnt;
    uint8_t	StopStep;
    uint8_t	StopStep_cnt;
    uint8_t	ReStart_Cnt;
    uint32_t	MaxRunTimeCnt;
    uint32_t	vRMS_u32;
    uint32_t	iRMSA_u32;
    uint32_t	instan_ActPwA_i32;
    uint16_t	power_Factor_i16;
    uint16_t	frequency_i16;
    uint8_t	year;
    uint8_t	month;
    uint8_t	date;
    uint8_t	hour;
    uint8_t	mins;
    uint8_t	secs;
}sATS_Variable_Struct;


typedef struct
{
    uint16_t  indoorTemp;
    uint16_t  outdoorTemp;
    uint32_t  airCon1Runtime;
    uint32_t  airCon2Runtime;
    uint8_t  airCon1Status;
    uint8_t  airCon2Status;
    uint8_t  fanStatus;
    uint8_t	year;
    uint8_t	month;
    uint8_t	date;
    uint8_t	hour;
    uint8_t	mins;
    uint8_t	secs;
}sAirCon_Variable_Struct;

#define NUMBER_OF_SETTING   50

extern sActive_Alarm_Struct		sActive_Alarm[ACTIVE_ALARM_NUMBER];
extern sKey_Control_Struct      sKey_Control;
extern sMenu_Object_Struct		sMenu_Object[19];
extern sMenu_Control_Struct		sMenu_Control;
extern sSetting_Values_Struct    sSetting_Values[NUMBER_OF_SETTING];
extern sMenu_Variable_Struct	sMenu_Variable;
extern sATS_Variable_Struct	    sATS_Variable;
extern sAirCon_Variable_Struct  sAirCon_Variable;
extern TimeFormat               GTime;
extern TimeFormat               STime;


void SetTime(TimeFormat Time);
TimeFormat GetTime(void);

extern   uint8_t   AccessIdTemp[8];
extern uint8_t trap_flag[20];
extern uint32_t setCount_test;
extern uint32_t trapStatus_TimePeriod;
extern uint8_t DigitalInput[10];
extern char deviceName[DEVICE_NAME_MAX_LENGTH + 1];


#endif /* SOURCE_VARIABLES_H_ */
