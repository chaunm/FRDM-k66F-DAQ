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

#ifndef _PIN_MUX_H_
#define _PIN_MUX_H_


/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief Direction type  */
typedef enum _pin_mux_direction
{
  kPIN_MUX_DirectionInput = 0U,         /* Input direction */
  kPIN_MUX_DirectionOutput = 1U,        /* Output direction */
  kPIN_MUX_DirectionInputOrOutput = 2U  /* Input or output direction */
} pin_mux_direction_t;

/*!
 * @addtogroup pin_mux
 * @{
 */

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitPins(void);

/* PORTD11 (number 140), SPI2_CS */
#define BOARD_INITSPIS_SPI2_CS_PERIPHERAL                                   SPI2   /*!< Device name: SPI2 */
#define BOARD_INITSPIS_SPI2_CS_SIGNAL                                    PCS0_SS   /*!< SPI2 signal: PCS0_SS */
#define BOARD_INITSPIS_SPI2_CS_PIN_NAME                                SPI2_PCS0   /*!< Pin name */
#define BOARD_INITSPIS_SPI2_CS_LABEL                                   "SPI2_CS"   /*!< Label */
#define BOARD_INITSPIS_SPI2_CS_NAME                                    "SPI2_CS"   /*!< Identifier name */
#define BOARD_INITSPIS_SPI2_CS_DIRECTION                kPIN_MUX_DirectionOutput   /*!< Direction */

/* PORTD12 (number 141), SPI2_SCK */
#define BOARD_INITSPIS_SPI2_SCK_PERIPHERAL                                  SPI2   /*!< Device name: SPI2 */
#define BOARD_INITSPIS_SPI2_SCK_SIGNAL                                       SCK   /*!< SPI2 signal: SCK */
#define BOARD_INITSPIS_SPI2_SCK_PIN_NAME                                SPI2_SCK   /*!< Pin name */
#define BOARD_INITSPIS_SPI2_SCK_LABEL                                 "SPI2_SCK"   /*!< Label */
#define BOARD_INITSPIS_SPI2_SCK_NAME                                  "SPI2_SCK"   /*!< Identifier name */
#define BOARD_INITSPIS_SPI2_SCK_DIRECTION               kPIN_MUX_DirectionOutput   /*!< Direction */

/* PORTD14 (number 143), SPI2_MISO */
#define BOARD_INITSPIS_SPI2_MISO_PERIPHERAL                                 SPI2   /*!< Device name: SPI2 */
#define BOARD_INITSPIS_SPI2_MISO_SIGNAL                                      SIN   /*!< SPI2 signal: SIN */
#define BOARD_INITSPIS_SPI2_MISO_PIN_NAME                               SPI2_SIN   /*!< Pin name */
#define BOARD_INITSPIS_SPI2_MISO_LABEL                               "SPI2_MISO"   /*!< Label */
#define BOARD_INITSPIS_SPI2_MISO_NAME                                "SPI2_MISO"   /*!< Identifier name */

/* PORTD13 (number 142), SPI2_MOSI */
#define BOARD_INITSPIS_SPI2_MOSI_PERIPHERAL                                 SPI2   /*!< Device name: SPI2 */
#define BOARD_INITSPIS_SPI2_MOSI_SIGNAL                                     SOUT   /*!< SPI2 signal: SOUT */
#define BOARD_INITSPIS_SPI2_MOSI_PIN_NAME                              SPI2_SOUT   /*!< Pin name */
#define BOARD_INITSPIS_SPI2_MOSI_LABEL                               "SPI2_MOSI"   /*!< Label */
#define BOARD_INITSPIS_SPI2_MOSI_NAME                                "SPI2_MOSI"   /*!< Identifier name */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitSPIs(void);

/* PORTD5 (number 132), LCD_DB0 */
#define BOARD_INITLCD_LCD_DB0_GPIO                                         GPIOD   /*!< GPIO device name: GPIOD */
#define BOARD_INITLCD_LCD_DB0_PORT                                         PORTD   /*!< PORT device name: PORTD */
#define BOARD_INITLCD_LCD_DB0_GPIO_PIN                                        5U   /*!< PORTD pin index: 5 */
#define BOARD_INITLCD_LCD_DB0_PIN_NAME                                      PTD5   /*!< Pin name */
#define BOARD_INITLCD_LCD_DB0_LABEL                                    "LCD_DB0"   /*!< Label */
#define BOARD_INITLCD_LCD_DB0_NAME                                     "LCD_DB0"   /*!< Identifier name */
#define BOARD_INITLCD_LCD_DB0_DIRECTION                 kPIN_MUX_DirectionOutput   /*!< Direction */

/* PORTD4 (number 131), LCD_DB1 */
#define BOARD_INITLCD_LCD_DB1_GPIO                                         GPIOD   /*!< GPIO device name: GPIOD */
#define BOARD_INITLCD_LCD_DB1_PORT                                         PORTD   /*!< PORT device name: PORTD */
#define BOARD_INITLCD_LCD_DB1_GPIO_PIN                                        4U   /*!< PORTD pin index: 4 */
#define BOARD_INITLCD_LCD_DB1_PIN_NAME                                      PTD4   /*!< Pin name */
#define BOARD_INITLCD_LCD_DB1_LABEL                                    "LCD_DB1"   /*!< Label */
#define BOARD_INITLCD_LCD_DB1_NAME                                     "LCD_DB1"   /*!< Identifier name */
#define BOARD_INITLCD_LCD_DB1_DIRECTION                 kPIN_MUX_DirectionOutput   /*!< Direction */

/* PORTD3 (number 130), LCD_DB2 */
#define BOARD_INITLCD_LCD_DB2_GPIO                                         GPIOD   /*!< GPIO device name: GPIOD */
#define BOARD_INITLCD_LCD_DB2_PORT                                         PORTD   /*!< PORT device name: PORTD */
#define BOARD_INITLCD_LCD_DB2_GPIO_PIN                                        3U   /*!< PORTD pin index: 3 */
#define BOARD_INITLCD_LCD_DB2_PIN_NAME                                      PTD3   /*!< Pin name */
#define BOARD_INITLCD_LCD_DB2_LABEL                                    "LCD_DB2"   /*!< Label */
#define BOARD_INITLCD_LCD_DB2_NAME                                     "LCD_DB2"   /*!< Identifier name */
#define BOARD_INITLCD_LCD_DB2_DIRECTION                 kPIN_MUX_DirectionOutput   /*!< Direction */

/* PORTD2 (number 129), LCD_DB3 */
#define BOARD_INITLCD_LCD_DB3_GPIO                                         GPIOD   /*!< GPIO device name: GPIOD */
#define BOARD_INITLCD_LCD_DB3_PORT                                         PORTD   /*!< PORT device name: PORTD */
#define BOARD_INITLCD_LCD_DB3_GPIO_PIN                                        2U   /*!< PORTD pin index: 2 */
#define BOARD_INITLCD_LCD_DB3_PIN_NAME                                      PTD2   /*!< Pin name */
#define BOARD_INITLCD_LCD_DB3_LABEL                                    "LCD_DB3"   /*!< Label */
#define BOARD_INITLCD_LCD_DB3_NAME                                     "LCD_DB3"   /*!< Identifier name */
#define BOARD_INITLCD_LCD_DB3_DIRECTION                 kPIN_MUX_DirectionOutput   /*!< Direction */

/* PORTD1 (number 128), LCD_DB4 */
#define BOARD_INITLCD_LCD_DB4_GPIO                                         GPIOD   /*!< GPIO device name: GPIOD */
#define BOARD_INITLCD_LCD_DB4_PORT                                         PORTD   /*!< PORT device name: PORTD */
#define BOARD_INITLCD_LCD_DB4_GPIO_PIN                                        1U   /*!< PORTD pin index: 1 */
#define BOARD_INITLCD_LCD_DB4_PIN_NAME                                      PTD1   /*!< Pin name */
#define BOARD_INITLCD_LCD_DB4_LABEL                                    "LCD_DB4"   /*!< Label */
#define BOARD_INITLCD_LCD_DB4_NAME                                     "LCD_DB4"   /*!< Identifier name */
#define BOARD_INITLCD_LCD_DB4_DIRECTION                 kPIN_MUX_DirectionOutput   /*!< Direction */

/* PORTD0 (number 127), LCD_DB5 */
#define BOARD_INITLCD_LCD_DB5_GPIO                                         GPIOD   /*!< GPIO device name: GPIOD */
#define BOARD_INITLCD_LCD_DB5_PORT                                         PORTD   /*!< PORT device name: PORTD */
#define BOARD_INITLCD_LCD_DB5_GPIO_PIN                                        0U   /*!< PORTD pin index: 0 */
#define BOARD_INITLCD_LCD_DB5_PIN_NAME                                      PTD0   /*!< Pin name */
#define BOARD_INITLCD_LCD_DB5_LABEL                                    "LCD_DB5"   /*!< Label */
#define BOARD_INITLCD_LCD_DB5_NAME                                     "LCD_DB5"   /*!< Identifier name */
#define BOARD_INITLCD_LCD_DB5_DIRECTION                 kPIN_MUX_DirectionOutput   /*!< Direction */

/* PORTC9 (number 114), LCD_BL */
#define BOARD_INITLCD_LCD_BL_GPIO                                          GPIOC   /*!< GPIO device name: GPIOC */
#define BOARD_INITLCD_LCD_BL_PORT                                          PORTC   /*!< PORT device name: PORTC */
#define BOARD_INITLCD_LCD_BL_GPIO_PIN                                         9U   /*!< PORTC pin index: 9 */
#define BOARD_INITLCD_LCD_BL_PIN_NAME                                       PTC9   /*!< Pin name */
#define BOARD_INITLCD_LCD_BL_LABEL                                      "LCD_BL"   /*!< Label */
#define BOARD_INITLCD_LCD_BL_NAME                                       "LCD_BL"   /*!< Identifier name */
#define BOARD_INITLCD_LCD_BL_DIRECTION                  kPIN_MUX_DirectionOutput   /*!< Direction */

/* PORTC12 (number 117), LCD_WAIT */
#define BOARD_INITLCD_LCD_WAIT_GPIO                                        GPIOC   /*!< GPIO device name: GPIOC */
#define BOARD_INITLCD_LCD_WAIT_PORT                                        PORTC   /*!< PORT device name: PORTC */
#define BOARD_INITLCD_LCD_WAIT_GPIO_PIN                                      12U   /*!< PORTC pin index: 12 */
#define BOARD_INITLCD_LCD_WAIT_PIN_NAME                                    PTC12   /*!< Pin name */
#define BOARD_INITLCD_LCD_WAIT_LABEL                                  "LCD_WAIT"   /*!< Label */
#define BOARD_INITLCD_LCD_WAIT_NAME                                   "LCD_WAIT"   /*!< Identifier name */
#define BOARD_INITLCD_LCD_WAIT_DIRECTION                kPIN_MUX_DirectionOutput   /*!< Direction */

/* PORTC18 (number 125), LCD_DB7 */
#define BOARD_INITLCD_LCD_DB7_GPIO                                         GPIOC   /*!< GPIO device name: GPIOC */
#define BOARD_INITLCD_LCD_DB7_PORT                                         PORTC   /*!< PORT device name: PORTC */
#define BOARD_INITLCD_LCD_DB7_GPIO_PIN                                       18U   /*!< PORTC pin index: 18 */
#define BOARD_INITLCD_LCD_DB7_PIN_NAME                                     PTC18   /*!< Pin name */
#define BOARD_INITLCD_LCD_DB7_LABEL                                    "LCD_DB7"   /*!< Label */
#define BOARD_INITLCD_LCD_DB7_NAME                                     "LCD_DB7"   /*!< Identifier name */
#define BOARD_INITLCD_LCD_DB7_DIRECTION                 kPIN_MUX_DirectionOutput   /*!< Direction */

/* PORTC19 (number 126), LCD_DB6 */
#define BOARD_INITLCD_LCD_DB6_GPIO                                         GPIOC   /*!< GPIO device name: GPIOC */
#define BOARD_INITLCD_LCD_DB6_PORT                                         PORTC   /*!< PORT device name: PORTC */
#define BOARD_INITLCD_LCD_DB6_GPIO_PIN                                       19U   /*!< PORTC pin index: 19 */
#define BOARD_INITLCD_LCD_DB6_PIN_NAME                                     PTC19   /*!< Pin name */
#define BOARD_INITLCD_LCD_DB6_LABEL                                    "LCD_DB6"   /*!< Label */
#define BOARD_INITLCD_LCD_DB6_NAME                                     "LCD_DB6"   /*!< Identifier name */
#define BOARD_INITLCD_LCD_DB6_DIRECTION                 kPIN_MUX_DirectionOutput   /*!< Direction */

/* PORTE6 (number 9), LCD_RES */
#define BOARD_INITLCD_LCD_RES_GPIO                                         GPIOE   /*!< GPIO device name: GPIOE */
#define BOARD_INITLCD_LCD_RES_PORT                                         PORTE   /*!< PORT device name: PORTE */
#define BOARD_INITLCD_LCD_RES_GPIO_PIN                                        6U   /*!< PORTE pin index: 6 */
#define BOARD_INITLCD_LCD_RES_PIN_NAME                                      PTE6   /*!< Pin name */
#define BOARD_INITLCD_LCD_RES_LABEL                                    "LCD_RES"   /*!< Label */
#define BOARD_INITLCD_LCD_RES_NAME                                     "LCD_RES"   /*!< Identifier name */
#define BOARD_INITLCD_LCD_RES_DIRECTION                 kPIN_MUX_DirectionOutput   /*!< Direction */

/* PORTE7 (number 10), LCD_A0 */
#define BOARD_INITLCD_LCD_A0_GPIO                                          GPIOE   /*!< GPIO device name: GPIOE */
#define BOARD_INITLCD_LCD_A0_PORT                                          PORTE   /*!< PORT device name: PORTE */
#define BOARD_INITLCD_LCD_A0_GPIO_PIN                                         7U   /*!< PORTE pin index: 7 */
#define BOARD_INITLCD_LCD_A0_PIN_NAME                                       PTE7   /*!< Pin name */
#define BOARD_INITLCD_LCD_A0_LABEL                                      "LCD_A0"   /*!< Label */
#define BOARD_INITLCD_LCD_A0_NAME                                       "LCD_A0"   /*!< Identifier name */
#define BOARD_INITLCD_LCD_A0_DIRECTION                  kPIN_MUX_DirectionOutput   /*!< Direction */

/* PORTE8 (number 11), LCD_CS */
#define BOARD_INITLCD_LCD_CS_GPIO                                          GPIOE   /*!< GPIO device name: GPIOE */
#define BOARD_INITLCD_LCD_CS_PORT                                          PORTE   /*!< PORT device name: PORTE */
#define BOARD_INITLCD_LCD_CS_GPIO_PIN                                         8U   /*!< PORTE pin index: 8 */
#define BOARD_INITLCD_LCD_CS_PIN_NAME                                       PTE8   /*!< Pin name */
#define BOARD_INITLCD_LCD_CS_LABEL                                      "LCD_CS"   /*!< Label */
#define BOARD_INITLCD_LCD_CS_NAME                                       "LCD_CS"   /*!< Identifier name */
#define BOARD_INITLCD_LCD_CS_DIRECTION                  kPIN_MUX_DirectionOutput   /*!< Direction */

/* PORTE9 (number 12), LCD_RD */
#define BOARD_INITLCD_LCD_RD_GPIO                                          GPIOE   /*!< GPIO device name: GPIOE */
#define BOARD_INITLCD_LCD_RD_PORT                                          PORTE   /*!< PORT device name: PORTE */
#define BOARD_INITLCD_LCD_RD_GPIO_PIN                                         9U   /*!< PORTE pin index: 9 */
#define BOARD_INITLCD_LCD_RD_PIN_NAME                                       PTE9   /*!< Pin name */
#define BOARD_INITLCD_LCD_RD_LABEL                                      "LCD_RD"   /*!< Label */
#define BOARD_INITLCD_LCD_RD_NAME                                       "LCD_RD"   /*!< Identifier name */
#define BOARD_INITLCD_LCD_RD_DIRECTION                  kPIN_MUX_DirectionOutput   /*!< Direction */

/* PORTE10 (number 13), LCD_WR */
#define BOARD_INITLCD_LCD_WR_GPIO                                          GPIOE   /*!< GPIO device name: GPIOE */
#define BOARD_INITLCD_LCD_WR_PORT                                          PORTE   /*!< PORT device name: PORTE */
#define BOARD_INITLCD_LCD_WR_GPIO_PIN                                        10U   /*!< PORTE pin index: 10 */
#define BOARD_INITLCD_LCD_WR_PIN_NAME                                      PTE10   /*!< Pin name */
#define BOARD_INITLCD_LCD_WR_LABEL                                      "LCD_WR"   /*!< Label */
#define BOARD_INITLCD_LCD_WR_NAME                                       "LCD_WR"   /*!< Identifier name */
#define BOARD_INITLCD_LCD_WR_DIRECTION                  kPIN_MUX_DirectionOutput   /*!< Direction */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitLCD(void);

/* ADC0_SE21 (number 36), ADC_TEMP2 */
#define BOARD_INITADCS_ADC_TEMP2_PERIPHERAL                                 ADC0   /*!< Device name: ADC0 */
#define BOARD_INITADCS_ADC_TEMP2_SIGNAL                                       SE   /*!< ADC0 signal: SE */
#define BOARD_INITADCS_ADC_TEMP2_CHANNEL                                      21   /*!< ADC0 SE channel: 21 */
#define BOARD_INITADCS_ADC_TEMP2_PIN_NAME                              ADC0_SE21   /*!< Pin name */
#define BOARD_INITADCS_ADC_TEMP2_LABEL                               "ADC_TEMP2"   /*!< Label */
#define BOARD_INITADCS_ADC_TEMP2_NAME                                "ADC_TEMP2"   /*!< Identifier name */

/* ADC0_SE22 (number 35), ADC_TEMP1 */
#define BOARD_INITADCS_ADC_TEMP1_PERIPHERAL                                 ADC0   /*!< Device name: ADC0 */
#define BOARD_INITADCS_ADC_TEMP1_SIGNAL                                       SE   /*!< ADC0 signal: SE */
#define BOARD_INITADCS_ADC_TEMP1_CHANNEL                                      22   /*!< ADC0 SE channel: 22 */
#define BOARD_INITADCS_ADC_TEMP1_PIN_NAME                              ADC0_SE22   /*!< Pin name */
#define BOARD_INITADCS_ADC_TEMP1_LABEL                               "ADC_TEMP1"   /*!< Label */
#define BOARD_INITADCS_ADC_TEMP1_NAME                                "ADC_TEMP1"   /*!< Identifier name */

/* ADC0_SE23 (number 38), ADC_VBATT */
#define BOARD_INITADCS_ADC_VBATT_PERIPHERAL                                 ADC0   /*!< Device name: ADC0 */
#define BOARD_INITADCS_ADC_VBATT_SIGNAL                                       SE   /*!< ADC0 signal: SE */
#define BOARD_INITADCS_ADC_VBATT_CHANNEL                                      23   /*!< ADC0 SE channel: 23 */
#define BOARD_INITADCS_ADC_VBATT_PIN_NAME                              ADC0_SE23   /*!< Pin name */
#define BOARD_INITADCS_ADC_VBATT_LABEL                               "ADC_VBATT"   /*!< Label */
#define BOARD_INITADCS_ADC_VBATT_NAME                                "ADC_VBATT"   /*!< Identifier name */

/* ADC1_SE18 (number 37), ADC_TEMP3 */
#define BOARD_INITADCS_ADC_TEMP3_PERIPHERAL                                 ADC1   /*!< Device name: ADC1 */
#define BOARD_INITADCS_ADC_TEMP3_SIGNAL                                       SE   /*!< ADC1 signal: SE */
#define BOARD_INITADCS_ADC_TEMP3_CHANNEL                                      18   /*!< ADC1 SE channel: 18 */
#define BOARD_INITADCS_ADC_TEMP3_PIN_NAME                              ADC1_SE18   /*!< Pin name */
#define BOARD_INITADCS_ADC_TEMP3_LABEL                               "ADC_TEMP3"   /*!< Label */
#define BOARD_INITADCS_ADC_TEMP3_NAME                                "ADC_TEMP3"   /*!< Identifier name */

/* ADC1_SE23 (number 39), ADC1_SE23 */
#define BOARD_INITADCS_ADC1_SE23_PERIPHERAL                                 ADC1   /*!< Device name: ADC1 */
#define BOARD_INITADCS_ADC1_SE23_SIGNAL                                       SE   /*!< ADC1 signal: SE */
#define BOARD_INITADCS_ADC1_SE23_CHANNEL                                      23   /*!< ADC1 SE channel: 23 */
#define BOARD_INITADCS_ADC1_SE23_PIN_NAME                              ADC1_SE23   /*!< Pin name */
#define BOARD_INITADCS_ADC1_SE23_LABEL                               "ADC1_SE23"   /*!< Label */
#define BOARD_INITADCS_ADC1_SE23_NAME                                "ADC1_SE23"   /*!< Identifier name */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitADCs(void);

/* PORTB3 (number 84), LED_RUN */
#define BOARD_INITLEDS_LED_RUN_GPIO                                        GPIOB   /*!< GPIO device name: GPIOB */
#define BOARD_INITLEDS_LED_RUN_PORT                                        PORTB   /*!< PORT device name: PORTB */
#define BOARD_INITLEDS_LED_RUN_GPIO_PIN                                       3U   /*!< PORTB pin index: 3 */
#define BOARD_INITLEDS_LED_RUN_PIN_NAME                                     PTB3   /*!< Pin name */
#define BOARD_INITLEDS_LED_RUN_LABEL                                   "LED_RUN"   /*!< Label */
#define BOARD_INITLEDS_LED_RUN_NAME                                    "LED_RUN"   /*!< Identifier name */
#define BOARD_INITLEDS_LED_RUN_DIRECTION                kPIN_MUX_DirectionOutput   /*!< Direction */

/* PORTB4 (number 85), LED_CON_GPRS */
#define BOARD_INITLEDS_LED_CON_GPRS_GPIO                                   GPIOB   /*!< GPIO device name: GPIOB */
#define BOARD_INITLEDS_LED_CON_GPRS_PORT                                   PORTB   /*!< PORT device name: PORTB */
#define BOARD_INITLEDS_LED_CON_GPRS_GPIO_PIN                                  4U   /*!< PORTB pin index: 4 */
#define BOARD_INITLEDS_LED_CON_GPRS_PIN_NAME                                PTB4   /*!< Pin name */
#define BOARD_INITLEDS_LED_CON_GPRS_LABEL                         "LED_CON_GPRS"   /*!< Label */
#define BOARD_INITLEDS_LED_CON_GPRS_NAME                          "LED_CON_GPRS"   /*!< Identifier name */
#define BOARD_INITLEDS_LED_CON_GPRS_DIRECTION           kPIN_MUX_DirectionOutput   /*!< Direction */

/* PORTB5 (number 86), LED_CON_ETH */
#define BOARD_INITLEDS_LED_CON_ETH_GPIO                                    GPIOB   /*!< GPIO device name: GPIOB */
#define BOARD_INITLEDS_LED_CON_ETH_PORT                                    PORTB   /*!< PORT device name: PORTB */
#define BOARD_INITLEDS_LED_CON_ETH_GPIO_PIN                                   5U   /*!< PORTB pin index: 5 */
#define BOARD_INITLEDS_LED_CON_ETH_PIN_NAME                                 PTB5   /*!< Pin name */
#define BOARD_INITLEDS_LED_CON_ETH_LABEL                           "LED_CON_ETH"   /*!< Label */
#define BOARD_INITLEDS_LED_CON_ETH_NAME                            "LED_CON_ETH"   /*!< Identifier name */
#define BOARD_INITLEDS_LED_CON_ETH_DIRECTION            kPIN_MUX_DirectionOutput   /*!< Direction */

/* PORTB6 (number 87), LED_ALARM */
#define BOARD_INITLEDS_LED_ALARM_GPIO                                      GPIOB   /*!< GPIO device name: GPIOB */
#define BOARD_INITLEDS_LED_ALARM_PORT                                      PORTB   /*!< PORT device name: PORTB */
#define BOARD_INITLEDS_LED_ALARM_GPIO_PIN                                     6U   /*!< PORTB pin index: 6 */
#define BOARD_INITLEDS_LED_ALARM_PIN_NAME                                   PTB6   /*!< Pin name */
#define BOARD_INITLEDS_LED_ALARM_LABEL                               "LED_ALARM"   /*!< Label */
#define BOARD_INITLEDS_LED_ALARM_NAME                                "LED_ALARM"   /*!< Identifier name */
#define BOARD_INITLEDS_LED_ALARM_DIRECTION              kPIN_MUX_DirectionOutput   /*!< Direction */

/* PORTC4 (number 109), LED_STATUS */
#define BOARD_INITLEDS_LED_STATUS_GPIO                                     GPIOC   /*!< GPIO device name: GPIOC */
#define BOARD_INITLEDS_LED_STATUS_PORT                                     PORTC   /*!< PORT device name: PORTC */
#define BOARD_INITLEDS_LED_STATUS_GPIO_PIN                                    4U   /*!< PORTC pin index: 4 */
#define BOARD_INITLEDS_LED_STATUS_PIN_NAME                                  PTC4   /*!< Pin name */
#define BOARD_INITLEDS_LED_STATUS_LABEL                             "LED_STATUS"   /*!< Label */
#define BOARD_INITLEDS_LED_STATUS_NAME                              "LED_STATUS"   /*!< Identifier name */
#define BOARD_INITLEDS_LED_STATUS_DIRECTION             kPIN_MUX_DirectionOutput   /*!< Direction */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitLEDs(void);

 // CanhLT - 23/12
/* PORTA26 (number 77), GPRS_EN */
#define BOARD_GPRS_EN_GPIO                                        GPIOA   /*!< GPIO device name: GPIOA */
#define BOARD_GPRS_EN_PORT                                        PORTA   /*!< PORT device name: PORTA */
#define BOARD_GPRS_EN_GPIO_PIN                                       26U   /*!< PORTA pin index: 26 */
#define BOARD_GPRS_EN_PIN_NAME                                     PTA26   /*!< Pin name */
#define BOARD_GPRS_EN_LABEL                                   "GPRS_EN"   /*!< Label */
#define BOARD_GPRS_EN_NAME                                    "GPRS_EN"   /*!< Identifier name */
#define BOARD_GPRS_EN_DIRECTION                kPIN_MUX_DirectionOutput   /*!< Direction */

/* PORTA27 (number 78), GPRS_PWR */
#define BOARD_GPRS_PWR_GPIO                                        GPIOA   /*!< GPIO device name: GPIOA */
#define BOARD_GPRS_PWR_PORT                                        PORTA   /*!< PORT device name: PORTA */
#define BOARD_GPRS_PWR_GPIO_PIN                                       27U   /*!< PORTA pin index: 26 */
#define BOARD_GPRS_PWR_PIN_NAME                                     PTA27   /*!< Pin name */
#define BOARD_GPRS_PWR_LABEL                                   "GPRS_PWR"   /*!< Label */
#define BOARD_GPRS_PWR_NAME                                    "GPRS_PWR"   /*!< Identifier name */
#define BOARD_GPRS_PWR_DIRECTION                kPIN_MUX_DirectionOutput   /*!< Direction */
/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitGPRS(void);

/* PORTE26 (number 47), ENET_1588_CLKIN */
#define BOARD_INITENET_ENET_1588_CLKIN_PERIPHERAL                           ENET   /*!< Device name: ENET */
#define BOARD_INITENET_ENET_1588_CLKIN_SIGNAL                         CLKIN_1588   /*!< ENET signal: CLKIN_1588 */
#define BOARD_INITENET_ENET_1588_CLKIN_PIN_NAME                  ENET_1588_CLKIN   /*!< Pin name */
#define BOARD_INITENET_ENET_1588_CLKIN_LABEL                   "ENET_1588_CLKIN"   /*!< Label */
#define BOARD_INITENET_ENET_1588_CLKIN_NAME                    "ENET_1588_CLKIN"   /*!< Identifier name */

/* PORTA8 (number 60), RMII0_MDC */
#define BOARD_INITENET_RMII0_MDC_PERIPHERAL                                 ENET   /*!< Device name: ENET */
#define BOARD_INITENET_RMII0_MDC_SIGNAL                                 RMII_MDC   /*!< ENET signal: RMII_MDC */
#define BOARD_INITENET_RMII0_MDC_PIN_NAME                              RMII0_MDC   /*!< Pin name */
#define BOARD_INITENET_RMII0_MDC_LABEL                               "RMII0_MDC"   /*!< Label */
#define BOARD_INITENET_RMII0_MDC_NAME                                "RMII0_MDC"   /*!< Identifier name */

/* PORTA7 (number 59), RMII0_MDIO */
#define BOARD_INITENET_RMII0_MDIO_PERIPHERAL                                ENET   /*!< Device name: ENET */
#define BOARD_INITENET_RMII0_MDIO_SIGNAL                               RMII_MDIO   /*!< ENET signal: RMII_MDIO */
#define BOARD_INITENET_RMII0_MDIO_PIN_NAME                            RMII0_MDIO   /*!< Pin name */
#define BOARD_INITENET_RMII0_MDIO_LABEL                             "RMII0_MDIO"   /*!< Label */
#define BOARD_INITENET_RMII0_MDIO_NAME                              "RMII0_MDIO"   /*!< Identifier name */

/* PORTA5 (number 55), RMII0_RXER */
#define BOARD_INITENET_RMII0_RXER_PERIPHERAL                                ENET   /*!< Device name: ENET */
#define BOARD_INITENET_RMII0_RXER_SIGNAL                               RMII_RXER   /*!< ENET signal: RMII_RXER */
#define BOARD_INITENET_RMII0_RXER_PIN_NAME                            RMII0_RXER   /*!< Pin name */
#define BOARD_INITENET_RMII0_RXER_LABEL                             "RMII0_RXER"   /*!< Label */
#define BOARD_INITENET_RMII0_RXER_NAME                              "RMII0_RXER"   /*!< Identifier name */

/* PORTA13 (number 65), RMII0_RXD0 */
#define BOARD_INITENET_RMII0_RXD0_PERIPHERAL                                ENET   /*!< Device name: ENET */
#define BOARD_INITENET_RMII0_RXD0_SIGNAL                               RMII_RXD0   /*!< ENET signal: RMII_RXD0 */
#define BOARD_INITENET_RMII0_RXD0_PIN_NAME                            RMII0_RXD0   /*!< Pin name */
#define BOARD_INITENET_RMII0_RXD0_LABEL                             "RMII0_RXD0"   /*!< Label */
#define BOARD_INITENET_RMII0_RXD0_NAME                              "RMII0_RXD0"   /*!< Identifier name */

/* PORTA17 (number 69), RMII0_TXD1 */
#define BOARD_INITENET_RMII0_TXD1_PERIPHERAL                                ENET   /*!< Device name: ENET */
#define BOARD_INITENET_RMII0_TXD1_SIGNAL                               RMII_TXD1   /*!< ENET signal: RMII_TXD1 */
#define BOARD_INITENET_RMII0_TXD1_PIN_NAME                            RMII0_TXD1   /*!< Pin name */
#define BOARD_INITENET_RMII0_TXD1_LABEL                             "RMII0_TXD1"   /*!< Label */
#define BOARD_INITENET_RMII0_TXD1_NAME                              "RMII0_TXD1"   /*!< Identifier name */

/* PORTA15 (number 67), RMII0_TXEN */
#define BOARD_INITENET_RMII0_TXEN_PERIPHERAL                                ENET   /*!< Device name: ENET */
#define BOARD_INITENET_RMII0_TXEN_SIGNAL                               RMII_TXEN   /*!< ENET signal: RMII_TXEN */
#define BOARD_INITENET_RMII0_TXEN_PIN_NAME                            RMII0_TXEN   /*!< Pin name */
#define BOARD_INITENET_RMII0_TXEN_LABEL                             "RMII0_TXEN"   /*!< Label */
#define BOARD_INITENET_RMII0_TXEN_NAME                              "RMII0_TXEN"   /*!< Identifier name */

/* PORTA12 (number 64), RMII0_RXD1 */
#define BOARD_INITENET_RMII0_RXD1_PERIPHERAL                                ENET   /*!< Device name: ENET */
#define BOARD_INITENET_RMII0_RXD1_SIGNAL                               RMII_RXD1   /*!< ENET signal: RMII_RXD1 */
#define BOARD_INITENET_RMII0_RXD1_PIN_NAME                            RMII0_RXD1   /*!< Pin name */
#define BOARD_INITENET_RMII0_RXD1_LABEL                             "RMII0_RXD1"   /*!< Label */
#define BOARD_INITENET_RMII0_RXD1_NAME                              "RMII0_RXD1"   /*!< Identifier name */

/* PORTA16 (number 68), RMII0_TXD0 */
#define BOARD_INITENET_RMII0_TXD0_PERIPHERAL                                ENET   /*!< Device name: ENET */
#define BOARD_INITENET_RMII0_TXD0_SIGNAL                               RMII_TXD0   /*!< ENET signal: RMII_TXD0 */
#define BOARD_INITENET_RMII0_TXD0_PIN_NAME                            RMII0_TXD0   /*!< Pin name */
#define BOARD_INITENET_RMII0_TXD0_LABEL                             "RMII0_TXD0"   /*!< Label */
#define BOARD_INITENET_RMII0_TXD0_NAME                              "RMII0_TXD0"   /*!< Identifier name */

/* PORTA14 (number 66), RMII0_CRS_DV */
#define BOARD_INITENET_RMII0_CRS_DV_PERIPHERAL                              ENET   /*!< Device name: ENET */
#define BOARD_INITENET_RMII0_CRS_DV_SIGNAL                           RMII_CRS_DV   /*!< ENET signal: RMII_CRS_DV */
#define BOARD_INITENET_RMII0_CRS_DV_PIN_NAME                        RMII0_CRS_DV   /*!< Pin name */
#define BOARD_INITENET_RMII0_CRS_DV_LABEL                         "RMII0_CRS_DV"   /*!< Label */
#define BOARD_INITENET_RMII0_CRS_DV_NAME                          "RMII0_CRS_DV"   /*!< Identifier name */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitENET(void);

/* PORTC5 (number 110), BUTTON_ESC */
#define BOARD_INITBUTTONS_BUTTON_ESC_GPIO                                  GPIOC   /*!< GPIO device name: GPIOC */
#define BOARD_INITBUTTONS_BUTTON_ESC_PORT                                  PORTC   /*!< PORT device name: PORTC */
#define BOARD_INITBUTTONS_BUTTON_ESC_GPIO_PIN                                 5U   /*!< PORTC pin index: 5 */
#define BOARD_INITBUTTONS_BUTTON_ESC_PIN_NAME                               PTC5   /*!< Pin name */
#define BOARD_INITBUTTONS_BUTTON_ESC_LABEL                          "BUTTON_ESC"   /*!< Label */
#define BOARD_INITBUTTONS_BUTTON_ESC_NAME                           "BUTTON_ESC"   /*!< Identifier name */
#define BOARD_INITBUTTONS_BUTTON_ESC_DIRECTION           kPIN_MUX_DirectionInput   /*!< Direction */

/* PORTC6 (number 111), BUTTON_UP */
#define BOARD_INITBUTTONS_BUTTON_UP_GPIO                                   GPIOC   /*!< GPIO device name: GPIOC */
#define BOARD_INITBUTTONS_BUTTON_UP_PORT                                   PORTC   /*!< PORT device name: PORTC */
#define BOARD_INITBUTTONS_BUTTON_UP_GPIO_PIN                                  6U   /*!< PORTC pin index: 6 */
#define BOARD_INITBUTTONS_BUTTON_UP_PIN_NAME                                PTC6   /*!< Pin name */
#define BOARD_INITBUTTONS_BUTTON_UP_LABEL                            "BUTTON_UP"   /*!< Label */
#define BOARD_INITBUTTONS_BUTTON_UP_NAME                             "BUTTON_UP"   /*!< Identifier name */
#define BOARD_INITBUTTONS_BUTTON_UP_DIRECTION            kPIN_MUX_DirectionInput   /*!< Direction */

/* PORTC7 (number 112), BUTTON_DOWN */
#define BOARD_INITBUTTONS_BUTTON_DOWN_GPIO                                 GPIOC   /*!< GPIO device name: GPIOC */
#define BOARD_INITBUTTONS_BUTTON_DOWN_PORT                                 PORTC   /*!< PORT device name: PORTC */
#define BOARD_INITBUTTONS_BUTTON_DOWN_GPIO_PIN                                7U   /*!< PORTC pin index: 7 */
#define BOARD_INITBUTTONS_BUTTON_DOWN_PIN_NAME                              PTC7   /*!< Pin name */
#define BOARD_INITBUTTONS_BUTTON_DOWN_LABEL                        "BUTTON_DOWN"   /*!< Label */
#define BOARD_INITBUTTONS_BUTTON_DOWN_NAME                         "BUTTON_DOWN"   /*!< Identifier name */
#define BOARD_INITBUTTONS_BUTTON_DOWN_DIRECTION          kPIN_MUX_DirectionInput   /*!< Direction */

/* PORTC8 (number 113), BUTTON_ENTER */
#define BOARD_INITBUTTONS_BUTTON_ENTER_GPIO                                GPIOC   /*!< GPIO device name: GPIOC */
#define BOARD_INITBUTTONS_BUTTON_ENTER_PORT                                PORTC   /*!< PORT device name: PORTC */
#define BOARD_INITBUTTONS_BUTTON_ENTER_GPIO_PIN                               8U   /*!< PORTC pin index: 8 */
#define BOARD_INITBUTTONS_BUTTON_ENTER_PIN_NAME                             PTC8   /*!< Pin name */
#define BOARD_INITBUTTONS_BUTTON_ENTER_LABEL                      "BUTTON_ENTER"   /*!< Label */
#define BOARD_INITBUTTONS_BUTTON_ENTER_NAME                       "BUTTON_ENTER"   /*!< Identifier name */
#define BOARD_INITBUTTONS_BUTTON_ENTER_DIRECTION         kPIN_MUX_DirectionInput   /*!< Direction */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitBUTTONs(void);

/* PORTD8 (number 137), SCL */
#define BOARD_INITI2C_SCL_PERIPHERAL                                        I2C0   /*!< Device name: I2C0 */
#define BOARD_INITI2C_SCL_SIGNAL                                             SCL   /*!< I2C0 signal: SCL */
#define BOARD_INITI2C_SCL_PIN_NAME                                      I2C0_SCL   /*!< Pin name */
#define BOARD_INITI2C_SCL_LABEL                                            "SCL"   /*!< Label */
#define BOARD_INITI2C_SCL_NAME                                             "SCL"   /*!< Identifier name */

/* PORTD9 (number 138), SDA */
#define BOARD_INITI2C_SDA_PERIPHERAL                                        I2C0   /*!< Device name: I2C0 */
#define BOARD_INITI2C_SDA_SIGNAL                                             SDA   /*!< I2C0 signal: SDA */
#define BOARD_INITI2C_SDA_PIN_NAME                                      I2C0_SDA   /*!< Pin name */
#define BOARD_INITI2C_SDA_LABEL                                            "SDA"   /*!< Label */
#define BOARD_INITI2C_SDA_NAME                                             "SDA"   /*!< Identifier name */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitI2C(void);

/* PORTE0 (number 1), GSM_TX */
#define BOARD_INITUARTS_GSM_TX_PERIPHERAL                                  UART1   /*!< Device name: UART1 */
#define BOARD_INITUARTS_GSM_TX_SIGNAL                                         TX   /*!< UART1 signal: TX */
#define BOARD_INITUARTS_GSM_TX_PIN_NAME                                 UART1_TX   /*!< Pin name */
#define BOARD_INITUARTS_GSM_TX_LABEL                                    "GSM_TX"   /*!< Label */
#define BOARD_INITUARTS_GSM_TX_NAME                                     "GSM_TX"   /*!< Identifier name */
#define BOARD_INITUARTS_GSM_TX_DIRECTION                kPIN_MUX_DirectionOutput   /*!< Direction */

/* PORTE1 (number 2), GSM_RX */
#define BOARD_INITUARTS_GSM_RX_PERIPHERAL                                  UART1   /*!< Device name: UART1 */
#define BOARD_INITUARTS_GSM_RX_SIGNAL                                         RX   /*!< UART1 signal: RX */
#define BOARD_INITUARTS_GSM_RX_PIN_NAME                                 UART1_RX   /*!< Pin name */
#define BOARD_INITUARTS_GSM_RX_LABEL                                    "GSM_RX"   /*!< Label */
#define BOARD_INITUARTS_GSM_RX_NAME                                     "GSM_RX"   /*!< Identifier name */

/* PORTC15 (number 120), DEBUG_TX */
#define BOARD_INITUARTS_DEBUG_TX_PERIPHERAL                                UART4   /*!< Device name: UART4 */
#define BOARD_INITUARTS_DEBUG_TX_SIGNAL                                       TX   /*!< UART4 signal: TX */
#define BOARD_INITUARTS_DEBUG_TX_PIN_NAME                               UART4_TX   /*!< Pin name */
#define BOARD_INITUARTS_DEBUG_TX_LABEL                                "DEBUG_TX"   /*!< Label */
#define BOARD_INITUARTS_DEBUG_TX_NAME                                 "DEBUG_TX"   /*!< Identifier name */
#define BOARD_INITUARTS_DEBUG_TX_DIRECTION              kPIN_MUX_DirectionOutput   /*!< Direction */

/* PORTC14 (number 119), DEBUG_RX */
#define BOARD_INITUARTS_DEBUG_RX_PERIPHERAL                                UART4   /*!< Device name: UART4 */
#define BOARD_INITUARTS_DEBUG_RX_SIGNAL                                       RX   /*!< UART4 signal: RX */
#define BOARD_INITUARTS_DEBUG_RX_PIN_NAME                               UART4_RX   /*!< Pin name */
#define BOARD_INITUARTS_DEBUG_RX_LABEL                                "DEBUG_RX"   /*!< Label */
#define BOARD_INITUARTS_DEBUG_RX_NAME                                 "DEBUG_RX"   /*!< Identifier name */

/* PORTE4 (number 7), RS485_TX */
#define BOARD_INITUARTS_RS485_TX_PERIPHERAL                                UART3   /*!< Device name: UART3 */
#define BOARD_INITUARTS_RS485_TX_SIGNAL                                       TX   /*!< UART3 signal: TX */
#define BOARD_INITUARTS_RS485_TX_PIN_NAME                               UART3_TX   /*!< Pin name */
#define BOARD_INITUARTS_RS485_TX_LABEL                                "RS485_TX"   /*!< Label */
#define BOARD_INITUARTS_RS485_TX_NAME                                 "RS485_TX"   /*!< Identifier name */
#define BOARD_INITUARTS_RS485_TX_DIRECTION              kPIN_MUX_DirectionOutput   /*!< Direction */

/* PORTE5 (number 8), RS485_RX */
#define BOARD_INITUARTS_RS485_RX_PERIPHERAL                                UART3   /*!< Device name: UART3 */
#define BOARD_INITUARTS_RS485_RX_SIGNAL                                       RX   /*!< UART3 signal: RX */
#define BOARD_INITUARTS_RS485_RX_PIN_NAME                               UART3_RX   /*!< Pin name */
#define BOARD_INITUARTS_RS485_RX_LABEL                                "RS485_RX"   /*!< Label */
#define BOARD_INITUARTS_RS485_RX_NAME                                 "RS485_RX"   /*!< Identifier name */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitUARTs(void);

/* PORTD15 (number 144), MUX_S1 */
#define BOARD_INITIOS_MUX_S1_GPIO                                          GPIOD   /*!< GPIO device name: GPIOD */
#define BOARD_INITIOS_MUX_S1_PORT                                          PORTD   /*!< PORT device name: PORTD */
#define BOARD_INITIOS_MUX_S1_GPIO_PIN                                        15U   /*!< PORTD pin index: 15 */
#define BOARD_INITIOS_MUX_S1_PIN_NAME                                      PTD15   /*!< Pin name */
#define BOARD_INITIOS_MUX_S1_LABEL                                      "MUX_S1"   /*!< Label */
#define BOARD_INITIOS_MUX_S1_NAME                                       "MUX_S1"   /*!< Identifier name */
#define BOARD_INITIOS_MUX_S1_DIRECTION                  kPIN_MUX_DirectionOutput   /*!< Direction */

/* PORTD10 (number 139), MUX_S3 */
#define BOARD_INITIOS_MUX_S3_GPIO                                          GPIOD   /*!< GPIO device name: GPIOD */
#define BOARD_INITIOS_MUX_S3_PORT                                          PORTD   /*!< PORT device name: PORTD */
#define BOARD_INITIOS_MUX_S3_GPIO_PIN                                        10U   /*!< PORTD pin index: 10 */
#define BOARD_INITIOS_MUX_S3_PIN_NAME                                      PTD10   /*!< Pin name */
#define BOARD_INITIOS_MUX_S3_LABEL                                      "MUX_S3"   /*!< Label */
#define BOARD_INITIOS_MUX_S3_NAME                                       "MUX_S3"   /*!< Identifier name */
#define BOARD_INITIOS_MUX_S3_DIRECTION                  kPIN_MUX_DirectionOutput   /*!< Direction */

/* PORTE2 (number 3), MUX_S0 */
#define BOARD_INITIOS_MUX_S0_GPIO                                          GPIOE   /*!< GPIO device name: GPIOE */
#define BOARD_INITIOS_MUX_S0_PORT                                          PORTE   /*!< PORT device name: PORTE */
#define BOARD_INITIOS_MUX_S0_GPIO_PIN                                         2U   /*!< PORTE pin index: 2 */
#define BOARD_INITIOS_MUX_S0_PIN_NAME                                       PTE2   /*!< Pin name */
#define BOARD_INITIOS_MUX_S0_LABEL                                      "MUX_S0"   /*!< Label */
#define BOARD_INITIOS_MUX_S0_NAME                                       "MUX_S0"   /*!< Identifier name */
#define BOARD_INITIOS_MUX_S0_DIRECTION                  kPIN_MUX_DirectionOutput   /*!< Direction */

/* PORTE3 (number 4), MUX_S2 */
#define BOARD_INITIOS_MUX_S2_GPIO                                          GPIOE   /*!< GPIO device name: GPIOE */
#define BOARD_INITIOS_MUX_S2_PORT                                          PORTE   /*!< PORT device name: PORTE */
#define BOARD_INITIOS_MUX_S2_GPIO_PIN                                         3U   /*!< PORTE pin index: 3 */
#define BOARD_INITIOS_MUX_S2_PIN_NAME                                       PTE3   /*!< Pin name */
#define BOARD_INITIOS_MUX_S2_LABEL                                      "MUX_S2"   /*!< Label */
#define BOARD_INITIOS_MUX_S2_NAME                                       "MUX_S2"   /*!< Identifier name */
#define BOARD_INITIOS_MUX_S2_DIRECTION                  kPIN_MUX_DirectionOutput   /*!< Direction */

/* PORTE11 (number 14), DI6 */
#define BOARD_INITIOS_DI6_GPIO                                             GPIOE   /*!< GPIO device name: GPIOE */
#define BOARD_INITIOS_DI6_PORT                                             PORTE   /*!< PORT device name: PORTE */
#define BOARD_INITIOS_DI6_GPIO_PIN                                           11U   /*!< PORTE pin index: 11 */
#define BOARD_INITIOS_DI6_PIN_NAME                                         PTE11   /*!< Pin name */
#define BOARD_INITIOS_DI6_LABEL                                            "DI6"   /*!< Label */
#define BOARD_INITIOS_DI6_NAME                                             "DI6"   /*!< Identifier name */
#define BOARD_INITIOS_DI6_DIRECTION                      kPIN_MUX_DirectionInput   /*!< Direction */

/* PORTE12 (number 15), DI5 */
#define BOARD_INITIOS_DI5_GPIO                                             GPIOE   /*!< GPIO device name: GPIOE */
#define BOARD_INITIOS_DI5_PORT                                             PORTE   /*!< PORT device name: PORTE */
#define BOARD_INITIOS_DI5_GPIO_PIN                                           12U   /*!< PORTE pin index: 12 */
#define BOARD_INITIOS_DI5_PIN_NAME                                         PTE12   /*!< Pin name */
#define BOARD_INITIOS_DI5_LABEL                                            "DI5"   /*!< Label */
#define BOARD_INITIOS_DI5_NAME                                             "DI5"   /*!< Identifier name */
#define BOARD_INITIOS_DI5_DIRECTION                      kPIN_MUX_DirectionInput   /*!< Direction */

/* PORTB22 (number 101), DI4 */
#define BOARD_INITIOS_DI4_GPIO                                             GPIOB   /*!< GPIO device name: GPIOB */
#define BOARD_INITIOS_DI4_PORT                                             PORTB   /*!< PORT device name: PORTB */
#define BOARD_INITIOS_DI4_GPIO_PIN                                           22U   /*!< PORTB pin index: 22 */
#define BOARD_INITIOS_DI4_PIN_NAME                                         PTB22   /*!< Pin name */
#define BOARD_INITIOS_DI4_LABEL                                            "DI4"   /*!< Label */
#define BOARD_INITIOS_DI4_NAME                                             "DI4"   /*!< Identifier name */
#define BOARD_INITIOS_DI4_DIRECTION                      kPIN_MUX_DirectionInput   /*!< Direction */

/* PORTB21 (number 100), DI3 */
#define BOARD_INITIOS_DI3_GPIO                                             GPIOB   /*!< GPIO device name: GPIOB */
#define BOARD_INITIOS_DI3_PORT                                             PORTB   /*!< PORT device name: PORTB */
#define BOARD_INITIOS_DI3_GPIO_PIN                                           21U   /*!< PORTB pin index: 21 */
#define BOARD_INITIOS_DI3_PIN_NAME                                         PTB21   /*!< Pin name */
#define BOARD_INITIOS_DI3_LABEL                                            "DI3"   /*!< Label */
#define BOARD_INITIOS_DI3_NAME                                             "DI3"   /*!< Identifier name */
#define BOARD_INITIOS_DI3_DIRECTION                      kPIN_MUX_DirectionInput   /*!< Direction */

/* PORTB20 (number 99), DI2 */
#define BOARD_INITIOS_DI2_GPIO                                             GPIOB   /*!< GPIO device name: GPIOB */
#define BOARD_INITIOS_DI2_PORT                                             PORTB   /*!< PORT device name: PORTB */
#define BOARD_INITIOS_DI2_GPIO_PIN                                           20U   /*!< PORTB pin index: 20 */
#define BOARD_INITIOS_DI2_PIN_NAME                                         PTB20   /*!< Pin name */
#define BOARD_INITIOS_DI2_LABEL                                            "DI2"   /*!< Label */
#define BOARD_INITIOS_DI2_NAME                                             "DI2"   /*!< Identifier name */
#define BOARD_INITIOS_DI2_DIRECTION                      kPIN_MUX_DirectionInput   /*!< Direction */

/* PORTB19 (number 98), DI1 */
#define BOARD_INITIOS_DI1_GPIO                                             GPIOB   /*!< GPIO device name: GPIOB */
#define BOARD_INITIOS_DI1_PORT                                             PORTB   /*!< PORT device name: PORTB */
#define BOARD_INITIOS_DI1_GPIO_PIN                                           19U   /*!< PORTB pin index: 19 */
#define BOARD_INITIOS_DI1_PIN_NAME                                         PTB19   /*!< Pin name */
#define BOARD_INITIOS_DI1_LABEL                                            "DI1"   /*!< Label */
#define BOARD_INITIOS_DI1_NAME                                             "DI1"   /*!< Identifier name */
#define BOARD_INITIOS_DI1_DIRECTION                      kPIN_MUX_DirectionInput   /*!< Direction */

/* PORTB10 (number 91), ALARM_LIGHT */
#define BOARD_INITIOS_ALARM_SPEAKER_GPIO                                   GPIOB   /*!< GPIO device name: GPIOB */
#define BOARD_INITIOS_ALARM_SPEAKER_PORT                                   PORTB   /*!< PORT device name: PORTB */
#define BOARD_INITIOS_ALARM_SPEAKER_GPIO_PIN                                 10U   /*!< PORTB pin index: 10 */
#define BOARD_INITIOS_ALARM_SPEAKER_PIN_NAME                               PTB10   /*!< Pin name */
#define BOARD_INITIOS_ALARM_SPEAKER_LABEL                          "ALARM_LIGHT"   /*!< Label */
#define BOARD_INITIOS_ALARM_SPEAKER_NAME                         "ALARM_SPEAKER"   /*!< Identifier name */
#define BOARD_INITIOS_ALARM_SPEAKER_DIRECTION           kPIN_MUX_DirectionOutput   /*!< Direction */

/* PORTB11 (number 92), ALARM_LIGHT */
#define BOARD_INITIOS_ALARM_LIGHT_GPIO                                     GPIOB   /*!< GPIO device name: GPIOB */
#define BOARD_INITIOS_ALARM_LIGHT_PORT                                     PORTB   /*!< PORT device name: PORTB */
#define BOARD_INITIOS_ALARM_LIGHT_GPIO_PIN                                   11U   /*!< PORTB pin index: 11 */
#define BOARD_INITIOS_ALARM_LIGHT_PIN_NAME                                 PTB11   /*!< Pin name */
#define BOARD_INITIOS_ALARM_LIGHT_LABEL                            "ALARM_LIGHT"   /*!< Label */
#define BOARD_INITIOS_ALARM_LIGHT_NAME                             "ALARM_LIGHT"   /*!< Identifier name */
#define BOARD_INITIOS_ALARM_LIGHT_DIRECTION             kPIN_MUX_DirectionOutput   /*!< Direction */

/* PORTB8 (number 89), WAIT_MCU1 */
#define BOARD_INITIOS_WAIT_MCU1_GPIO                                       GPIOB   /*!< GPIO device name: GPIOB */
#define BOARD_INITIOS_WAIT_MCU1_PORT                                       PORTB   /*!< PORT device name: PORTB */
#define BOARD_INITIOS_WAIT_MCU1_GPIO_PIN                                      8U   /*!< PORTB pin index: 8 */
#define BOARD_INITIOS_WAIT_MCU1_PIN_NAME                                    PTB8   /*!< Pin name */
#define BOARD_INITIOS_WAIT_MCU1_LABEL                                "WAIT_MCU1"   /*!< Label */
#define BOARD_INITIOS_WAIT_MCU1_NAME                                 "WAIT_MCU1"   /*!< Identifier name */
#define BOARD_INITIOS_WAIT_MCU1_DIRECTION               kPIN_MUX_DirectionOutput   /*!< Direction */

/* PORTB16 (number 95), WAIT_MCU2 */
#define BOARD_INITIOS_WAIT_MCU2_GPIO                                       GPIOB   /*!< GPIO device name: GPIOB */
#define BOARD_INITIOS_WAIT_MCU2_PORT                                       PORTB   /*!< PORT device name: PORTB */
#define BOARD_INITIOS_WAIT_MCU2_GPIO_PIN                                     16U   /*!< PORTB pin index: 16 */
#define BOARD_INITIOS_WAIT_MCU2_PIN_NAME                                   PTB16   /*!< Pin name */
#define BOARD_INITIOS_WAIT_MCU2_LABEL                                "WAIT_MCU2"   /*!< Label */
#define BOARD_INITIOS_WAIT_MCU2_NAME                                 "WAIT_MCU2"   /*!< Identifier name */
#define BOARD_INITIOS_WAIT_MCU2_DIRECTION               kPIN_MUX_DirectionOutput   /*!< Direction */

/* PORTB18 (number 97), WAIT_MCU3 */
#define BOARD_INITIOS_WAIT_MCU3_GPIO                                       GPIOB   /*!< GPIO device name: GPIOB */
#define BOARD_INITIOS_WAIT_MCU3_PORT                                       PORTB   /*!< PORT device name: PORTB */
#define BOARD_INITIOS_WAIT_MCU3_GPIO_PIN                                     18U   /*!< PORTB pin index: 18 */
#define BOARD_INITIOS_WAIT_MCU3_PIN_NAME                                   PTB18   /*!< Pin name */
#define BOARD_INITIOS_WAIT_MCU3_LABEL                                "WAIT_MCU3"   /*!< Label */
#define BOARD_INITIOS_WAIT_MCU3_NAME                                 "WAIT_MCU3"   /*!< Identifier name */
#define BOARD_INITIOS_WAIT_MCU3_DIRECTION               kPIN_MUX_DirectionOutput   /*!< Direction */

/* PORTB7 (number 88), WAIT_MCU4 */
#define BOARD_INITIOS_WAIT_MCU4_GPIO                                       GPIOB   /*!< GPIO device name: GPIOB */
#define BOARD_INITIOS_WAIT_MCU4_PORT                                       PORTB   /*!< PORT device name: PORTB */
#define BOARD_INITIOS_WAIT_MCU4_GPIO_PIN                                      7U   /*!< PORTB pin index: 7 */
#define BOARD_INITIOS_WAIT_MCU4_PIN_NAME                                    PTB7   /*!< Pin name */
#define BOARD_INITIOS_WAIT_MCU4_LABEL                                "WAIT_MCU4"   /*!< Label */
#define BOARD_INITIOS_WAIT_MCU4_NAME                                 "WAIT_MCU4"   /*!< Identifier name */
#define BOARD_INITIOS_WAIT_MCU4_DIRECTION               kPIN_MUX_DirectionOutput   /*!< Direction */

/* PORTB17 (number 96), WAIT_MCU5 */
#define BOARD_INITIOS_WAIT_MCU5_GPIO                                       GPIOB   /*!< GPIO device name: GPIOB */
#define BOARD_INITIOS_WAIT_MCU5_PORT                                       PORTB   /*!< PORT device name: PORTB */
#define BOARD_INITIOS_WAIT_MCU5_GPIO_PIN                                     17U   /*!< PORTB pin index: 17 */
#define BOARD_INITIOS_WAIT_MCU5_PIN_NAME                                   PTB17   /*!< Pin name */
#define BOARD_INITIOS_WAIT_MCU5_LABEL                                "WAIT_MCU5"   /*!< Label */
#define BOARD_INITIOS_WAIT_MCU5_NAME                                 "WAIT_MCU5"   /*!< Identifier name */
#define BOARD_INITIOS_WAIT_MCU5_DIRECTION               kPIN_MUX_DirectionOutput   /*!< Direction */

/* PORTB9 (number 90), OPENDOOR_MCU */
#define BOARD_INITIOS_OPENDOOR_MCU_GPIO                                    GPIOB   /*!< GPIO device name: GPIOB */
#define BOARD_INITIOS_OPENDOOR_MCU_PORT                                    PORTB   /*!< PORT device name: PORTB */
#define BOARD_INITIOS_OPENDOOR_MCU_GPIO_PIN                                   9U   /*!< PORTB pin index: 9 */
#define BOARD_INITIOS_OPENDOOR_MCU_PIN_NAME                                 PTB9   /*!< Pin name */
#define BOARD_INITIOS_OPENDOOR_MCU_LABEL                          "OPENDOOR_MCU"   /*!< Label */
#define BOARD_INITIOS_OPENDOOR_MCU_NAME                           "OPENDOOR_MCU"   /*!< Identifier name */
#define BOARD_INITIOS_OPENDOOR_MCU_DIRECTION            kPIN_MUX_DirectionOutput   /*!< Direction */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitIOs(void);

#if defined(__cplusplus)
}
#endif

/*!
 * @}
 */
#endif /* _PIN_MUX_H_ */

/*******************************************************************************
 * EOF
 ******************************************************************************/
