#include <string.h>
#include "board.h"
#include "pin_mux.h"
#include "fsl_gpio.h"
#include "menu.h"
#include "lm2068.h"
#include "eeprom_rtc.h"
#include "rs485.h"
#include <stdio.h>
#include "variables.h"
#include "core/net.h"
#include "access_control.h"
#include "net_config.h"
#include "am2320.h"
#include "freeRTOS.h"
#include "task.h"
#include "i2c_lock.h"

#define EEPROM_WRITE_FLAG   0x55

uint8_t		mTempVal_u8[16];
uint16_t    mTempVal_u16[16];
uint32_t	mTempVal_u32[16];
uint16_t    deviceNameLength;
char deviceName[DEVICE_NAME_MAX_LENGTH + 1];

sATS_Variable_Struct		sATS_Variable;          
sAirCon_Variable_Struct     sAirCon_Variable;
sMenu_Variable_Struct	    sMenu_Variable;
sKey_Control_Struct    	    sKey_Control;
sMenu_Control_Struct		sMenu_Control;
sSetting_Values_Struct		sSetting_Values[NUMBER_OF_SETTING] =          // chaunm add initialize
{       //step    lower     upper   addr  rVal   dVal    point
    {1     ,     1,    1000,    0,     0,   180,      0},  //_AC_LOW
    {1     ,     1,    1000,    2,     0,   485,      1},  //_DC_LOW
    {1     ,     1,    1000,    4,     0,   250,      1},  //_TEMP1
    {1     ,     1,    1000,    6,     0,   250,      1},  //_TEMP2
    {1     ,     1,    1000,    8,     0,   250,      1},  //_TEMP3
    {1     ,     1,    1000,   10,     0,   250,      1},  //_TEMP4
    {1     ,     0,     255,   12,     0,   192,      0},  //_DEV_IP1
    {1     ,     0,     255,   14,     0,   168,      0},  //_DEV_IP2
    {1     ,     0,     255,   16,     0,     1,      0},  //_DEV_IP3
    {1     ,     0,     255,   18,     0,   247,      0},  //_DEV_IP4
    {1     ,     0,     255,   20,     0,   255,      0},  //_DEV_SUBNET1
    {1     ,     0,     255,   22,     0,   255,      0},  //_DEV_SUBNET2
    {1     ,     0,     255,   24,     0,   255,      0},  //_DEV_SUBNET3
    {1     ,     0,     255,   26,     0,     0,      0},  //_DEV_SUBNET4
    {1     ,     0,     255,   28,     0,   192,      0},  //_DEV_GATEW1
    {1     ,     0,     255,   30,     0,   168,      0},  //_DEV_GATEW2
    {1     ,     0,     255,   32,     0,     1,      0},  //_DEV_GATEW3
    {1     ,     0,     255,   34,     0,     1,      0},  //_DEV_GATEW4
    {1     ,     0,   65535,   36,     0,   161,      0},  //_DEV_PORT
    {1     ,     0,     255,   38,     0,   117,      0},  //_SERVER_IP1
    {1     ,     0,     255,   40,     0,     0,      0},  //_SERVER_IP2
    {1     ,     0,     255,   42,     0,   212,      0},  //_SERVER_IP3
    {1     ,     0,     255,   44,     0,    56,      0},  //_SERVER_IP4
    {1     ,     0,   65535,   46,     0,   162,      0},  //_SERVER_PORT
    {5     ,     5,    4320,   48,     0,     4,      0},  //_GEN_MAX_RUNTIME	3 days
    {5     ,   100,     500,   50,     0,   130,      0},  //_GEN_UNDER_VOLT
    {1     ,     0,       1,   52,     0,     0,      0},  //_GEN_ERROR_RESET_EN
    {1     ,     0,     255,   54,     0,    30,      0},  //_GEN_ERROR_RESET_MIN
    {5     ,     0,    1800,   56,     0,    60,      0},  //_GEN_WARM_UP_TIME
    {5     ,     0,    1800,   58,     0,   180,      0},  //_GEN_COOL_DOWN_TIME
    {1     ,     0,       1,   60,     0,     0,      0},  //_GEN_NIGHT_EN
    {5     ,     0,    1435,   62,     0,  1320,      0},  //_GEN_NIGHT_BEGIN
    {5     ,     0,    1435,   64,     0,   360,      0},  //_GEN_NIGHT_END
    {1     ,     0,       2,   66,     0,     1,      0},  //_DC_LOW_INPUT	0-NC 1-NO 2-By voltage
    {1     ,     0,    1000,   68,     0,   470,      1},  //_DC_LOW_VOLT
    {1     ,     1,    1000,   70,     0,   250,      1},  //_AIRCON_TEMP1
    {1     ,     1,    1000,   72,     0,   250,      1},  //_AIRCON_TEMP2
    {1     ,     1,    1000,   74,     0,   250,      1},  //_AIRCON_TEMP3
    {1     ,     1,    1000,   76,     0,   250,      1},  //_AIRCON_TEMP4  
    {1     ,     1,    1000,   78,     0,   250,      0},  //_AIRCON_TIME1
    {1     ,     1,    1000,   80,     0,   250,      0},  //_AIRCON_TIME2
    {1     ,     0,      23,    0,     0,     0,      0},  //_HOUR
    {1     ,     0,      59,    0,     0,     0,      0},  //_MINS
    {1     ,     0,      59,    0,     0,     0,      0},  //_SECS
    {1     ,     1,      31,    0,     0,     0,      0},  //_DATE
    {1     ,     1,      12,    0,     0,     0,      0},  //_MONTH
    {1     ,     0,      99,    0,     0,     0,      1},  //_YEAR
    {1     ,     0,     255,   82,     0,   170,      0},   //_DEFAULT_WRITE      
    {1     ,     0,     255,   84,     0,     7,      0}
};

sMenu_Object_Struct		sMenu_Object[19] =
{// menuIndex			menuName				                parentIndex	  numberSubMenu	    subMenuIndex
    {_MAIN_MENU,		"Main Menu           ",	                0xFF,				4,			0},
    {_ACTIVE_ALARM,		"Active Alarms       ",	                _MAIN_MENU,			0,			0},
    {_SYS_MONITOR,		"System Monitor      ",	                _MAIN_MENU,			0,			1},
    {_SYS_LOG,		    "System Logs         ",	                _MAIN_MENU,	    	2,			2},
    {_SYS_SETTING,		"System Settings     ",	                _MAIN_MENU,			6,			3},
    {_OPERATION_LOGS,	"Operation Logs      ",	                _SYS_LOG,			0,			0},
    {_ATS_LOGS,		    "ATS Logs            ",	                _SYS_LOG,			0,			1},
    {_ALARM_SETTING,	"Alarm Settings      ",                 _SYS_SETTING,		0,			0},
    {_ATS_SETTING,		"ATS Settings        ",                 _SYS_SETTING,		0,			1},
    {_ACCES_SETTING,	"Accessories Sets    ",                 _SYS_SETTING,		2,			2},
    {_NETWORK_CONFIG,	"Network Config      ",                 _SYS_SETTING,		3,			3},
    {_DATETIME_CONFIG,	"Datetime Config     ",                 _SYS_SETTING,		0,			4},
    {_DEVICE_INFO,		"Device Info         ",                 _SYS_SETTING,		0,			5},
    {_DEVICE_ADDRESS,	"Device Address      ",                 _NETWORK_CONFIG,	0,			0},
    {_SERVER_ADDRESS,	"Server Address      ",                 _NETWORK_CONFIG,	0,			1},
    {_USER_INFO,		"User Account        ",                 _NETWORK_CONFIG,	0,			2},
    {_USER_ID,   	    "User ID             ",                 _ACCES_SETTING,		0,			0},
    {_AIRCON,		    "Air Conditioner     ",                 _ACCES_SETTING,	    0,			1},
    {_SETTING_CONFIRM,	"Setting Confirm     ",                 0xFF,				0,			0},
};

sActive_Alarm_Struct		sActive_Alarm[ACTIVE_ALARM_NUMBER] = 
{// status	checked	index		alarmName	
    { 0,      0,       0,         ":AC Low          "},//0
    { 0,      0,       0,         ":DC Low          "},//1
    { 0,      0,       0,         ":Fire Detected   "},//2 Digital Input 1
    { 0,      0,       0,         ":Fire Wire Error "},//3 Digital Input 1
    { 0,      0,       0,         ":Smoke Detected  "},//4 Digital Input 2
    { 0,      0,       0,         ":Smoke Wire Error"},//5 Digital Input 2
    { 0,      0,       0,         ":Move Detected   "},//6 Digital Input 3
    { 0,      0,       0,         ":Move Wire Error "},//7 Digital Input 3
    { 0,      0,       0,         ":Water Detected  "},//8 Digital Input 4
    { 0,      0,       0,         ":Water Wire Error"},//9 Digital Input 4
    { 0,      0,       0,         ":Door Opened     "},//10 Digital Input 5
    { 0,      0,       0,         ":Door Wire Error "},//11 Digital Input 5
    { 0,      0,       0,         ":ATS: GEN Error  "},//12
    { 0,      0,       0,         ":ATS: DC Low     "},//13
    { 0,      0,       0,         ":ATS: GEN STOP   "},//14
    { 0,      0,       0,         ":ATS Offline     "},//15
    { 0,      0,       0,         ":AirCon Offline  "},//16
    { 0,      0,       0,         ":Over AC Current "},//17
};

void Active_Alarm_Scane (void)
{
    uint8_t   i=0,j=0;
    
    for(i=0;i<ACTIVE_ALARM_NUMBER;i++)
    {
        if((sActive_Alarm[i].status == 1)&&(sActive_Alarm[i].checked == 0))
        {
            sMenu_Control.totalActiveAlarm++;
            sMenu_Control.refesh = 1;
            for(j=0;j<ACTIVE_ALARM_NUMBER;j++)
                if(sActive_Alarm[j].index != 0)
                    sActive_Alarm[j].index++;
            sActive_Alarm[i].checked = 1;
            sActive_Alarm[i].index = 1;
        }
        if((sActive_Alarm[i].status == 0)&&(sActive_Alarm[i].checked == 1))
        {
            for(j=0;j<ACTIVE_ALARM_NUMBER;j++)
                if(sActive_Alarm[j].index > sActive_Alarm[i].index)
                    sActive_Alarm[j].index--;
            sMenu_Control.totalActiveAlarm--;
            sMenu_Control.refesh = 1;
            sActive_Alarm[i].checked = 0;
            sActive_Alarm[i].index = 0;
        }
    }
}

//Call every 10ms -> if pressed more than 150ms -> button pressed
void Key_Scane (void)
{
    if(!READ_KEY1)
    {
        sKey_Control.key1Count++;
        if(sKey_Control.key1Count >= 10)
        {
            sKey_Control.key1Pressed = 1;
            sKey_Control.pressedKey = _ENTER_KEY;
        }
    }
    else
    {
        sKey_Control.key1Count = 0;
    }
    
    if(!READ_KEY2)
    {
        sKey_Control.key2Count++;
        if(sKey_Control.key2Count >= 10)
        {
            sKey_Control.key2Pressed = 1;
            sKey_Control.pressedKey = _DOWN_KEY;
        }
    }
    else
    {
        sKey_Control.key2Count = 0;
    }
    
    if(!READ_KEY3)
    {
        sKey_Control.key3Count++;
        if(sKey_Control.key3Count >= 10)
        {
            sKey_Control.key3Pressed = 1;
            sKey_Control.pressedKey = _UP_KEY;
        }
    }
    else
    {
        sKey_Control.key3Count = 0;
    }
    
    if(!READ_KEY4)
    {
        sKey_Control.key4Count++;
        if(sKey_Control.key4Count >= 10)
        {
            sKey_Control.key4Pressed = 1;
            sKey_Control.pressedKey = _ESC_KEY;
        }
    }
    else
    {
        sKey_Control.key4Count = 0;
    }
    
    if(sKey_Control.pressedKey != 0)
    {
        //sMenu_Control.timeOut_u16 = 0;
        sKey_Control.key1Count = 0;
        sKey_Control.key2Count = 0;
        sKey_Control.key3Count = 0;
        sKey_Control.key4Count = 0;
    }
}

void Menu_Scane (void)
{
    switch(sMenu_Control.mode)
    {
    case 0:
        Menu_Nomarl_Display();
        break;
    case 1:
        switch(sMenu_Control.menu)
        {
        case _MAIN_MENU:
            Display_Menu_Directory(_MAIN_MENU);
            break;
        case _ACTIVE_ALARM:
            Display_Active_Alarm();
            break;
        case _SYS_MONITOR:
            Display_System_Monitor();
            break;
        case _SYS_LOG:
            Display_Menu_Directory(_SYS_LOG);
            break;
        case _SYS_SETTING:
            Display_Menu_Directory(_SYS_SETTING);
            break;
        case _ALARM_SETTING:
            Display_Alarm_Settings();
            break;
        case _ATS_SETTING:
            Display_ATS_Settings();
            break;
        case _ACCES_SETTING: 
            Display_Menu_Directory(_ACCES_SETTING);
            break;
        case _NETWORK_CONFIG:
            Display_Menu_Directory(_NETWORK_CONFIG);
            break;
        case _DEVICE_ADDRESS:
            Display_DeviceAddress_Settings();
            break;
        case _SERVER_ADDRESS:
            Display_ServerAddress_Settings();
            break;
        case _USER_INFO:
            Display_UserAccount_Settings();
            break;
        case _OPERATION_LOGS:
            Display_Operation_Logs();
            break;
        case _ATS_LOGS:
            Display_ATS_Logs();
            break;
        case _DATETIME_CONFIG:
            Display_Datetime_Setting();
            break;
        case _DEVICE_INFO:
            Display_DeviceInfo_Setting();
            break;
        case _USER_ID:
            Display_User_ID_Setting();
            break;
        case _AIRCON:
            Display_AirCon_Setting();
            break;
        case _SETTING_CONFIRM:
            Display_Setting_Confirm();
            break;
        default:
            break;
        }
        break;
    }
}

void TimerCntShow(uint8_t row, uint8_t col,uint32_t count){
    uint32_t temp1 = 0;
    uint32_t temp2 = 0;
    temp1 = count/3600;
    temp2 = count%3600;
    glcd_writeString("  :  :  ",row,col);
    glcd_Display_Integer_With_Zero(row,col,2,temp1);
    temp1 = temp2/60;
    glcd_Display_Integer_With_Zero(row,col+3,2,temp1);
    temp1 = temp2%60;
    glcd_Display_Integer_With_Zero(row,col+6,2,temp1);
}

void Menu_Nomarl_Display(void)
{
    if(sMenu_Control.refreshMainPageCount>=300)
    {
        sMenu_Control.refesh = 1;
        sMenu_Control.refreshMainPageCount = 0;
    }
    if(sMenu_Control.init == 1)
    {
        gldc_clearGraphicScreen(LAYER1_ADDR);
        gldc_clearGraphicScreen(LAYER2_ADDR);
        
        switch(sMenu_Control.mainPageIndex)
        {
        case 0:
            glcd_writeString("  :  ",1,16);
            glcd_writeString("Home",2,9);
            glcd_writeString("Temp:",6,1);
            glcd_writeString("Humi:",6,12);
            glcd_writeString("ATS:",7,1);
            glcd_writeString("AirCon:",8,1);
            glcd_Draw_Black_Rectangle(LAYER2_ADDR,1,1,20);
            break;
        case 1:
            glcd_Draw_Black_Rectangle(LAYER2_ADDR,1,1,20);
            glcd_writeString("  :  :  ",1,13);
            glcd_writeString("Active Power:       ",2,1);
            glcd_writeString("Battery:    V       ",3,1);
            glcd_writeString("AC:      V        A ",4,1);
            glcd_writeString("      W    Hz PF:   ",5,1);
            glcd_writeString("Gen Status:         ",6,1);
            glcd_writeString("Gen Control:        ",7,1);
            break;
        case 2:
            glcd_Draw_Black_Rectangle(LAYER2_ADDR,1,1,20);
            glcd_writeString("  :  :  ",1,13);
            glcd_writeString("Indoor Temp:      C ",2,1);
            glcd_writeString("Outdoor Temp:     C ",3,1);
            glcd_writeString("Air-Con 1:          ",4,1);
            glcd_writeString("  Runtime:        m ",5,1);
            glcd_writeString("Air-Con 2:          ",6,1);
            glcd_writeString("  Runtime:        m ",7,1);
            glcd_writeString("Fan:                ",8,1);
            break;
        default:
            sMenu_Control.mainPageIndex = 0;
            break;
        }
        sMenu_Control.init = 0;
    }
    
    if(sMenu_Control.refesh)
    {
        int16_t temp1 = 0;
        int16_t temp2 = 0;
        
        switch(sMenu_Control.mainPageIndex)
        {
        case 0:
            glcd_Display_Integer_With_Zero(1,16,2,GTime.hour);
            glcd_Display_Integer_With_Zero(1,19,2,GTime.min);
            //glcd_Display_Integer_With_Zero(1,19,2,GTime.sec);
            
            if(sMenu_Control.accessUID == -1)
                glcd_writeString("   Access Denied    ",4,1);
            else if (sMenu_Control.accessUID == 0)
                glcd_writeString("                    ",4,1);
            else
            {
                glcd_writeString("      User          ",4,1);
                glcd_Display_Integer(4,11,1,sMenu_Control.accessUID);
            }
            glcd_Display_Float_1Dec(6,6,4,u16Temper);
            glcd_Display_Float_1Dec(6,17,4,u16HumiRh);
            if(Modbus.atsError == 0) 
                glcd_writeString("Online ",7,5);
            else
                glcd_writeString("Offline",7,5);
            
            if(Modbus.airConError == 0) 
                glcd_writeString("Online ",8,8);
            else
                glcd_writeString("Offline",8,8);
            break;
        case 1:
            if(Modbus.atsError == 0) 
                glcd_writeString("Online ",1,1);
            else
                glcd_writeString("Offline",1,1);
            
            glcd_Display_Integer_With_Zero(1,13,2,sATS_Variable.hour);
            glcd_Display_Integer_With_Zero(1,16,2,sATS_Variable.mins);
            glcd_Display_Integer_With_Zero(1,19,2,sATS_Variable.secs);
            if(sATS_Variable.activePower == 1)
                glcd_writeString("Grid",2,15);
            else if (sATS_Variable.activePower == 2)
                glcd_writeString("Gen ",2,15);
            else
                glcd_writeString("Batt",2,15);
            
            glcd_Display_Float_1Dec(3,9,4,sATS_Variable.battVolt/10);
            if(sATS_Variable.batt_DCLow)
                glcd_writeString("DCLow",3,16);
            else
                glcd_writeString("     ",3,16);
            glcd_Display_Float_1Dec(4,5,5,sATS_Variable.vRMS_u32/10);
            glcd_Display_Float_1Dec(4,14,5,sATS_Variable.iRMSA_u32/10);
            glcd_Display_Float_1Dec(5,1,6,sATS_Variable.instan_ActPwA_i32/10);
            
            temp1 = sATS_Variable.frequency_i16/100;
            temp2 = temp1*100;
            if(sATS_Variable.frequency_i16-50>=temp2) temp1++;
            glcd_Display_Integer(5,10,2,temp1); 
            if(sATS_Variable.power_Factor_i16>1000) sATS_Variable.power_Factor_i16 = 2000-sATS_Variable.power_Factor_i16;
            glcd_Display_Float_1Dec(5,18,3,sATS_Variable.power_Factor_i16/100);
            
            if(sATS_Variable.Gen1Fail==1){
                glcd_writeString("Fault",6,13);
            }
            else{
                if(sATS_Variable.Gen1Voltage==1)        glcd_writeString("Run  ",6,13);
                else if(sATS_Variable.Gen1Voltage==0)   glcd_writeString("Stop ",6,13);
            }
            
            if(sATS_Variable.grid_Status==2) //Cho dien luoi on dinh
            {
                glcd_writeString("Grid: ",8,1);
                glcd_Display_Integer_With_Zero(8,7,2,sATS_Variable.grid_VoltCheckCnt);
            }
            else //Co dien luoi hoac mat dien luoi
            {
                glcd_writeString("          ",8,1);
            }
            
            switch(sATS_Variable.Gen1ControlStatus){
            case 0:
                glcd_writeString("Stop    ",7,13);
                glcd_writeString("        ",8,13);
                break;
            case 1:
                glcd_writeString("Run     ",7,13);
                TimerCntShow(8,13,sATS_Variable.MaxRunTimeCnt);
                break;
            case 2:
                glcd_writeString("Start   ",7,13);
                glcd_Display_Integer_With_Zero(8,18,3,sATS_Variable.StartStep_cnt);
                glcd_writeString("     ",8,13);
                break;
            case 3:
                glcd_writeString("Check   ",7,13);
                glcd_Display_Integer_With_Zero(8,18,3,sATS_Variable.StartStep_cnt);
                glcd_writeString("     ",8,13);
                break;
            case 4:
                glcd_writeString("ReStart ",7,13);
                break;
            case 5:
                glcd_writeString("Warmup  ",7,13);
                glcd_Display_Integer_With_Zero(8,18,3,sATS_Variable.StartStep_cnt);
                glcd_writeString("     ",8,13);
                break;
            case 6:
                glcd_writeString("CoolDown",7,13);
                glcd_Display_Integer_With_Zero(8,18,3,sATS_Variable.StopStep_cnt);
                glcd_writeString("     ",8,13);
                break;
            case 7:
                glcd_writeString("Fault   ",7,13);
                glcd_writeString("        ",8,13);
                break;
            case 10:
                glcd_writeString("WaitOff ",7,13);
                glcd_Display_Integer_With_Zero(8,18,3,sATS_Variable.StopStep_cnt);
                glcd_writeString("     ",8,13);
                break;   
            }
            break;
        case 2:
            if(Modbus.airConError == 0) 
                glcd_writeString("Online ",1,1);
            else
                glcd_writeString("Offline",1,1);
            
            glcd_Display_Integer_With_Zero(1,13,2,sAirCon_Variable.hour);
            glcd_Display_Integer_With_Zero(1,16,2,sAirCon_Variable.mins);
            glcd_Display_Integer_With_Zero(1,19,2,sAirCon_Variable.secs);
            
            glcd_Display_Float_1Dec(2,15,4,sAirCon_Variable.indoorTemp);
            glcd_Display_Float_1Dec(3,15,4,sAirCon_Variable.outdoorTemp);
            if(sAirCon_Variable.airCon1Status == 1)
            {
                glcd_writeString("Running",4,14);
                glcd_Display_Integer(5,14,5,(uint16_t)sAirCon_Variable.airCon1Runtime);
            }
            else
            {
                glcd_writeString(" Stop  ",4,14);
                glcd_writeString(" --- ",5,14);
            }
            if(sAirCon_Variable.airCon2Status == 1)
            {
                glcd_writeString("Running",6,14);
                glcd_Display_Integer(7,14,5,(uint16_t)sAirCon_Variable.airCon2Runtime);
            }
            else
            {
                glcd_writeString(" Stop  ",6,14);
                glcd_writeString(" --- ",7,14);
            }
            if(sAirCon_Variable.fanStatus == 1)
                glcd_writeString("Running",8,7);
            else
                glcd_writeString(" Stop  ",8,7);
            break;
        default:
            sMenu_Control.mainPageIndex = 0;
            break;
        }
        sMenu_Control.refesh = 0;
    }
    
    switch(sKey_Control.pressedKey)
    {
    case _UP_KEY:
        sMenu_Control.mainPageIndex++;
        if(sMenu_Control.mainPageIndex>2)
            sMenu_Control.mainPageIndex = 0;
        sMenu_Control.init = 1;
        sMenu_Control.refesh = 1;
        sKey_Control.pressedKey = 0;
        break;
    case _DOWN_KEY:
        sMenu_Control.mainPageIndex--;
        if(sMenu_Control.mainPageIndex<0)
            sMenu_Control.mainPageIndex = 2;
        sMenu_Control.init = 1;
        sMenu_Control.refesh = 1;
        sKey_Control.pressedKey = 0;
        break;
    case _ENTER_KEY:
        //Goto user menu
        sMenu_Control.mode = 1;
        sMenu_Control.menu = _MAIN_MENU;
        sMenu_Control.index = 0;
        
        sMenu_Control.init = 1;
        sMenu_Control.refesh = 1;
        sKey_Control.pressedKey = 0;
        break;
    case _ESC_KEY:
        sMenu_Control.refesh = 1;
        sKey_Control.pressedKey = 0;
        break;
    default:
        break;
    }
}

void Display_Menu_Directory (uint8_t mMenu)
{
    uint8_t i=0;
    
    if(sMenu_Control.init)
    {
        gldc_clearGraphicScreen_DataArea(LAYER1_ADDR);
        gldc_clearGraphicScreen_DataArea(LAYER2_ADDR);
        glcd_writeString(sMenu_Object[mMenu].menuName,1,1);
        glcd_Draw_Black_Rectangle(LAYER2_ADDR,1,1,20);
        
        for(i=0;i<19;i++)
        {
            if(sMenu_Object[i].parentIndex == mMenu)
                glcd_writeString(sMenu_Object[i].menuName,sMenu_Object[i].subMenuIndex+2,3);
        }
        
        sMenu_Control.init = 0;
    }
    
    if(sMenu_Control.refesh)
    {
        gldc_clearGraphicScreen_DataArea(LAYER2_ADDR);
        glcd_Draw_Black_Rectangle(LAYER2_ADDR,1,1,20);
        glcd_Under_Line_Text(LAYER2_ADDR,sMenu_Control.index+2,3,20);
        glcd_Draw_Black_Rectangle(LAYER2_ADDR,sMenu_Control.index+2,20,20);
        sMenu_Control.refesh = 0;
    }
    
    switch(sKey_Control.pressedKey)
    {
    case _UP_KEY:
        sMenu_Control.index--;
        if(sMenu_Control.index<0)
            sMenu_Control.index = sMenu_Object[mMenu].numberSubMenu - 1;
        
        sMenu_Control.refesh = 1;
        sKey_Control.pressedKey = 0;
        break;
    case _DOWN_KEY:
        sMenu_Control.index++;
        if(sMenu_Control.index >= sMenu_Object[mMenu].numberSubMenu)
            sMenu_Control.index = 0;
        
        sMenu_Control.refesh = 1;
        sKey_Control.pressedKey = 0;
        break;
    case _ENTER_KEY:
        for(i=0;i<19;i++)
            if((sMenu_Object[i].parentIndex == mMenu)&&(sMenu_Object[i].subMenuIndex == sMenu_Control.index))
            {
                sMenu_Control.menu = sMenu_Object[i].menuIndex;
                break;
            }
        sMenu_Control.previousIndex = sMenu_Control.index;
        sMenu_Control.index = 0;
        sMenu_Control.init = 1;
        sMenu_Control.refesh = 1;
        sKey_Control.pressedKey = 0;
        break;
    case _ESC_KEY:
        if(mMenu == _MAIN_MENU)
            sMenu_Control.mode = 0;
        else
            sMenu_Control.menu = sMenu_Object[mMenu].parentIndex;
        sMenu_Control.index = sMenu_Control.previousIndex;
        sMenu_Control.previousIndex = 0;
        sMenu_Control.init = 1;
        sMenu_Control.refesh = 1;
        sKey_Control.pressedKey = 0;
        break;
    default:
        break;
    }
}

void Display_Active_Alarm(void)
{
    uint8_t   totalActiveAlarmPage = 0, i = 0;
    
    if((sMenu_Control.totalActiveAlarm%7)==0)
        totalActiveAlarmPage = sMenu_Control.totalActiveAlarm/7;
    else
        totalActiveAlarmPage = sMenu_Control.totalActiveAlarm/7 + 1;
    
    if(sMenu_Control.init)
    {
        gldc_clearGraphicScreen_DataArea(LAYER1_ADDR);
        gldc_clearGraphicScreen_DataArea(LAYER2_ADDR);
        glcd_writeString(sMenu_Object[_ACTIVE_ALARM].menuName,1,1);
        glcd_Draw_Black_Rectangle(LAYER2_ADDR,1,1,20);
        sMenu_Control.init = 0;
    }
    if(sMenu_Control.refesh)
    {
        if(sMenu_Control.totalActiveAlarm == 0)
        {
            gldc_clearGraphicScreen_DataArea(LAYER1_ADDR);
            glcd_Draw_Black_Rectangle(LAYER2_ADDR,1,1,20);
            glcd_writeString("     Don't have     ",3,1);
            glcd_writeString("    Active Alarm    ",4,1);
        }else{
            gldc_clearGraphicScreen_DataArea(LAYER1_ADDR);
            glcd_Draw_Black_Rectangle(LAYER2_ADDR,1,1,20);
            for(i=0;i<17;i++)
            {
                if(sActive_Alarm[i].index>0)
                    if((sActive_Alarm[i].index-1)/7 == sMenu_Control.alarmPageIndex)
                    {
                        if((sActive_Alarm[i].index%7) != 0)
                        {
                            glcd_writeString(sActive_Alarm[i].alarmName,sActive_Alarm[i].index%7+1,4);
                            glcd_Display_Integer_With_Zero(sActive_Alarm[i].index%7+1,2,2,sActive_Alarm[i].index);
                        }
                        else
                        {
                            glcd_writeString(sActive_Alarm[i].alarmName,8,4);
                            glcd_Display_Integer_With_Zero(8,2,2,sActive_Alarm[i].index);
                        }
                    }
            }
        }
        sMenu_Control.refesh = 0;
    }
    
    switch(sKey_Control.pressedKey)
    {
    case _UP_KEY:
        sMenu_Control.alarmPageIndex--;
        if(sMenu_Control.alarmPageIndex < 0)
            sMenu_Control.alarmPageIndex = totalActiveAlarmPage-1;
        sMenu_Control.refesh = 1;
        sKey_Control.pressedKey = 0;
        break;
    case _DOWN_KEY:
        sMenu_Control.alarmPageIndex++;
        if(sMenu_Control.alarmPageIndex > (totalActiveAlarmPage-1))
            sMenu_Control.alarmPageIndex = 0;
        sMenu_Control.refesh = 1;
        sKey_Control.pressedKey = 0;
        break;
    case _ENTER_KEY:
        sMenu_Control.refesh = 1;
        sKey_Control.pressedKey = 0;
        break;
    case _ESC_KEY:
        sMenu_Control.menu = _MAIN_MENU;
        sMenu_Control.index = sMenu_Control.previousIndex;
        sMenu_Control.init = 1;
        sMenu_Control.refesh = 1;
        sKey_Control.pressedKey = 0;
        break;
    default:
        break;
    }  
}

void Display_System_Monitor(void)
{
    if(sMenu_Control.init)
    {
        gldc_clearGraphicScreen_DataArea(LAYER1_ADDR);
        gldc_clearGraphicScreen_DataArea(LAYER2_ADDR);
        glcd_writeString(sMenu_Object[_SYS_MONITOR].menuName,1,1);
        glcd_Draw_Black_Rectangle(LAYER2_ADDR,1,1,20);
        sMenu_Control.init = 0;
    }
    if(sMenu_Control.refesh)
    {
        
        sMenu_Control.refesh = 0;
    }
    
    switch(sKey_Control.pressedKey)
    {
    case _UP_KEY:
        sMenu_Control.refesh = 1;
        sKey_Control.pressedKey = 0;
        break;
    case _DOWN_KEY:
        sMenu_Control.refesh = 1;
        sKey_Control.pressedKey = 0;
        break;
    case _ENTER_KEY:
        sMenu_Control.refesh = 1;
        sKey_Control.pressedKey = 0;
        break;
    case _ESC_KEY:
        sMenu_Control.menu = _MAIN_MENU;
        sMenu_Control.index = sMenu_Control.previousIndex;
        sMenu_Control.init = 1;
        sMenu_Control.refesh = 1;
        sKey_Control.pressedKey = 0;
        break;
    default:
        break;
    }    
}

void Display_Alarm_Settings(void)
{
    if(sMenu_Control.init)
    {
        mTempVal_u16[0] = sMenu_Variable.u16AcThresVolt[0];
        mTempVal_u16[1] = sMenu_Variable.u16BattThresVolt[0];
        mTempVal_u16[2] = sMenu_Variable.u16ThresTemp[0];
        mTempVal_u16[3] = sMenu_Variable.u16ThresTemp[1];
        mTempVal_u16[4] = sMenu_Variable.u16ThresTemp[2];
        mTempVal_u16[5] = sMenu_Variable.u16ThresTemp[3];
        
        gldc_clearGraphicScreen_DataArea(LAYER1_ADDR);
        gldc_clearGraphicScreen_DataArea(LAYER2_ADDR);
        glcd_writeString(sMenu_Object[_ALARM_SETTING].menuName,1,1);
        glcd_Draw_Black_Rectangle(LAYER2_ADDR,1,1,20);
        
        glcd_writeString("  AC Low Voltage:   ",2,1);
        glcd_writeString("               VAC  ",3,1);
        glcd_writeString("  DC Low Voltage:   ",4,1);
        glcd_writeString("               VDC  ",5,1);
        glcd_writeString("  Temperature:      ",6,1);
        glcd_writeString("   1:       2:      ",7,1);
        glcd_writeString("   3:       4:      ",8,1);
        
        sMenu_Control.init = 0;
    }
    if(sMenu_Control.refesh)
    {
        glcd_Display_Integer(3,12,3,mTempVal_u16[0]);
        glcd_Display_Float_1Dec(5,11,4,mTempVal_u16[1]);
        glcd_Display_Float_1Dec(7,7,4,mTempVal_u16[2]);
        glcd_Display_Float_1Dec(7,16,4,mTempVal_u16[3]);
        glcd_Display_Float_1Dec(8,7,4,mTempVal_u16[4]);
        glcd_Display_Float_1Dec(8,16,4,mTempVal_u16[5]);
        
        if(sMenu_Control.ajustValue == 0)
            switch(sMenu_Control.index)
            {
            case 0: gldc_clearGraphicScreen_DataArea(LAYER2_ADDR);	glcd_Under_Line_Text(LAYER2_ADDR,3,12,14); break;
            case 1: gldc_clearGraphicScreen_DataArea(LAYER2_ADDR);	glcd_Under_Line_Text(LAYER2_ADDR,5,11,14); break;
            case 2: gldc_clearGraphicScreen_DataArea(LAYER2_ADDR);	glcd_Under_Line_Text(LAYER2_ADDR,7,7,10); break;
            case 3: gldc_clearGraphicScreen_DataArea(LAYER2_ADDR);	glcd_Under_Line_Text(LAYER2_ADDR,7,16,19); break;
            case 4: gldc_clearGraphicScreen_DataArea(LAYER2_ADDR);	glcd_Under_Line_Text(LAYER2_ADDR,8,7,10); break;
            case 5: gldc_clearGraphicScreen_DataArea(LAYER2_ADDR);	glcd_Under_Line_Text(LAYER2_ADDR,8,16,19); break;
            default: break;
            }
        sMenu_Control.refesh = 0;
    }
    
    switch(sKey_Control.pressedKey)
    {
    case _UP_KEY:
        if(sMenu_Control.ajustValue == 0){
            sMenu_Control.index--;
            if(sMenu_Control.index<0)
                sMenu_Control.index = 5;
        }else{
            switch(sMenu_Control.index)
            {
            case 0: Ajust_Variable_Value(0,&mTempVal_u16[0],_AC_LOW); break;
            case 1: Ajust_Variable_Value(0,&mTempVal_u16[1],_DC_LOW); break;
            case 2: Ajust_Variable_Value(0,&mTempVal_u16[2],_TEMP1); break;
            case 3: Ajust_Variable_Value(0,&mTempVal_u16[3],_TEMP2); break;
            case 4: Ajust_Variable_Value(0,&mTempVal_u16[4],_TEMP3); break;
            case 5: Ajust_Variable_Value(0,&mTempVal_u16[5],_TEMP4); break;
            default:
                break;
            }
        }
        sMenu_Control.refesh = 1;
        sKey_Control.pressedKey = 0;
        break;
    case _DOWN_KEY:
        if(sMenu_Control.ajustValue == 0){
            sMenu_Control.index++;
            if(sMenu_Control.index>5)
                sMenu_Control.index = 0;
        }else{
            switch(sMenu_Control.index)
            {
                
            case 0: Ajust_Variable_Value(1,&mTempVal_u16[0],_AC_LOW); break;
            case 1: Ajust_Variable_Value(1,&mTempVal_u16[1],_DC_LOW); break;
            case 2: Ajust_Variable_Value(1,&mTempVal_u16[2],_TEMP1); break;
            case 3: Ajust_Variable_Value(1,&mTempVal_u16[3],_TEMP2); break;
            case 4: Ajust_Variable_Value(1,&mTempVal_u16[4],_TEMP3); break;
            case 5: Ajust_Variable_Value(1,&mTempVal_u16[5],_TEMP4); break;
            default:
                break;
            }		
        }
        sMenu_Control.refesh = 1;
        sKey_Control.pressedKey = 0;
        break;
    case _ENTER_KEY:
        if(sMenu_Control.ajustValue == 0){
            switch(sMenu_Control.index)
            {
            case 0: glcd_Draw_Black_Rectangle(LAYER2_ADDR,3,12,14); break;
            case 1: glcd_Draw_Black_Rectangle(LAYER2_ADDR,5,11,14); break;
            case 2: glcd_Draw_Black_Rectangle(LAYER2_ADDR,7,7,10); break;
            case 3: glcd_Draw_Black_Rectangle(LAYER2_ADDR,7,16,19); break;
            case 4: glcd_Draw_Black_Rectangle(LAYER2_ADDR,8,7,10); break;
            case 5: glcd_Draw_Black_Rectangle(LAYER2_ADDR,8,16,19); break;
            default: break;
            }
            sMenu_Control.ajustValue = 1;
        }else{
            switch(sMenu_Control.index)
            {
            case 0: 
                sMenu_Variable.u16AcThresVolt[0] = mTempVal_u16[0]; 
                I2C_Get_Lock();
                vTaskSuspendAll();
                WriteEEPROM_Word(sSetting_Values[_AC_LOW].addrEEPROM,sMenu_Variable.u16AcThresVolt[0]); 
                xTaskResumeAll();
                I2C_Release_Lock();
                break;
            case 1: 
                sMenu_Variable.u16BattThresVolt[0] = mTempVal_u16[1]; 
                I2C_Get_Lock();
                vTaskSuspendAll();
                WriteEEPROM_Word(sSetting_Values[_DC_LOW].addrEEPROM,sMenu_Variable.u16BattThresVolt[0]); 
                xTaskResumeAll();
                I2C_Release_Lock();                          
                break;
            case 2: 
                sMenu_Variable.u16ThresTemp[0] = mTempVal_u16[2]; 
                I2C_Get_Lock();
                vTaskSuspendAll();
                WriteEEPROM_Word(sSetting_Values[_TEMP1].addrEEPROM,sMenu_Variable.u16ThresTemp[0]); 
                xTaskResumeAll();
                I2C_Release_Lock();                                 
                break;
            case 3: 
                sMenu_Variable.u16ThresTemp[1] = mTempVal_u16[3]; 
                I2C_Get_Lock();
                vTaskSuspendAll();
                WriteEEPROM_Word(sSetting_Values[_TEMP2].addrEEPROM,sMenu_Variable.u16ThresTemp[1]); 
                xTaskResumeAll();
                I2C_Release_Lock();                          
                break;
            case 4: 
                sMenu_Variable.u16ThresTemp[2] = mTempVal_u16[4]; 
                I2C_Get_Lock();
                vTaskSuspendAll();
                WriteEEPROM_Word(sSetting_Values[_TEMP3].addrEEPROM,sMenu_Variable.u16ThresTemp[2]);
                xTaskResumeAll();
                I2C_Release_Lock();                          
                break;
            case 5: 
                sMenu_Variable.u16ThresTemp[3] = mTempVal_u16[5]; 
                I2C_Get_Lock();
                vTaskSuspendAll();
                WriteEEPROM_Word(sSetting_Values[_TEMP4].addrEEPROM,sMenu_Variable.u16ThresTemp[3]);
                xTaskResumeAll();
                I2C_Release_Lock(); 
                break;
            default: 
                break;
            }		
            sMenu_Control.ajustValue = 0;
        }
        sMenu_Control.refesh = 1;
        sKey_Control.pressedKey = 0;
        break;
    case _ESC_KEY:
        sMenu_Control.menu = sMenu_Object[_ALARM_SETTING].parentIndex;
        sMenu_Control.index = sMenu_Control.previousIndex;
        sMenu_Control.previousIndex = 0;
        sMenu_Control.ajustValue = 0;
        sMenu_Control.init = 1;
        sMenu_Control.refesh = 1;
        sKey_Control.pressedKey = 0;
        break;
    default:
        break;
    }      
}

void Display_ATS_Settings(void)
{
    if(sMenu_Control.init)
    {
        mTempVal_u16[0] = sMenu_Variable.u16GENMaxRuntime;
        mTempVal_u16[1] = sMenu_Variable.u16GENUnderVolt;
        mTempVal_u16[2] = sMenu_Variable.u16GENErrorResetEnable;
        mTempVal_u16[3] = sMenu_Variable.u16GENErrorResetTime;
        mTempVal_u16[4] = sMenu_Variable.u16GENWarmUpTime;
        mTempVal_u16[5] = sMenu_Variable.u16GENCoolDownTime;
        mTempVal_u16[6] = sMenu_Variable.u16GENNightEnable;
        mTempVal_u16[7] = sMenu_Variable.u16GENNightStart;
        mTempVal_u16[8] = sMenu_Variable.u16GENNightEnd;
        mTempVal_u16[9] = sMenu_Variable.u16GENDCLowInput;
        mTempVal_u16[10] = sMenu_Variable.u16GENDCLowVolt;
        
        gldc_clearGraphicScreen_DataArea(LAYER1_ADDR);
        gldc_clearGraphicScreen_DataArea(LAYER2_ADDR);
        glcd_writeString(sMenu_Object[_ATS_SETTING].menuName,1,1);
        glcd_Draw_Black_Rectangle(LAYER2_ADDR,1,1,20);
        
        sMenu_Control.init = 0;
    }
    
    if(sMenu_Control.refesh)
    {
        if(sMenu_Control.index>8)
        {
            glcd_writeString("  DC Low Input:     ",2,1);
            glcd_writeString("        NC Input    ",3,1);
            glcd_writeString("  DC Low Voltage:   ",4,1);
            glcd_writeString("                VDC ",5,1);
            glcd_writeString("                    ",6,1);
            glcd_writeString("                    ",7,1);
            glcd_writeString("                    ",8,1);
            
            if(mTempVal_u16[9] == 0){
                glcd_writeString("        NC Input    ",3,1);
                glcd_writeString("           ---- VDC ",5,1);
            }else if(mTempVal_u16[9] == 1){
                glcd_writeString("        NO Input    ",3,1);
                glcd_writeString("           ---- VDC ",5,1);
            }else{
                glcd_writeString("      By Batt Volt  ",3,1);
                glcd_Display_Float_1Dec(5,12,4,mTempVal_u16[10]);
            }
        }
        else if(sMenu_Control.index>3)
        {
            glcd_writeString("  Warm Up Time:     ",2,1);
            glcd_writeString("                 s  ",3,1);
            glcd_writeString("  Cool Down Time:   ",4,1);
            glcd_writeString("                 s  ",5,1);
            glcd_writeString("  Night Running:    ",6,1);
            glcd_writeString("     Begin:   :     ",7,1);
            glcd_writeString("     End:     :     ",8,1);
            
            glcd_Display_Integer(3,14,4,mTempVal_u16[4]);
            glcd_Display_Integer(5,14,4,mTempVal_u16[5]);
            if(mTempVal_u16[6] == 0){
                glcd_writeString("Dis",6,18);
                glcd_writeString("--:--",7,13);
                glcd_writeString("--:--",8,13);
            }else{
                glcd_writeString("En ",6,18);
                mTempVal_u8[0] = mTempVal_u16[7]/60;	mTempVal_u8[1] = mTempVal_u16[7]&60;
                mTempVal_u8[2] = mTempVal_u16[8]/60;	mTempVal_u8[3] = mTempVal_u16[8]&60;
                glcd_Display_Integer(7,13,2,mTempVal_u8[0]);
                glcd_Display_Integer(7,16,2,mTempVal_u8[1]);
                glcd_Display_Integer(8,13,2,mTempVal_u8[2]);
                glcd_Display_Integer(8,16,2,mTempVal_u8[3]);
            }
        }
        else
        {
            glcd_writeString("  GEN Max Runtime:  ",2,1);
            glcd_writeString("              h  m  ",3,1);
            glcd_writeString("  GEN Under Voltage:",4,1);
            glcd_writeString("                VAC ",5,1);
            glcd_writeString("  GEN Error Reset:  ",6,1);
            glcd_writeString("           Disable  ",7,1);
            glcd_writeString("    Reset after:   m",8,1);
            
            mTempVal_u8[0] = mTempVal_u16[0]/60;	mTempVal_u8[1] = mTempVal_u16[0]&60;
            glcd_Display_Integer(3,12,3,mTempVal_u8[0]);
            glcd_Display_Integer(3,16,2,mTempVal_u8[1]);
            glcd_Display_Integer(5,13,3,mTempVal_u16[1]);
            if(mTempVal_u16[2] == 0){
                glcd_writeString("           Disable  ",7,1);
                glcd_writeString("---",8,17);
            }else{
                glcd_writeString("           Enable   ",7,1);
                glcd_Display_Integer(8,17,3,mTempVal_u16[3]);
            }
        }
        
        if(sMenu_Control.ajustValue == 0)
            switch(sMenu_Control.index)
            {
            case 0: gldc_clearGraphicScreen_DataArea(LAYER2_ADDR);	glcd_Under_Line_Text(LAYER2_ADDR,3,12,18); break;
            case 1: gldc_clearGraphicScreen_DataArea(LAYER2_ADDR);	glcd_Under_Line_Text(LAYER2_ADDR,5,13,15); break;
            case 2: gldc_clearGraphicScreen_DataArea(LAYER2_ADDR);	glcd_Under_Line_Text(LAYER2_ADDR,7,12,18); break;
            case 3: gldc_clearGraphicScreen_DataArea(LAYER2_ADDR);	glcd_Under_Line_Text(LAYER2_ADDR,8,17,19); break;
            case 4: gldc_clearGraphicScreen_DataArea(LAYER2_ADDR);	glcd_Under_Line_Text(LAYER2_ADDR,3,14,17); break;
            case 5: gldc_clearGraphicScreen_DataArea(LAYER2_ADDR);	glcd_Under_Line_Text(LAYER2_ADDR,5,14,17); break;
            case 6: gldc_clearGraphicScreen_DataArea(LAYER2_ADDR);	glcd_Under_Line_Text(LAYER2_ADDR,6,18,20); break;
            case 7: gldc_clearGraphicScreen_DataArea(LAYER2_ADDR);	glcd_Under_Line_Text(LAYER2_ADDR,7,13,17); break;
            case 8: gldc_clearGraphicScreen_DataArea(LAYER2_ADDR);	glcd_Under_Line_Text(LAYER2_ADDR,8,13,17); break;
            case 9: gldc_clearGraphicScreen_DataArea(LAYER2_ADDR);	glcd_Under_Line_Text(LAYER2_ADDR,3,7,18); break;
            case 10: gldc_clearGraphicScreen_DataArea(LAYER2_ADDR);	glcd_Under_Line_Text(LAYER2_ADDR,5,12,15); break;
            default: break;
            }
        
        sMenu_Control.refesh = 0;
    }
    
    switch(sKey_Control.pressedKey)
    {
    case _UP_KEY:
        if(sMenu_Control.ajustValue == 0){
            if(sMenu_Control.index == 0)
            {
                if(mTempVal_u16[9] == 2)
                    sMenu_Control.index = 10;
                else
                    sMenu_Control.index = 9;
            }else if(sMenu_Control.index == 9) {
                if(mTempVal_u16[6] == 1)
                    sMenu_Control.index = 8;
                else
                    sMenu_Control.index = 6;
            }else if(sMenu_Control.index == 4) {
                if(mTempVal_u16[2] == 1)
                    sMenu_Control.index = 3;
                else
                    sMenu_Control.index = 2;
            }else
                sMenu_Control.index--;
            
            if(sMenu_Control.index<0)
                sMenu_Control.index = 10;
        }else{
            switch(sMenu_Control.index)
            {
            case 0: Ajust_Variable_Value(0,&mTempVal_u16[0],_GEN_MAX_RUNTIME); break;
            case 1: Ajust_Variable_Value(0,&mTempVal_u16[1],_GEN_UNDER_VOLT); break;
            case 2: Ajust_Variable_Value(0,&mTempVal_u16[2],_GEN_ERROR_RESET_EN); break;
            case 3: Ajust_Variable_Value(0,&mTempVal_u16[3],_GEN_ERROR_RESET_MIN); break;
            case 4: Ajust_Variable_Value(0,&mTempVal_u16[4],_GEN_WARM_UP_TIME); break;
            case 5: Ajust_Variable_Value(0,&mTempVal_u16[5],_GEN_COOL_DOWN_TIME); break;
            case 6: Ajust_Variable_Value(0,&mTempVal_u16[6],_GEN_NIGHT_EN); break;
            case 7: Ajust_Variable_Value(0,&mTempVal_u16[7],_GEN_NIGHT_BEGIN); break;
            case 8: Ajust_Variable_Value(0,&mTempVal_u16[8],_GEN_NIGHT_END); break;
            case 9: Ajust_Variable_Value(0,&mTempVal_u16[9],_DC_LOW_INPUT); break;
            case 10: Ajust_Variable_Value(0,&mTempVal_u16[10],_DC_LOW_VOLT); break;
            default:
                break;
            }
        }
        sMenu_Control.refesh = 1;
        sKey_Control.pressedKey = 0;
        break;
    case _DOWN_KEY:
        if(sMenu_Control.ajustValue == 0){
            if(sMenu_Control.index == 2)
            {
                if(mTempVal_u16[2] == 1)
                    sMenu_Control.index = 3;
                else
                    sMenu_Control.index = 4;
            }else if(sMenu_Control.index == 6) {
                if(mTempVal_u16[6] == 1)
                    sMenu_Control.index = 7;
                else
                    sMenu_Control.index = 9;
            }else if(sMenu_Control.index == 9) {
                if(mTempVal_u16[9] == 2)
                    sMenu_Control.index = 10;
                else
                    sMenu_Control.index = 0;
            }else
                sMenu_Control.index++;
            
            if(sMenu_Control.index>10)
                sMenu_Control.index = 0;
        }else{
            switch(sMenu_Control.index)
            {
            case 0: Ajust_Variable_Value(1,&mTempVal_u16[0],_GEN_MAX_RUNTIME); break;
            case 1: Ajust_Variable_Value(1,&mTempVal_u16[1],_GEN_UNDER_VOLT); break;
            case 2: Ajust_Variable_Value(1,&mTempVal_u16[2],_GEN_ERROR_RESET_EN); break;
            case 3: Ajust_Variable_Value(1,&mTempVal_u16[3],_GEN_ERROR_RESET_MIN); break;
            case 4: Ajust_Variable_Value(1,&mTempVal_u16[4],_GEN_WARM_UP_TIME); break;
            case 5: Ajust_Variable_Value(1,&mTempVal_u16[5],_GEN_COOL_DOWN_TIME); break;
            case 6: Ajust_Variable_Value(1,&mTempVal_u16[6],_GEN_NIGHT_EN); break;
            case 7: Ajust_Variable_Value(1,&mTempVal_u16[7],_GEN_NIGHT_BEGIN); break;
            case 8: Ajust_Variable_Value(1,&mTempVal_u16[8],_GEN_NIGHT_END); break;
            case 9: Ajust_Variable_Value(1,&mTempVal_u16[9],_DC_LOW_INPUT); break;
            case 10: Ajust_Variable_Value(1,&mTempVal_u16[10],_DC_LOW_VOLT); break;
            default:
                break;
            }
        }
        sMenu_Control.refesh = 1;
        sKey_Control.pressedKey = 0;
        break;
    case _ENTER_KEY:
        if(sMenu_Control.ajustValue == 0){
            switch(sMenu_Control.index)
            {
            case 0: glcd_Draw_Black_Rectangle(LAYER2_ADDR,3,12,18); break;
            case 1: glcd_Draw_Black_Rectangle(LAYER2_ADDR,5,13,15); break;
            case 2: glcd_Draw_Black_Rectangle(LAYER2_ADDR,7,12,18); break;
            case 3: glcd_Draw_Black_Rectangle(LAYER2_ADDR,8,17,19); break;
            case 4: glcd_Draw_Black_Rectangle(LAYER2_ADDR,3,14,17); break;
            case 5: glcd_Draw_Black_Rectangle(LAYER2_ADDR,5,14,17); break;
            case 6: glcd_Draw_Black_Rectangle(LAYER2_ADDR,6,18,20); break;
            case 7: glcd_Draw_Black_Rectangle(LAYER2_ADDR,7,13,17); break;
            case 8: glcd_Draw_Black_Rectangle(LAYER2_ADDR,8,13,17); break;
            case 9: glcd_Draw_Black_Rectangle(LAYER2_ADDR,3,7,18); break;
            case 10: glcd_Draw_Black_Rectangle(LAYER2_ADDR,5,12,15); break;
            default: break;
            }
            sMenu_Control.ajustValue = 1;
        }else{
            switch(sMenu_Control.index)
            {
            case 0: 
                sMenu_Variable.changedVal = _GEN_MAX_RUNTIME; 
                sMenu_Variable.u16GENMaxRuntime = mTempVal_u16[0]; 
                I2C_Get_Lock();
                vTaskSuspendAll();
                WriteEEPROM_Word(sSetting_Values[_GEN_MAX_RUNTIME].addrEEPROM,sMenu_Variable.u16GENMaxRuntime);                          
                xTaskResumeAll();
                I2C_Release_Lock();
                break;
            case 1: 
                sMenu_Variable.changedVal = _GEN_UNDER_VOLT; 
                sMenu_Variable.u16GENUnderVolt = mTempVal_u16[1]; 
                I2C_Get_Lock();
                vTaskSuspendAll();
                WriteEEPROM_Word(sSetting_Values[_GEN_UNDER_VOLT].addrEEPROM,sMenu_Variable.u16GENUnderVolt); 
                xTaskResumeAll();
                I2C_Release_Lock();
                break;
            case 2: 
                sMenu_Variable.changedVal = _GEN_ERROR_RESET_EN; 
                sMenu_Variable.u16GENErrorResetEnable = mTempVal_u16[2]; 
                I2C_Get_Lock();
                vTaskSuspendAll();
                WriteEEPROM_Word(sSetting_Values[_GEN_ERROR_RESET_EN].addrEEPROM,sMenu_Variable.u16GENErrorResetEnable); 
                xTaskResumeAll();
                I2C_Release_Lock();
                break;
            case 3: 
                sMenu_Variable.changedVal = _GEN_ERROR_RESET_MIN; 
                sMenu_Variable.u16GENErrorResetTime = mTempVal_u16[3]; 
                I2C_Get_Lock();
                vTaskSuspendAll();
                WriteEEPROM_Word(sSetting_Values[_GEN_ERROR_RESET_MIN].addrEEPROM,sMenu_Variable.u16GENErrorResetTime);
                xTaskResumeAll();
                I2C_Release_Lock();
                break;
            case 4: 
                sMenu_Variable.changedVal = _GEN_WARM_UP_TIME; 
                sMenu_Variable.u16GENWarmUpTime = mTempVal_u16[4]; 
                I2C_Get_Lock();
                vTaskSuspendAll();
                WriteEEPROM_Word(sSetting_Values[_GEN_WARM_UP_TIME].addrEEPROM,sMenu_Variable.u16GENWarmUpTime); 
                xTaskResumeAll();
                I2C_Release_Lock();
                break;
            case 5: 
                sMenu_Variable.changedVal = _GEN_COOL_DOWN_TIME; 
                sMenu_Variable.u16GENCoolDownTime = mTempVal_u16[5];
                I2C_Get_Lock();
                vTaskSuspendAll();
                WriteEEPROM_Word(sSetting_Values[_GEN_COOL_DOWN_TIME].addrEEPROM,sMenu_Variable.u16GENCoolDownTime); 
                xTaskResumeAll();
                I2C_Release_Lock();
                break;
            case 6: 
                sMenu_Variable.changedVal = _GEN_NIGHT_EN;
                sMenu_Variable.u16GENNightEnable = mTempVal_u16[6];
                I2C_Get_Lock();
                vTaskSuspendAll();                           
                WriteEEPROM_Word(sSetting_Values[_GEN_NIGHT_EN].addrEEPROM,sMenu_Variable.u16GENNightEnable); 
                xTaskResumeAll();
                I2C_Release_Lock();
                break;
            case 7: 
                sMenu_Variable.changedVal = _GEN_NIGHT_BEGIN; 
                sMenu_Variable.u16GENNightStart = mTempVal_u16[7]; 
                I2C_Get_Lock();
                vTaskSuspendAll();
                WriteEEPROM_Word(sSetting_Values[_GEN_NIGHT_BEGIN].addrEEPROM,sMenu_Variable.u16GENNightStart);
                xTaskResumeAll();
                I2C_Release_Lock();
                break;
            case 8: 
                sMenu_Variable.changedVal = _GEN_NIGHT_END; 
                sMenu_Variable.u16GENNightEnd = mTempVal_u16[8]; 
                I2C_Get_Lock();
                vTaskSuspendAll();
                WriteEEPROM_Word(sSetting_Values[_GEN_NIGHT_END].addrEEPROM,sMenu_Variable.u16GENNightEnd); 
                xTaskResumeAll();
                I2C_Release_Lock();
                break;
            case 9: 
                sMenu_Variable.changedVal = _DC_LOW_INPUT;
                sMenu_Variable.u16GENDCLowInput = mTempVal_u16[9]; 
                I2C_Get_Lock();
                vTaskSuspendAll();                          
                WriteEEPROM_Word(sSetting_Values[_DC_LOW_INPUT].addrEEPROM,sMenu_Variable.u16GENDCLowInput); 
                xTaskResumeAll();
                I2C_Release_Lock();
                break;
            case 10: 
                sMenu_Variable.changedVal = _DC_LOW_VOLT; 
                sMenu_Variable.u16GENDCLowVolt = mTempVal_u16[10]; 
                I2C_Get_Lock();
                vTaskSuspendAll();
                WriteEEPROM_Word(sSetting_Values[_DC_LOW_VOLT].addrEEPROM,sMenu_Variable.u16GENDCLowVolt); 
                xTaskResumeAll();
                I2C_Release_Lock();
                break;
            default: break;
            }
            sMenu_Control.ajustValue = 0;
        }
        sMenu_Control.refesh = 1;
        sKey_Control.pressedKey = 0;
        break;
    case _ESC_KEY:
        sMenu_Control.menu = sMenu_Object[_ATS_SETTING].parentIndex;
        sMenu_Control.index = sMenu_Control.previousIndex;
        sMenu_Control.previousIndex = 0;
        sMenu_Control.ajustValue = 0;
        sMenu_Control.init = 1;
        sMenu_Control.refesh = 1;
        sKey_Control.pressedKey = 0;
        break;
    default:
        break;
    }      
}

void Display_Accessories_Settings(void)
{
    if(sMenu_Control.init)
    {
        gldc_clearGraphicScreen_DataArea(LAYER1_ADDR);
        gldc_clearGraphicScreen_DataArea(LAYER2_ADDR);
        glcd_writeString(sMenu_Object[_ACCES_SETTING].menuName,1,1);
        glcd_Draw_Black_Rectangle(LAYER2_ADDR,1,1,20);
        
        sMenu_Control.init = 0;
    }
    if(sMenu_Control.refesh)
    {
        
        sMenu_Control.refesh = 0;
    }
    
    switch(sKey_Control.pressedKey)
    {
    case _UP_KEY:
        sMenu_Control.refesh = 1;
        sKey_Control.pressedKey = 0;
        break;
    case _DOWN_KEY:
        sMenu_Control.refesh = 1;
        sKey_Control.pressedKey = 0;
        break;
    case _ENTER_KEY:
        sMenu_Control.refesh = 1;
        sKey_Control.pressedKey = 0;
        break;
    case _ESC_KEY:
        sMenu_Control.menu = sMenu_Object[_ACCES_SETTING].parentIndex;
        sMenu_Control.index = sMenu_Control.previousIndex;
        sMenu_Control.init = 1;
        sMenu_Control.refesh = 1;
        sKey_Control.pressedKey = 0;
        break;
    default:
        break;
    }    
}

void Display_DeviceAddress_Settings(void)
{
    if(sMenu_Control.init)
    {
        mTempVal_u16[0] = sMenu_Variable.sEthernetSetting.u16DevIP[0];
        mTempVal_u16[1] = sMenu_Variable.sEthernetSetting.u16DevIP[1];
        mTempVal_u16[2] = sMenu_Variable.sEthernetSetting.u16DevIP[2];
        mTempVal_u16[3] = sMenu_Variable.sEthernetSetting.u16DevIP[3];
        
        mTempVal_u16[4] = sMenu_Variable.sEthernetSetting.u16DevSubnet[0];
        mTempVal_u16[5] = sMenu_Variable.sEthernetSetting.u16DevSubnet[1];
        mTempVal_u16[6] = sMenu_Variable.sEthernetSetting.u16DevSubnet[2];
        mTempVal_u16[7] = sMenu_Variable.sEthernetSetting.u16DevSubnet[3];
        
        mTempVal_u16[8] = sMenu_Variable.sEthernetSetting.u16DevGateway[0];
        mTempVal_u16[9] = sMenu_Variable.sEthernetSetting.u16DevGateway[1];
        mTempVal_u16[10] = sMenu_Variable.sEthernetSetting.u16DevGateway[2];
        mTempVal_u16[11] = sMenu_Variable.sEthernetSetting.u16DevGateway[3];
        
        mTempVal_u16[12] = sMenu_Variable.sEthernetSetting.u16DevPort;
        
        sMenu_Variable.sEthernetSetting_temp.u16DevIP[0] = sMenu_Variable.sEthernetSetting.u16DevIP[0];
        sMenu_Variable.sEthernetSetting_temp.u16DevIP[1] = sMenu_Variable.sEthernetSetting.u16DevIP[1];
        sMenu_Variable.sEthernetSetting_temp.u16DevIP[2] = sMenu_Variable.sEthernetSetting.u16DevIP[2];
        sMenu_Variable.sEthernetSetting_temp.u16DevIP[3] = sMenu_Variable.sEthernetSetting.u16DevIP[3];
        sMenu_Variable.sEthernetSetting_temp.u16DevSubnet[0] = sMenu_Variable.sEthernetSetting.u16DevSubnet[0];
        sMenu_Variable.sEthernetSetting_temp.u16DevSubnet[1] = sMenu_Variable.sEthernetSetting.u16DevSubnet[1];
        sMenu_Variable.sEthernetSetting_temp.u16DevSubnet[2] = sMenu_Variable.sEthernetSetting.u16DevSubnet[2];
        sMenu_Variable.sEthernetSetting_temp.u16DevSubnet[3] = sMenu_Variable.sEthernetSetting.u16DevSubnet[3];
        sMenu_Variable.sEthernetSetting_temp.u16DevGateway[0] = sMenu_Variable.sEthernetSetting.u16DevGateway[0];
        sMenu_Variable.sEthernetSetting_temp.u16DevGateway[1] = sMenu_Variable.sEthernetSetting.u16DevGateway[1];
        sMenu_Variable.sEthernetSetting_temp.u16DevGateway[2] = sMenu_Variable.sEthernetSetting.u16DevGateway[2];
        sMenu_Variable.sEthernetSetting_temp.u16DevGateway[3] = sMenu_Variable.sEthernetSetting.u16DevGateway[3];
        sMenu_Variable.sEthernetSetting_temp.u16DevPort = sMenu_Variable.sEthernetSetting.u16DevPort;
        
        gldc_clearGraphicScreen_DataArea(LAYER1_ADDR);
        gldc_clearGraphicScreen_DataArea(LAYER2_ADDR);
        glcd_writeString(sMenu_Object[_DEVICE_ADDRESS].menuName,1,1);
        glcd_Draw_Black_Rectangle(LAYER2_ADDR,1,1,20);
        
        glcd_writeString("  IP Address:       ",2,1);
        glcd_writeString("       .   .   .    ",3,1);
        glcd_writeString("  Subnet Mask:      ",4,1);
        glcd_writeString("       .   .   .    ",5,1);
        glcd_writeString("  Gateway Address:  ",6,1);
        glcd_writeString("       .   .   .    ",7,1);
        glcd_writeString("  Port:             ",8,1);
        
        sMenu_Control.init = 0;
    }
    if(sMenu_Control.refesh)
    {
        glcd_Display_Integer(3,5,3,mTempVal_u16[0]);
        glcd_Display_Integer(3,9,3,mTempVal_u16[1]);
        glcd_Display_Integer(3,13,3,mTempVal_u16[2]);
        glcd_Display_Integer(3,17,3,mTempVal_u16[3]);
        glcd_Display_Integer(5,5,3,mTempVal_u16[4]);
        glcd_Display_Integer(5,9,3,mTempVal_u16[5]);
        glcd_Display_Integer(5,13,3,mTempVal_u16[6]);
        glcd_Display_Integer(5,17,3,mTempVal_u16[7]);
        glcd_Display_Integer(7,5,3,mTempVal_u16[8]);
        glcd_Display_Integer(7,9,3,mTempVal_u16[9]);
        glcd_Display_Integer(7,13,3,mTempVal_u16[10]);
        glcd_Display_Integer(7,17,3,mTempVal_u16[11]);
        glcd_Display_Integer(8,9,5,mTempVal_u16[12]);
        
        if(sMenu_Control.ajustValue == 0)
            switch(sMenu_Control.index)
            {
            case 0: gldc_clearGraphicScreen_DataArea(LAYER2_ADDR);	glcd_Under_Line_Text(LAYER2_ADDR,3,5,7); break;
            case 1: gldc_clearGraphicScreen_DataArea(LAYER2_ADDR);	glcd_Under_Line_Text(LAYER2_ADDR,3,9,11); break;
            case 2: gldc_clearGraphicScreen_DataArea(LAYER2_ADDR);	glcd_Under_Line_Text(LAYER2_ADDR,3,13,15); break;
            case 3: gldc_clearGraphicScreen_DataArea(LAYER2_ADDR);	glcd_Under_Line_Text(LAYER2_ADDR,3,17,19); break;
            case 4: gldc_clearGraphicScreen_DataArea(LAYER2_ADDR);	glcd_Under_Line_Text(LAYER2_ADDR,5,5,7); break;
            case 5: gldc_clearGraphicScreen_DataArea(LAYER2_ADDR);	glcd_Under_Line_Text(LAYER2_ADDR,5,9,11); break;
            case 6: gldc_clearGraphicScreen_DataArea(LAYER2_ADDR);	glcd_Under_Line_Text(LAYER2_ADDR,5,13,15); break;
            case 7: gldc_clearGraphicScreen_DataArea(LAYER2_ADDR);	glcd_Under_Line_Text(LAYER2_ADDR,5,17,19); break;
            case 8: gldc_clearGraphicScreen_DataArea(LAYER2_ADDR);	glcd_Under_Line_Text(LAYER2_ADDR,7,5,7); break;
            case 9: gldc_clearGraphicScreen_DataArea(LAYER2_ADDR);	glcd_Under_Line_Text(LAYER2_ADDR,7,9,11); break;
            case 10: gldc_clearGraphicScreen_DataArea(LAYER2_ADDR);	glcd_Under_Line_Text(LAYER2_ADDR,7,13,15); break;
            case 11: gldc_clearGraphicScreen_DataArea(LAYER2_ADDR);	glcd_Under_Line_Text(LAYER2_ADDR,7,17,19); break;
            case 12: gldc_clearGraphicScreen_DataArea(LAYER2_ADDR);	glcd_Under_Line_Text(LAYER2_ADDR,8,9,13); break;
            default: break;
            }
        
        sMenu_Control.refesh = 0;
    }
    
    switch(sKey_Control.pressedKey)
    {
    case _UP_KEY:
        if(sMenu_Control.ajustValue == 0){
            sMenu_Control.index--;
            if(sMenu_Control.index<0)
                sMenu_Control.index = 12;
        }else{
            switch(sMenu_Control.index)
            {
            case 0: Ajust_Variable_Value(0,&mTempVal_u16[0],_DEV_IP1); break;
            case 1: Ajust_Variable_Value(0,&mTempVal_u16[1],_DEV_IP2); break;
            case 2: Ajust_Variable_Value(0,&mTempVal_u16[2],_DEV_IP3); break;
            case 3: Ajust_Variable_Value(0,&mTempVal_u16[3],_DEV_IP4); break;
            case 4: Ajust_Variable_Value(0,&mTempVal_u16[4],_DEV_SUBNET1); break;
            case 5: Ajust_Variable_Value(0,&mTempVal_u16[5],_DEV_SUBNET2); break;
            case 6: Ajust_Variable_Value(0,&mTempVal_u16[6],_DEV_SUBNET3); break;
            case 7: Ajust_Variable_Value(0,&mTempVal_u16[7],_DEV_SUBNET4); break;
            case 8: Ajust_Variable_Value(0,&mTempVal_u16[8],_DEV_GATEW1); break;
            case 9: Ajust_Variable_Value(0,&mTempVal_u16[9],_DEV_GATEW2); break;
            case 10: Ajust_Variable_Value(0,&mTempVal_u16[10],_DEV_GATEW3); break;
            case 11: Ajust_Variable_Value(0,&mTempVal_u16[11],_DEV_GATEW4); break;
            case 12: Ajust_Variable_Value(0,&mTempVal_u16[12],_DEV_PORT); break;
            default:
                break;
            }
        }
        sMenu_Control.refesh = 1;
        sKey_Control.pressedKey = 0;
        break;
    case _DOWN_KEY:
        if(sMenu_Control.ajustValue == 0){
            sMenu_Control.index++;
            if(sMenu_Control.index>12)
                sMenu_Control.index = 0;
        }else{
            switch(sMenu_Control.index)
            {
            case 0: Ajust_Variable_Value(1,&mTempVal_u16[0],_DEV_IP1); break;
            case 1: Ajust_Variable_Value(1,&mTempVal_u16[1],_DEV_IP2); break;
            case 2: Ajust_Variable_Value(1,&mTempVal_u16[2],_DEV_IP3); break;
            case 3: Ajust_Variable_Value(1,&mTempVal_u16[3],_DEV_IP4); break;
            case 4: Ajust_Variable_Value(1,&mTempVal_u16[4],_DEV_SUBNET1); break;
            case 5: Ajust_Variable_Value(1,&mTempVal_u16[5],_DEV_SUBNET2); break;
            case 6: Ajust_Variable_Value(1,&mTempVal_u16[6],_DEV_SUBNET3); break;
            case 7: Ajust_Variable_Value(1,&mTempVal_u16[7],_DEV_SUBNET4); break;
            case 8: Ajust_Variable_Value(1,&mTempVal_u16[8],_DEV_GATEW1); break;
            case 9: Ajust_Variable_Value(1,&mTempVal_u16[9],_DEV_GATEW2); break;
            case 10: Ajust_Variable_Value(1,&mTempVal_u16[10],_DEV_GATEW3); break;
            case 11: Ajust_Variable_Value(1,&mTempVal_u16[11],_DEV_GATEW4); break;
            case 12: Ajust_Variable_Value(1,&mTempVal_u16[12],_DEV_PORT); break;
            default:
                break;
            }
        }
        sMenu_Control.refesh = 1;
        sKey_Control.pressedKey = 0;
        break;
    case _ENTER_KEY:
        if(sMenu_Control.ajustValue == 0){
            switch(sMenu_Control.index)
            {
            case 0: glcd_Draw_Black_Rectangle(LAYER2_ADDR,3,5,7); break;
            case 1: glcd_Draw_Black_Rectangle(LAYER2_ADDR,3,9,11); break;
            case 2: glcd_Draw_Black_Rectangle(LAYER2_ADDR,3,13,15); break;
            case 3: glcd_Draw_Black_Rectangle(LAYER2_ADDR,3,17,19); break;
            case 4: glcd_Draw_Black_Rectangle(LAYER2_ADDR,5,5,7); break;
            case 5: glcd_Draw_Black_Rectangle(LAYER2_ADDR,5,9,11); break;
            case 6: glcd_Draw_Black_Rectangle(LAYER2_ADDR,5,13,15); break;
            case 7: glcd_Draw_Black_Rectangle(LAYER2_ADDR,5,17,19); break;
            case 8: glcd_Draw_Black_Rectangle(LAYER2_ADDR,7,5,7); break;
            case 9: glcd_Draw_Black_Rectangle(LAYER2_ADDR,7,9,11); break;
            case 10: glcd_Draw_Black_Rectangle(LAYER2_ADDR,7,13,15); break;
            case 11: glcd_Draw_Black_Rectangle(LAYER2_ADDR,7,17,19); break;
            case 12: glcd_Draw_Black_Rectangle(LAYER2_ADDR,8,9,13); break;
            default: break;
            }
            sMenu_Control.ajustValue = 1;
        }else{
            switch(sMenu_Control.index)
            {
            case 0: sMenu_Variable.sEthernetSetting_temp.u16DevIP[0] = mTempVal_u16[0]; break;
            case 1: sMenu_Variable.sEthernetSetting_temp.u16DevIP[1] = mTempVal_u16[1]; break;
            case 2: sMenu_Variable.sEthernetSetting_temp.u16DevIP[2] = mTempVal_u16[2]; break;
            case 3: sMenu_Variable.sEthernetSetting_temp.u16DevIP[3] = mTempVal_u16[3]; break;
            case 4: sMenu_Variable.sEthernetSetting_temp.u16DevSubnet[0] = mTempVal_u16[4]; break;
            case 5: sMenu_Variable.sEthernetSetting_temp.u16DevSubnet[1] = mTempVal_u16[5]; break;
            case 6: sMenu_Variable.sEthernetSetting_temp.u16DevSubnet[2] = mTempVal_u16[6]; break;
            case 7: sMenu_Variable.sEthernetSetting_temp.u16DevSubnet[3] = mTempVal_u16[7]; break;
            case 8: sMenu_Variable.sEthernetSetting_temp.u16DevGateway[0] = mTempVal_u16[8]; break;
            case 9: sMenu_Variable.sEthernetSetting_temp.u16DevGateway[1] = mTempVal_u16[9]; break;
            case 10: sMenu_Variable.sEthernetSetting_temp.u16DevGateway[2] = mTempVal_u16[10]; break;
            case 11: sMenu_Variable.sEthernetSetting_temp.u16DevGateway[3] = mTempVal_u16[11]; break;
            case 12: sMenu_Variable.sEthernetSetting_temp.u16DevPort = mTempVal_u16[12]; break;
            default: break;
            }		
            sMenu_Control.ajustValue = 0;
        }
        sMenu_Control.refesh = 1;
        sKey_Control.pressedKey = 0;
        break;
    case _ESC_KEY:
        //	 sMenu_Control.menu = sMenu_Object[_DEVICE_ADDRESS].parentIndex;
        //	 sMenu_Control.index = sMenu_Control.previousIndex;
        //	sMenu_Control.mode = 1;
        sMenu_Control.menu = _SETTING_CONFIRM;
        sMenu_Control.index = 0;
        sMenu_Control.previousIndex = 0;
        sMenu_Control.ajustValue = 0;
        sMenu_Control.init = 1;
        sMenu_Control.refesh = 1;
        sKey_Control.pressedKey = 0;
        break;
    default:
        break;
    }    
}

void Display_ServerAddress_Settings(void)
{
    if(sMenu_Control.init)
    {
        mTempVal_u16[0] = sMenu_Variable.u16ServerIP[0];
        mTempVal_u16[1] = sMenu_Variable.u16ServerIP[1];
        mTempVal_u16[2] = sMenu_Variable.u16ServerIP[2];
        mTempVal_u16[3] = sMenu_Variable.u16ServerIP[3];
        
        mTempVal_u16[4] = sMenu_Variable.u16ServerPort;
        
        gldc_clearGraphicScreen_DataArea(LAYER1_ADDR);
        gldc_clearGraphicScreen_DataArea(LAYER2_ADDR);
        glcd_writeString(sMenu_Object[_SERVER_ADDRESS].menuName,1,1);
        glcd_Draw_Black_Rectangle(LAYER2_ADDR,1,1,20);
        
        glcd_writeString("  IP Address:       ",2,1);
        glcd_writeString("       .   .   .    ",3,1);
        glcd_writeString("  Port:             ",4,1);
        glcd_writeString("                    ",6,1);
        glcd_writeString("                    ",8,1);
        
        sMenu_Control.init = 0;
    }
    if(sMenu_Control.refesh)
    {
        glcd_Display_Integer(3,5,3,mTempVal_u16[0]);
        glcd_Display_Integer(3,9,3,mTempVal_u16[1]);
        glcd_Display_Integer(3,13,3,mTempVal_u16[2]);
        glcd_Display_Integer(3,17,3,mTempVal_u16[3]);
        glcd_Display_Integer(4,9,5,mTempVal_u16[4]);
        
        if(sMenu_Control.ajustValue == 0)
            switch(sMenu_Control.index)
            {
            case 0: gldc_clearGraphicScreen_DataArea(LAYER2_ADDR);	glcd_Under_Line_Text(LAYER2_ADDR,3,5,7); break;
            case 1: gldc_clearGraphicScreen_DataArea(LAYER2_ADDR);	glcd_Under_Line_Text(LAYER2_ADDR,3,9,11); break;
            case 2: gldc_clearGraphicScreen_DataArea(LAYER2_ADDR);	glcd_Under_Line_Text(LAYER2_ADDR,3,13,15); break;
            case 3: gldc_clearGraphicScreen_DataArea(LAYER2_ADDR);	glcd_Under_Line_Text(LAYER2_ADDR,3,17,19); break;
            case 4: gldc_clearGraphicScreen_DataArea(LAYER2_ADDR);	glcd_Under_Line_Text(LAYER2_ADDR,4,9,13); break;
            default: break;
            }
        
        sMenu_Control.refesh = 0;
    }
    
    switch(sKey_Control.pressedKey)
    {
    case _UP_KEY:
        if(sMenu_Control.ajustValue == 0){
            sMenu_Control.index--;
            if(sMenu_Control.index<0)
                sMenu_Control.index = 4;
        }else{
            switch(sMenu_Control.index)
            {
            case 0: Ajust_Variable_Value(0,&mTempVal_u16[0],_SERVER_IP1); break;
            case 1: Ajust_Variable_Value(0,&mTempVal_u16[1],_SERVER_IP2); break;
            case 2: Ajust_Variable_Value(0,&mTempVal_u16[2],_SERVER_IP3); break;
            case 3: Ajust_Variable_Value(0,&mTempVal_u16[3],_SERVER_IP4); break;
            case 4: Ajust_Variable_Value(0,&mTempVal_u16[4],_SERVER_PORT); break;
            default:
                break;
            }
        }
        sMenu_Control.refesh = 1;
        sKey_Control.pressedKey = 0;
        break;
    case _DOWN_KEY:
        if(sMenu_Control.ajustValue == 0){
            sMenu_Control.index++;
            if(sMenu_Control.index>4)
                sMenu_Control.index = 0;
        }else{
            switch(sMenu_Control.index)
            {
            case 0: Ajust_Variable_Value(1,&mTempVal_u16[0],_SERVER_IP1); break;
            case 1: Ajust_Variable_Value(1,&mTempVal_u16[1],_SERVER_IP2); break;
            case 2: Ajust_Variable_Value(1,&mTempVal_u16[2],_SERVER_IP3); break;
            case 3: Ajust_Variable_Value(1,&mTempVal_u16[3],_SERVER_IP4); break;
            case 4: Ajust_Variable_Value(1,&mTempVal_u16[4],_SERVER_PORT); break;
            default:
                break;
            }
        }
        sMenu_Control.refesh = 1;
        sKey_Control.pressedKey = 0;
        break;
    case _ENTER_KEY:
        if(sMenu_Control.ajustValue == 0){
            switch(sMenu_Control.index)
            {
            case 0: glcd_Draw_Black_Rectangle(LAYER2_ADDR,3,5,7); break;
            case 1: glcd_Draw_Black_Rectangle(LAYER2_ADDR,3,9,11); break;
            case 2: glcd_Draw_Black_Rectangle(LAYER2_ADDR,3,13,15); break;
            case 3: glcd_Draw_Black_Rectangle(LAYER2_ADDR,3,17,19); break;
            case 4: glcd_Draw_Black_Rectangle(LAYER2_ADDR,4,9,13); break;
            default: break;
            }
            sMenu_Control.ajustValue = 1;
        }else{
            switch(sMenu_Control.index)
            {
            case 0: 
                sMenu_Variable.u16ServerIP[0] = mTempVal_u16[0];
                I2C_Get_Lock();
                vTaskSuspendAll();                          
                WriteEEPROM_Word(sSetting_Values[_SERVER_IP1].addrEEPROM,sMenu_Variable.u16ServerIP[0]); 
                xTaskResumeAll();
                I2C_Release_Lock();
                break;
            case 1: 
                sMenu_Variable.u16ServerIP[1] = mTempVal_u16[1]; 
                I2C_Get_Lock();
                vTaskSuspendAll();  
                WriteEEPROM_Word(sSetting_Values[_SERVER_IP2].addrEEPROM,sMenu_Variable.u16ServerIP[1]);
                xTaskResumeAll();
                I2C_Release_Lock();
                break;
            case 2: 
                sMenu_Variable.u16ServerIP[2] = mTempVal_u16[2];
                I2C_Get_Lock();
                vTaskSuspendAll();  
                WriteEEPROM_Word(sSetting_Values[_SERVER_IP3].addrEEPROM,sMenu_Variable.u16ServerIP[2]); 
                xTaskResumeAll();
                I2C_Release_Lock();
                break;
            case 3: 
                sMenu_Variable.u16ServerIP[3] = mTempVal_u16[3];                           
                I2C_Get_Lock();
                vTaskSuspendAll();  
                WriteEEPROM_Word(sSetting_Values[_SERVER_IP4].addrEEPROM,sMenu_Variable.u16ServerIP[3]);
                xTaskResumeAll();
                I2C_Release_Lock();
                break;
            case 4: 
                sMenu_Variable.u16ServerPort = mTempVal_u16[4];
                I2C_Get_Lock();
                vTaskSuspendAll();  
                WriteEEPROM_Word(sSetting_Values[_SERVER_PORT].addrEEPROM,sMenu_Variable.u16ServerPort);
                xTaskResumeAll();
                I2C_Release_Lock();
                break;
            default: 
                break;
            }		
            sMenu_Control.ajustValue = 0;
        }
        sMenu_Control.refesh = 1;
        sKey_Control.pressedKey = 0;
        break;
    case _ESC_KEY:
        sprintf((char_t*)sMenu_Variable.ucSIP,"%d.%d.%d.%d",
                sMenu_Variable.u16ServerIP[0],
                sMenu_Variable.u16ServerIP[1],
                sMenu_Variable.u16ServerIP[2],
                sMenu_Variable.u16ServerIP[3]);
        
        sMenu_Control.menu = sMenu_Object[_SERVER_ADDRESS].parentIndex;
        sMenu_Control.index = sMenu_Control.previousIndex;
        sMenu_Control.previousIndex = 0;
        sMenu_Control.ajustValue = 0;
        sMenu_Control.init = 1;
        sMenu_Control.refesh = 1;
        sKey_Control.pressedKey = 0;
        break;
    default:
        break;
    }    
}

void Display_UserAccount_Settings(void)
{
    if(sMenu_Control.init)
    {
        gldc_clearGraphicScreen_DataArea(LAYER1_ADDR);
        gldc_clearGraphicScreen_DataArea(LAYER2_ADDR);
        glcd_writeString(sMenu_Object[_USER_INFO].menuName,1,1);
        glcd_Draw_Black_Rectangle(LAYER2_ADDR,1,1,20);
        
        glcd_writeString("  SNMP Username:    ",2,1);
        glcd_writeString("  SNMP Auth Pass:   ",4,1);
        glcd_writeString("  SNMP Private Pass:",6,1);
        glcd_writeString("  Key:              ",8,1);
        
        sMenu_Control.init = 0;
    }
    
    if(sMenu_Control.refesh)
    {
        
        sMenu_Control.refesh = 0;
    }
    
    switch(sKey_Control.pressedKey)
    {
    case _UP_KEY:
        sMenu_Control.index--;
        if(sMenu_Control.index<0)
            sMenu_Control.index = 4;
        sMenu_Control.refesh = 1;
        sKey_Control.pressedKey = 0;
        break;
    case _DOWN_KEY:
        sMenu_Control.index++;
        if(sMenu_Control.index>4)
            sMenu_Control.index = 0;
        sMenu_Control.refesh = 1;
        sKey_Control.pressedKey = 0;
        break;
    case _ENTER_KEY:
        sMenu_Control.refesh = 1;
        sKey_Control.pressedKey = 0;
        break;
    case _ESC_KEY:
        sMenu_Control.menu = sMenu_Object[_USER_INFO].parentIndex;
        sMenu_Control.index = sMenu_Control.previousIndex;
        sMenu_Control.previousIndex = 0;
        sMenu_Control.ajustValue = 0;
        sMenu_Control.init = 1;
        sMenu_Control.refesh = 1;
        sKey_Control.pressedKey = 0;
        break;
    default:
        break;
    }     
}

void Display_Device_Info_Settings(void)
{
    if(sMenu_Control.init)
    {
        gldc_clearGraphicScreen_DataArea(LAYER1_ADDR);
        gldc_clearGraphicScreen_DataArea(LAYER2_ADDR);
        glcd_writeString(sMenu_Object[_DEVICE_INFO].menuName,1,1);
        glcd_Draw_Black_Rectangle(LAYER2_ADDR,1,1,20);
        
        sMenu_Control.init = 0;
    }
    if(sMenu_Control.refesh)
    {
        
        sMenu_Control.refesh = 0;
    }
    
    switch(sKey_Control.pressedKey)
    {
    case _UP_KEY:
        sMenu_Control.refesh = 1;
        sKey_Control.pressedKey = 0;
        break;
    case _DOWN_KEY:
        sMenu_Control.refesh = 1;
        sKey_Control.pressedKey = 0;
        break;
    case _ENTER_KEY:
        sMenu_Control.refesh = 1;
        sKey_Control.pressedKey = 0;
        break;
    case _ESC_KEY:
        sMenu_Control.menu = sMenu_Object[_DEVICE_INFO].parentIndex;
        sMenu_Control.index = sMenu_Control.previousIndex;
        sMenu_Control.previousIndex = 0;
        sMenu_Control.ajustValue = 0;
        sMenu_Control.init = 1;
        sMenu_Control.refesh = 1;
        sKey_Control.pressedKey = 0;
        break;
    default:
        break;
    }        
}

void Display_Operation_Logs(void)
{
    if(sMenu_Control.init)
    {
        gldc_clearGraphicScreen_DataArea(LAYER1_ADDR);
        gldc_clearGraphicScreen_DataArea(LAYER2_ADDR);
        glcd_writeString(sMenu_Object[_OPERATION_LOGS].menuName,1,1);
        glcd_Draw_Black_Rectangle(LAYER2_ADDR,1,1,20);
        sMenu_Control.init = 0;
    }
    if(sMenu_Control.refesh)
    {
        
        sMenu_Control.refesh = 0;
    }
    
    switch(sKey_Control.pressedKey)
    {
    case _UP_KEY:
        sMenu_Control.refesh = 1;
        sKey_Control.pressedKey = 0;
        break;
    case _DOWN_KEY:
        sMenu_Control.refesh = 1;
        sKey_Control.pressedKey = 0;
        break;
    case _ENTER_KEY:
        sMenu_Control.refesh = 1;
        sKey_Control.pressedKey = 0;
        break;
    case _ESC_KEY:
        sMenu_Control.menu = sMenu_Object[_OPERATION_LOGS].parentIndex;
        sMenu_Control.index = sMenu_Control.previousIndex;
        sMenu_Control.previousIndex = 0;
        sMenu_Control.ajustValue = 0;
        sMenu_Control.init = 1;
        sMenu_Control.refesh = 1;
        sKey_Control.pressedKey = 0;
        break;
    default:
        break;
    }    
}

void Display_ATS_Logs(void)
{
    if(sMenu_Control.init)
    {
        gldc_clearGraphicScreen_DataArea(LAYER1_ADDR);
        gldc_clearGraphicScreen_DataArea(LAYER2_ADDR);
        glcd_writeString(sMenu_Object[_ATS_LOGS].menuName,1,1);
        glcd_Draw_Black_Rectangle(LAYER2_ADDR,1,1,20);
        sMenu_Control.init = 0;
    }
    if(sMenu_Control.refesh)
    {
        
        sMenu_Control.refesh = 0;
    }
    
    switch(sKey_Control.pressedKey)
    {
    case _UP_KEY:
        sMenu_Control.refesh = 1;
        sKey_Control.pressedKey = 0;
        break;
    case _DOWN_KEY:
        sMenu_Control.refesh = 1;
        sKey_Control.pressedKey = 0;
        break;
    case _ENTER_KEY:
        sMenu_Control.refesh = 1;
        sKey_Control.pressedKey = 0;
        break;
    case _ESC_KEY:
        sMenu_Control.menu = sMenu_Object[_ATS_LOGS].parentIndex;
        sMenu_Control.index = sMenu_Control.previousIndex;
        sMenu_Control.previousIndex = 0;
        sMenu_Control.ajustValue = 0;
        sMenu_Control.init = 1;
        sMenu_Control.refesh = 1;
        sKey_Control.pressedKey = 0;
        break;
    default:
        break;
    }    
}

void Display_Datetime_Setting(void)
{
    if(sMenu_Control.init)
    {
        mTempVal_u16[0] = GTime.hour;
        mTempVal_u16[1] = GTime.min;
        mTempVal_u16[2] = GTime.sec;
        mTempVal_u16[3] = GTime.date;
        mTempVal_u16[4] = GTime.month;
        mTempVal_u16[5] = GTime.year;
        
        gldc_clearGraphicScreen_DataArea(LAYER1_ADDR);
        gldc_clearGraphicScreen_DataArea(LAYER2_ADDR);
        glcd_writeString(sMenu_Object[_DATETIME_CONFIG].menuName,1,1);
        glcd_Draw_Black_Rectangle(LAYER2_ADDR,1,1,20);
        glcd_writeString("  Time:             ",2,1);
        glcd_writeString("           :  :     ",3,1);
        glcd_writeString("  Date:             ",4,1);
        glcd_writeString("           /  /20   ",5,1);
        glcd_writeString("                    ",6,1);
        glcd_writeString("                    ",7,1);
        glcd_writeString("                    ",8,1);
        sMenu_Control.init = 0;
    }
    if(sMenu_Control.refesh)
    {
        glcd_Display_Integer_With_Zero(3,10,2,mTempVal_u16[0]);
        glcd_Display_Integer_With_Zero(3,13,2,mTempVal_u16[1]);
        glcd_Display_Integer_With_Zero(3,16,2,mTempVal_u16[2]);
        glcd_Display_Integer_With_Zero(5,10,2,mTempVal_u16[3]);
        glcd_Display_Integer_With_Zero(5,13,2,mTempVal_u16[4]);
        glcd_Display_Integer_With_Zero(5,18,2,mTempVal_u16[5]);
        
        if(sMenu_Control.ajustValue == 0)
            switch(sMenu_Control.index)
            {
            case 0: gldc_clearGraphicScreen_DataArea(LAYER2_ADDR);	glcd_Under_Line_Text(LAYER2_ADDR,3,10,11); break;
            case 1: gldc_clearGraphicScreen_DataArea(LAYER2_ADDR);	glcd_Under_Line_Text(LAYER2_ADDR,3,13,14); break;
            case 2: gldc_clearGraphicScreen_DataArea(LAYER2_ADDR);	glcd_Under_Line_Text(LAYER2_ADDR,3,16,17); break;
            case 3: gldc_clearGraphicScreen_DataArea(LAYER2_ADDR);	glcd_Under_Line_Text(LAYER2_ADDR,5,10,11); break;
            case 4: gldc_clearGraphicScreen_DataArea(LAYER2_ADDR);	glcd_Under_Line_Text(LAYER2_ADDR,5,13,14); break;
            case 5: gldc_clearGraphicScreen_DataArea(LAYER2_ADDR);	glcd_Under_Line_Text(LAYER2_ADDR,5,16,19); break;
            default: break;
            }
        
        sMenu_Control.refesh = 0;
    }
    
    switch(sKey_Control.pressedKey)
    {
    case _UP_KEY:
        if(sMenu_Control.ajustValue == 0){
            sMenu_Control.index--;
            if(sMenu_Control.index<0)
                sMenu_Control.index = 5;
        }else{
            switch(sMenu_Control.index)
            {
            case 0: Ajust_Variable_Value(0,&mTempVal_u16[0],_HOUR); break;
            case 1: Ajust_Variable_Value(0,&mTempVal_u16[1],_MINS); break;
            case 2: Ajust_Variable_Value(0,&mTempVal_u16[2],_SECS); break;
            case 3: Ajust_Variable_Value(0,&mTempVal_u16[3],_DATE); break;
            case 4: Ajust_Variable_Value(0,&mTempVal_u16[4],_MONTH); break;
            case 5: Ajust_Variable_Value(0,&mTempVal_u16[5],_YEAR); break;
            default:
                break;
            }
        }
        sMenu_Control.refesh = 1;
        sKey_Control.pressedKey = 0;
        break;
    case _DOWN_KEY:
        if(sMenu_Control.ajustValue == 0){
            sMenu_Control.index++;
            if(sMenu_Control.index>5)
                sMenu_Control.index = 0;
        }else{
            switch(sMenu_Control.index)
            {
            case 0: Ajust_Variable_Value(1,&mTempVal_u16[0],_HOUR); break;
            case 1: Ajust_Variable_Value(1,&mTempVal_u16[1],_MINS); break;
            case 2: Ajust_Variable_Value(1,&mTempVal_u16[2],_SECS); break;
            case 3: Ajust_Variable_Value(1,&mTempVal_u16[3],_DATE); break;
            case 4: Ajust_Variable_Value(1,&mTempVal_u16[4],_MONTH); break;
            case 5: Ajust_Variable_Value(1,&mTempVal_u16[5],_YEAR); break;
            default:
                break;
            }
        }
        sMenu_Control.refesh = 1;
        sKey_Control.pressedKey = 0;
        break;
    case _ENTER_KEY:
        if(sMenu_Control.ajustValue == 0){
            switch(sMenu_Control.index)
            {
            case 0: glcd_Draw_Black_Rectangle(LAYER2_ADDR,3,10,11); break;
            case 1: glcd_Draw_Black_Rectangle(LAYER2_ADDR,3,13,14); break;
            case 2: glcd_Draw_Black_Rectangle(LAYER2_ADDR,3,16,17); break;
            case 3: glcd_Draw_Black_Rectangle(LAYER2_ADDR,5,10,11); break;
            case 4: glcd_Draw_Black_Rectangle(LAYER2_ADDR,5,13,14); break;
            case 5: glcd_Draw_Black_Rectangle(LAYER2_ADDR,5,16,19); break;
            default: break;
            }
            sMenu_Control.ajustValue = 1;
        }else{
            GTime.year = mTempVal_u16[5];  STime.year=GTime.year;
            GTime.month = mTempVal_u16[4]; STime.month=GTime.month;
            GTime.date = mTempVal_u16[3];  STime.date=GTime.date;
            GTime.hour = mTempVal_u16[0];  STime.hour=GTime.hour;
            GTime.min = mTempVal_u16[1];   STime.min=GTime.min;
            GTime.sec = mTempVal_u16[2];   STime.sec=GTime.sec;
            SetTime(STime);
            sMenu_Control.ajustValue = 0;
            sMenu_Variable.changedVal = _HOUR;
        }
        sMenu_Control.refesh = 1;
        sKey_Control.pressedKey = 0;
        break;
    case _ESC_KEY:
        sMenu_Control.menu = sMenu_Object[_DATETIME_CONFIG].parentIndex;
        sMenu_Control.index = sMenu_Control.previousIndex;
        sMenu_Control.previousIndex = 0;
        sMenu_Control.ajustValue = 0;
        sMenu_Control.init = 1;
        sMenu_Control.refesh = 1;
        sKey_Control.pressedKey = 0;
        break;
    default:
        break;
    }   
}

void Display_DeviceInfo_Setting(void)
{
    if(sMenu_Control.init)
    {
        gldc_clearGraphicScreen_DataArea(LAYER1_ADDR);
        gldc_clearGraphicScreen_DataArea(LAYER2_ADDR);
        glcd_writeString(sMenu_Object[_DEVICE_INFO].menuName,1,1);
        glcd_Draw_Black_Rectangle(LAYER2_ADDR,1,1,20);
        sMenu_Control.init = 0;
    }
    if(sMenu_Control.refesh)
    {
        
        sMenu_Control.refesh = 0;
    }
    
    switch(sKey_Control.pressedKey)
    {
    case _UP_KEY:
        sMenu_Control.refesh = 1;
        sKey_Control.pressedKey = 0;
        break;
    case _DOWN_KEY:
        sMenu_Control.refesh = 1;
        sKey_Control.pressedKey = 0;
        break;
    case _ENTER_KEY:
        sMenu_Control.refesh = 1;
        sKey_Control.pressedKey = 0;
        break;
    case _ESC_KEY:
        
        sMenu_Control.menu = sMenu_Object[_DEVICE_INFO].parentIndex;
        sMenu_Control.index = sMenu_Control.previousIndex;
        sMenu_Control.previousIndex = 0;
        sMenu_Control.ajustValue = 0;
        sMenu_Control.init = 1;
        sMenu_Control.refesh = 1;
        sKey_Control.pressedKey = 0;
        break;
    default:
        break;
    }   
}

void Display_Setting_Confirm(void)
{
    NetInterface *interface;
    Ipv4Addr ipv4Addr;
    if(sMenu_Control.init)
    {
        gldc_clearGraphicScreen_DataArea(LAYER1_ADDR);
        gldc_clearGraphicScreen_DataArea(LAYER2_ADDR);
        glcd_writeString(sMenu_Object[_SETTING_CONFIRM].menuName,1,1);
        glcd_Draw_Black_Rectangle(LAYER2_ADDR,1,1,20);
        glcd_writeString("                    ",2,1);
        glcd_writeString("  Confirm to save   ",3,1);
        glcd_writeString("  all IP setting?   ",4,1);
        glcd_writeString("  ENTER to save     ",5,1);
        glcd_writeString("  ESC to quit       ",6,1);
        glcd_writeString("                    ",7,1);
        glcd_writeString("                    ",8,1);
        
        sMenu_Control.init = 0;
    }
    if(sMenu_Control.refesh)
    {
        
        sMenu_Control.refesh = 0;
    }
    
    switch(sKey_Control.pressedKey)
    {
    case _UP_KEY:
        sMenu_Control.refesh = 1;
        sKey_Control.pressedKey = 0;
        break;
    case _DOWN_KEY:
        sMenu_Control.refesh = 1;
        sKey_Control.pressedKey = 0;
        break;
    case _ENTER_KEY:		
        sMenu_Variable.sEthernetSetting.u16DevIP[0] = sMenu_Variable.sEthernetSetting_temp.u16DevIP[0]; 
        I2C_Get_Lock();
        vTaskSuspendAll();
        WriteEEPROM_Word(sSetting_Values[_DEV_IP1].addrEEPROM,sMenu_Variable.sEthernetSetting.u16DevIP[0]);
        xTaskResumeAll();
        I2C_Release_Lock();
        sMenu_Variable.sEthernetSetting.u16DevIP[1] = sMenu_Variable.sEthernetSetting_temp.u16DevIP[1]; 
        I2C_Get_Lock();
        vTaskSuspendAll();
        WriteEEPROM_Word(sSetting_Values[_DEV_IP2].addrEEPROM,sMenu_Variable.sEthernetSetting.u16DevIP[1]);
        xTaskResumeAll();
        I2C_Release_Lock();
        sMenu_Variable.sEthernetSetting.u16DevIP[2] = sMenu_Variable.sEthernetSetting_temp.u16DevIP[2]; 
        I2C_Get_Lock();
        vTaskSuspendAll();
        WriteEEPROM_Word(sSetting_Values[_DEV_IP3].addrEEPROM,sMenu_Variable.sEthernetSetting.u16DevIP[2]);
        xTaskResumeAll();
        I2C_Release_Lock();
        sMenu_Variable.sEthernetSetting.u16DevIP[3] = sMenu_Variable.sEthernetSetting_temp.u16DevIP[3]; 
        I2C_Get_Lock();
        vTaskSuspendAll();
        WriteEEPROM_Word(sSetting_Values[_DEV_IP4].addrEEPROM,sMenu_Variable.sEthernetSetting.u16DevIP[3]);
        xTaskResumeAll();
        I2C_Release_Lock();
        sMenu_Variable.sEthernetSetting.u16DevSubnet[0] = sMenu_Variable.sEthernetSetting_temp.u16DevSubnet[0]; 
        I2C_Get_Lock();
        vTaskSuspendAll();
        WriteEEPROM_Word(sSetting_Values[_DEV_SUBNET1].addrEEPROM,sMenu_Variable.sEthernetSetting.u16DevSubnet[0]);
        xTaskResumeAll();
        I2C_Release_Lock();
        sMenu_Variable.sEthernetSetting.u16DevSubnet[1] = sMenu_Variable.sEthernetSetting_temp.u16DevSubnet[1]; 
        I2C_Get_Lock();
        vTaskSuspendAll();
        WriteEEPROM_Word(sSetting_Values[_DEV_SUBNET2].addrEEPROM,sMenu_Variable.sEthernetSetting.u16DevSubnet[1]);
        xTaskResumeAll();
        I2C_Release_Lock();
        sMenu_Variable.sEthernetSetting.u16DevSubnet[2] = sMenu_Variable.sEthernetSetting_temp.u16DevSubnet[2];
        I2C_Get_Lock();
        vTaskSuspendAll();
        WriteEEPROM_Word(sSetting_Values[_DEV_SUBNET3].addrEEPROM,sMenu_Variable.sEthernetSetting.u16DevSubnet[2]);
        xTaskResumeAll();
        I2C_Release_Lock();
        sMenu_Variable.sEthernetSetting.u16DevSubnet[3] = sMenu_Variable.sEthernetSetting_temp.u16DevSubnet[3];
        I2C_Get_Lock();
        vTaskSuspendAll();
        WriteEEPROM_Word(sSetting_Values[_DEV_SUBNET4].addrEEPROM,sMenu_Variable.sEthernetSetting.u16DevSubnet[3]);
        xTaskResumeAll();
        I2C_Release_Lock();
        sMenu_Variable.sEthernetSetting.u16DevGateway[0] = sMenu_Variable.sEthernetSetting_temp.u16DevGateway[0];
        I2C_Get_Lock();
        vTaskSuspendAll();
        WriteEEPROM_Word(sSetting_Values[_DEV_GATEW1].addrEEPROM,sMenu_Variable.sEthernetSetting.u16DevGateway[0]);
        xTaskResumeAll();
        I2C_Release_Lock();
        sMenu_Variable.sEthernetSetting.u16DevGateway[1] = sMenu_Variable.sEthernetSetting_temp.u16DevGateway[1];
        I2C_Get_Lock();
        vTaskSuspendAll();
        WriteEEPROM_Word(sSetting_Values[_DEV_GATEW2].addrEEPROM,sMenu_Variable.sEthernetSetting.u16DevGateway[1]);
        xTaskResumeAll();
        I2C_Release_Lock();
        sMenu_Variable.sEthernetSetting.u16DevGateway[2] = sMenu_Variable.sEthernetSetting_temp.u16DevGateway[2];
        I2C_Get_Lock();
        vTaskSuspendAll();
        WriteEEPROM_Word(sSetting_Values[_DEV_GATEW3].addrEEPROM,sMenu_Variable.sEthernetSetting.u16DevGateway[2]);
        xTaskResumeAll();
        I2C_Release_Lock();
        sMenu_Variable.sEthernetSetting.u16DevGateway[3] = sMenu_Variable.sEthernetSetting_temp.u16DevGateway[3];
        I2C_Get_Lock();
        vTaskSuspendAll();
        WriteEEPROM_Word(sSetting_Values[_DEV_GATEW4].addrEEPROM,sMenu_Variable.sEthernetSetting.u16DevGateway[3]);
        xTaskResumeAll();
        I2C_Release_Lock();
        sMenu_Variable.sEthernetSetting.u16DevPort = sMenu_Variable.sEthernetSetting_temp.u16DevPort;
        I2C_Get_Lock();
        vTaskSuspendAll();
        WriteEEPROM_Word(sSetting_Values[_DEV_PORT].addrEEPROM,sMenu_Variable.sEthernetSetting.u16DevPort);
        xTaskResumeAll();
        I2C_Release_Lock();
        
        //	sMenu_Variable.u32IP =  (sMenu_Variable.sEthernetSetting.u16DevIP[3] << 24)|(sMenu_Variable.sEthernetSetting.u16DevIP[2] << 16)|(sMenu_Variable.sEthernetSetting.u16DevIP[1] << 8)|(sMenu_Variable.sEthernetSetting.u16DevIP[0]);
        //	sMenu_Variable.u32SN =  (sMenu_Variable.sEthernetSetting.u16DevSubnet[3] << 24)|(sMenu_Variable.sEthernetSetting.u16DevSubnet[2] << 16)|(sMenu_Variable.sEthernetSetting.u16DevSubnet[1] << 8)|(sMenu_Variable.sEthernetSetting.u16DevSubnet[0]);
        //	sMenu_Variable.u32GW =  (sMenu_Variable.sEthernetSetting.u16DevGateway[3] << 24)|(sMenu_Variable.sEthernetSetting.u16DevGateway[2] << 16)|(sMenu_Variable.sEthernetSetting.u16DevGateway[1] << 8)|(sMenu_Variable.sEthernetSetting.u16DevGateway[0]);
        
        sprintf((char_t*)sMenu_Variable.ucIP,"%d.%d.%d.%d",
                sMenu_Variable.sEthernetSetting.u16DevIP[0],
                sMenu_Variable.sEthernetSetting.u16DevIP[1],
                sMenu_Variable.sEthernetSetting.u16DevIP[2],
                sMenu_Variable.sEthernetSetting.u16DevIP[3]);
        
        sprintf((char_t*)sMenu_Variable.ucGW,"%d.%d.%d.%d",
                sMenu_Variable.sEthernetSetting.u16DevGateway[0],
                sMenu_Variable.sEthernetSetting.u16DevGateway[1],
                sMenu_Variable.sEthernetSetting.u16DevGateway[2],
                sMenu_Variable.sEthernetSetting.u16DevGateway[3]);
        
        sprintf((char_t*)sMenu_Variable.ucSN,"%d.%d.%d.%d",
                sMenu_Variable.sEthernetSetting.u16DevSubnet[0],
                sMenu_Variable.sEthernetSetting.u16DevSubnet[1],
                sMenu_Variable.sEthernetSetting.u16DevSubnet[2],
                sMenu_Variable.sEthernetSetting.u16DevSubnet[3]);
        //Configure the first Ethernet interface
        interface = &netInterface[0];
        
        //Set IPv4 host address
        ipv4StringToAddr((const char_t*)sMenu_Variable.ucIP, &ipv4Addr);//"192.168.100.104"
        ipv4SetHostAddr(interface, ipv4Addr);
        
        //Set subnet mask
        ipv4StringToAddr((const char_t*)sMenu_Variable.ucSN, &ipv4Addr);//"255.255.255.0"
        ipv4SetSubnetMask(interface, ipv4Addr);
        
        //Set default gateway
        ipv4StringToAddr((const char_t*)sMenu_Variable.ucGW, &ipv4Addr);//"192.168.100.1"
        ipv4SetDefaultGateway(interface, ipv4Addr);
        
        memset(&sMenu_Variable.sEthernetSetting_temp,0,sizeof(sMenu_Variable.sEthernetSetting_temp));
        
        sMenu_Control.menu = _NETWORK_CONFIG;// sMenu_Object[_DEVICE_INFO].parentIndex;
        //	 sMenu_Control.index = 0;//sMenu_Control.previousIndex;
        //	 sMenu_Control.previousIndex = 0;
        //	 sMenu_Control.ajustValue = 0;
        sMenu_Control.init = 1;
        sMenu_Control.refesh = 1;
        sKey_Control.pressedKey = 0;
        break;
    case _ESC_KEY:
        sMenu_Control.menu = _NETWORK_CONFIG;// sMenu_Object[_DEVICE_INFO].parentIndex;
        //	 sMenu_Control.index = 0;//sMenu_Control.previousIndex;
        //	 sMenu_Control.previousIndex = 0;
        //	 sMenu_Control.ajustValue = 0;
        sMenu_Control.init = 1;
        sMenu_Control.refesh = 1;
        sKey_Control.pressedKey = 0;
        break;
    default:
        break;
    }
}

void Display_User_ID_Setting(void)
{
    uint8_t i=0,j=0;
    if(sMenu_Control.init)
    {
        for(i=0;i<5;i++)
            for(j=0;j<8;j++)
                TempUserID[i][j] = sMenu_Variable.u8UserID[i][j];
        
        gldc_clearGraphicScreen_DataArea(LAYER1_ADDR);
        gldc_clearGraphicScreen_DataArea(LAYER2_ADDR);
        glcd_writeString(sMenu_Object[_USER_ID].menuName,1,1);
        glcd_Draw_Black_Rectangle(LAYER2_ADDR,1,1,20);
        glcd_writeString("  User1:            ",2,1);
        glcd_writeString("  User2:            ",3,1);
        glcd_writeString("  User3:            ",4,1);
        glcd_writeString("  User4:            ",5,1);
        glcd_writeString("  User5:            ",6,1);
        glcd_writeString("                    ",7,1);
        glcd_writeString("                    ",8,1);
        sMenu_Control.init = 0;
    }
    
    if(sMenu_Control.refesh)
    {
        glcd_writeString((char*)TempUserID[0],2,10);
        glcd_writeString((char*)TempUserID[1],3,10);
        glcd_writeString((char*)TempUserID[2],4,10);
        glcd_writeString((char*)TempUserID[3],5,10);
        glcd_writeString((char*)TempUserID[4],6,10);
        if(sMenu_Control.ajustValue == 0)
            switch(sMenu_Control.index)
            {
            case 0: gldc_clearGraphicScreen_DataArea(LAYER2_ADDR);	glcd_Under_Line_Text(LAYER2_ADDR,2,10,17); break;
            case 1: gldc_clearGraphicScreen_DataArea(LAYER2_ADDR);	glcd_Under_Line_Text(LAYER2_ADDR,3,10,17); break;
            case 2: gldc_clearGraphicScreen_DataArea(LAYER2_ADDR);	glcd_Under_Line_Text(LAYER2_ADDR,4,10,17); break;
            case 3: gldc_clearGraphicScreen_DataArea(LAYER2_ADDR);	glcd_Under_Line_Text(LAYER2_ADDR,5,10,17); break;
            case 4: gldc_clearGraphicScreen_DataArea(LAYER2_ADDR);	glcd_Under_Line_Text(LAYER2_ADDR,6,10,17); break;
            default: break;
            }
        sMenu_Control.refesh = 0;
    }
    
    switch(sKey_Control.pressedKey)
    {
    case _UP_KEY:
        if(sMenu_Control.ajustValue == 0){
            sMenu_Control.index--;
            if(sMenu_Control.index<0)
                sMenu_Control.index = 4;
        }
        else
        {
            for(j=0;j<8;j++)
                TempUserID[sMenu_Control.index][j] = 0xFF;
        }
        sMenu_Control.refesh = 1;
        sKey_Control.pressedKey = 0;
        break;
    case _DOWN_KEY:
        if(sMenu_Control.ajustValue == 0){
            sMenu_Control.index++;
            if(sMenu_Control.index>4)
                sMenu_Control.index = 0;
        }
        else
        {
            for(j=0;j<8;j++)
                TempUserID[sMenu_Control.index][j] = 0xFF;
        }
        sMenu_Control.refesh = 1;
        sKey_Control.pressedKey = 0;
        break;
    case _ENTER_KEY:
        if(sMenu_Control.ajustValue == 0){
            switch(sMenu_Control.index)
            {
            case 0: glcd_Draw_Black_Rectangle(LAYER2_ADDR,2,10,17); break;
            case 1: glcd_Draw_Black_Rectangle(LAYER2_ADDR,3,10,17); break;
            case 2: glcd_Draw_Black_Rectangle(LAYER2_ADDR,4,10,17); break;
            case 3: glcd_Draw_Black_Rectangle(LAYER2_ADDR,5,10,17); break;
            case 4: glcd_Draw_Black_Rectangle(LAYER2_ADDR,6,10,17); break;
            default: break;
            }
            sMenu_Control.ajustValue = 1;
            sMenu_Control.learnUID = 1;
        }else{
            for(j=0;j<8;j++)
                sMenu_Variable.u8UserID[sMenu_Control.index][j] = TempUserID[sMenu_Control.index][j];
            ACS_SaveUserID(sMenu_Variable.u8UserIDAddr[sMenu_Control.index],TempUserID[sMenu_Control.index]);
            sMenu_Control.learnUID = 0;
            sMenu_Control.ajustValue = 0;
        }
        sMenu_Control.refesh = 1;
        sKey_Control.pressedKey = 0;
        break;
    case _ESC_KEY:
        sMenu_Control.menu = sMenu_Object[_USER_ID].parentIndex;
        sMenu_Control.index = sMenu_Control.previousIndex;
        sMenu_Control.previousIndex = 0;
        sMenu_Control.ajustValue = 0;
        sMenu_Control.init = 1;
        sMenu_Control.refesh = 1;
        sKey_Control.pressedKey = 0;
        break;
    default:
        break;
    } 
}


void Display_AirCon_Setting(void)
{
    sSetting_Values[_AIRCON_TEMP1].lowerVal = sMenu_Variable.u16AirConTemp[1] + 1;
    sSetting_Values[_AIRCON_TEMP2].upperVal = sMenu_Variable.u16AirConTemp[0] - 1;
    sSetting_Values[_AIRCON_TEMP2].lowerVal = sMenu_Variable.u16AirConTemp[2] + 1;
    sSetting_Values[_AIRCON_TEMP3].upperVal = sMenu_Variable.u16AirConTemp[1] - 1;
    sSetting_Values[_AIRCON_TEMP3].lowerVal = sMenu_Variable.u16AirConTemp[3] + 1;
    sSetting_Values[_AIRCON_TEMP4].upperVal = sMenu_Variable.u16AirConTemp[2] - 1;
    if(sMenu_Control.init)
    {
        mTempVal_u16[0] = sMenu_Variable.u16AirConTime1;
        mTempVal_u16[1] = sMenu_Variable.u16AirConTime2;
        mTempVal_u16[2] = sMenu_Variable.u16AirConTemp[0];
        mTempVal_u16[3] = sMenu_Variable.u16AirConTemp[1];
        mTempVal_u16[4] = sMenu_Variable.u16AirConTemp[2];
        mTempVal_u16[5] = sMenu_Variable.u16AirConTemp[3];
        
        gldc_clearGraphicScreen_DataArea(LAYER1_ADDR);
        gldc_clearGraphicScreen_DataArea(LAYER2_ADDR);
        glcd_writeString(sMenu_Object[_AIRCON].menuName,1,1);
        glcd_Draw_Black_Rectangle(LAYER2_ADDR,1,1,20);
        
        glcd_writeString("  Time T1:          ",2,1);
        glcd_writeString("               min  ",3,1);
        glcd_writeString("  Time T2:          ",4,1);
        glcd_writeString("               min  ",5,1);
        glcd_writeString("  Temperature:      ",6,1);
        glcd_writeString("   1:       2:      ",7,1);
        glcd_writeString("   3:       4:      ",8,1);
        
        sMenu_Control.init = 0;
    }
    if(sMenu_Control.refesh)
    {
        glcd_Display_Integer(3,12,3,mTempVal_u16[0]);
        glcd_Display_Integer(5,12,3,mTempVal_u16[1]);
        glcd_Display_Float_1Dec(7,7,4,mTempVal_u16[2]);
        glcd_Display_Float_1Dec(7,16,4,mTempVal_u16[3]);
        glcd_Display_Float_1Dec(8,7,4,mTempVal_u16[4]);
        glcd_Display_Float_1Dec(8,16,4,mTempVal_u16[5]);
        
        if(sMenu_Control.ajustValue == 0)
            switch(sMenu_Control.index)
            {
            case 0: gldc_clearGraphicScreen_DataArea(LAYER2_ADDR);	glcd_Under_Line_Text(LAYER2_ADDR,3,12,14); break;
            case 1: gldc_clearGraphicScreen_DataArea(LAYER2_ADDR);	glcd_Under_Line_Text(LAYER2_ADDR,5,12,14); break;
            case 2: gldc_clearGraphicScreen_DataArea(LAYER2_ADDR);	glcd_Under_Line_Text(LAYER2_ADDR,7,7,10); break;
            case 3: gldc_clearGraphicScreen_DataArea(LAYER2_ADDR);	glcd_Under_Line_Text(LAYER2_ADDR,7,16,19); break;
            case 4: gldc_clearGraphicScreen_DataArea(LAYER2_ADDR);	glcd_Under_Line_Text(LAYER2_ADDR,8,7,10); break;
            case 5: gldc_clearGraphicScreen_DataArea(LAYER2_ADDR);	glcd_Under_Line_Text(LAYER2_ADDR,8,16,19); break;
            default: break;
            }
        
        sMenu_Control.refesh = 0;
    }
    
    switch(sKey_Control.pressedKey)
    {
    case _UP_KEY:
        if(sMenu_Control.ajustValue == 0){
            sMenu_Control.index--;
            if(sMenu_Control.index<0)
                sMenu_Control.index = 5;
        }else{
            switch(sMenu_Control.index)
            {
            case 0: Ajust_Variable_Value(0,&mTempVal_u16[0],_AIRCON_TIME1); break;
            case 1: Ajust_Variable_Value(0,&mTempVal_u16[1],_AIRCON_TIME2); break;
            case 2: Ajust_Variable_Value(0,&mTempVal_u16[2],_AIRCON_TEMP1); break;
            case 3: Ajust_Variable_Value(0,&mTempVal_u16[3],_AIRCON_TEMP2); break;
            case 4: Ajust_Variable_Value(0,&mTempVal_u16[4],_AIRCON_TEMP3); break;
            case 5: Ajust_Variable_Value(0,&mTempVal_u16[5],_AIRCON_TEMP4); break;
            default:
                break;
            }
        }
        sMenu_Control.refesh = 1;
        sKey_Control.pressedKey = 0;
        break;
    case _DOWN_KEY:
        if(sMenu_Control.ajustValue == 0){
            sMenu_Control.index++;
            if(sMenu_Control.index>5)
                sMenu_Control.index = 0;
        }else{
            switch(sMenu_Control.index)
            {
            case 0: Ajust_Variable_Value(1,&mTempVal_u16[0],_AIRCON_TIME1); break;
            case 1: Ajust_Variable_Value(1,&mTempVal_u16[1],_AIRCON_TIME2); break;
            case 2: Ajust_Variable_Value(1,&mTempVal_u16[2],_AIRCON_TEMP1); break;
            case 3: Ajust_Variable_Value(1,&mTempVal_u16[3],_AIRCON_TEMP2); break;
            case 4: Ajust_Variable_Value(1,&mTempVal_u16[4],_AIRCON_TEMP3); break;
            case 5: Ajust_Variable_Value(1,&mTempVal_u16[5],_AIRCON_TEMP4); break;
            break;
            }		
        }
        sMenu_Control.refesh = 1;
        sKey_Control.pressedKey = 0;
        break;
    case _ENTER_KEY:
        if(sMenu_Control.ajustValue == 0){
            switch(sMenu_Control.index)
            {
            case 0: glcd_Draw_Black_Rectangle(LAYER2_ADDR,3,12,14); break;
            case 1: glcd_Draw_Black_Rectangle(LAYER2_ADDR,5,11,14); break;
            case 2: glcd_Draw_Black_Rectangle(LAYER2_ADDR,7,7,10); break;
            case 3: glcd_Draw_Black_Rectangle(LAYER2_ADDR,7,16,19); break;
            case 4: glcd_Draw_Black_Rectangle(LAYER2_ADDR,8,7,10); break;
            case 5: glcd_Draw_Black_Rectangle(LAYER2_ADDR,8,16,19); break;
            default: break;
            }
            sMenu_Control.ajustValue = 1;
        }else{
            switch(sMenu_Control.index)
            {
            case 0: 
                sMenu_Variable.changedVal = _AIRCON_TIME1; 
                sMenu_Variable.u16AirConTime1 = mTempVal_u16[0]; 
                I2C_Get_Lock();
                vTaskSuspendAll();
                WriteEEPROM_Word(sSetting_Values[_AIRCON_TIME1].addrEEPROM,sMenu_Variable.u16AirConTime1); 
                xTaskResumeAll();
                I2C_Release_Lock();
                break;
            case 1: 
                sMenu_Variable.changedVal = _AIRCON_TIME2; 
                sMenu_Variable.u16AirConTime2 = mTempVal_u16[1];
                I2C_Get_Lock();
                vTaskSuspendAll();
                WriteEEPROM_Word(sSetting_Values[_AIRCON_TIME2].addrEEPROM,sMenu_Variable.u16AirConTime2); 
                xTaskResumeAll();
                I2C_Release_Lock();
                break;
            case 2: 
                sMenu_Variable.changedVal = _AIRCON_TEMP1; 
                sMenu_Variable.u16AirConTemp[0] = mTempVal_u16[2]; 
                I2C_Get_Lock();
                vTaskSuspendAll();
                WriteEEPROM_Word(sSetting_Values[_AIRCON_TEMP1].addrEEPROM,sMenu_Variable.u16AirConTemp[0]);
                xTaskResumeAll();
                I2C_Release_Lock();
                break;
            case 3: 
                sMenu_Variable.changedVal = _AIRCON_TEMP2;
                sMenu_Variable.u16AirConTemp[1] = mTempVal_u16[3];
                I2C_Get_Lock();
                vTaskSuspendAll();
                WriteEEPROM_Word(sSetting_Values[_AIRCON_TEMP2].addrEEPROM,sMenu_Variable.u16AirConTemp[1]);
                xTaskResumeAll();
                I2C_Release_Lock();
                break;
            case 4: 
                sMenu_Variable.changedVal = _AIRCON_TEMP3;
                sMenu_Variable.u16AirConTemp[2] = mTempVal_u16[4];
                I2C_Get_Lock();
                vTaskSuspendAll();
                WriteEEPROM_Word(sSetting_Values[_AIRCON_TEMP3].addrEEPROM,sMenu_Variable.u16AirConTemp[2]);
                xTaskResumeAll();
                I2C_Release_Lock();
                break;
            case 5:
                sMenu_Variable.changedVal = _AIRCON_TEMP4;
                sMenu_Variable.u16AirConTemp[3] = mTempVal_u16[5];
                I2C_Get_Lock();
                vTaskSuspendAll();
                WriteEEPROM_Word(sSetting_Values[_AIRCON_TEMP4].addrEEPROM,sMenu_Variable.u16AirConTemp[3]);
                xTaskResumeAll();
                I2C_Release_Lock();
                break;
            default: break;
            }		
            sMenu_Control.ajustValue = 0;
        }
        sMenu_Control.refesh = 1;
        sKey_Control.pressedKey = 0;
        break;
    case _ESC_KEY:
        sMenu_Control.menu = sMenu_Object[_AIRCON].parentIndex;
        sMenu_Control.index = sMenu_Control.previousIndex;
        sMenu_Control.previousIndex = 0;
        sMenu_Control.ajustValue = 0;
        sMenu_Control.init = 1;
        sMenu_Control.refesh = 1;
        sKey_Control.pressedKey = 0;
        break;
    default:
        break;
    }     
}

void Ajust_Variable_Value (uint8_t inc_dec, uint16_t* mVariable, uint8_t	variableIndex)
{
    switch(inc_dec)
    {
    case 0://increase
        *mVariable += sSetting_Values[variableIndex].step;
        if(*mVariable > sSetting_Values[variableIndex].upperVal)
            *mVariable = sSetting_Values[variableIndex].lowerVal;
        break;
    case 1://decrease
        *mVariable -= sSetting_Values[variableIndex].step;
        if(*mVariable < sSetting_Values[variableIndex].lowerVal)
            *mVariable = sSetting_Values[variableIndex].upperVal;
        if(*mVariable > sSetting_Values[variableIndex].upperVal)
            *mVariable = sSetting_Values[variableIndex].upperVal;
        break;
    default:
        break;
    }
    
}

void Write_Default_All_Variable(void){
    Default(sSetting_Values[_AC_LOW].addrEEPROM,&sMenu_Variable.u16AcThresVolt[0],sSetting_Values[_AC_LOW].defaultVal);
    Default(sSetting_Values[_DC_LOW].addrEEPROM,&sMenu_Variable.u16BattThresVolt[0],sSetting_Values[_DC_LOW].defaultVal);
    Default(sSetting_Values[_TEMP1].addrEEPROM,&sMenu_Variable.u16ThresTemp[0],sSetting_Values[_TEMP1].defaultVal);
    Default(sSetting_Values[_TEMP2].addrEEPROM,&sMenu_Variable.u16ThresTemp[1],sSetting_Values[_TEMP2].defaultVal);
    Default(sSetting_Values[_TEMP3].addrEEPROM,&sMenu_Variable.u16ThresTemp[2],sSetting_Values[_TEMP3].defaultVal);
    Default(sSetting_Values[_TEMP4].addrEEPROM,&sMenu_Variable.u16ThresTemp[3],sSetting_Values[_TEMP4].defaultVal);
    Default(sSetting_Values[_DEV_IP1].addrEEPROM,&sMenu_Variable.sEthernetSetting.u16DevIP[0],sSetting_Values[_DEV_IP1].defaultVal);
    Default(sSetting_Values[_DEV_IP2].addrEEPROM,&sMenu_Variable.sEthernetSetting.u16DevIP[1],sSetting_Values[_DEV_IP2].defaultVal);
    Default(sSetting_Values[_DEV_IP3].addrEEPROM,&sMenu_Variable.sEthernetSetting.u16DevIP[2],sSetting_Values[_DEV_IP3].defaultVal);
    Default(sSetting_Values[_DEV_IP4].addrEEPROM,&sMenu_Variable.sEthernetSetting.u16DevIP[3],sSetting_Values[_DEV_IP4].defaultVal);
    Default(sSetting_Values[_DEV_SUBNET1].addrEEPROM,&sMenu_Variable.sEthernetSetting.u16DevSubnet[0],sSetting_Values[_DEV_SUBNET1].defaultVal);
    Default(sSetting_Values[_DEV_SUBNET2].addrEEPROM,&sMenu_Variable.sEthernetSetting.u16DevSubnet[1],sSetting_Values[_DEV_SUBNET2].defaultVal);
    Default(sSetting_Values[_DEV_SUBNET3].addrEEPROM,&sMenu_Variable.sEthernetSetting.u16DevSubnet[2],sSetting_Values[_DEV_SUBNET3].defaultVal);
    Default(sSetting_Values[_DEV_SUBNET4].addrEEPROM,&sMenu_Variable.sEthernetSetting.u16DevSubnet[3],sSetting_Values[_DEV_SUBNET4].defaultVal);
    Default(sSetting_Values[_DEV_GATEW1].addrEEPROM,&sMenu_Variable.sEthernetSetting.u16DevGateway[0],sSetting_Values[_DEV_GATEW1].defaultVal);
    Default(sSetting_Values[_DEV_GATEW2].addrEEPROM,&sMenu_Variable.sEthernetSetting.u16DevGateway[1],sSetting_Values[_DEV_GATEW2].defaultVal);
    Default(sSetting_Values[_DEV_GATEW3].addrEEPROM,&sMenu_Variable.sEthernetSetting.u16DevGateway[2],sSetting_Values[_DEV_GATEW3].defaultVal);
    Default(sSetting_Values[_DEV_GATEW4].addrEEPROM,&sMenu_Variable.sEthernetSetting.u16DevGateway[3],sSetting_Values[_DEV_GATEW4].defaultVal);
    Default(sSetting_Values[_DEV_PORT].addrEEPROM,&sMenu_Variable.sEthernetSetting.u16DevPort,sSetting_Values[_DEV_PORT].defaultVal);
    Default(sSetting_Values[_SERVER_IP1].addrEEPROM,&sMenu_Variable.u16ServerIP[0],sSetting_Values[_SERVER_IP1].defaultVal);
    Default(sSetting_Values[_SERVER_IP2].addrEEPROM,&sMenu_Variable.u16ServerIP[1],sSetting_Values[_SERVER_IP2].defaultVal);
    Default(sSetting_Values[_SERVER_IP3].addrEEPROM,&sMenu_Variable.u16ServerIP[2],sSetting_Values[_SERVER_IP3].defaultVal);
    Default(sSetting_Values[_SERVER_IP4].addrEEPROM,&sMenu_Variable.u16ServerIP[3],sSetting_Values[_SERVER_IP4].defaultVal);
    Default(sSetting_Values[_SERVER_PORT].addrEEPROM,&sMenu_Variable.u16ServerPort,sSetting_Values[_SERVER_PORT].defaultVal);
    Default(sSetting_Values[_GEN_MAX_RUNTIME].addrEEPROM,&sMenu_Variable.u16GENMaxRuntime,sSetting_Values[_GEN_MAX_RUNTIME].defaultVal);
    Default(sSetting_Values[_GEN_UNDER_VOLT].addrEEPROM,&sMenu_Variable.u16GENUnderVolt,sSetting_Values[_GEN_UNDER_VOLT].defaultVal);
    Default(sSetting_Values[_GEN_ERROR_RESET_EN].addrEEPROM,&sMenu_Variable.u16GENErrorResetEnable,sSetting_Values[_GEN_ERROR_RESET_EN].defaultVal);
    Default(sSetting_Values[_GEN_ERROR_RESET_MIN].addrEEPROM,&sMenu_Variable.u16GENErrorResetTime,sSetting_Values[_GEN_ERROR_RESET_MIN].defaultVal);	
    Default(sSetting_Values[_GEN_WARM_UP_TIME].addrEEPROM,&sMenu_Variable.u16GENWarmUpTime,sSetting_Values[_GEN_WARM_UP_TIME].defaultVal);
    Default(sSetting_Values[_GEN_COOL_DOWN_TIME].addrEEPROM,&sMenu_Variable.u16GENCoolDownTime,sSetting_Values[_GEN_COOL_DOWN_TIME].defaultVal);
    Default(sSetting_Values[_GEN_NIGHT_EN].addrEEPROM,&sMenu_Variable.u16GENNightEnable,sSetting_Values[_GEN_NIGHT_EN].defaultVal);	
    Default(sSetting_Values[_GEN_NIGHT_BEGIN].addrEEPROM,&sMenu_Variable.u16GENNightStart,sSetting_Values[_GEN_NIGHT_BEGIN].defaultVal);
    Default(sSetting_Values[_GEN_NIGHT_END].addrEEPROM,&sMenu_Variable.u16GENNightEnd,sSetting_Values[_GEN_NIGHT_END].defaultVal);
    Default(sSetting_Values[_DC_LOW_INPUT].addrEEPROM,&sMenu_Variable.u16GENDCLowInput,sSetting_Values[_DC_LOW_INPUT].defaultVal);
    Default(sSetting_Values[_DC_LOW_VOLT].addrEEPROM,&sMenu_Variable.u16GENDCLowVolt,sSetting_Values[_DC_LOW_VOLT].defaultVal);
    Default(sSetting_Values[_AIRCON_TEMP1].addrEEPROM,&sMenu_Variable.u16AirConTemp[0],sSetting_Values[_AIRCON_TEMP1].defaultVal);
    Default(sSetting_Values[_AIRCON_TEMP2].addrEEPROM,&sMenu_Variable.u16AirConTemp[1],sSetting_Values[_AIRCON_TEMP2].defaultVal);
    Default(sSetting_Values[_AIRCON_TEMP3].addrEEPROM,&sMenu_Variable.u16AirConTemp[2],sSetting_Values[_AIRCON_TEMP3].defaultVal);
    Default(sSetting_Values[_AIRCON_TEMP4].addrEEPROM,&sMenu_Variable.u16AirConTemp[3],sSetting_Values[_AIRCON_TEMP4].defaultVal);
    Default(sSetting_Values[_AIRCON_TIME1].addrEEPROM,&sMenu_Variable.u16AirConTime1,sSetting_Values[_AIRCON_TIME1].defaultVal);
    Default(sSetting_Values[_AIRCON_TIME2].addrEEPROM,&sMenu_Variable.u16AirConTime2,sSetting_Values[_AIRCON_TIME2].defaultVal);
}

void Default(uint16_t MemoryAdress,uint16_t *Value,uint16_t default_value)
{
    *Value = default_value;
    WriteEEPROM_Word(MemoryAdress,*Value);
}

void ReadMemory(uint16_t Index,uint16_t *Value)
{
    uint16_t uiValue16=0;
    uiValue16 = ReadEEPROM_Word(sSetting_Values[Index].addrEEPROM);
    if(uiValue16>=sSetting_Values[Index].lowerVal && uiValue16<=sSetting_Values[Index].upperVal) 
        *Value = uiValue16;
    else 
    {
        *Value = sSetting_Values[Index].defaultVal;
        WriteEEPROM_Word(sSetting_Values[Index].addrEEPROM,*Value);
    }
}

void Init_All_Variable (void)
{
    uint8_t i=0;
    uint16_t defaultWrite;
    // chaunm - initialize all setting
    memset(deviceName, 0, sizeof(deviceName));
    ReadMemory(_DEFAULT_WRITE, &defaultWrite);
    if (defaultWrite != EEPROM_WRITE_FLAG)
    {
        Write_Default_All_Variable();
        WriteEEPROM_Word(sSetting_Values[_DEFAULT_WRITE].addrEEPROM, EEPROM_WRITE_FLAG);
        // init device name
        deviceNameLength = 7;
        WriteEEPROM_Word(sSetting_Values[_DEV_NAME_LENGTH].addrEEPROM, 7);
        memcpy(deviceName, "BOX0001", 7);
        for (i = 0; i < 7; i++)
        {
            WriteEEPROM_Byte(DEVICE_NAME_EEPROM_ADDR + i, deviceName[i]);
        }
    }
    else
    {
        ReadMemory(_DEV_NAME_LENGTH, &deviceNameLength);
        for (i = 0; i < deviceNameLength; i++)
        {
            deviceName[i] = ReadEEPROM_Byte(DEVICE_NAME_EEPROM_ADDR + i);
        }
    }
    ReadMemory(_AC_LOW,&sMenu_Variable.u16AcThresVolt[0]);
    ReadMemory(_DC_LOW,&sMenu_Variable.u16BattThresVolt[0]);
    ReadMemory(_TEMP1,&sMenu_Variable.u16ThresTemp[0]);
    ReadMemory(_TEMP2,&sMenu_Variable.u16ThresTemp[1]);
    ReadMemory(_TEMP3,&sMenu_Variable.u16ThresTemp[2]);
    ReadMemory(_TEMP4,&sMenu_Variable.u16ThresTemp[3]);
    ReadMemory(_DEV_IP1,&sMenu_Variable.sEthernetSetting.u16DevIP[0]);
    ReadMemory(_DEV_IP2,&sMenu_Variable.sEthernetSetting.u16DevIP[1]);
    ReadMemory(_DEV_IP3,&sMenu_Variable.sEthernetSetting.u16DevIP[2]);
    ReadMemory(_DEV_IP4,&sMenu_Variable.sEthernetSetting.u16DevIP[3]);
    ReadMemory(_DEV_SUBNET1,&sMenu_Variable.sEthernetSetting.u16DevSubnet[0]);
    ReadMemory(_DEV_SUBNET2,&sMenu_Variable.sEthernetSetting.u16DevSubnet[1]);
    ReadMemory(_DEV_SUBNET3,&sMenu_Variable.sEthernetSetting.u16DevSubnet[2]);
    ReadMemory(_DEV_SUBNET4,&sMenu_Variable.sEthernetSetting.u16DevSubnet[3]);
    ReadMemory(_DEV_GATEW1,&sMenu_Variable.sEthernetSetting.u16DevGateway[0]);
    ReadMemory(_DEV_GATEW2,&sMenu_Variable.sEthernetSetting.u16DevGateway[1]);
    ReadMemory(_DEV_GATEW3,&sMenu_Variable.sEthernetSetting.u16DevGateway[2]);
    ReadMemory(_DEV_GATEW4,&sMenu_Variable.sEthernetSetting.u16DevGateway[3]);
    ReadMemory(_DEV_PORT,&sMenu_Variable.sEthernetSetting.u16DevPort);
    ReadMemory(_SERVER_IP1,&sMenu_Variable.u16ServerIP[0]);
    ReadMemory(_SERVER_IP2,&sMenu_Variable.u16ServerIP[1]);
    ReadMemory(_SERVER_IP3,&sMenu_Variable.u16ServerIP[2]);
    ReadMemory(_SERVER_IP4,&sMenu_Variable.u16ServerIP[3]);
    ReadMemory(_SERVER_PORT,&sMenu_Variable.u16ServerPort);
    ReadMemory(_GEN_MAX_RUNTIME,&sMenu_Variable.u16GENMaxRuntime);
    ReadMemory(_GEN_UNDER_VOLT,&sMenu_Variable.u16GENUnderVolt);
    ReadMemory(_GEN_ERROR_RESET_EN,&sMenu_Variable.u16GENErrorResetEnable);
    ReadMemory(_GEN_ERROR_RESET_MIN,&sMenu_Variable.u16GENErrorResetTime);	
    ReadMemory(_GEN_WARM_UP_TIME,&sMenu_Variable.u16GENWarmUpTime);
    ReadMemory(_GEN_COOL_DOWN_TIME,&sMenu_Variable.u16GENCoolDownTime);
    ReadMemory(_GEN_NIGHT_EN,&sMenu_Variable.u16GENNightEnable);	
    ReadMemory(_GEN_NIGHT_BEGIN,&sMenu_Variable.u16GENNightStart);
    ReadMemory(_GEN_NIGHT_END,&sMenu_Variable.u16GENNightEnd);
    ReadMemory(_DC_LOW_INPUT,&sMenu_Variable.u16GENDCLowInput);
    ReadMemory(_DC_LOW_VOLT,&sMenu_Variable.u16GENDCLowVolt);  
    ReadMemory(_AIRCON_TEMP1,&sMenu_Variable.u16AirConTemp[0]); 
    ReadMemory(_AIRCON_TEMP2,&sMenu_Variable.u16AirConTemp[1]); 
    ReadMemory(_AIRCON_TEMP3,&sMenu_Variable.u16AirConTemp[2]); 
    ReadMemory(_AIRCON_TEMP4,&sMenu_Variable.u16AirConTemp[3]); 
    ReadMemory(_AIRCON_TIME1,&sMenu_Variable.u16AirConTime1); 
    ReadMemory(_AIRCON_TIME2,&sMenu_Variable.u16AirConTime2); 
#if (USERDEF_CHAUNM_TEST == ENABLED)
    sMenu_Variable.sEthernetSetting.u16DevIP[0] = 192;
    sMenu_Variable.sEthernetSetting.u16DevIP[1] = 168;
    sMenu_Variable.sEthernetSetting.u16DevIP[2] = 100;
    sMenu_Variable.sEthernetSetting.u16DevIP[3] = 247;	
    
    sMenu_Variable.sEthernetSetting.u16DevGateway[0] = 192;
    sMenu_Variable.sEthernetSetting.u16DevGateway[1] = 168;
    sMenu_Variable.sEthernetSetting.u16DevGateway[2] = 100;
    sMenu_Variable.sEthernetSetting.u16DevGateway[3] = 1;
    
    sMenu_Variable.sEthernetSetting.u16DevSubnet[0] = 255;
    sMenu_Variable.sEthernetSetting.u16DevSubnet[1] = 255;
    sMenu_Variable.sEthernetSetting.u16DevSubnet[2] = 255;
    sMenu_Variable.sEthernetSetting.u16DevSubnet[3] = 0;
    
    sMenu_Variable.u16ServerIP[0] = 192;
    sMenu_Variable.u16ServerIP[1] = 168;
    sMenu_Variable.u16ServerIP[2] = 100;
    sMenu_Variable.u16ServerIP[3] = 29;
    sMenu_Variable.u16ServerPort = 162;
#endif // #if (USERDEF_CHAUNM_TEST == ENABLED)
    sprintf((char_t *)sMenu_Variable.ucIP,"%d.%d.%d.%d",
            sMenu_Variable.sEthernetSetting.u16DevIP[0],
            sMenu_Variable.sEthernetSetting.u16DevIP[1],
            sMenu_Variable.sEthernetSetting.u16DevIP[2],
            sMenu_Variable.sEthernetSetting.u16DevIP[3]);
    
    sprintf((char_t *)sMenu_Variable.ucGW,"%d.%d.%d.%d",
            sMenu_Variable.sEthernetSetting.u16DevGateway[0],
            sMenu_Variable.sEthernetSetting.u16DevGateway[1],
            sMenu_Variable.sEthernetSetting.u16DevGateway[2],
            sMenu_Variable.sEthernetSetting.u16DevGateway[3]);
    
    sprintf((char_t *)sMenu_Variable.ucSN,"%d.%d.%d.%d",
            sMenu_Variable.sEthernetSetting.u16DevSubnet[0],
            sMenu_Variable.sEthernetSetting.u16DevSubnet[1],
            sMenu_Variable.sEthernetSetting.u16DevSubnet[2],
            sMenu_Variable.sEthernetSetting.u16DevSubnet[3]);
    
    sprintf((char_t *)sMenu_Variable.ucSIP,"%d.%d.%d.%d",
            sMenu_Variable.u16ServerIP[0],
            sMenu_Variable.u16ServerIP[1],
            sMenu_Variable.u16ServerIP[2],
            sMenu_Variable.u16ServerIP[3]);
    
    //User ID init
    sMenu_Variable.u8UserIDAddr[0] = USER1_ADDR;
    sMenu_Variable.u8UserIDAddr[1] = USER2_ADDR;
    sMenu_Variable.u8UserIDAddr[2] = USER3_ADDR;
    sMenu_Variable.u8UserIDAddr[3] = USER4_ADDR;
    sMenu_Variable.u8UserIDAddr[4] = USER5_ADDR;  
    
    
    for(i=0;i<8;i++)
        sMenu_Variable.u8UserID[0][i] = ReadEEPROM_Byte (USER1_ADDR+i);
    for(i=0;i<8;i++)
        sMenu_Variable.u8UserID[1][i] = ReadEEPROM_Byte (USER2_ADDR+i);
    for(i=0;i<8;i++)
        sMenu_Variable.u8UserID[2][i] = ReadEEPROM_Byte (USER3_ADDR+i);
    for(i=0;i<8;i++)
        sMenu_Variable.u8UserID[3][i] = ReadEEPROM_Byte (USER4_ADDR+i);
    for(i=0;i<8;i++)
        sMenu_Variable.u8UserID[4][i] = ReadEEPROM_Byte (USER5_ADDR+i);
    
}

void MenuGetDeviceIpv4(uint32_t* ipAddr)
{
    (*ipAddr) = (((sMenu_Variable.sEthernetSetting.u16DevIP[0] & 0x00FF) << 24) |
                 ((sMenu_Variable.sEthernetSetting.u16DevIP[1] & 0x00FF) << 16) |
                     ((sMenu_Variable.sEthernetSetting.u16DevIP[2] & 0x00FF) << 8)  |
                         ((sMenu_Variable.sEthernetSetting.u16DevIP[3] & 0x00FF)));
}