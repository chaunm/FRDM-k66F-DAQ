#include "board.h"
#include "pin_mux.h"
#include "fsl_gpio.h"

#define  KEY_1_PORT   GPIOC
#define  KEY_1_PIN    8u
#define  KEY_2_PORT   GPIOC
#define  KEY_2_PIN    7
#define  KEY_3_PORT   GPIOC
#define  KEY_3_PIN    6u
#define  KEY_4_PORT   GPIOC
#define  KEY_4_PIN    5u

#define READ_KEY1    GPIO_ReadPinInput(KEY_1_PORT,KEY_1_PIN)
#define READ_KEY2    GPIO_ReadPinInput(KEY_2_PORT,KEY_2_PIN)
#define READ_KEY3    GPIO_ReadPinInput(KEY_3_PORT,KEY_3_PIN)
#define READ_KEY4    GPIO_ReadPinInput(KEY_4_PORT,KEY_4_PIN)

#define USER1_ADDR   90
#define USER2_ADDR   100
#define USER3_ADDR   110
#define USER4_ADDR   120
#define USER5_ADDR   130

//typedef enum
//{
//  _DONT_USE,
//  _ESC_KEY,
//  _UP_KEY,
//  _DOWN_KEY,
//  _ENTER_KEY
//}key_names;
//
//typedef struct
//{
//  uint8_t   pressedKey;
//  uint8_t   key1Pressed;   //1-pressed 0-released
//  uint8_t   key2Pressed;
//  uint8_t   key3Pressed;
//  uint8_t   key4Pressed;
//  uint16_t  key1Count;
//  uint16_t  key2Count;
//  uint16_t  key3Count;
//  uint16_t  key4Count;
//}sKey_Control_Struct;
//extern sKey_Control_Struct    sKey_Control;
//
//typedef enum
//{
//  _MAIN_MENU,
//  _ACTIVE_ALARM, _SYS_MONITOR, _SYS_LOG, _SYS_SETTING,
//  _OPERATION_LOGS, _ATS_LOGS,
//  _ALARM_SETTING, _ATS_SETTING, _ACCES_SETTING, _NETWORK_CONFIG, _DATETIME_CONFIG, _DEVICE_INFO,
//  _DEVICE_ADDRESS, _SERVER_ADDRESS, _USER_INFO,
//  _USER_ID, _AIRCON
//}menu_names;
//
//typedef struct
//{
//  uint8_t	menuIndex;
//  char		menuName[22];
//  uint8_t	parentIndex;
//  uint8_t	numberSubMenu;
//  uint8_t	subMenuIndex;
//}sMenu_Object_Struct;
//extern sMenu_Object_Struct		sMenu_Object[18];
//
//typedef struct
//{
//  uint8_t	mode;						//0-Normal		1-User
//  uint8_t   learnUID;
//  int8_t    accessUID;
//  int8_t		mainPageIndex;
//  int8_t		alarmPageIndex;
//  uint8_t	menu;
//  int8_t	 	index;
//  uint8_t	previousIndex;
//  uint8_t	refesh;
//  uint8_t	init;
//  uint8_t	ajustValue;
//  uint16_t	refreshMainPageCount;
//  uint8_t   totalActiveAlarm;
//}sMenu_Control_Struct;
//extern sMenu_Control_Struct		sMenu_Control;
//
//typedef enum
//{
//  _AC_LOW, _DC_LOW, _TEMP1, _TEMP2, _TEMP3, _TEMP4,
//  _DEV_IP1, _DEV_IP2, _DEV_IP3, _DEV_IP4, _DEV_SUBNET1, _DEV_SUBNET2, _DEV_SUBNET3, _DEV_SUBNET4, _DEV_GATEW1, _DEV_GATEW2, _DEV_GATEW3, _DEV_GATEW4, _DEV_PORT,
//  _SERVER_IP1, _SERVER_IP2, _SERVER_IP3, _SERVER_IP4, _SERVER_PORT,
//  _GEN_MAX_RUNTIME, _GEN_UNDER_VOLT, _GEN_ERROR_RESET_EN, _GEN_ERROR_RESET_MIN, _GEN_WARM_UP_TIME, _GEN_COOL_DOWN_TIME,
//  _GEN_NIGHT_EN, _GEN_NIGHT_BEGIN, _GEN_NIGHT_END, _DC_LOW_INPUT, _DC_LOW_VOLT,
//  _AIRCON_TEMP1,_AIRCON_TEMP2,_AIRCON_TEMP3,_AIRCON_TEMP4,_AIRCON_TIME1,_AIRCON_TIME2,
//  _HOUR, _MINS, _SECS, _DATE, _MONTH, _YEAR
//}setting_values;
//
//typedef struct
//{
//  uint8_t   step;
//  uint16_t  lowerVal;
//  uint16_t  upperVal;
//  uint16_t  addrEEPROM;
//  int16_t   realVal;
//  int16_t   defaultVal;
//  uint8_t   point;
//}sSetting_Values_Struct;
//extern sSetting_Values_Struct		sSetting_Values[48];
//
//typedef struct
//{
//  uint8_t   status;
//  uint8_t   checked;
//  uint16_t  index;
//  char      alarmName[21];
//}sActive_Alarm_Struct;
//extern sActive_Alarm_Struct		sActive_Alarm[17];

void Key_Scane (void);
void Menu_Scane (void);
void Active_Alarm_Scane (void);
void Menu_Nomarl_Display(void);
void Display_Menu_Directory (uint8_t mMenu);
void Display_Active_Alarm(void);
void Display_System_Monitor(void);

void Display_Alarm_Settings(void);
void Display_ATS_Settings(void);
void Display_Accessories_Settings(void);
void Display_DeviceAddress_Settings(void);
void Display_ServerAddress_Settings(void);
void Display_UserAccount_Settings(void);
void Display_Device_Info_Settings(void);
void Display_Operation_Logs(void);
void Display_ATS_Logs(void);
void Display_Datetime_Setting(void);
void Display_DeviceInfo_Setting(void);
void Display_Setting_Confirm(void);
void Display_User_ID_Setting(void);
void Display_AirCon_Setting(void);

void Save_User_ID (uint16_t EEPROM_Addr, uint8_t* UserID);
void Delete_User_ID (uint16_t EEPROM_Addr, uint8_t* UserID);

void Ajust_Variable_Value (uint8_t inc_dec, uint16_t* mVariable, uint8_t	variableIndex);
void Write_Default_All_Variable (void);
void Init_All_Variable (void);
void Default(uint16_t MemoryAdress,uint16_t *Value,uint16_t default_value);
void ReadMemory(uint16_t Index,uint16_t *Value);


extern uint8_t		mTempVal_u8[16];
extern uint16_t	mTempVal_u16[16];
extern uint32_t	mTempVal_u32[16];
extern uint8_t    TempUserID[5][9];

//typedef struct
//{
//  uint16_t	u16AcThresVolt[3];
//  uint16_t	u16BattThresVolt[2];
//  uint16_t	u16ThresTemp[4];
//  uint16_t	u16DevIP[4];
//  uint16_t	u16DevGateway[4];
//  uint16_t	u16DevSubnet[4];
//  uint16_t	u16DevPort;
//  uint16_t	u16ServerIP[4];
//  uint16_t	u16ServerPort;
//  uint8_t	u8userName[12];
//  uint8_t	u8AuthPass[12];
//  uint8_t	u8PrivPass[12];
//  uint8_t	u8Key[12];
//  uint16_t	u16RefreshInterval;
//  uint8_t	u8UserID[5][8];
//  uint16_t	u8UserIDAddr[5];
//  uint16_t	u16AirConTemp[4];
//  uint16_t	u16AirConTime1;
//  uint16_t	u16AirConTime2;
//
//  uint16_t	u16GENMaxRuntime;
//  uint16_t	u16GENUnderVolt;
//  uint16_t	u16GENErrorResetEnable;
//  uint16_t	u16GENErrorResetTime;
//  uint16_t	u16GENWarmUpTime;
//  uint16_t	u16GENCoolDownTime;
//  uint16_t	u16GENNightEnable;
//  uint16_t	u16GENNightStart;
//  uint16_t	u16GENNightEnd;
//  uint16_t	u16GENDCLowInput;
//  uint16_t	u16GENDCLowVolt;
//
//  uint16_t	changedVal;
//}sMenu_Variable_Struct;
//extern sMenu_Variable_Struct	sMenu_Variable;
//
//typedef struct
//{
//	uint16_t battVolt;
//	uint16_t	grid_Volt;
//	uint16_t	genVolt;
//	uint8_t	grid_VoltCheckCnt;
//	uint8_t	grid_Status;
//	uint8_t	activePower;
//	uint8_t	batt_DCLow;
//	uint8_t	switch_Grid_ON;
//	uint8_t	switch_GEN_ON;
//	uint8_t	Gen1Voltage;
//	uint8_t	GenEnable;
//	uint8_t	GenStart;
//	uint8_t	Gen1Fail;
//	uint8_t	Gen1ControlStatus;
//	uint8_t	StartStep;
//	uint8_t	StartStep_cnt;
//	uint8_t	StopStep;
//	uint8_t	StopStep_cnt;
//	uint8_t	ReStart_Cnt;
//	uint32_t	MaxRunTimeCnt;
//	uint32_t	vRMS_u32;
//	uint32_t	iRMSA_u32;
//	uint32_t	instan_ActPwA_i32;
//	uint16_t	power_Factor_i16;
//	uint16_t	frequency_i16;
//	uint8_t	year;
//	uint8_t	month;
//	uint8_t	date;
//	uint8_t	hour;
//	uint8_t	mins;
//	uint8_t	secs;
//}sATS_Variable_Struct;
//extern sATS_Variable_Struct	sATS_Variable;
//
//typedef struct
//{
//   uint16_t  indorTemp;
//   uint16_t  outdoorTemp;
//   uint32_t  airCon1Runtime;
//   uint32_t  airCon2Runtime;
//   uint8_t  airCon1Status;
//   uint8_t  airCon2Status;
//   uint8_t  fanStatus;
//	uint8_t	year;
//	uint8_t	month;
//	uint8_t	date;
//	uint8_t	hour;
//	uint8_t	mins;
//	uint8_t	secs;
//}sAirCon_Variable_Struct;
//extern sAirCon_Variable_Struct   sAirCon_Variable;
