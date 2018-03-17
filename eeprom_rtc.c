/*
 * eeprom_rtc.c
 *
 *  Created on: Aug 9, 2016
 *      Author: luanlt
 */

#include "board.h"
#include "pin_mux.h"
#include "fsl_gpio.h"
#include "eeprom_rtc.h"
#include "string.h"
#include "FreeRTOS.h"
#include "task.h"

/* Define the init structure */
gpio_pin_config_t SDA_out_config = {
	 kGPIO_DigitalOutput, 0,
};
gpio_pin_config_t SDA_in_config = {
	 kGPIO_DigitalInput, 0,
};

TimeFormat GTime;
TimeFormat STime;

uint8_t dec2bcd(uint8_t val){
  return ((val/0xA*0x10)+(val%0xA));
}
// convert BCD to binary
uint8_t bcd2dec(uint8_t val){
  return ((val/0x10*0xA)+(val%0x10));
}

void SDA_Output(void)
{
  GPIO_PinInit(SDA_PORT, SDA_PIN, &SDA_out_config);
}
void SDA_Input(void)
{
  GPIO_PinInit(SDA_PORT, SDA_PIN, &SDA_in_config);
}

void Delay_us(uint32_t us){
  uint32_t nCount;
  nCount = us*CORE_CLOCK_1us;
  while(nCount--);
}

void Init_I2CE(void){
  SDA_Output();
  SDA_ON;
  Delay_us(Delay);
  SCL_ON;
  WRITE_RTC(0x08,0x20);
  WRITE_RTC(0x09,0x45);
  
  GTime=GetTime();
  if((GTime.min>59) || (GTime.hour>23) || (GTime.year>99)  || (GTime.month>12) || (GTime.date>31)) {
		GTime.year = 16;  STime.year=GTime.year;
      GTime.month = 11; STime.month=GTime.month;
		GTime.date = 22;  STime.date=GTime.date;
		GTime.hour = 23;  STime.hour=GTime.hour;
		GTime.min = 59;   STime.min=GTime.min;
      GTime.sec = 00;   STime.sec=GTime.sec;
		SetTime(STime);
  }
}

void Start(void)
{
  SDA_Output();
  Delay_us(Delay);
  SDA_ON;
  Delay_us(Delay);
  SCL_ON;
  Delay_us(Delay);
  SDA_OFF;
  Delay_us(Delay);
  SCL_OFF;
  Delay_us(Delay);
}

void Stop(void)
{
  SDA_Output();
  Delay_us(Delay);
  SDA_OFF;
  Delay_us(Delay);
  SCL_ON;
  Delay_us(Delay);
  SDA_ON;
  Delay_us(Delay);
}

void WriteI2C(unsigned char Data)
{
  uint8_t i; 
  SDA_Output();
  Delay_us(Delay);
  for (i=0;i<8;i++){
    if((Data & 0x80)==0)
    SDA_OFF;
    else
    SDA_ON;
    SCL_ON;
    Delay_us(Delay);
    SCL_OFF;
    Data<<=1;
    Delay_us(Delay);
  }
  SDA_OFF;
  SCL_ON; 
  Delay_us(Delay);
  SCL_OFF;
  Delay_us(Delay);
}

unsigned char ReadI2C(unsigned char ACK_Bit)
{
  uint8_t i;
  uint8_t Data=0;
  SDA_Output();
  Delay_us(Delay);
  SDA_ON;
  
  Delay_us(ms);
  
  SDA_Input();
  Delay_us(Delay);
  for (i=0;i<8;i++)
  {
    SCL_ON;
    Delay_us(Delay);
    Data<<= 1;
    if(SDA_DATA_READ==0)
    Data  = (Data | 0);
    else
    Data  = (Data | 1);
    SCL_OFF;
    Delay_us(Delay);
  }
  SDA_Output();
  if (ACK_Bit == 1)
  SDA_OFF; // Send ACK
  else
  SDA_ON; // Send NO ACK
  SCL_ON;
  Delay_us(Delay);
  SCL_OFF;
  Delay_us(Delay);
  return Data;
}

//================================ RTC ================================================//
//Giao tiep voi RTC BQ32000

uint8_t READ_RTC(uint8_t Addr){
	uint8_t Data = 0;

	Start();
	WriteI2C(0xD0);
	WriteI2C(Addr);
	Start();
	WriteI2C(0xD1);
	Data = ReadI2C(NO_ACK);
	Stop();
	return(Data);
}

void WRITE_RTC(uint8_t Addr,uint8_t Data){
	Start();
	WriteI2C(0xD0);
	WriteI2C(Addr);
	WriteI2C(Data);
	Stop();
}

void SetTime(TimeFormat Time){
	WRITE_RTC(0,dec2bcd(Time.sec)&0x7f);	//Seconds
	WRITE_RTC(1,dec2bcd(Time.min)&0x7f);	//Minutes
	WRITE_RTC(2,dec2bcd(Time.hour)&0x3f);	//Hours
	WRITE_RTC(3,dec2bcd(Time.day));				//Day
	WRITE_RTC(4,dec2bcd(Time.date));			//Date
	WRITE_RTC(5,dec2bcd(Time.month));			//Month
	WRITE_RTC(6,dec2bcd(Time.year));			//Year
}

TimeFormat GetTime(void){
	TimeFormat Time;

	Time.sec=READ_RTC(0);   // read second
	Time.min=READ_RTC(1);   // read minute
	Time.hour=READ_RTC(2);  // read hour
	Time.day=READ_RTC(3);   // read day
	Time.date=READ_RTC(4);  // read date
	Time.month=READ_RTC(5); // read month
	Time.year=READ_RTC(6);  // read year

	Time.sec 	= bcd2dec(Time.sec&0x7f);
	Time.min 	= bcd2dec(Time.min&0x7f);
	Time.hour 	= bcd2dec(Time.hour&0x3f);
	Time.day 	= bcd2dec(Time.day);
	Time.date 	= bcd2dec(Time.date);
	Time.month 	= bcd2dec(Time.month);
	Time.year 	= bcd2dec(Time.year);

	return Time;
}
//Giao tiep voi RTC BQ32000
//================================ RTC ================================================//

//================================ EEPROM ================================================//
void WriteEEPROM_Byte(uint16_t uiAdrress,uint8_t ucData){

	unsigned char High_Adrr;
	unsigned char Low_Adrr;
	
	vTaskDelay(10);	//Waiting time for write data :10ms
	Low_Adrr  =(unsigned char) uiAdrress;
	High_Adrr =(unsigned char) (uiAdrress>>8);

	Start();
	WriteI2C(EEPROM_ADDRESS);
	WriteI2C(High_Adrr);
	WriteI2C(Low_Adrr);
	WriteI2C(ucData);
	Stop();

	vTaskDelay(10);	//Waiting time for write data :10ms
}

uint8_t ReadEEPROM_Byte(uint16_t uiAdrress){

	unsigned char High_Adrr;
	unsigned char Low_Adrr;
	unsigned char ucData;
	
	Low_Adrr=(unsigned char) uiAdrress;
	High_Adrr=(unsigned char) (uiAdrress>>8);

	Start();
	WriteI2C(EEPROM_ADDRESS);
	WriteI2C(High_Adrr);
	WriteI2C(Low_Adrr);
	Start();

	WriteI2C(EEPROM_ADDRESS|0x01);
	ucData = ReadI2C(NO_ACK);
	Stop();
	return ucData;
}

void WriteEEPROM_Word(uint16_t uiAdrress,uint16_t uiData){
	uint16_t DataHigh = 0;
	uint16_t DataLow = 0;
	DataHigh = (uiData >> 8) & 0x00FF;
	DataLow = uiData & 0x00ff;

	unsigned char High_Adrr;
	unsigned char Low_Adrr;

	vTaskDelay(10);	//Waiting time for write data :10ms
	Low_Adrr  =(unsigned char) uiAdrress;
	High_Adrr =(unsigned char) (uiAdrress>>8);
	Start();
	WriteI2C(EEPROM_ADDRESS);
	WriteI2C(High_Adrr);
	WriteI2C(Low_Adrr);
	WriteI2C(DataHigh);
	WriteI2C(DataLow);
	Stop();
	vTaskDelay(10);	//Waiting time for write data :10ms
}

uint16_t ReadEEPROM_Word(uint16_t uiAdrress){
	uint16_t uiData;
	uint16_t DataHigh = 0;
	uint16_t DataLow = 0;
	
	unsigned char High_Adrr;
	unsigned char Low_Adrr;
	Low_Adrr=(unsigned char)  uiAdrress;
	High_Adrr=(unsigned char) (uiAdrress>>8);

	Start();
	WriteI2C(EEPROM_ADDRESS);
	WriteI2C(High_Adrr);
	WriteI2C(Low_Adrr);
	Start();

	WriteI2C(EEPROM_ADDRESS|0x01);
	DataHigh = ReadI2C(ACK);
	DataLow = ReadI2C(NO_ACK);
	Stop();

	uiData = (DataHigh<<8) + DataLow;

	return uiData;
}

void WriteEEPROM_long(uint16_t uiAdrress,uint32_t uiData){
	uint32_t temp[4] = {0};

	memcpy(&temp,&uiData,4);

	unsigned char High_Adrr;
	unsigned char Low_Adrr;

	vTaskDelay(10);	//Waiting time for write data :10ms
	Low_Adrr  =(unsigned char) uiAdrress;
	High_Adrr =(unsigned char) (uiAdrress>>8);

	Start();
	WriteI2C(EEPROM_ADDRESS);
	WriteI2C(High_Adrr);
	WriteI2C(Low_Adrr);
	WriteI2C(temp[3]);
	WriteI2C(temp[2]);
	WriteI2C(temp[1]);
	WriteI2C(temp[0]);
	Stop();

	vTaskDelay(10);	//Waiting time for write data :10ms
}

uint32_t ReadEEPROM_long(uint32_t uiAdrress){
	uint32_t uiData;
	uint32_t temp[4];
	unsigned char High_Adrr;
	unsigned char Low_Adrr;
	Low_Adrr=(unsigned char)  uiAdrress;
	High_Adrr=(unsigned char) (uiAdrress>>8);
		
	Start();
	WriteI2C(EEPROM_ADDRESS);
	WriteI2C(High_Adrr);
	WriteI2C(Low_Adrr);
	Start();
	WriteI2C(EEPROM_ADDRESS|0x01);
	temp[3] = ReadI2C(ACK);
	temp[2] = ReadI2C(ACK);
	temp[1] = ReadI2C(ACK);
	temp[0] = ReadI2C(NO_ACK);
	Stop();

	memcpy(&temp,&uiData,4);

	return uiData;
}

void WriteBlock(uint16_t u16Address, char *blocks){
	uint8_t len=0;
	len = strlen(blocks);
	uint8_t i = 0;

	unsigned char High_Adrr;
	unsigned char Low_Adrr;

	vTaskDelay(10);	//Waiting time for write data :10ms
	Low_Adrr  =(unsigned char) u16Address;
	High_Adrr =(unsigned char) (u16Address>>8);

	Start();
	WriteI2C(EEPROM_ADDRESS);
	WriteI2C(High_Adrr);
	WriteI2C(Low_Adrr);
	for(i=0;i<len;i++){
		WriteI2C(blocks[i]);
	}
	Stop();

	vTaskDelay(10);	//Waiting time for write data :10ms
}

void WriteEEPROMu32(uint16_t uiAdrress,uint32_t uiData){
	WriteEEPROM_Byte(uiAdrress,(uint8_t)		(uiData>>24));		//Write high byte
	WriteEEPROM_Byte(uiAdrress+1,(uint8_t)		(uiData>>16));		//Write high byte
	WriteEEPROM_Byte(uiAdrress+2,(uint8_t)		(uiData>>8));		//Write high byte
	WriteEEPROM_Byte((uiAdrress+3),(uint8_t)	uiData);			//Write low byte
}

uint32_t ReadEEPROMu32(uint16_t uiAdrress){
	uint32_t uiData;
	uiData = (uint32_t)ReadEEPROM_Byte(uiAdrress); 						//Read high byte
	uiData = uiData<<24;
	uiData += (uint32_t)ReadEEPROM_Byte(uiAdrress+1)<<16; 						//Read high byte
	uiData += (uint32_t)ReadEEPROM_Byte(uiAdrress+2)<<8; 						//Read high byte
	uiData += ReadEEPROM_Byte(uiAdrress+3);	//Write low byte
	return uiData;
}
//================================ EEPROM ================================================//
