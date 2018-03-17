/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
PinsProfile:
- !!product 'Pins v2.0'
- !!processor 'MK66FN2M0xxx18'
- !!package 'MK66FN2M0VLQ18'
- !!mcu_data 'ksdk2_0'
- !!processor_version '1.0.1'
- pin_labels:
  - {pin_num: '140', pin_signal: PTD11/LLWU_P25/SPI2_PCS0/SDHC0_CLKIN/LPUART0_CTS_b/FB_A19, label: SPI2_CS, identifier: SPI2_CS}
  - {pin_num: '141', pin_signal: PTD12/SPI2_SCK/FTM3_FLT0/SDHC0_D4/FB_A20, label: SPI2_SCK, identifier: SPI2_SCK}
  - {pin_num: '142', pin_signal: PTD13/SPI2_SOUT/SDHC0_D5/FB_A21, label: SPI2_MOSI, identifier: SPI2_MOSI}
  - {pin_num: '143', pin_signal: PTD14/SPI2_SIN/SDHC0_D6/FB_A22, label: SPI2_MISO, identifier: SPI2_MISO}
  - {pin_num: '132', pin_signal: ADC0_SE6b/PTD5/SPI0_PCS2/UART0_CTS_b/UART0_COL_b/FTM0_CH5/FB_AD1/SDRAM_A9/EWM_OUT_b/SPI1_SCK, label: LCD_DB0, identifier: LCD_DB0}
  - {pin_num: '128', pin_signal: ADC0_SE5b/PTD1/SPI0_SCK/UART2_CTS_b/FTM3_CH1/FB_CS0_b, label: LCD_DB4, identifier: LCD_DB4}
  - {pin_num: '131', pin_signal: PTD4/LLWU_P14/SPI0_PCS1/UART0_RTS_b/FTM0_CH4/FB_AD2/SDRAM_A10/EWM_IN/SPI1_PCS0, label: LCD_DB1, identifier: LCD_DB1}
  - {pin_num: '130', pin_signal: PTD3/SPI0_SIN/UART2_TX/FTM3_CH3/FB_AD3/SDRAM_A11/I2C0_SDA, label: LCD_DB2, identifier: LCD_DB2}
  - {pin_num: '129', pin_signal: PTD2/LLWU_P13/SPI0_SOUT/UART2_RX/FTM3_CH2/FB_AD4/SDRAM_A12/I2C0_SCL, label: LCD_DB3, identifier: LCD_DB3}
  - {pin_num: '127', pin_signal: PTD0/LLWU_P12/SPI0_PCS0/UART2_RTS_b/FTM3_CH0/FB_ALE/FB_CS1_b/FB_TS_b, label: LCD_DB5, identifier: LCD_DB5}
  - {pin_num: '114', pin_signal: ADC1_SE5b/CMP0_IN3/PTC9/FTM3_CH5/I2S0_RX_BCLK/FB_AD6/SDRAM_A14/FTM2_FLT0, label: LCD_BL, identifier: LCD_BL}
  - {pin_num: '117', pin_signal: PTC12/UART4_RTS_b/FTM_CLKIN0/FB_AD27/SDRAM_D27/FTM3_FLT0/TPM_CLKIN0, label: LCD_WAIT, identifier: LCD_WAIT}
  - {pin_num: '125', pin_signal: PTC18/UART3_RTS_b/ENET0_1588_TMR2/FB_TBST_b/FB_CS2_b/FB_BE15_8_BLS23_16_b/SDRAM_DQM1, label: LCD_DB7, identifier: LCD_DB7}
  - {pin_num: '126', pin_signal: PTC19/UART3_CTS_b/ENET0_1588_TMR3/FB_CS3_b/FB_BE7_0_BLS31_24_b/SDRAM_DQM0/FB_TA_b, label: LCD_DB6, identifier: LCD_DB6}
  - {pin_num: '9', pin_signal: PTE6/LLWU_P16/SPI1_PCS3/UART3_CTS_b/I2S0_MCLK/FTM3_CH1/USB0_SOF_OUT, label: LCD_RES, identifier: LCD_RES}
  - {pin_num: '10', pin_signal: PTE7/UART3_RTS_b/I2S0_RXD0/FTM3_CH2, label: LCD_A0, identifier: LCD_A0}
  - {pin_num: '11', pin_signal: PTE8/I2S0_RXD1/I2S0_RX_FS/LPUART0_TX/FTM3_CH3, label: LCD_CS, identifier: LCD_CS}
  - {pin_num: '12', pin_signal: PTE9/LLWU_P17/I2S0_TXD1/I2S0_RX_BCLK/LPUART0_RX/FTM3_CH4, label: LCD_RD, identifier: LCD_RD}
  - {pin_num: '13', pin_signal: PTE10/LLWU_P18/I2C3_SDA/I2S0_TXD0/LPUART0_CTS_b/FTM3_CH5/USB1_ID, label: LCD_WR, identifier: LCD_WR}
  - {pin_num: '35', pin_signal: ADC1_SE16/CMP2_IN2/ADC0_SE22, label: ADC_TEMP1, identifier: ADC_TEMP1}
  - {pin_num: '36', pin_signal: ADC0_SE16/CMP1_IN2/ADC0_SE21, label: ADC_TEMP2, identifier: ADC_TEMP2}
  - {pin_num: '38', pin_signal: DAC0_OUT/CMP1_IN3/ADC0_SE23, label: ADC_VBATT, identifier: ADC_VBATT}
  - {pin_num: '37', pin_signal: VREF_OUT/CMP1_IN5/CMP0_IN5/ADC1_SE18, label: ADC_TEMP3, identifier: ADC_TEMP3}
  - {pin_num: '39', pin_signal: DAC1_OUT/CMP0_IN4/CMP2_IN3/ADC1_SE23, label: ADC1_SE23, identifier: ADC1_SE23}
  - {pin_num: '84', pin_signal: ADC0_SE13/TSI0_CH8/PTB3/I2C0_SDA/UART0_CTS_b/UART0_COL_b/ENET0_1588_TMR1/SDRAM_CS0_b/FTM0_FLT0, label: LED_RUN, identifier: LED_RUN}
  - {pin_num: '85', pin_signal: ADC1_SE10/PTB4/ENET0_1588_TMR2/SDRAM_CS1_b/FTM1_FLT0, label: LED_CON_GPRS, identifier: LED_CON_GPRS}
  - {pin_num: '86', pin_signal: ADC1_SE11/PTB5/ENET0_1588_TMR3/FTM2_FLT0, label: LED_CON_ETH, identifier: LED_CON_ETH}
  - {pin_num: '87', pin_signal: ADC1_SE12/PTB6/FB_AD23/SDRAM_D23, label: LED_ALARM, identifier: LED_ALARM}
  - {pin_num: '109', pin_signal: PTC4/LLWU_P8/SPI0_PCS0/UART1_TX/FTM0_CH3/FB_AD11/SDRAM_A19/CMP1_OUT, label: LED_STATUS, identifier: LED_STATUS}
  - {pin_num: '59', pin_signal: ADC0_SE10/PTA7/FTM0_CH4/RMII0_MDIO/MII0_MDIO/TRACE_D3, label: RMII0_MDIO, identifier: RMII0_MDIO}
  - {pin_num: '60', pin_signal: ADC0_SE11/PTA8/FTM1_CH0/RMII0_MDC/MII0_MDC/FTM1_QD_PHA/TPM1_CH0/TRACE_D2, label: RMII0_MDC, identifier: RMII0_MDC}
  - {pin_num: '64', pin_signal: CMP2_IN0/PTA12/CAN0_TX/FTM1_CH0/RMII0_RXD1/MII0_RXD1/I2C2_SCL/I2S0_TXD0/FTM1_QD_PHA/TPM1_CH0, label: RMII0_RXD1, identifier: RMII0_RXD1}
  - {pin_num: '65', pin_signal: CMP2_IN1/PTA13/LLWU_P4/CAN0_RX/FTM1_CH1/RMII0_RXD0/MII0_RXD0/I2C2_SDA/I2S0_TX_FS/FTM1_QD_PHB/TPM1_CH1, label: RMII0_RXD0, identifier: RMII0_RXD0}
  - {pin_num: '66', pin_signal: PTA14/SPI0_PCS0/UART0_TX/RMII0_CRS_DV/MII0_RXDV/I2C2_SCL/I2S0_RX_BCLK/I2S0_TXD1, label: RMII0_CRS_DV, identifier: RMII0_CRS_DV}
  - {pin_num: '67', pin_signal: CMP3_IN1/PTA15/SPI0_SCK/UART0_RX/RMII0_TXEN/MII0_TXEN/I2S0_RXD0, label: RMII0_TXEN, identifier: RMII0_TXEN}
  - {pin_num: '68', pin_signal: CMP3_IN2/PTA16/SPI0_SOUT/UART0_CTS_b/UART0_COL_b/RMII0_TXD0/MII0_TXD0/I2S0_RX_FS/I2S0_RXD1, label: RMII0_TXD0, identifier: RMII0_TXD0}
  - {pin_num: '69', pin_signal: ADC1_SE17/PTA17/SPI0_SIN/UART0_RTS_b/RMII0_TXD1/MII0_TXD1/I2S0_MCLK, label: RMII0_TXD1, identifier: RMII0_TXD1}
  - {pin_num: '55', pin_signal: PTA5/USB0_CLKIN/FTM0_CH2/RMII0_RXER/MII0_RXER/CMP2_OUT/I2S0_TX_BCLK/JTAG_TRST_b, label: RMII0_RXER, identifier: RMII0_RXER}
  - {pin_num: '47', pin_signal: PTE26/ENET_1588_CLKIN/UART4_CTS_b/RTC_CLKOUT/USB0_CLKIN, label: ENET_1588_CLKIN, identifier: ENET_1588_CLKIN}
  - {pin_num: '110', pin_signal: PTC5/LLWU_P9/SPI0_SCK/LPTMR0_ALT2/I2S0_RXD0/FB_AD10/SDRAM_A18/CMP0_OUT/FTM0_CH2, label: BUTTON_ESC, identifier: BUTTON_ESC}
  - {pin_num: '111', pin_signal: CMP0_IN0/PTC6/LLWU_P10/SPI0_SOUT/PDB0_EXTRG/I2S0_RX_BCLK/FB_AD9/SDRAM_A17/I2S0_MCLK, label: BUTTON_UP, identifier: BUTTON_UP}
  - {pin_num: '112', pin_signal: CMP0_IN1/PTC7/SPI0_SIN/USB0_SOF_OUT/I2S0_RX_FS/FB_AD8/SDRAM_A16, label: BUTTON_DOWN, identifier: BUTTON_DOWN}
  - {pin_num: '113', pin_signal: ADC1_SE4b/CMP0_IN2/PTC8/FTM3_CH4/I2S0_MCLK/FB_AD7/SDRAM_A15, label: BUTTON_ENTER, identifier: BUTTON_ENTER}
  - {pin_num: '137', pin_signal: PTD8/LLWU_P24/I2C0_SCL/LPUART0_RX/FB_A16, label: SCL, identifier: SCL}
  - {pin_num: '138', pin_signal: PTD9/I2C0_SDA/LPUART0_TX/FB_A17, label: SDA, identifier: SDA}
  - {pin_num: '1', pin_signal: ADC1_SE4a/PTE0/SPI1_PCS1/UART1_TX/SDHC0_D1/TRACE_CLKOUT/I2C1_SDA/RTC_CLKOUT, label: GSM_TX, identifier: GSM_TX}
  - {pin_num: '2', pin_signal: ADC1_SE5a/PTE1/LLWU_P0/SPI1_SOUT/UART1_RX/SDHC0_D0/TRACE_D3/I2C1_SCL/SPI1_SIN, label: GSM_RX, identifier: GSM_RX}
  - {pin_num: '119', pin_signal: PTC14/UART4_RX/FB_AD25/SDRAM_D25, label: DEBUG_RX, identifier: DEBUG_RX}
  - {pin_num: '120', pin_signal: PTC15/UART4_TX/FB_AD24/SDRAM_D24, label: DEBUG_TX, identifier: DEBUG_TX}
  - {pin_num: '7', pin_signal: PTE4/LLWU_P2/SPI1_PCS0/UART3_TX/SDHC0_D3/TRACE_D0, label: RS485_TX, identifier: RS485_TX}
  - {pin_num: '8', pin_signal: PTE5/SPI1_PCS2/UART3_RX/SDHC0_D2/FTM3_CH0, label: RS485_RX, identifier: RS485_RX}
  - {pin_num: '14', pin_signal: PTE11/I2C3_SCL/I2S0_TX_FS/LPUART0_RTS_b/FTM3_CH6, label: DI6, identifier: DI6}
  - {pin_num: '15', pin_signal: PTE12/I2S0_TX_BCLK/FTM3_CH7, label: DI5, identifier: DI5}
  - {pin_num: '3', pin_signal: ADC1_SE6a/PTE2/LLWU_P1/SPI1_SCK/UART1_CTS_b/SDHC0_DCLK/TRACE_D2, label: MUX_S0, identifier: MUX_S0}
  - {pin_num: '4', pin_signal: ADC1_SE7a/PTE3/SPI1_SIN/UART1_RTS_b/SDHC0_CMD/TRACE_D1/SPI1_SOUT, label: MUX_S2, identifier: MUX_S2}
  - {pin_num: '101', pin_signal: PTB22/SPI2_SOUT/FB_AD29/SDRAM_D29/CMP2_OUT, label: DI4, identifier: DI4}
  - {pin_num: '100', pin_signal: PTB21/SPI2_SCK/FB_AD30/SDRAM_D30/CMP1_OUT, label: DI3, identifier: DI3}
  - {pin_num: '99', pin_signal: PTB20/SPI2_PCS0/FB_AD31/SDRAM_D31/CMP0_OUT, label: DI2, identifier: DI2}
  - {pin_num: '98', pin_signal: TSI0_CH12/PTB19/CAN0_RX/FTM2_CH1/I2S0_TX_FS/FB_OE_b/FTM2_QD_PHB/TPM2_CH1, label: DI1, identifier: DI1}
  - {pin_num: '144', pin_signal: PTD15/SPI2_PCS1/SDHC0_D7/FB_A23, label: MUX_S1, identifier: MUX_S1}
  - {pin_num: '139', pin_signal: PTD10/LPUART0_RTS_b/FB_A18, label: MUX_S3, identifier: MUX_S3}
  - {pin_num: '91', pin_signal: ADC1_SE14/PTB10/SPI1_PCS0/UART3_RX/FB_AD19/SDRAM_D19/FTM0_FLT1, label: ALARM_LIGHT, identifier: ALARM_SPEAKER}
  - {pin_num: '92', pin_signal: ADC1_SE15/PTB11/SPI1_SCK/UART3_TX/FB_AD18/SDRAM_D18/FTM0_FLT2, label: ALARM_LIGHT, identifier: ALARM_LIGHT}
  - {pin_num: '95', pin_signal: TSI0_CH9/PTB16/SPI1_SOUT/UART0_RX/FTM_CLKIN0/FB_AD17/SDRAM_D17/EWM_IN/TPM_CLKIN0, label: WAIT_MCU2, identifier: WAIT_MCU2}
  - {pin_num: '96', pin_signal: TSI0_CH10/PTB17/SPI1_SIN/UART0_TX/FTM_CLKIN1/FB_AD16/SDRAM_D16/EWM_OUT_b/TPM_CLKIN1, label: WAIT_MCU5, identifier: WAIT_MCU5}
  - {pin_num: '97', pin_signal: TSI0_CH11/PTB18/CAN0_TX/FTM2_CH0/I2S0_TX_BCLK/FB_AD15/SDRAM_A23/FTM2_QD_PHA/TPM2_CH0, label: WAIT_MCU3, identifier: WAIT_MCU3}
  - {pin_num: '89', pin_signal: PTB8/UART3_RTS_b/FB_AD21/SDRAM_D21, label: WAIT_MCU1, identifier: WAIT_MCU1}
  - {pin_num: '88', pin_signal: ADC1_SE13/PTB7/FB_AD22/SDRAM_D22, label: WAIT_MCU4, identifier: WAIT_MCU4}
  - {pin_num: '90', pin_signal: PTB9/SPI1_PCS1/UART3_CTS_b/FB_AD20/SDRAM_D20, label: OPENDOOR_MCU, identifier: OPENDOOR_MCU}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

#include "fsl_common.h"
#include "fsl_port.h"
#include "pin_mux.h"
#include "board.h"

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
BOARD_InitPins:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list: []
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : BOARD_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void BOARD_InitPins(void) {
}


#define PIN11_IDX                       11u   /*!< Pin number for pin 11 in a port */
#define PIN12_IDX                       12u   /*!< Pin number for pin 12 in a port */
#define PIN13_IDX                       13u   /*!< Pin number for pin 13 in a port */
#define PIN14_IDX                       14u   /*!< Pin number for pin 14 in a port */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
BOARD_InitSPIs:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: '140', peripheral: SPI2, signal: PCS0_SS, pin_signal: PTD11/LLWU_P25/SPI2_PCS0/SDHC0_CLKIN/LPUART0_CTS_b/FB_A19, direction: OUTPUT}
  - {pin_num: '141', peripheral: SPI2, signal: SCK, pin_signal: PTD12/SPI2_SCK/FTM3_FLT0/SDHC0_D4/FB_A20, direction: OUTPUT}
  - {pin_num: '143', peripheral: SPI2, signal: SIN, pin_signal: PTD14/SPI2_SIN/SDHC0_D6/FB_A22}
  - {pin_num: '142', peripheral: SPI2, signal: SOUT, pin_signal: PTD13/SPI2_SOUT/SDHC0_D5/FB_A21}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : BOARD_InitSPIs
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void BOARD_InitSPIs(void) {
  CLOCK_EnableClock(kCLOCK_PortD);                           /* Port D Clock Gate Control: Clock enabled */

  PORT_SetPinMux(PORTD, PIN11_IDX, kPORT_MuxAlt2);           /* PORTD11 (pin 140) is configured as SPI2_PCS0 */
  PORT_SetPinMux(PORTD, PIN12_IDX, kPORT_MuxAlt2);           /* PORTD12 (pin 141) is configured as SPI2_SCK */
  PORT_SetPinMux(PORTD, PIN13_IDX, kPORT_MuxAlt2);           /* PORTD13 (pin 142) is configured as SPI2_SOUT */
  PORT_SetPinMux(PORTD, PIN14_IDX, kPORT_MuxAlt2);           /* PORTD14 (pin 143) is configured as SPI2_SIN */
}


#define PIN0_IDX                         0u   /*!< Pin number for pin 0 in a port */
#define PIN1_IDX                         1u   /*!< Pin number for pin 1 in a port */
#define PIN2_IDX                         2u   /*!< Pin number for pin 2 in a port */
#define PIN3_IDX                         3u   /*!< Pin number for pin 3 in a port */
#define PIN4_IDX                         4u   /*!< Pin number for pin 4 in a port */
#define PIN5_IDX                         5u   /*!< Pin number for pin 5 in a port */
#define PIN6_IDX                         6u   /*!< Pin number for pin 6 in a port */
#define PIN7_IDX                         7u   /*!< Pin number for pin 7 in a port */
#define PIN8_IDX                         8u   /*!< Pin number for pin 8 in a port */
#define PIN9_IDX                         9u   /*!< Pin number for pin 9 in a port */
#define PIN10_IDX                       10u   /*!< Pin number for pin 10 in a port */
#define PIN12_IDX                       12u   /*!< Pin number for pin 12 in a port */
#define PIN18_IDX                       18u   /*!< Pin number for pin 18 in a port */
#define PIN19_IDX                       19u   /*!< Pin number for pin 19 in a port */
#define PIN26_IDX                       26u   /*!< Pin number for pin 26 in a port */
#define PIN27_IDX                       27u   /*!< Pin number for pin 27 in a port */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
BOARD_InitLCD:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: '132', peripheral: GPIOD, signal: 'GPIO, 5', pin_signal: ADC0_SE6b/PTD5/SPI0_PCS2/UART0_CTS_b/UART0_COL_b/FTM0_CH5/FB_AD1/SDRAM_A9/EWM_OUT_b/SPI1_SCK,
    direction: OUTPUT}
  - {pin_num: '131', peripheral: GPIOD, signal: 'GPIO, 4', pin_signal: PTD4/LLWU_P14/SPI0_PCS1/UART0_RTS_b/FTM0_CH4/FB_AD2/SDRAM_A10/EWM_IN/SPI1_PCS0, direction: OUTPUT}
  - {pin_num: '130', peripheral: GPIOD, signal: 'GPIO, 3', pin_signal: PTD3/SPI0_SIN/UART2_TX/FTM3_CH3/FB_AD3/SDRAM_A11/I2C0_SDA, direction: OUTPUT}
  - {pin_num: '129', peripheral: GPIOD, signal: 'GPIO, 2', pin_signal: PTD2/LLWU_P13/SPI0_SOUT/UART2_RX/FTM3_CH2/FB_AD4/SDRAM_A12/I2C0_SCL, direction: OUTPUT}
  - {pin_num: '128', peripheral: GPIOD, signal: 'GPIO, 1', pin_signal: ADC0_SE5b/PTD1/SPI0_SCK/UART2_CTS_b/FTM3_CH1/FB_CS0_b, direction: OUTPUT}
  - {pin_num: '127', peripheral: GPIOD, signal: 'GPIO, 0', pin_signal: PTD0/LLWU_P12/SPI0_PCS0/UART2_RTS_b/FTM3_CH0/FB_ALE/FB_CS1_b/FB_TS_b, direction: OUTPUT}
  - {pin_num: '114', peripheral: GPIOC, signal: 'GPIO, 9', pin_signal: ADC1_SE5b/CMP0_IN3/PTC9/FTM3_CH5/I2S0_RX_BCLK/FB_AD6/SDRAM_A14/FTM2_FLT0, direction: OUTPUT}
  - {pin_num: '117', peripheral: GPIOC, signal: 'GPIO, 12', pin_signal: PTC12/UART4_RTS_b/FTM_CLKIN0/FB_AD27/SDRAM_D27/FTM3_FLT0/TPM_CLKIN0, direction: OUTPUT}
  - {pin_num: '125', peripheral: GPIOC, signal: 'GPIO, 18', pin_signal: PTC18/UART3_RTS_b/ENET0_1588_TMR2/FB_TBST_b/FB_CS2_b/FB_BE15_8_BLS23_16_b/SDRAM_DQM1, direction: OUTPUT}
  - {pin_num: '126', peripheral: GPIOC, signal: 'GPIO, 19', pin_signal: PTC19/UART3_CTS_b/ENET0_1588_TMR3/FB_CS3_b/FB_BE7_0_BLS31_24_b/SDRAM_DQM0/FB_TA_b, direction: OUTPUT}
  - {pin_num: '9', peripheral: GPIOE, signal: 'GPIO, 6', pin_signal: PTE6/LLWU_P16/SPI1_PCS3/UART3_CTS_b/I2S0_MCLK/FTM3_CH1/USB0_SOF_OUT, direction: OUTPUT}
  - {pin_num: '10', peripheral: GPIOE, signal: 'GPIO, 7', pin_signal: PTE7/UART3_RTS_b/I2S0_RXD0/FTM3_CH2, direction: OUTPUT}
  - {pin_num: '11', peripheral: GPIOE, signal: 'GPIO, 8', pin_signal: PTE8/I2S0_RXD1/I2S0_RX_FS/LPUART0_TX/FTM3_CH3, direction: OUTPUT}
  - {pin_num: '12', peripheral: GPIOE, signal: 'GPIO, 9', pin_signal: PTE9/LLWU_P17/I2S0_TXD1/I2S0_RX_BCLK/LPUART0_RX/FTM3_CH4, direction: OUTPUT}
  - {pin_num: '13', peripheral: GPIOE, signal: 'GPIO, 10', pin_signal: PTE10/LLWU_P18/I2C3_SDA/I2S0_TXD0/LPUART0_CTS_b/FTM3_CH5/USB1_ID, direction: OUTPUT}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : BOARD_InitLCD
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void BOARD_InitLCD(void) {
  CLOCK_EnableClock(kCLOCK_PortC);                           /* Port C Clock Gate Control: Clock enabled */
  CLOCK_EnableClock(kCLOCK_PortD);                           /* Port D Clock Gate Control: Clock enabled */
  CLOCK_EnableClock(kCLOCK_PortE);                           /* Port E Clock Gate Control: Clock enabled */

  PORT_SetPinMux(PORTC, PIN12_IDX, kPORT_MuxAsGpio);         /* PORTC12 (pin 117) is configured as PTC12 */
  PORT_SetPinMux(PORTC, PIN18_IDX, kPORT_MuxAsGpio);         /* PORTC18 (pin 125) is configured as PTC18 */
  PORT_SetPinMux(PORTC, PIN19_IDX, kPORT_MuxAsGpio);         /* PORTC19 (pin 126) is configured as PTC19 */
  PORT_SetPinMux(PORTC, PIN9_IDX, kPORT_MuxAsGpio);          /* PORTC9 (pin 114) is configured as PTC9 */
  PORT_SetPinMux(PORTD, PIN0_IDX, kPORT_MuxAsGpio);          /* PORTD0 (pin 127) is configured as PTD0 */
  PORT_SetPinMux(PORTD, PIN1_IDX, kPORT_MuxAsGpio);          /* PORTD1 (pin 128) is configured as PTD1 */
  PORT_SetPinMux(PORTD, PIN2_IDX, kPORT_MuxAsGpio);          /* PORTD2 (pin 129) is configured as PTD2 */
  PORT_SetPinMux(PORTD, PIN3_IDX, kPORT_MuxAsGpio);          /* PORTD3 (pin 130) is configured as PTD3 */
  PORT_SetPinMux(PORTD, PIN4_IDX, kPORT_MuxAsGpio);          /* PORTD4 (pin 131) is configured as PTD4 */
  PORT_SetPinMux(PORTD, PIN5_IDX, kPORT_MuxAsGpio);          /* PORTD5 (pin 132) is configured as PTD5 */
  PORT_SetPinMux(PORTE, PIN10_IDX, kPORT_MuxAsGpio);         /* PORTE10 (pin 13) is configured as PTE10 */
  PORT_SetPinMux(PORTE, PIN6_IDX, kPORT_MuxAsGpio);          /* PORTE6 (pin 9) is configured as PTE6 */
  PORT_SetPinMux(PORTE, PIN7_IDX, kPORT_MuxAsGpio);          /* PORTE7 (pin 10) is configured as PTE7 */
  PORT_SetPinMux(PORTE, PIN8_IDX, kPORT_MuxAsGpio);          /* PORTE8 (pin 11) is configured as PTE8 */
  PORT_SetPinMux(PORTE, PIN9_IDX, kPORT_MuxAsGpio);          /* PORTE9 (pin 12) is configured as PTE9 */
}


/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
BOARD_InitADCs:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: '36', peripheral: ADC0, signal: 'SE, 21', pin_signal: ADC0_SE16/CMP1_IN2/ADC0_SE21}
  - {pin_num: '35', peripheral: ADC0, signal: 'SE, 22', pin_signal: ADC1_SE16/CMP2_IN2/ADC0_SE22}
  - {pin_num: '38', peripheral: ADC0, signal: 'SE, 23', pin_signal: DAC0_OUT/CMP1_IN3/ADC0_SE23}
  - {pin_num: '37', peripheral: ADC1, signal: 'SE, 18', pin_signal: VREF_OUT/CMP1_IN5/CMP0_IN5/ADC1_SE18}
  - {pin_num: '39', peripheral: ADC1, signal: 'SE, 23', pin_signal: DAC1_OUT/CMP0_IN4/CMP2_IN3/ADC1_SE23}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : BOARD_InitADCs
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void BOARD_InitADCs(void) {
}


#define PIN3_IDX                         3u   /*!< Pin number for pin 3 in a port */
#define PIN4_IDX                         4u   /*!< Pin number for pin 4 in a port */
#define PIN5_IDX                         5u   /*!< Pin number for pin 5 in a port */
#define PIN6_IDX                         6u   /*!< Pin number for pin 6 in a port */
/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
BOARD_InitLEDs:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: '84', peripheral: GPIOB, signal: 'GPIO, 3', pin_signal: ADC0_SE13/TSI0_CH8/PTB3/I2C0_SDA/UART0_CTS_b/UART0_COL_b/ENET0_1588_TMR1/SDRAM_CS0_b/FTM0_FLT0,
    direction: OUTPUT, slew_rate: slow, open_drain: disable, pull_select: down, pull_enable: disable}
  - {pin_num: '85', peripheral: GPIOB, signal: 'GPIO, 4', pin_signal: ADC1_SE10/PTB4/ENET0_1588_TMR2/SDRAM_CS1_b/FTM1_FLT0, direction: OUTPUT, slew_rate: slow, open_drain: disable,
    pull_select: down, pull_enable: disable}
  - {pin_num: '86', peripheral: GPIOB, signal: 'GPIO, 5', pin_signal: ADC1_SE11/PTB5/ENET0_1588_TMR3/FTM2_FLT0, direction: OUTPUT, slew_rate: slow, open_drain: disable,
    pull_select: down, pull_enable: disable}
  - {pin_num: '87', peripheral: GPIOB, signal: 'GPIO, 6', pin_signal: ADC1_SE12/PTB6/FB_AD23/SDRAM_D23, direction: OUTPUT, slew_rate: slow, open_drain: disable, pull_select: down,
    pull_enable: disable}
  - {pin_num: '109', peripheral: GPIOC, signal: 'GPIO, 4', pin_signal: PTC4/LLWU_P8/SPI0_PCS0/UART1_TX/FTM0_CH3/FB_AD11/SDRAM_A19/CMP1_OUT, direction: OUTPUT, slew_rate: slow,
    open_drain: disable, drive_strength: low, pull_select: down, pull_enable: disable}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : BOARD_InitLEDs
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void BOARD_InitLEDs(void) {
  CLOCK_EnableClock(kCLOCK_PortB);                           /* Port B Clock Gate Control: Clock enabled */
  CLOCK_EnableClock(kCLOCK_PortC);                           /* Port C Clock Gate Control: Clock enabled */

  const port_pin_config_t portb3_pin84_config = {
    kPORT_PullDisable,                                       /* Internal pull-up/down resistor is disabled */
    kPORT_SlowSlewRate,                                      /* Slow slew rate is configured */
    kPORT_PassiveFilterDisable,                              /* Passive filter is disabled */
    kPORT_OpenDrainDisable,                                  /* Open drain is disabled */
    kPORT_LowDriveStrength,                                  /* Low drive strength is configured */
    kPORT_MuxAsGpio,                                         /* Pin is configured as PTB3 */
    kPORT_UnlockRegister                                     /* Pin Control Register fields [15:0] are not locked */
  };
  PORT_SetPinConfig(PORTB, PIN3_IDX, &portb3_pin84_config);  /* PORTB3 (pin 84) is configured as PTB3 */
  const port_pin_config_t portb4_pin85_config = {
    kPORT_PullDisable,                                       /* Internal pull-up/down resistor is disabled */
    kPORT_SlowSlewRate,                                      /* Slow slew rate is configured */
    kPORT_PassiveFilterDisable,                              /* Passive filter is disabled */
    kPORT_OpenDrainDisable,                                  /* Open drain is disabled */
    kPORT_LowDriveStrength,                                  /* Low drive strength is configured */
    kPORT_MuxAsGpio,                                         /* Pin is configured as PTB4 */
    kPORT_UnlockRegister                                     /* Pin Control Register fields [15:0] are not locked */
  };
  PORT_SetPinConfig(PORTB, PIN4_IDX, &portb4_pin85_config);  /* PORTB4 (pin 85) is configured as PTB4 */
  const port_pin_config_t portb5_pin86_config = {
    kPORT_PullDisable,                                       /* Internal pull-up/down resistor is disabled */
    kPORT_SlowSlewRate,                                      /* Slow slew rate is configured */
    kPORT_PassiveFilterDisable,                              /* Passive filter is disabled */
    kPORT_OpenDrainDisable,                                  /* Open drain is disabled */
    kPORT_LowDriveStrength,                                  /* Low drive strength is configured */
    kPORT_MuxAsGpio,                                         /* Pin is configured as PTB5 */
    kPORT_UnlockRegister                                     /* Pin Control Register fields [15:0] are not locked */
  };
  PORT_SetPinConfig(PORTB, PIN5_IDX, &portb5_pin86_config);  /* PORTB5 (pin 86) is configured as PTB5 */
  const port_pin_config_t portb6_pin87_config = {
    kPORT_PullDisable,                                       /* Internal pull-up/down resistor is disabled */
    kPORT_SlowSlewRate,                                      /* Slow slew rate is configured */
    kPORT_PassiveFilterDisable,                              /* Passive filter is disabled */
    kPORT_OpenDrainDisable,                                  /* Open drain is disabled */
    kPORT_LowDriveStrength,                                  /* Low drive strength is configured */
    kPORT_MuxAsGpio,                                         /* Pin is configured as PTB6 */
    kPORT_UnlockRegister                                     /* Pin Control Register fields [15:0] are not locked */
  };
  PORT_SetPinConfig(PORTB, PIN6_IDX, &portb6_pin87_config);  /* PORTB6 (pin 87) is configured as PTB6 */
  const port_pin_config_t portc4_pin109_config = {
    kPORT_PullDisable,                                       /* Internal pull-up/down resistor is disabled */
    kPORT_SlowSlewRate,                                      /* Slow slew rate is configured */
    kPORT_PassiveFilterDisable,                              /* Passive filter is disabled */
    kPORT_OpenDrainDisable,                                  /* Open drain is disabled */
    kPORT_LowDriveStrength,                                  /* Low drive strength is configured */
    kPORT_MuxAsGpio,                                         /* Pin is configured as PTC4 */
    kPORT_UnlockRegister                                     /* Pin Control Register fields [15:0] are not locked */
  };
  PORT_SetPinConfig(PORTC, PIN4_IDX, &portc4_pin109_config); /* PORTC4 (pin 109) is configured as PTC4 */
}

/*FUNCTION**********************************************************************
 *
 * Function Name : BOARD_InitGPRS
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void BOARD_InitGPRS(void) {
  CLOCK_EnableClock(kCLOCK_PortA);                           /* Port A Clock Gate Control: Clock enabled */

  const port_pin_config_t porta26_pin77_config = {
    kPORT_PullDisable,                                       /* Internal pull-up/down resistor is disabled */
    kPORT_SlowSlewRate,                                      /* Slow slew rate is configured */
    kPORT_PassiveFilterDisable,                              /* Passive filter is disabled */
    kPORT_OpenDrainDisable,                                  /* Open drain is disabled */
    kPORT_LowDriveStrength,                                  /* Low drive strength is configured */
    kPORT_MuxAsGpio,                                         /* Pin is configured as PTB3 */
    kPORT_UnlockRegister                                     /* Pin Control Register fields [15:0] are not locked */
  };
  PORT_SetPinConfig(PORTA, PIN26_IDX, &porta26_pin77_config);  /* PORTB3 (pin 84) is configured as PTB3 */
  const port_pin_config_t porta27_pin78_config = {
    kPORT_PullDisable,                                       /* Internal pull-up/down resistor is disabled */
    kPORT_SlowSlewRate,                                      /* Slow slew rate is configured */
    kPORT_PassiveFilterDisable,                              /* Passive filter is disabled */
    kPORT_OpenDrainDisable,                                  /* Open drain is disabled */
    kPORT_LowDriveStrength,                                  /* Low drive strength is configured */
    kPORT_MuxAsGpio,                                         /* Pin is configured as PTB4 */
    kPORT_UnlockRegister                                     /* Pin Control Register fields [15:0] are not locked */
  };
  PORT_SetPinConfig(PORTA, PIN27_IDX, &porta27_pin78_config);  /* PORTB4 (pin 85) is configured as PTB4 */
}
#define PIN5_IDX                         5u   /*!< Pin number for pin 5 in a port */
#define PIN7_IDX                         7u   /*!< Pin number for pin 7 in a port */
#define PIN8_IDX                         8u   /*!< Pin number for pin 8 in a port */
#define PIN12_IDX                       12u   /*!< Pin number for pin 12 in a port */
#define PIN13_IDX                       13u   /*!< Pin number for pin 13 in a port */
#define PIN14_IDX                       14u   /*!< Pin number for pin 14 in a port */
#define PIN15_IDX                       15u   /*!< Pin number for pin 15 in a port */
#define PIN16_IDX                       16u   /*!< Pin number for pin 16 in a port */
#define PIN17_IDX                       17u   /*!< Pin number for pin 17 in a port */
#define PIN26_IDX                       26u   /*!< Pin number for pin 26 in a port */
#define SOPT2_RMIISRC_ENET            0x01u   /*!< RMII clock source select: External bypass clock (ENET_1588_CLKIN). */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
BOARD_InitENET:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: '47', peripheral: ENET, signal: CLKIN_1588, pin_signal: PTE26/ENET_1588_CLKIN/UART4_CTS_b/RTC_CLKOUT/USB0_CLKIN}
  - {pin_num: '47', peripheral: ENET, signal: RMII_CLKIN, pin_signal: PTE26/ENET_1588_CLKIN/UART4_CTS_b/RTC_CLKOUT/USB0_CLKIN, identifier: ''}
  - {pin_num: '60', peripheral: ENET, signal: RMII_MDC, pin_signal: ADC0_SE11/PTA8/FTM1_CH0/RMII0_MDC/MII0_MDC/FTM1_QD_PHA/TPM1_CH0/TRACE_D2}
  - {pin_num: '59', peripheral: ENET, signal: RMII_MDIO, pin_signal: ADC0_SE10/PTA7/FTM0_CH4/RMII0_MDIO/MII0_MDIO/TRACE_D3}
  - {pin_num: '55', peripheral: ENET, signal: RMII_RXER, pin_signal: PTA5/USB0_CLKIN/FTM0_CH2/RMII0_RXER/MII0_RXER/CMP2_OUT/I2S0_TX_BCLK/JTAG_TRST_b}
  - {pin_num: '65', peripheral: ENET, signal: RMII_RXD0, pin_signal: CMP2_IN1/PTA13/LLWU_P4/CAN0_RX/FTM1_CH1/RMII0_RXD0/MII0_RXD0/I2C2_SDA/I2S0_TX_FS/FTM1_QD_PHB/TPM1_CH1}
  - {pin_num: '69', peripheral: ENET, signal: RMII_TXD1, pin_signal: ADC1_SE17/PTA17/SPI0_SIN/UART0_RTS_b/RMII0_TXD1/MII0_TXD1/I2S0_MCLK}
  - {pin_num: '67', peripheral: ENET, signal: RMII_TXEN, pin_signal: CMP3_IN1/PTA15/SPI0_SCK/UART0_RX/RMII0_TXEN/MII0_TXEN/I2S0_RXD0}
  - {pin_num: '64', peripheral: ENET, signal: RMII_RXD1, pin_signal: CMP2_IN0/PTA12/CAN0_TX/FTM1_CH0/RMII0_RXD1/MII0_RXD1/I2C2_SCL/I2S0_TXD0/FTM1_QD_PHA/TPM1_CH0}
  - {pin_num: '68', peripheral: ENET, signal: RMII_TXD0, pin_signal: CMP3_IN2/PTA16/SPI0_SOUT/UART0_CTS_b/UART0_COL_b/RMII0_TXD0/MII0_TXD0/I2S0_RX_FS/I2S0_RXD1}
  - {pin_num: '66', peripheral: ENET, signal: RMII_CRS_DV, pin_signal: PTA14/SPI0_PCS0/UART0_TX/RMII0_CRS_DV/MII0_RXDV/I2C2_SCL/I2S0_RX_BCLK/I2S0_TXD1}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : BOARD_InitENET
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void BOARD_InitENET(void) {
  CLOCK_EnableClock(kCLOCK_PortA);                           /* Port A Clock Gate Control: Clock enabled */
  CLOCK_EnableClock(kCLOCK_PortE);                           /* Port E Clock Gate Control: Clock enabled */

  PORT_SetPinMux(PORTA, PIN12_IDX, kPORT_MuxAlt4);           /* PORTA12 (pin 64) is configured as RMII0_RXD1 */
  PORT_SetPinMux(PORTA, PIN13_IDX, kPORT_MuxAlt4);           /* PORTA13 (pin 65) is configured as RMII0_RXD0 */
  PORT_SetPinMux(PORTA, PIN14_IDX, kPORT_MuxAlt4);           /* PORTA14 (pin 66) is configured as RMII0_CRS_DV */
  PORT_SetPinMux(PORTA, PIN15_IDX, kPORT_MuxAlt4);           /* PORTA15 (pin 67) is configured as RMII0_TXEN */
  PORT_SetPinMux(PORTA, PIN16_IDX, kPORT_MuxAlt4);           /* PORTA16 (pin 68) is configured as RMII0_TXD0 */
  PORT_SetPinMux(PORTA, PIN17_IDX, kPORT_MuxAlt4);           /* PORTA17 (pin 69) is configured as RMII0_TXD1 */
  PORT_SetPinMux(PORTA, PIN5_IDX, kPORT_MuxAlt4);            /* PORTA5 (pin 55) is configured as RMII0_RXER */
  PORT_SetPinMux(PORTA, PIN7_IDX, kPORT_MuxAlt5);            /* PORTA7 (pin 59) is configured as RMII0_MDIO */
  PORT_SetPinMux(PORTA, PIN8_IDX, kPORT_MuxAlt5);            /* PORTA8 (pin 60) is configured as RMII0_MDC */
  PORT_SetPinMux(PORTE, PIN26_IDX, kPORT_MuxAlt2);           /* PORTE26 (pin 47) is configured as ENET_1588_CLKIN */
  SIM->SOPT2 = ((SIM->SOPT2 &
    (~(SIM_SOPT2_RMIISRC_MASK)))                             /* Mask bits to zero which are setting */
      | SIM_SOPT2_RMIISRC(SOPT2_RMIISRC_ENET)                /* RMII clock source select: External bypass clock (ENET_1588_CLKIN). */
    );
}


#define PIN5_IDX                         5u   /*!< Pin number for pin 5 in a port */
#define PIN6_IDX                         6u   /*!< Pin number for pin 6 in a port */
#define PIN7_IDX                         7u   /*!< Pin number for pin 7 in a port */
#define PIN8_IDX                         8u   /*!< Pin number for pin 8 in a port */
/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
BOARD_InitBUTTONs:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: '110', peripheral: GPIOC, signal: 'GPIO, 5', pin_signal: PTC5/LLWU_P9/SPI0_SCK/LPTMR0_ALT2/I2S0_RXD0/FB_AD10/SDRAM_A18/CMP0_OUT/FTM0_CH2, direction: INPUT,
    slew_rate: fast, open_drain: disable, pull_select: up, pull_enable: enable}
  - {pin_num: '111', peripheral: GPIOC, signal: 'GPIO, 6', pin_signal: CMP0_IN0/PTC6/LLWU_P10/SPI0_SOUT/PDB0_EXTRG/I2S0_RX_BCLK/FB_AD9/SDRAM_A17/I2S0_MCLK, direction: INPUT,
    slew_rate: fast, open_drain: disable, pull_select: up, pull_enable: enable}
  - {pin_num: '112', peripheral: GPIOC, signal: 'GPIO, 7', pin_signal: CMP0_IN1/PTC7/SPI0_SIN/USB0_SOF_OUT/I2S0_RX_FS/FB_AD8/SDRAM_A16, direction: INPUT, slew_rate: fast,
    open_drain: disable, pull_select: up, pull_enable: enable}
  - {pin_num: '113', peripheral: GPIOC, signal: 'GPIO, 8', pin_signal: ADC1_SE4b/CMP0_IN2/PTC8/FTM3_CH4/I2S0_MCLK/FB_AD7/SDRAM_A15, direction: INPUT, slew_rate: fast,
    open_drain: disable, pull_select: up, pull_enable: enable}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : BOARD_InitBUTTONs
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void BOARD_InitBUTTONs(void) {
  CLOCK_EnableClock(kCLOCK_PortC);                           /* Port C Clock Gate Control: Clock enabled */

  const port_pin_config_t portc5_pin110_config = {
    kPORT_PullUp,                                            /* Internal pull-up resistor is enabled */
    kPORT_FastSlewRate,                                      /* Fast slew rate is configured */
    kPORT_PassiveFilterDisable,                              /* Passive filter is disabled */
    kPORT_OpenDrainDisable,                                  /* Open drain is disabled */
    kPORT_LowDriveStrength,                                  /* Low drive strength is configured */
    kPORT_MuxAsGpio,                                         /* Pin is configured as PTC5 */
    kPORT_UnlockRegister                                     /* Pin Control Register fields [15:0] are not locked */
  };
  PORT_SetPinConfig(PORTC, PIN5_IDX, &portc5_pin110_config); /* PORTC5 (pin 110) is configured as PTC5 */
  const port_pin_config_t portc6_pin111_config = {
    kPORT_PullUp,                                            /* Internal pull-up resistor is enabled */
    kPORT_FastSlewRate,                                      /* Fast slew rate is configured */
    kPORT_PassiveFilterDisable,                              /* Passive filter is disabled */
    kPORT_OpenDrainDisable,                                  /* Open drain is disabled */
    kPORT_LowDriveStrength,                                  /* Low drive strength is configured */
    kPORT_MuxAsGpio,                                         /* Pin is configured as PTC6 */
    kPORT_UnlockRegister                                     /* Pin Control Register fields [15:0] are not locked */
  };
  PORT_SetPinConfig(PORTC, PIN6_IDX, &portc6_pin111_config); /* PORTC6 (pin 111) is configured as PTC6 */
  const port_pin_config_t portc7_pin112_config = {
    kPORT_PullUp,                                            /* Internal pull-up resistor is enabled */
    kPORT_FastSlewRate,                                      /* Fast slew rate is configured */
    kPORT_PassiveFilterDisable,                              /* Passive filter is disabled */
    kPORT_OpenDrainDisable,                                  /* Open drain is disabled */
    kPORT_LowDriveStrength,                                  /* Low drive strength is configured */
    kPORT_MuxAsGpio,                                         /* Pin is configured as PTC7 */
    kPORT_UnlockRegister                                     /* Pin Control Register fields [15:0] are not locked */
  };
  PORT_SetPinConfig(PORTC, PIN7_IDX, &portc7_pin112_config); /* PORTC7 (pin 112) is configured as PTC7 */
  const port_pin_config_t portc8_pin113_config = {
    kPORT_PullUp,                                            /* Internal pull-up resistor is enabled */
    kPORT_FastSlewRate,                                      /* Fast slew rate is configured */
    kPORT_PassiveFilterDisable,                              /* Passive filter is disabled */
    kPORT_OpenDrainDisable,                                  /* Open drain is disabled */
    kPORT_LowDriveStrength,                                  /* Low drive strength is configured */
    kPORT_MuxAsGpio,                                         /* Pin is configured as PTC8 */
    kPORT_UnlockRegister                                     /* Pin Control Register fields [15:0] are not locked */
  };
  PORT_SetPinConfig(PORTC, PIN8_IDX, &portc8_pin113_config); /* PORTC8 (pin 113) is configured as PTC8 */
}


#define PIN8_IDX                         8u   /*!< Pin number for pin 8 in a port */
#define PIN9_IDX                         9u   /*!< Pin number for pin 9 in a port */
#define PORT_DFER_DFE_8_MASK        0x0100u   /*!< Digital Filter Enable Mask for item 8. */
#define PORT_DFER_DFE_9_MASK        0x0200u   /*!< Digital Filter Enable Mask for item 9. */
#define PCR_PE_ENABLED                0x01u   /*!< Pull Enable: Internal pullup or pulldown resistor is enabled on the corresponding pin, if the pin is configured as a digital input. */
#define PCR_PS_UP                     0x01u   /*!< Pull Select: Internal pullup resistor is enabled on the corresponding pin, if the corresponding PE field is set. */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
BOARD_InitI2C:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: '137', peripheral: I2C0, signal: SCL, pin_signal: PTD8/LLWU_P24/I2C0_SCL/LPUART0_RX/FB_A16, slew_rate: fast, open_drain: enable, pull_select: down,
    pull_enable: disable, digital_filter: disable}
  - {pin_num: '138', peripheral: I2C0, signal: SDA, pin_signal: PTD9/I2C0_SDA/LPUART0_TX/FB_A17, slew_rate: fast, open_drain: enable, pull_select: down, pull_enable: disable,
    digital_filter: disable}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : BOARD_InitI2C
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void BOARD_InitI2C(void) {
  CLOCK_EnableClock(kCLOCK_PortD);                           /* Port D Clock Gate Control: Clock enabled */

  PORT_SetPinMux(PORTD, PIN8_IDX, kPORT_MuxAsGpio);          /* PORTD8 (pin 137) is configured as PTD8 */
  PORTD->PCR[8] = ((PORTD->PCR[8] &
    (~(PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_ISF_MASK))) /* Mask bits to zero which are setting */
      | PORT_PCR_PS(PCR_PS_UP)                               /* Pull Select: Internal pullup resistor is enabled on the corresponding pin, if the corresponding PE field is set. */
      | PORT_PCR_PE(PCR_PE_ENABLED)                          /* Pull Enable: Internal pullup or pulldown resistor is enabled on the corresponding pin, if the pin is configured as a digital input. */
    );
  PORT_SetPinMux(PORTD, PIN9_IDX, kPORT_MuxAsGpio);          /* PORTD9 (pin 138) is configured as PTD9 */
  PORTD->PCR[9] = ((PORTD->PCR[9] &
    (~(PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_ISF_MASK))) /* Mask bits to zero which are setting */
      | PORT_PCR_PS(PCR_PS_UP)                               /* Pull Select: Internal pullup resistor is enabled on the corresponding pin, if the corresponding PE field is set. */
      | PORT_PCR_PE(PCR_PE_ENABLED)                          /* Pull Enable: Internal pullup or pulldown resistor is enabled on the corresponding pin, if the pin is configured as a digital input. */
    );

//  PORT_EnablePinsDigitalFilter(                              /* Configure digital filter */
//    PORTD,                                                   /* Digital filter is configured on port D */
//      PORT_DFER_DFE_8_MASK                                   /* Digital filter is configured for PORTD0 */
//    | PORT_DFER_DFE_9_MASK,                                  /* Digital filter is configured for PORTD1 */
//    false                                                    /* Disable digital filter */
//  );
//  const port_pin_config_t portd8_pin137_config = {
//    kPORT_PullDisable,                                       /* Internal pull-up/down resistor is disabled */
//    kPORT_FastSlewRate,                                      /* Fast slew rate is configured */
//    kPORT_PassiveFilterDisable,                              /* Passive filter is disabled */
//    kPORT_OpenDrainEnable,                                   /* Open drain is enabled */
//    kPORT_LowDriveStrength,                                  /* Low drive strength is configured */
//    kPORT_MuxAlt2,                                           /* Pin is configured as I2C0_SCL */
//    kPORT_UnlockRegister                                     /* Pin Control Register fields [15:0] are not locked */
//  };
//  PORT_SetPinConfig(PORTD, PIN8_IDX, &portd8_pin137_config); /* PORTD8 (pin 137) is configured as I2C0_SCL */
//  const port_pin_config_t portd9_pin138_config = {
//    kPORT_PullDisable,                                       /* Internal pull-up/down resistor is disabled */
//    kPORT_FastSlewRate,                                      /* Fast slew rate is configured */
//    kPORT_PassiveFilterDisable,                              /* Passive filter is disabled */
//    kPORT_OpenDrainEnable,                                   /* Open drain is enabled */
//    kPORT_LowDriveStrength,                                  /* Low drive strength is configured */
//    kPORT_MuxAlt2,                                           /* Pin is configured as I2C0_SDA */
//    kPORT_UnlockRegister                                     /* Pin Control Register fields [15:0] are not locked */
//  };
//  PORT_SetPinConfig(PORTD, PIN9_IDX, &portd9_pin138_config); /* PORTD9 (pin 138) is configured as I2C0_SDA */
}


#define PIN0_IDX                         0u   /*!< Pin number for pin 0 in a port */
#define PIN1_IDX                         1u   /*!< Pin number for pin 1 in a port */
#define PIN4_IDX                         4u   /*!< Pin number for pin 4 in a port */
#define PIN5_IDX                         5u   /*!< Pin number for pin 5 in a port */
#define PIN14_IDX                       14u   /*!< Pin number for pin 14 in a port */
#define PIN15_IDX                       15u   /*!< Pin number for pin 15 in a port */
#define PIN24_IDX                       24u   /*!< Pin number for pin 14 in a port */
#define PIN25_IDX                       25u   /*!< Pin number for pin 15 in a port */
#define SOPT5_UART1TXSRC_UART_TX      0x00u   /*!< UART 1 transmit data source select: UART1_TX pin */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
BOARD_InitUARTs:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: '1', peripheral: UART1, signal: TX, pin_signal: ADC1_SE4a/PTE0/SPI1_PCS1/UART1_TX/SDHC0_D1/TRACE_CLKOUT/I2C1_SDA/RTC_CLKOUT, direction: OUTPUT, slew_rate: fast,
    open_drain: disable, pull_select: down, pull_enable: disable}
  - {pin_num: '2', peripheral: UART1, signal: RX, pin_signal: ADC1_SE5a/PTE1/LLWU_P0/SPI1_SOUT/UART1_RX/SDHC0_D0/TRACE_D3/I2C1_SCL/SPI1_SIN, slew_rate: fast, open_drain: disable,
    pull_select: down, pull_enable: disable}
  - {pin_num: '120', peripheral: UART4, signal: TX, pin_signal: PTC15/UART4_TX/FB_AD24/SDRAM_D24, direction: OUTPUT, slew_rate: fast, open_drain: disable, pull_select: down,
    pull_enable: disable}
  - {pin_num: '119', peripheral: UART4, signal: RX, pin_signal: PTC14/UART4_RX/FB_AD25/SDRAM_D25, slew_rate: fast, open_drain: disable, pull_select: down, pull_enable: disable}
  - {pin_num: '7', peripheral: UART3, signal: TX, pin_signal: PTE4/LLWU_P2/SPI1_PCS0/UART3_TX/SDHC0_D3/TRACE_D0, direction: OUTPUT, slew_rate: fast, open_drain: disable,
    pull_select: down, pull_enable: disable}
  - {pin_num: '8', peripheral: UART3, signal: RX, pin_signal: PTE5/SPI1_PCS2/UART3_RX/SDHC0_D2/FTM3_CH0, slew_rate: fast, open_drain: disable, pull_select: down,
    pull_enable: disable}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : BOARD_InitUARTs
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void BOARD_InitUARTs(void) {
  CLOCK_EnableClock(kCLOCK_PortC);                           /* Port C Clock Gate Control: Clock enabled */
  CLOCK_EnableClock(kCLOCK_PortE);                           /* Port E Clock Gate Control: Clock enabled */
//#if (USERDEF_DEBUG_USING == ENABLED) 
#ifdef DEBUG_CONSOLE_UART4
  const port_pin_config_t portc14_pin119_config = {
    kPORT_PullDisable,                                       /* Internal pull-up/down resistor is disabled */
    kPORT_FastSlewRate,                                      /* Fast slew rate is configured */
    kPORT_PassiveFilterDisable,                              /* Passive filter is disabled */
    kPORT_OpenDrainDisable,                                  /* Open drain is disabled */
    kPORT_LowDriveStrength,                                  /* Low drive strength is configured */
    kPORT_MuxAlt3,                                           /* Pin is configured as UART4_RX */
    kPORT_UnlockRegister                                     /* Pin Control Register fields [15:0] are not locked */
  };
  PORT_SetPinConfig(PORTC, PIN14_IDX, &portc14_pin119_config); /* PORTC14 (pin 119) is configured as UART4_RX */
  const port_pin_config_t portc15_pin120_config = {
    kPORT_PullDisable,                                       /* Internal pull-up/down resistor is disabled */
    kPORT_FastSlewRate,                                      /* Fast slew rate is configured */
    kPORT_PassiveFilterDisable,                              /* Passive filter is disabled */
    kPORT_OpenDrainDisable,                                  /* Open drain is disabled */
    kPORT_LowDriveStrength,                                  /* Low drive strength is configured */
    kPORT_MuxAlt3,                                           /* Pin is configured as UART4_TX */
    kPORT_UnlockRegister                                     /* Pin Control Register fields [15:0] are not locked */
  };
  PORT_SetPinConfig(PORTC, PIN15_IDX, &portc15_pin120_config); /* PORTC15 (pin 120) is configured as UART4_TX */
//#elif (USERDEF_DEBUG_USING == DISABLED)
#else
  PORT_SetPinMux(PORTE, PIN24_IDX, kPORT_MuxAlt3);           /* PORTE24 (pin 45) is configured as UART4_TX */
  PORT_SetPinMux(PORTE, PIN25_IDX, kPORT_MuxAlt3);           /* PORTE25 (pin 46) is configured as UART4_RX */  
#endif
  
  const port_pin_config_t porte0_pin1_config = {
    kPORT_PullDisable,                                       /* Internal pull-up/down resistor is disabled */
    kPORT_FastSlewRate,                                      /* Fast slew rate is configured */
    kPORT_PassiveFilterDisable,                              /* Passive filter is disabled */
    kPORT_OpenDrainDisable,                                  /* Open drain is disabled */
    kPORT_LowDriveStrength,                                  /* Low drive strength is configured */
    kPORT_MuxAlt3,                                           /* Pin is configured as UART1_TX */
    kPORT_UnlockRegister                                     /* Pin Control Register fields [15:0] are not locked */
  };
  PORT_SetPinConfig(PORTE, PIN0_IDX, &porte0_pin1_config);   /* PORTE0 (pin 1) is configured as UART1_TX */
  const port_pin_config_t porte1_pin2_config = {
    kPORT_PullDisable,                                       /* Internal pull-up/down resistor is disabled */
    kPORT_FastSlewRate,                                      /* Fast slew rate is configured */
    kPORT_PassiveFilterDisable,                              /* Passive filter is disabled */
    kPORT_OpenDrainDisable,                                  /* Open drain is disabled */
    kPORT_LowDriveStrength,                                  /* Low drive strength is configured */
    kPORT_MuxAlt3,                                           /* Pin is configured as UART1_RX */
    kPORT_UnlockRegister                                     /* Pin Control Register fields [15:0] are not locked */
  };
  PORT_SetPinConfig(PORTE, PIN1_IDX, &porte1_pin2_config);   /* PORTE1 (pin 2) is configured as UART1_RX */
  const port_pin_config_t porte4_pin7_config = {
    kPORT_PullDisable,                                       /* Internal pull-up/down resistor is disabled */
    kPORT_FastSlewRate,                                      /* Fast slew rate is configured */
    kPORT_PassiveFilterDisable,                              /* Passive filter is disabled */
    kPORT_OpenDrainDisable,                                  /* Open drain is disabled */
    kPORT_LowDriveStrength,                                  /* Low drive strength is configured */
    kPORT_MuxAlt3,                                           /* Pin is configured as UART3_TX */
    kPORT_UnlockRegister                                     /* Pin Control Register fields [15:0] are not locked */
  };
  PORT_SetPinConfig(PORTE, PIN4_IDX, &porte4_pin7_config);   /* PORTE4 (pin 7) is configured as UART3_TX */
  const port_pin_config_t porte5_pin8_config = {
    kPORT_PullDisable,                                       /* Internal pull-up/down resistor is disabled */
    kPORT_FastSlewRate,                                      /* Fast slew rate is configured */
    kPORT_PassiveFilterDisable,                              /* Passive filter is disabled */
    kPORT_OpenDrainDisable,                                  /* Open drain is disabled */
    kPORT_LowDriveStrength,                                  /* Low drive strength is configured */
    kPORT_MuxAlt3,                                           /* Pin is configured as UART3_RX */
    kPORT_UnlockRegister                                     /* Pin Control Register fields [15:0] are not locked */
  };
  PORT_SetPinConfig(PORTE, PIN5_IDX, &porte5_pin8_config);   /* PORTE5 (pin 8) is configured as UART3_RX */
  SIM->SOPT5 = ((SIM->SOPT5 &
    (~(SIM_SOPT5_UART1TXSRC_MASK)))                          /* Mask bits to zero which are setting */
      | SIM_SOPT5_UART1TXSRC(SOPT5_UART1TXSRC_UART_TX)       /* UART 1 transmit data source select: UART1_TX pin */
    );
}


#define PIN2_IDX                         2u   /*!< Pin number for pin 2 in a port */
#define PIN3_IDX                         3u   /*!< Pin number for pin 3 in a port */
#define PIN7_IDX                         7u   /*!< Pin number for pin 7 in a port */
#define PIN8_IDX                         8u   /*!< Pin number for pin 8 in a port */
#define PIN9_IDX                         9u   /*!< Pin number for pin 9 in a port */
#define PIN10_IDX                       10u   /*!< Pin number for pin 10 in a port */
#define PIN11_IDX                       11u   /*!< Pin number for pin 11 in a port */
#define PIN12_IDX                       12u   /*!< Pin number for pin 12 in a port */
#define PIN15_IDX                       15u   /*!< Pin number for pin 15 in a port */
#define PIN16_IDX                       16u   /*!< Pin number for pin 16 in a port */
#define PIN17_IDX                       17u   /*!< Pin number for pin 17 in a port */
#define PIN18_IDX                       18u   /*!< Pin number for pin 18 in a port */
#define PIN19_IDX                       19u   /*!< Pin number for pin 19 in a port */
#define PIN20_IDX                       20u   /*!< Pin number for pin 20 in a port */
#define PIN21_IDX                       21u   /*!< Pin number for pin 21 in a port */
#define PIN22_IDX                       22u   /*!< Pin number for pin 22 in a port */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
BOARD_InitIOs:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: '144', peripheral: GPIOD, signal: 'GPIO, 15', pin_signal: PTD15/SPI2_PCS1/SDHC0_D7/FB_A23, direction: OUTPUT}
  - {pin_num: '139', peripheral: GPIOD, signal: 'GPIO, 10', pin_signal: PTD10/LPUART0_RTS_b/FB_A18, direction: OUTPUT}
  - {pin_num: '3', peripheral: GPIOE, signal: 'GPIO, 2', pin_signal: ADC1_SE6a/PTE2/LLWU_P1/SPI1_SCK/UART1_CTS_b/SDHC0_DCLK/TRACE_D2, direction: OUTPUT}
  - {pin_num: '4', peripheral: GPIOE, signal: 'GPIO, 3', pin_signal: ADC1_SE7a/PTE3/SPI1_SIN/UART1_RTS_b/SDHC0_CMD/TRACE_D1/SPI1_SOUT, direction: OUTPUT}
  - {pin_num: '14', peripheral: GPIOE, signal: 'GPIO, 11', pin_signal: PTE11/I2C3_SCL/I2S0_TX_FS/LPUART0_RTS_b/FTM3_CH6, direction: INPUT}
  - {pin_num: '15', peripheral: GPIOE, signal: 'GPIO, 12', pin_signal: PTE12/I2S0_TX_BCLK/FTM3_CH7, direction: INPUT}
  - {pin_num: '101', peripheral: GPIOB, signal: 'GPIO, 22', pin_signal: PTB22/SPI2_SOUT/FB_AD29/SDRAM_D29/CMP2_OUT, direction: INPUT}
  - {pin_num: '100', peripheral: GPIOB, signal: 'GPIO, 21', pin_signal: PTB21/SPI2_SCK/FB_AD30/SDRAM_D30/CMP1_OUT, direction: INPUT}
  - {pin_num: '99', peripheral: GPIOB, signal: 'GPIO, 20', pin_signal: PTB20/SPI2_PCS0/FB_AD31/SDRAM_D31/CMP0_OUT, direction: INPUT}
  - {pin_num: '98', peripheral: GPIOB, signal: 'GPIO, 19', pin_signal: TSI0_CH12/PTB19/CAN0_RX/FTM2_CH1/I2S0_TX_FS/FB_OE_b/FTM2_QD_PHB/TPM2_CH1, direction: INPUT}
  - {pin_num: '91', peripheral: GPIOB, signal: 'GPIO, 10', pin_signal: ADC1_SE14/PTB10/SPI1_PCS0/UART3_RX/FB_AD19/SDRAM_D19/FTM0_FLT1, direction: OUTPUT}
  - {pin_num: '92', peripheral: GPIOB, signal: 'GPIO, 11', pin_signal: ADC1_SE15/PTB11/SPI1_SCK/UART3_TX/FB_AD18/SDRAM_D18/FTM0_FLT2, direction: OUTPUT}
  - {pin_num: '89', peripheral: GPIOB, signal: 'GPIO, 8', pin_signal: PTB8/UART3_RTS_b/FB_AD21/SDRAM_D21, direction: OUTPUT}
  - {pin_num: '95', peripheral: GPIOB, signal: 'GPIO, 16', pin_signal: TSI0_CH9/PTB16/SPI1_SOUT/UART0_RX/FTM_CLKIN0/FB_AD17/SDRAM_D17/EWM_IN/TPM_CLKIN0, direction: OUTPUT}
  - {pin_num: '97', peripheral: GPIOB, signal: 'GPIO, 18', pin_signal: TSI0_CH11/PTB18/CAN0_TX/FTM2_CH0/I2S0_TX_BCLK/FB_AD15/SDRAM_A23/FTM2_QD_PHA/TPM2_CH0, direction: OUTPUT}
  - {pin_num: '88', peripheral: GPIOB, signal: 'GPIO, 7', pin_signal: ADC1_SE13/PTB7/FB_AD22/SDRAM_D22, direction: OUTPUT}
  - {pin_num: '96', peripheral: GPIOB, signal: 'GPIO, 17', pin_signal: TSI0_CH10/PTB17/SPI1_SIN/UART0_TX/FTM_CLKIN1/FB_AD16/SDRAM_D16/EWM_OUT_b/TPM_CLKIN1, direction: OUTPUT}
  - {pin_num: '90', peripheral: GPIOB, signal: 'GPIO, 9', pin_signal: PTB9/SPI1_PCS1/UART3_CTS_b/FB_AD20/SDRAM_D20, direction: OUTPUT}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : BOARD_InitIOs
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void BOARD_InitIOs(void) {
  CLOCK_EnableClock(kCLOCK_PortB);                           /* Port B Clock Gate Control: Clock enabled */
  CLOCK_EnableClock(kCLOCK_PortD);                           /* Port D Clock Gate Control: Clock enabled */
  CLOCK_EnableClock(kCLOCK_PortE);                           /* Port E Clock Gate Control: Clock enabled */

  PORT_SetPinMux(PORTB, PIN10_IDX, kPORT_MuxAsGpio);         /* PORTB10 (pin 91) is configured as PTB10 */
  PORT_SetPinMux(PORTB, PIN11_IDX, kPORT_MuxAsGpio);         /* PORTB11 (pin 92) is configured as PTB11 */
  PORT_SetPinMux(PORTB, PIN16_IDX, kPORT_MuxAsGpio);         /* PORTB16 (pin 95) is configured as PTB16 */
  PORT_SetPinMux(PORTB, PIN17_IDX, kPORT_MuxAsGpio);         /* PORTB17 (pin 96) is configured as PTB17 */
  PORT_SetPinMux(PORTB, PIN18_IDX, kPORT_MuxAsGpio);         /* PORTB18 (pin 97) is configured as PTB18 */
  PORT_SetPinMux(PORTB, PIN19_IDX, kPORT_MuxAsGpio);         /* PORTB19 (pin 98) is configured as PTB19 */
  PORT_SetPinMux(PORTB, PIN20_IDX, kPORT_MuxAsGpio);         /* PORTB20 (pin 99) is configured as PTB20 */
  PORT_SetPinMux(PORTB, PIN21_IDX, kPORT_MuxAsGpio);         /* PORTB21 (pin 100) is configured as PTB21 */
  PORT_SetPinMux(PORTB, PIN22_IDX, kPORT_MuxAsGpio);         /* PORTB22 (pin 101) is configured as PTB22 */
  PORT_SetPinMux(PORTB, PIN7_IDX, kPORT_MuxAsGpio);          /* PORTB7 (pin 88) is configured as PTB7 */
  PORT_SetPinMux(PORTB, PIN8_IDX, kPORT_MuxAsGpio);          /* PORTB8 (pin 89) is configured as PTB8 */
  PORT_SetPinMux(PORTB, PIN9_IDX, kPORT_MuxAsGpio);          /* PORTB9 (pin 90) is configured as PTB9 */
  PORT_SetPinMux(PORTD, PIN10_IDX, kPORT_MuxAsGpio);         /* PORTD10 (pin 139) is configured as PTD10 */
  PORT_SetPinMux(PORTD, PIN15_IDX, kPORT_MuxAsGpio);         /* PORTD15 (pin 144) is configured as PTD15 */
  PORT_SetPinMux(PORTE, PIN11_IDX, kPORT_MuxAsGpio);         /* PORTE11 (pin 14) is configured as PTE11 */
  PORT_SetPinMux(PORTE, PIN12_IDX, kPORT_MuxAsGpio);         /* PORTE12 (pin 15) is configured as PTE12 */
  PORT_SetPinMux(PORTE, PIN2_IDX, kPORT_MuxAsGpio);          /* PORTE2 (pin 3) is configured as PTE2 */
  PORT_SetPinMux(PORTE, PIN3_IDX, kPORT_MuxAsGpio);          /* PORTE3 (pin 4) is configured as PTE3 */
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
