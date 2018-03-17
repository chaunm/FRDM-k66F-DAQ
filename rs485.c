#include "board.h"
#include "fsl_gpio.h"
#include "fsl_uart.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "rs485.h"
#include "menu.h"
#include "eeprom_rtc.h"
#include "variables.h"

static const unsigned char CRCHighTable[] = {
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40 };

static const unsigned char CRCLowTable[] = {
    0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04,
    0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09, 0x08, 0xC8,
    0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC,
    0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3, 0x11, 0xD1, 0xD0, 0x10,
    0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,
    0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38,
    0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED, 0xEC, 0x2C,
    0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26, 0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0,
    0xA0, 0x60, 0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4,
    0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,
    0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C,
    0xB4, 0x74, 0x75, 0xB5, 0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0,
    0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54,
    0x9C, 0x5C, 0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98,
    0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
    0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83, 0x41, 0x81, 0x80, 0x40
};

sMODBUSRTU_struct Modbus;
sMODBUSRTU_struct DoorAccess;

uart_config_t config;

void Init_RS485_UART (void)
{
    UART_GetDefaultConfig(&config);
    config.baudRate_Bps = RS4851_UART_BAUDRATE;
    config.enableTx = true;
    config.enableRx = true;
    
    UART_Init(RS4851_UART, &config, CLOCK_GetFreq(RS4851_UART_CLKSRC));
    /* Enable RX interrupt. */
    UART_EnableInterrupts(RS4851_UART, kUART_RxDataRegFullInterruptEnable | kUART_RxOverrunInterruptEnable);
    EnableIRQ(RS4851_UART_IRQn);
    
    //#if (USERDEF_DEBUG_USING == ENABLED)  
    //#elif (USERDEF_DEBUG_USING == DISABLED)
    // CanhLT - 23/12
#ifndef DEBUG_CONSOLE_UART4  
    UART_GetDefaultConfig(&config);
    config.baudRate_Bps = RS4852_UART_BAUDRATE;
    config.enableTx = true;
    config.enableRx = true;
    
    UART_Init(RS4852_UART, &config, CLOCK_GetFreq(RS4852_UART_CLKSRC));
    /* Enable RX interrupt. */
    UART_EnableInterrupts(RS4852_UART, kUART_RxDataRegFullInterruptEnable | kUART_RxOverrunInterruptEnable);
    EnableIRQ(RS4852_UART_IRQn);
#endif
}

void RS4851_UART_IRQHandler(void)
{
    uint8_t ucChar;
    
    if ((kUART_RxDataRegFullFlag | kUART_RxOverrunFlag) & UART_GetStatusFlags(RS4851_UART))
    {
        ucChar = UART_ReadByte(RS4851_UART);
        
        if(Modbus.u8MosbusEn==1)
        {
            Modbus.u8BuffRead[Modbus.u8DataPointer++] = ucChar;
            if(Modbus.u8DataPointer == Modbus.u8ByteCount){
                Modbus.u8MosbusEn = 2;
            }
        }
        
        if((Modbus.u8BuffRead[1] >= 1)&&(Modbus.u8BuffRead[1] <= 50)) //Kiem tra Function Code
        {
            switch(Modbus.u8BuffRead[1]){
            case _READ_HOLDING_REGS:
                Modbus.u8ByteCount = Modbus.u8BuffRead[2] + 5;
                break;
            case _PRESET_SINGLE_REG:
                Modbus.u8ByteCount = 8;
                break;
            case _PRESET_MULTIPLE_REGS:
                
                break;
            case _SETUP_REGISTER:
                
                break;
            case _USER_REGISTER:
                Modbus.u8ByteCount = 8;
                break;
            default:
                Modbus.u8ByteCount = 8;
                break;
            }
        }
        else
        {
            Modbus.u8MosbusEn = 0;
            Modbus.u8DataPointer = 0;
        }
        
        if(ucChar==Modbus.u8SlaveID && Modbus.u8MosbusEn == 0){
            Modbus.u8MosbusEn = 1;
            Modbus.u8DataPointer = 0;
            Modbus.u8BuffRead[Modbus.u8DataPointer++] = ucChar;
        }
    }
}

void RS4852_UART_IRQHandler(void)
{
    uint8_t ucChar;
    
    if ((kUART_RxDataRegFullFlag | kUART_RxOverrunFlag) & UART_GetStatusFlags(RS4852_UART))
    {
        ucChar = UART_ReadByte(RS4852_UART);
        
        if(DoorAccess.u8MosbusEn == 0)
        {
            DoorAccess.u8BuffRead[DoorAccess.u8ByteCount++] = ucChar;
            if(DoorAccess.u8ByteCount == 8)
                DoorAccess.u8MosbusEn = 2;
        }  
    }
}

void Door_Access_Check (void)
{
    uint8_t   i=0;
    //  uint8_t   mTempBuff[8];
    
    if(DoorAccess.u8MosbusEn == 2)
    {
        if(sMenu_Control.learnUID)
        {
            //Save user ID
            for(i=0;i<8;i++)
                TempUserID[sMenu_Control.index][i] = DoorAccess.u8BuffRead[i];
            sMenu_Control.refesh = 1;
        }
        else
        {
            //Check user in system memory -> open door
            for(i=0;i<8;i++)
            {
                //        mTempBuff[i] = DoorAccess.u8BuffRead[i];
                AccessIdTemp[i] = DoorAccess.u8BuffRead[i];
                //        privateMibBase.siteInfoGroup.siteInfoAccessId[i] = mTempBuff[i];
                //        privateMibBase.siteInfoGroup.siteInfoAccessIdLen = 8;
            }
            //      sMenu_Control.accessUID = Find_UserID(mTempBuff);
            sMenu_Control.accessUID = Find_UserID(AccessIdTemp);
        }
        DoorAccess.u8MosbusEn = 0;
        DoorAccess.u8ByteCount = 0;
    }
}

int8_t Find_UserID (uint8_t * dataPointer)
{
    uint8_t i=0,j=0,k=0;
    
    for(i=0;i<5;i++)
    {
        for(j=0;j<8;j++)
            if(*(dataPointer+j) != sMenu_Variable.u8UserID[i][j]){
                k++;
                break;
            }
        
        if(k==0)
            return (i+1);
        else
            k=0;
    }
    return -1;
}

int8_t RS4851_Check_Respond_Data (void)
{
    uint16_t	mTemp = 0;
    
    if(Modbus.u8MosbusEn==2)
    {
        Modbus.u8CRCHighRead = Modbus.u8BuffRead[Modbus.u8ByteCount-2];
        Modbus.u8CRCLowRead  = Modbus.u8BuffRead[Modbus.u8ByteCount-1];
        
        ModbusCRC(&Modbus.u8BuffRead[0],Modbus.u8ByteCount-2);
        
        if(Modbus.u8CRCHigh == Modbus.u8CRCHighRead && Modbus.u8CRCLow==Modbus.u8CRCLowRead)
        {
            Modbus.u8FunctionCode 	= Modbus.u8BuffRead[1];
            
            switch(Modbus.u8FunctionCode)
            {
            case _READ_HOLDING_REGS:
                mTemp = (Modbus.u8NumberRegHigh<<8)|(Modbus.u8NumberRegLow); 
                if (Modbus.u8BuffRead[2] == (mTemp*2))
                {
                    switch(Modbus.u8SlaveID)
                    {
                    case 1:
                        Extract_Holding_Regs_Data_ATS();
                        break;
                    case 2:
                        Extract_Holding_Regs_Data_AIRCON();
                        break;
                    case 3:
                        Extract_Holding_Regs_Data_DOOR();
                        break;
                    default:
                        break;
                    }
                }
                else
                {
                    Modbus.u8MosbusEn = 0;
                    return -3;
                }
                break;
            case _PRESET_SINGLE_REG:
                
                break;
            case _USER_REGISTER:
                
                break;
            default:
                break;
            }
            Modbus.u8MosbusEn = 0;		
        }
        else
        {
            Modbus.u8MosbusEn = 0;
            return -2;
        }
    }
    else
    {
        Modbus.u8MosbusEn = 0;
        return -1;
    }
    
    return 1;
}

void Extract_Holding_Regs_Data_ATS (void)
{
    sATS_Variable.battVolt = (Modbus.u8BuffRead[3]<<8)|Modbus.u8BuffRead[4];
    sATS_Variable.grid_Volt = (Modbus.u8BuffRead[5]<<8)|Modbus.u8BuffRead[6];
    sATS_Variable.genVolt = (Modbus.u8BuffRead[7]<<8)|Modbus.u8BuffRead[8];
    
    sATS_Variable.grid_VoltCheckCnt = Modbus.u8BuffRead[12];
    sATS_Variable.grid_Status = Modbus.u8BuffRead[14];
    sATS_Variable.activePower = Modbus.u8BuffRead[16];
    sATS_Variable.batt_DCLow = Modbus.u8BuffRead[18];
    
    sATS_Variable.switch_Grid_ON = Modbus.u8BuffRead[20];
    sATS_Variable.switch_GEN_ON = Modbus.u8BuffRead[22];
    sATS_Variable.Gen1Voltage = Modbus.u8BuffRead[24];
    
    sATS_Variable.GenEnable = Modbus.u8BuffRead[26];
    sATS_Variable.GenStart = Modbus.u8BuffRead[28];
    sATS_Variable.Gen1Fail = Modbus.u8BuffRead[30];
    sATS_Variable.Gen1ControlStatus = Modbus.u8BuffRead[32];
    sATS_Variable.StartStep = Modbus.u8BuffRead[34];
    sATS_Variable.StartStep_cnt = Modbus.u8BuffRead[36];
    sATS_Variable.StopStep = Modbus.u8BuffRead[38];
    sATS_Variable.StopStep_cnt = Modbus.u8BuffRead[40];
    sATS_Variable.ReStart_Cnt = Modbus.u8BuffRead[42];
    
    sATS_Variable.MaxRunTimeCnt = (Modbus.u8BuffRead[43]<<24)|(Modbus.u8BuffRead[44]<<16)|(Modbus.u8BuffRead[45]<<8)|(Modbus.u8BuffRead[46]<<0);
    sATS_Variable.vRMS_u32 = (Modbus.u8BuffRead[47]<<24)|(Modbus.u8BuffRead[48]<<16)|(Modbus.u8BuffRead[49]<<8)|(Modbus.u8BuffRead[50]<<0);
    sATS_Variable.iRMSA_u32 = (Modbus.u8BuffRead[51]<<24)|(Modbus.u8BuffRead[52]<<16)|(Modbus.u8BuffRead[53]<<8)|(Modbus.u8BuffRead[54]<<0);
    sATS_Variable.instan_ActPwA_i32 = (Modbus.u8BuffRead[55]<<24)|(Modbus.u8BuffRead[56]<<16)|(Modbus.u8BuffRead[57]<<8)|(Modbus.u8BuffRead[58]<<0);
    sATS_Variable.power_Factor_i16 = (Modbus.u8BuffRead[59]<<8)|(Modbus.u8BuffRead[60]<<0);
    sATS_Variable.frequency_i16 = (Modbus.u8BuffRead[61]<<8)|(Modbus.u8BuffRead[62]<<0);
    
    sATS_Variable.year = Modbus.u8BuffRead[63];
    sATS_Variable.month = Modbus.u8BuffRead[64];
    sATS_Variable.date = Modbus.u8BuffRead[65];
    sATS_Variable.hour = Modbus.u8BuffRead[66];
    sATS_Variable.mins = Modbus.u8BuffRead[67];
    sATS_Variable.secs = Modbus.u8BuffRead[68];
}

void Extract_Holding_Regs_Data_AIRCON (void)
{
    sAirCon_Variable.indorTemp = (Modbus.u8BuffRead[3]<<8)|Modbus.u8BuffRead[4];
    sAirCon_Variable.outdoorTemp = (Modbus.u8BuffRead[5]<<8)|Modbus.u8BuffRead[6];
    sAirCon_Variable.airCon1Runtime = (Modbus.u8BuffRead[7]<<24)|(Modbus.u8BuffRead[8]<<16)|(Modbus.u8BuffRead[9]<<8)|(Modbus.u8BuffRead[10]<<0);
    sAirCon_Variable.airCon2Runtime = (Modbus.u8BuffRead[11]<<24)|(Modbus.u8BuffRead[12]<<16)|(Modbus.u8BuffRead[13]<<8)|(Modbus.u8BuffRead[14]<<0);
    sAirCon_Variable.airCon1Status = Modbus.u8BuffRead[16];
    sAirCon_Variable.airCon2Status = Modbus.u8BuffRead[18];
    sAirCon_Variable.fanStatus = Modbus.u8BuffRead[20];
    sAirCon_Variable.year = Modbus.u8BuffRead[21];
    sAirCon_Variable.month = Modbus.u8BuffRead[22];
    sAirCon_Variable.date = Modbus.u8BuffRead[23];
    sAirCon_Variable.hour = Modbus.u8BuffRead[24];
    sAirCon_Variable.mins = Modbus.u8BuffRead[25];
    sAirCon_Variable.secs = Modbus.u8BuffRead[26];
}

void Extract_Holding_Regs_Data_DOOR (void)
{
    
}

void Read_Holding_Regs_Query (uint8_t slaveAddr, uint16_t startingAddr, uint16_t noPoint)
{
    Modbus.u8SlaveID = slaveAddr;
    Modbus.u8FunctionCode = 0x03;
    Modbus.u8StartHigh = (uint8_t)(startingAddr>>8);
    Modbus.u8StartLow = (uint8_t)(startingAddr);
    Modbus.u8NumberRegHigh = (uint8_t)(noPoint>>8);
    Modbus.u8NumberRegLow = (uint8_t)(noPoint);
    
    Modbus.u8BuffWrite[0] = Modbus.u8SlaveID;
    Modbus.u8BuffWrite[1] = Modbus.u8FunctionCode;
    Modbus.u8BuffWrite[2] = Modbus.u8StartHigh;
    Modbus.u8BuffWrite[3] = Modbus.u8StartLow;
    Modbus.u8BuffWrite[4] = Modbus.u8NumberRegHigh;
    Modbus.u8BuffWrite[5] = Modbus.u8NumberRegLow;
    
    ModbusCRC(&Modbus.u8BuffWrite[0],6);
    
    Modbus.u8BuffWrite[6] = Modbus.u8CRCHigh;
    Modbus.u8BuffWrite[7] = Modbus.u8CRCLow;
    
    UART_WriteBlocking(RS4851_UART, Modbus.u8BuffWrite, 8);
}

void Write_Single_Reg (uint8_t slaveAddr, uint16_t regAddr, uint16_t writeVal)
{
    Modbus.u8SlaveID = slaveAddr;
    Modbus.u8FunctionCode = 0x06;
    Modbus.u8StartHigh = (uint8_t)(regAddr>>8);
    Modbus.u8StartLow = (uint8_t)(regAddr);
    Modbus.u8NumberRegHigh = (uint8_t)(writeVal>>8);
    Modbus.u8NumberRegLow = (uint8_t)(writeVal);
    
    Modbus.u8BuffWrite[0] = Modbus.u8SlaveID;
    Modbus.u8BuffWrite[1] = Modbus.u8FunctionCode;
    Modbus.u8BuffWrite[2] = Modbus.u8StartHigh;
    Modbus.u8BuffWrite[3] = Modbus.u8StartLow;
    Modbus.u8BuffWrite[4] = Modbus.u8NumberRegHigh;
    Modbus.u8BuffWrite[5] = Modbus.u8NumberRegLow;
    
    ModbusCRC(&Modbus.u8BuffWrite[0],6);
    
    Modbus.u8BuffWrite[6] = Modbus.u8CRCHigh;
    Modbus.u8BuffWrite[7] = Modbus.u8CRCLow;
    
    UART_WriteBlocking(RS4851_UART, Modbus.u8BuffWrite, 8);  
}

void Write_Time_Reg (uint8_t slaveAddr, uint16_t regAddr, uint16_t writeVal)
{
    Modbus.u8SlaveID = slaveAddr;
    Modbus.u8FunctionCode = 50;
    Modbus.u8StartHigh = (uint8_t)(regAddr>>8);
    Modbus.u8StartLow = (uint8_t)(regAddr);
    
    Modbus.u8BuffWrite[0] = Modbus.u8SlaveID;
    Modbus.u8BuffWrite[1] = Modbus.u8FunctionCode;
    Modbus.u8BuffWrite[2] = Modbus.u8StartHigh;
    Modbus.u8BuffWrite[3] = Modbus.u8StartLow;
    Modbus.u8BuffWrite[4] = GTime.date;
    Modbus.u8BuffWrite[5] = GTime.month;
    Modbus.u8BuffWrite[6] = GTime.year;
    Modbus.u8BuffWrite[7] = GTime.hour;
    Modbus.u8BuffWrite[8] = GTime.min;
    Modbus.u8BuffWrite[9] = GTime.sec;
    
    ModbusCRC(&Modbus.u8BuffWrite[0],20);
    
    Modbus.u8BuffWrite[20] = Modbus.u8CRCHigh;
    Modbus.u8BuffWrite[21] = Modbus.u8CRCLow;
    
    UART_WriteBlocking(RS4851_UART, Modbus.u8BuffWrite, 22);
}

void ModbusCRC(unsigned char *str, unsigned char len)
{
    unsigned char temp;
    Modbus.u8CRCHigh = 0xFF; Modbus.u8CRCLow = 0xFF;
    
    while( len--)
    {
        temp = *str++ ^ Modbus.u8CRCHigh;
        Modbus.u8CRCHigh = CRCHighTable[temp] ^ Modbus.u8CRCLow;
        Modbus.u8CRCLow  = CRCLowTable[temp];
    }
}