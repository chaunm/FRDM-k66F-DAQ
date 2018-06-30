#include "board.h"
#include "pin_mux.h"
#include "fsl_gpio.h"
#include "eeprom_rtc.h"
#include "am2320.h"
#include "i2c_lock.h"

static void AM2320_Delay(volatile long time);
static void AM2320_SDA_Output(void);
static void AM2320_SDA_Input(void);

static void AM2320_Start(void);
static void AM2320_Stop(void);
static void AM2320_WriteI2C(uint8_t Data,uint8_t ACK_Bit);
static uint8_t AM2320_ReadI2C(uint8_t ACK_Bit);
static void Wake_AM2320(void);
static void Trigger_AM2320(void);

void AM2320_SDA_Output(void)
{
  /* Define the init structure */
  gpio_pin_config_t AM2320_SDA_out_config = {
      kGPIO_DigitalOutput, 0,
  };
  GPIO_PinInit(SDA_PORT, SDA_PIN, &AM2320_SDA_out_config);
}
void AM2320_SDA_Input(void)
{
  /* Define the init structure */
  gpio_pin_config_t AM2320_SDA_in_config = {
	 kGPIO_DigitalInput, 0,
  };
  GPIO_PinInit(SDA_PORT, SDA_PIN, &AM2320_SDA_in_config);
}

void AM2320_Delay(volatile long time)  {
  for(;time > 0; time--);
}
//======================================================================================//
//=========================== SOFTWARE I2C =============================================//
//======================================================================================//
void AM2320_Start(void){
  AM2320_SDA_ON;
  AM2320_Delay(AM2320us);
  AM2320_SCL_ON;
  AM2320_Delay(AM2320us);//Start condition setup time (min=600ns)
  AM2320_SDA_OFF;
  AM2320_Delay(AM2320us);//Start condition hold time (min=600ns)
  AM2320_SCL_OFF;
  AM2320_Delay(AM2320us);
}

void AM2320_Stop(void){
  AM2320_SDA_OFF;
  AM2320_Delay(AM2320us);
  AM2320_SCL_ON;
  AM2320_Delay(AM2320us);
  AM2320_SDA_ON;
  AM2320_Delay(AM2320us);
}

void AM2320_WriteI2C(uint8_t Data,uint8_t ACK_Bit){   
  uint8_t i; 
  AM2320_SDA_Output();

  AM2320_Delay(AM2320us);
  for (i=0;i<8;i++){
    if((Data & 0x80)==0)
      AM2320_SDA_OFF;
    else 
      AM2320_SDA_ON;
    AM2320_Delay(20);
    AM2320_SCL_ON;
    AM2320_Delay(AM2320us);
    AM2320_SCL_OFF;
    Data<<=1;
    AM2320_Delay(AM2320us);
  }
  AM2320_SDA_ON;

  AM2320_SDA_Input();
  AM2320_Delay(20);
  AM2320_SCL_ON;
  AM2320_Delay(AM2320us);
  AM2320_SCL_OFF;
  AM2320_Delay(AM2320us);
  AM2320_SDA_Output();
  AM2320_SDA_ON;
}

uint8_t AM2320_ReadI2C(uint8_t ACK_Bit){
  uint8_t i;
  uint8_t Data=0;
  
  AM2320_SDA_Input();
  AM2320_Delay(AM2320us);
  for (i=0;i<8;i++)
  {
      AM2320_SCL_ON;
      AM2320_Delay(AM2320us);
      Data<<= 1;
      if(AM2320_SDA_DATA_READ==0)
        Data  = (Data | 0);
      else
        Data  = (Data | 1);
      AM2320_SCL_OFF;
      AM2320_Delay(AM2320us);
  }
  
  AM2320_SDA_Output();
  
  if (ACK_Bit == 1)
  {
    AM2320_SDA_OFF; // Send ACK
    AM2320_Delay(20);
    AM2320_SCL_ON;
    AM2320_Delay(AM2320us);
    AM2320_SCL_OFF;
    AM2320_Delay(20);
    AM2320_SDA_ON;
  }
  else
  {
    AM2320_SDA_ON;  // Send NO 
    AM2320_Delay(20);
    AM2320_SCL_ON;
    AM2320_Delay(AM2320us);
    AM2320_SCL_OFF;
    AM2320_Delay(20);
    AM2320_SDA_ON;
  }

  return Data;
}
//======================================================================================//
//======================================================================================//
//======================================================================================//
void AM2320_I2C_Init(void){
  AM2320_SDA_Output();
  AM2320_SDA_ON;
  AM2320_Delay(AM2320us);
  Wake_AM2320();
  AM2320_Delay(_AM2320_800us); // Delay min = 800us
  Trigger_AM2320();
}
void Wake_AM2320(void){
  AM2320_Start();
  AM2320_WriteI2C(AM2320_ADDR_W,AM2320_NO_ACK);
  AM2320_SDA_OFF;
  AM2320_Delay(_AM2320_800us); // Delay min = 800us
  AM2320_Stop();
}

void Trigger_AM2320(void){
  AM2320_Start();
  AM2320_WriteI2C(AM2320_ADDR_W,AM2320_ACK);
  AM2320_WriteI2C(0x03,AM2320_ACK);
  AM2320_WriteI2C(0x00,AM2320_ACK);
  AM2320_WriteI2C(0x04,AM2320_ACK);
  AM2320_Stop();
}

uint16_t u16HumiRh = 0;
uint16_t u16Temper = 0;
uint16_t u16CRC = 0;


void Getdata_AM2320(void){
  uint16_t u16HumiRh1=0,u16Temper1=0,u16CRC1=0;

  Wake_AM2320();
  AM2320_Delay(_AM2320_800us); // Delay min = 800us
  Trigger_AM2320();
  AM2320_Delay(_AM2320_1500); // Delay min = 1500us

  AM2320_Start();
  AM2320_WriteI2C(AM2320_ADDR_R,AM2320_ACK);
  AM2320_Delay(_AM2320_30us); // Delay min = 30us
  AM2320_ReadI2C(AM2320_ACK);
  AM2320_ReadI2C(AM2320_ACK);
  u16HumiRh1  = AM2320_ReadI2C(AM2320_ACK)<<8;
  u16HumiRh1 |= AM2320_ReadI2C(AM2320_ACK);
  u16Temper1  = AM2320_ReadI2C(AM2320_ACK)<<8;
  u16Temper1 |= AM2320_ReadI2C(AM2320_ACK);
  u16CRC1  = AM2320_ReadI2C(AM2320_ACK);
  u16CRC1 |= AM2320_ReadI2C(AM2320_ACK)<<8;
  AM2320_Stop();
  
  u16HumiRh = u16HumiRh1;
  u16Temper = u16Temper1;
  u16CRC = u16CRC1;
}		
