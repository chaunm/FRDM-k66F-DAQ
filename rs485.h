#include "board.h"
#include "fsl_gpio.h"
#include "fsl_uart.h"
#include "pin_mux.h"
#include "clock_config.h"

#define RS4851_UART 		        UART3
#define RS4851_UART_CLKSRC 		SYS_CLK
#define RS4851_UART_IRQn 		UART3_RX_TX_IRQn
#define RS4851_UART_IRQHandler 		UART3_RX_TX_IRQHandler

#define RS4852_UART 			UART4
#define RS4852_UART_CLKSRC 		SYS_CLK
#define RS4852_UART_IRQn 		UART4_RX_TX_IRQn
#define RS4852_UART_IRQHandler 		UART4_RX_TX_IRQHandler

#define RS4851_UART_BAUDRATE 43200//28800
#define RS4852_UART_BAUDRATE 43200//28800

enum
{
    _WAIT_MODE		= 0,
    _READ_ATS_STATUS	= 1,
    _READ_DOOR		= 2,
    _READ_AIR_COND 	= 3,
    _READ_ATS_HISTORY 	= 4,
    _READ_ATS_ACTIVE_ALARM = 5,
    _WAIT_ATS_RESPOND	= 6,
    _WAIT_DOOR_RESPOND 	= 7,
    _WAIT_AIR_COND_RESPOND	= 8,
    _WAIT_WRITE_SINGLE_REG	= 10,
    _WAIT_WRITE_TIME_REG_ATS	= 11,
    _WAIT_WRITE_TIME_REG_AIRCON	= 12,
};

enum
{
    _READ_COIL_STATUS 			= 1,
    _READ_INPUT_STATUS			= 2,
    _READ_HOLDING_REGS 			= 3,
    _READ_INPUT_REG 			= 4,
    _FORCE_SINGLE_COIL 			= 5,
    _PRESET_SINGLE_REG 			= 6,
    _READ_EXCEPTION_STATUS 		= 7,
    _FETCH_COMM_EVENT_COUNTER 	= 11,
    _FET_COMM_EVENT_LOG 		= 12,
    _FORCE_MULTIPLE_COILS  		= 15,
    _PRESET_MULTIPLE_REGS 		= 16,
    _REPORT_SLAVE_ID 			= 17,
    _READ_GENERAL_REFERENCE 	= 20,
    _WRITE_GENERAL_REFERENCE 	= 21,
    _MASK_WRITE_4X_REG 			= 22,
    _READ_WRITE_4X_REGS 		= 23,
    _READ_FIFO_QUEUE 			= 24,
    _SETUP_REGISTER				= 49,
    _USER_REGISTER 				= 50
};

typedef struct {
    uint8_t runningStep;
    uint8_t atsNorespond;
    uint8_t doorNorespond;
    uint8_t airConNorespond;
    uint8_t atsError;
    uint8_t doorError;
    uint8_t airConError;
    
    uint8_t u8BuffWrite[128];
    uint8_t u8BuffRead[128];
    
    uint8_t u8CRCHigh;
    uint8_t u8CRCLow;
    
    uint8_t u8CRCHighRead;
    uint8_t u8CRCLowRead;
    
    uint8_t u8SlaveID;
    uint8_t u8FunctionCode;
    
    uint8_t u8StartHigh;
    uint8_t u8StartLow;
    uint8_t u8IDHigh;
    uint8_t u8IDLow;
    uint8_t u8NumberRegHigh;
    uint8_t u8NumberRegLow;
    
    uint8_t u8ReadWriteSetup;
    uint8_t u8SetupType;
    
    uint8_t u8ByteCount;
    
    uint8_t u8MosbusEn;
    uint8_t u8DataPointer;
}sMODBUSRTU_struct;
extern sMODBUSRTU_struct Modbus;
extern sMODBUSRTU_struct DoorAccess;

void Init_RS485_UART (void);
void ModbusCRC(unsigned char *str, unsigned char len);
int8_t RS4851_Check_Respond_Data (void);
void Read_Holding_Regs_Query (uint8_t slaveAddr, uint16_t startingAddr, uint16_t noPoint);
void Write_Single_Reg (uint8_t slaveAddr, uint16_t regAddr, uint16_t writeVal);
void Extract_Holding_Regs_Data_ATS (void);
void Extract_Holding_Regs_Data_AIRCON (void);
void Extract_Holding_Regs_Data_DOOR (void);
void Write_Time_Reg (uint8_t slaveAddr, uint16_t regAddr, uint16_t writeVal);