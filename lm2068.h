#include "board.h"
#include "pin_mux.h"
#include "fsl_gpio.h"

extern uint16_t LUCIDA_FONT[];

#define LAYER1_ADDR			0x0000
#define LAYER2_ADDR			0x2580
#define LAYER3_ADDR			0x4B00

#define GLCD_WIDTH         320 
#define GLCD_HEIGHT        240 
#define GLCD_CHAR_WIDTH    8 
#define GLCD_CHAR_HEIGHT   16 

#define GLCD_TEXT_ADDR              0x0000 
#define GLCD_GRAPHICS_ADDR          GLCD_WIDTH * GLCD_HEIGHT / 64 
#define GLCD_GRAPHICS_ADDR_END      GLCD_GRAPHICS_ADDR + (GLCD_WIDTH * GLCD_HEIGHT / 8) 

#if GLCD_CHAR_WIDTH < 9 
   #define GLCD_CR (GLCD_WIDTH/8 - 1) 
#else 
   #define GLCD_CR (GLCD_WIDTH/4 - 2) 
#endif 

#define  LCD_DB0_PORT   GPIOD
#define  LCD_DB0_PIN    5u
#define  LCD_DB1_PORT   GPIOD
#define  LCD_DB1_PIN    4u
#define  LCD_DB2_PORT   GPIOD
#define  LCD_DB2_PIN    3u
#define  LCD_DB3_PORT   GPIOD
#define  LCD_DB3_PIN    2u
#define  LCD_DB4_PORT   GPIOD
#define  LCD_DB4_PIN    1u
#define  LCD_DB5_PORT   GPIOD
#define  LCD_DB5_PIN    0u
#define  LCD_DB6_PORT   GPIOC
#define  LCD_DB6_PIN    19u
#define  LCD_DB7_PORT   GPIOC
#define  LCD_DB7_PIN    18u

#define  LCD_BL_PORT    GPIOC
#define  LCD_BL_PIN     9u
#define  LCD_RES_PORT   GPIOE
#define  LCD_RES_PIN    6u
#define  LCD_A0_PORT    GPIOE
#define  LCD_A0_PIN     7u
#define  LCD_CS_PORT    GPIOE
#define  LCD_CS_PIN     8u
#define  LCD_RD_PORT    GPIOE
#define  LCD_RD_PIN     9u
#define  LCD_WR_PORT    GPIOE
#define  LCD_WR_PIN     10u

#define  LCD_DB0_ON     GPIO_SetPinsOutput(LCD_DB0_PORT,1<<LCD_DB0_PIN)
#define  LCD_DB0_OFF    GPIO_ClearPinsOutput(LCD_DB0_PORT,1<<LCD_DB0_PIN)
#define  LCD_DB1_ON     GPIO_SetPinsOutput(LCD_DB1_PORT,1<<LCD_DB1_PIN)
#define  LCD_DB1_OFF    GPIO_ClearPinsOutput(LCD_DB1_PORT,1<<LCD_DB1_PIN)
#define  LCD_DB2_ON     GPIO_SetPinsOutput(LCD_DB2_PORT,1<<LCD_DB2_PIN)
#define  LCD_DB2_OFF    GPIO_ClearPinsOutput(LCD_DB2_PORT,1<<LCD_DB2_PIN)
#define  LCD_DB3_ON     GPIO_SetPinsOutput(LCD_DB3_PORT,1<<LCD_DB3_PIN)
#define  LCD_DB3_OFF    GPIO_ClearPinsOutput(LCD_DB3_PORT,1<<LCD_DB3_PIN)
#define  LCD_DB4_ON     GPIO_SetPinsOutput(LCD_DB4_PORT,1<<LCD_DB4_PIN)
#define  LCD_DB4_OFF    GPIO_ClearPinsOutput(LCD_DB4_PORT,1<<LCD_DB4_PIN)
#define  LCD_DB5_ON     GPIO_SetPinsOutput(LCD_DB5_PORT,1<<LCD_DB5_PIN)
#define  LCD_DB5_OFF    GPIO_ClearPinsOutput(LCD_DB5_PORT,1<<LCD_DB5_PIN)
#define  LCD_DB6_ON     GPIO_SetPinsOutput(LCD_DB6_PORT,1<<LCD_DB6_PIN)
#define  LCD_DB6_OFF    GPIO_ClearPinsOutput(LCD_DB6_PORT,1<<LCD_DB6_PIN)
#define  LCD_DB7_ON     GPIO_SetPinsOutput(LCD_DB7_PORT,1<<LCD_DB7_PIN)
#define  LCD_DB7_OFF    GPIO_ClearPinsOutput(LCD_DB7_PORT,1<<LCD_DB7_PIN)

#define  LCD_BL_ON      GPIO_ClearPinsOutput(LCD_BL_PORT,1<<LCD_BL_PIN)
#define  LCD_BL_OFF     GPIO_SetPinsOutput(LCD_BL_PORT,1<<LCD_BL_PIN)

#define  LCD_RES_ON     GPIO_SetPinsOutput(LCD_RES_PORT,1<<LCD_RES_PIN)
#define  LCD_RES_OFF    GPIO_ClearPinsOutput(LCD_RES_PORT,1<<LCD_RES_PIN)

#define  LCD_A0_ON      GPIO_SetPinsOutput(LCD_A0_PORT,1<<LCD_A0_PIN)
#define  LCD_A0_OFF     GPIO_ClearPinsOutput(LCD_A0_PORT,1<<LCD_A0_PIN)

#define  LCD_CS_ON      GPIO_SetPinsOutput(LCD_CS_PORT,1<<LCD_CS_PIN)
#define  LCD_CS_OFF     GPIO_ClearPinsOutput(LCD_CS_PORT,1<<LCD_CS_PIN)

#define  LCD_RD_ON      GPIO_SetPinsOutput(LCD_RD_PORT,1<<LCD_RD_PIN)
#define  LCD_RD_OFF     GPIO_ClearPinsOutput(LCD_RD_PORT,1<<LCD_RD_PIN)

#define  LCD_WR_ON      GPIO_SetPinsOutput(LCD_WR_PORT,1<<LCD_WR_PIN)
#define  LCD_WR_OFF     GPIO_ClearPinsOutput(LCD_WR_PORT,1<<LCD_WR_PIN)

void delayns(uint8_t n);
void glcd_sendCMD(uint8_t Command);
void glcd_sendCMD(uint8_t Command);
void glcd_init(void);
void gldc_clearTextScreen (void);
void gldc_clearGraphicScreen (uint16_t	startAddr);
void gldc_clearGraphicScreen_DataArea (uint16_t	startAddr);
void setCursorAddress(uint16_t address);
void writeBitmapText(char* text,int x, int y, uint16_t* font);
uint8_t reverse_byte(uint8_t x);
void glcd_writeString(char* writeString, uint8_t line, uint8_t column);
void glcd_Draw_Black_Char (uint16_t LayerAddr, int x, int y);
void glcd_Draw_Black_Rectangle (uint16_t LayerAddr, uint8_t line, uint8_t startColumn, uint8_t endColumn);
void glcd_Under_Line_Char (uint16_t LayerAddr, int x, int y);
void glcd_Under_Line_Text (uint16_t LayerAddr, uint8_t line, uint8_t startColumn, uint8_t endColumn);
void glcd_writeOneChar(uint8_t writeChar, uint8_t line, uint8_t column);
void glcd_Display_Integer (uint8_t line, uint8_t startColumn, uint8_t displayLength, uint16_t displayVal);
void glcd_Display_Integer_With_Zero (uint8_t line, uint8_t startColumn, uint8_t displayLength, uint16_t displayVal);
void glcd_Display_Float_1Dec (uint8_t line, uint8_t startColumn, uint8_t displayLength, uint16_t displayVal);