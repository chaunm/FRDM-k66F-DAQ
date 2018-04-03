/*
 * eeprom_rtc.h
 *
 *  Created on: Aug 9, 2016
 *      Author: luanlt
 */
//#include "board.h"
//#include "pin_mux.h"
#include "fsl_gpio.h"
#include "variables.h"

#define CORE_CLOCK_1S   120000000
#define CORE_CLOCK_1ms  120000
#define CORE_CLOCK_1us  120

#define Delay 	5
#define ms 			1

#define  SDA_PORT   GPIOD
#define  SDA_PIN    9u
#define  SCL_PORT   GPIOD
#define  SCL_PIN    8u

#define SDA_ON 	GPIO_SetPinsOutput(SDA_PORT,1<<SDA_PIN)
#define SDA_OFF	GPIO_ClearPinsOutput(SDA_PORT,1<<SDA_PIN)
#define SCL_ON	   GPIO_SetPinsOutput(SCL_PORT,1<<SCL_PIN)
#define SCL_OFF	GPIO_ClearPinsOutput(SCL_PORT,1<<SCL_PIN)

#define SDA_DATA_READ   GPIO_ReadPinInput(SDA_PORT,SDA_PIN)

#define EEPROM_ADDRESS 0xA0 //DIA CHI EEPROM
#define ACK			1
#define NO_ACK		0
#define SLAVE		0xD0
#define WRITE   	0x00
#define READ    	0x01
#define ERR_ACK 	0x01

//typedef struct TimeFormat
//{
//	unsigned char  hour;
//	unsigned char  min;
//	unsigned char  sec;
//	unsigned char  day;
//	unsigned char  date;
//	unsigned char  month;
//	unsigned char  year;
//}TimeFormat;
//
//extern TimeFormat GTime;
//extern TimeFormat STime;

uint8_t dec2bcd(uint8_t val);
uint8_t bcd2dec(uint8_t val);

void Init_I2CE(void);
void I2C_Init(void);
void Start(void);
void Stop(void);
void WriteI2C(uint8_t Data);
uint8_t ReadI2C(uint8_t ACK_Bit);

void WRITE_RTC(uint8_t Addr,uint8_t Data);
uint8_t READ_RTC(uint8_t Addr);
//void SetTime(TimeFormat Time);
//TimeFormat GetTime(void);

void WriteEEPROM_Byte(uint16_t uiAdrress,uint8_t ucData);
uint8_t ReadEEPROM_Byte(uint16_t uiAdrress);
void WriteEEPROM_Word(uint16_t uiAdrress,uint16_t uiData);
uint16_t ReadEEPROM_Word(uint16_t uiAdrress);
void WriteEEPROM_long(uint16_t uiAdrress,uint32_t uiData);
uint32_t ReadEEPROM_long(uint32_t uiAdrress);
void WriteEEPROMu32(uint16_t uiAdrress,uint32_t uiData);
uint32_t ReadEEPROMu32(uint16_t uiAdrress);

