//**************************************************************************************//
//******************* define pin *******************************************************//
#define  SDA_PORT   GPIOD
#define  SDA_PIN    9u
#define  SCL_PORT   GPIOD
#define  SCL_PIN    8u

#define AM2320_SDA_ON 	GPIO_SetPinsOutput(SDA_PORT,1<<SDA_PIN)
#define AM2320_SDA_OFF	GPIO_ClearPinsOutput(SDA_PORT,1<<SDA_PIN)
#define AM2320_SCL_ON	GPIO_SetPinsOutput(SCL_PORT,1<<SCL_PIN)
#define AM2320_SCL_OFF	GPIO_ClearPinsOutput(SCL_PORT,1<<SCL_PIN)

#define AM2320_SDA_DATA_READ   GPIO_ReadPinInput(SDA_PORT,SDA_PIN)

//**********************************//
#define AM2320us   40 //~6.125us pulse on && ~12.37us pulse off  f=80,08 Hz ( AM2320 Max clock = 100kHz)

#define _AM2320_800us  8000
#define _AM2320_30us   300
#define _AM2320_1500   15000

//BH1750 I2C Address
#define AM2320_ADDR_R	0xB9
#define AM2320_ADDR_W	0xB8

#define AM2320_ACK	1
#define AM2320_NO_ACK	0


extern uint16_t u16HumiRh;
extern uint16_t u16Temper;
extern uint16_t u16CRC;

//**********************************//
void AM2320_Delay(volatile long time);
void AM2320_SDA_Output(void);
void AM2320_SDA_Input(void);

void AM2320_Start(void);
void AM2320_Stop(void);
void AM2320_WriteI2C(uint8_t Data,uint8_t ACK_Bit);
uint8_t AM2320_ReadI2C(uint8_t ACK_Bit);

void AM2320_I2C_Init(void);
void Wake_AM2320(void);
void Trigger_AM2320(void);
void Write_AM2320(uint8_t Addr,uint8_t ACK_Bit);
void Getdata_AM2320(void);

extern uint16_t u16CRC;
