#include "app_init.h"
#include "fsl_gpio.h"
#include "fsl_adc16.h"
#include "adc_config.h"
#include "eeprom_rtc.h"
#include "lm2068.h"
#include "menu.h"
#include "rs485.h"
#include "fsl_debug_console.h"
#include "am2320.h"
#include "i2c_lock.h"

void AppInitUserInterface()
{
  /* Define the init structure */
  gpio_pin_config_t out_config = {
    kGPIO_DigitalOutput, 0,
  };
  gpio_pin_config_t in_config = {
    kGPIO_DigitalInput, 0,
  };
  /* Init output */
  //GLCD
  GPIO_PinInit(GPIOC, 4u, &out_config);
  GPIO_PinInit(LCD_BL_PORT, LCD_BL_PIN, &out_config);
  GPIO_PinInit(LCD_RES_PORT, LCD_RES_PIN, &out_config);
  GPIO_PinInit(LCD_A0_PORT, LCD_A0_PIN, &out_config);
  GPIO_PinInit(LCD_CS_PORT, LCD_CS_PIN, &out_config);
  GPIO_PinInit(LCD_RD_PORT, LCD_RD_PIN, &out_config);
  GPIO_PinInit(LCD_WR_PORT, LCD_WR_PIN, &out_config);
  GPIO_PinInit(LCD_DB0_PORT, LCD_DB0_PIN, &out_config);
  GPIO_PinInit(LCD_DB1_PORT, LCD_DB1_PIN, &out_config);
  GPIO_PinInit(LCD_DB2_PORT, LCD_DB2_PIN, &out_config);
  GPIO_PinInit(LCD_DB3_PORT, LCD_DB3_PIN, &out_config);
  GPIO_PinInit(LCD_DB4_PORT, LCD_DB4_PIN, &out_config);
  GPIO_PinInit(LCD_DB5_PORT, LCD_DB5_PIN, &out_config);
  GPIO_PinInit(LCD_DB6_PORT, LCD_DB6_PIN, &out_config);
  GPIO_PinInit(LCD_DB7_PORT, LCD_DB7_PIN, &out_config);
  //EEPROM-RTC
  GPIO_PinInit(SDA_PORT, SDA_PIN, &out_config);
  GPIO_PinInit(SCL_PORT, SCL_PIN, &out_config);
  /* Init input */
  //KEY BOARD
  GPIO_PinInit(KEY_1_PORT, KEY_1_PIN, &in_config);
  GPIO_PinInit(KEY_2_PORT, KEY_2_PIN, &in_config);
  GPIO_PinInit(KEY_3_PORT, KEY_3_PIN, &in_config);
  GPIO_PinInit(KEY_4_PORT, KEY_4_PIN, &in_config);
  I2C_Lock_Init();
  Init_I2CE();
  Init_All_Variable();
  Init_RS485_UART();
  AM2320_I2C_Init();
}

void AppInitAdc()
{
  adc16_config_t adc16ConfigStruct;
#if defined(FSL_FEATURE_ADC16_HAS_DIFF_MODE) && FSL_FEATURE_ADC16_HAS_DIFF_MODE
  adc16_channel_config_t adc16ChannelConfigStruct;
#endif
  /*
  * adc16ConfigStruct.referenceVoltageSource = kADC16_ReferenceVoltageSourceVref;
  * adc16ConfigStruct.clockSource = kADC16_ClockSourceAsynchronousClock;
  * adc16ConfigStruct.enableAsynchronousClock = true;
  * adc16ConfigStruct.clockDivider = kADC16_ClockDivider8;
  * adc16ConfigStruct.resolution = kADC16_ResolutionSE12Bit;
  * adc16ConfigStruct.longSampleMode = kADC16_LongSampleDisabled;
  * adc16ConfigStruct.enableHighSpeed = false;
  * adc16ConfigStruct.enableLowPower = false;
  * adc16ConfigStruct.enableContinuousConversion = false;
  */
  ADC16_GetDefaultConfig(&adc16ConfigStruct);
  adc16ConfigStruct.enableHighSpeed = true;
  adc16ConfigStruct.enableContinuousConversion = true;
  //========================= ADC0 Initialization ================================//
  ADC16_Init(BOARD_ADC16_0_BASE, &adc16ConfigStruct);
  ADC16_EnableHardwareTrigger(BOARD_ADC16_0_BASE, false); /* Make sure the software trigger is used. */
#if defined(FSL_FEATURE_ADC16_HAS_CALIBRATION) && FSL_FEATURE_ADC16_HAS_CALIBRATION
  if (kStatus_Success == ADC16_DoAutoCalibration(BOARD_ADC16_0_BASE))
  {
    PRINTF("ADC16_DoAutoCalibration() Done.\r\n");
  }
  else
  {
    PRINTF("ADC16_DoAutoCalibration() Failed.\r\n");
  }
#endif /* FSL_FEATURE_ADC16_HAS_CALIBRATION */
  //========================= ADC1 Initialization ================================//
  
  ADC16_Init(BOARD_ADC16_1_BASE, &adc16ConfigStruct);
  ADC16_EnableHardwareTrigger(BOARD_ADC16_1_BASE, false); /* Make sure the software trigger is used. */
#if defined(FSL_FEATURE_ADC16_HAS_CALIBRATION) && FSL_FEATURE_ADC16_HAS_CALIBRATION
  if (kStatus_Success == ADC16_DoAutoCalibration(BOARD_ADC16_1_BASE))
  {
    PRINTF("ADC16_DoAutoCalibration() Done.\r\n");
  }
  else
  {
    PRINTF("ADC16_DoAutoCalibration() Failed.\r\n");
  }
#endif /* FSL_FEATURE_ADC16_HAS_CALIBRATION */

#if defined(FSL_FEATURE_ADC16_HAS_DIFF_MODE) && FSL_FEATURE_ADC16_HAS_DIFF_MODE
  adc16ChannelConfigStruct.channelNumber = BOARD_ADC16_1_USER_CHANNEL1;
  adc16ChannelConfigStruct.enableInterruptOnConversionCompleted = false;
  adc16ChannelConfigStruct.enableDifferentialConversion = false;
#endif /* FSL_FEATURE_ADC16_HAS_DIFF_MODE */
}

void AppLedInit()
{
  LED_STATUS_INIT(1);
  LED_RUN_INIT(1);
  LED_CON_GPRS_INIT(1);
  LED_CON_ETH_INIT(1);
  LED_ALARM_INIT(1);
}

void AppIoInit()
{
  IO_MUX_S1_INIT(0);
  IO_MUX_S2_INIT(0);
  IO_MUX_S3_INIT(0);
  IO_MUX_S0_INIT(0);
  
  IO_WAITMCU_1_INIT(0);
  IO_WAITMCU_2_INIT(0);
  IO_WAITMCU_3_INIT(0);
  IO_WAITMCU_4_INIT(0);
  IO_WAITMCU_5_INIT(0);
  IO_OPENDOOR_MCU_INIT(0);
  IO_ALARM_LIGHT_INIT(0);
  IO_ALARM_SPEAKER_INIT(0);
}
