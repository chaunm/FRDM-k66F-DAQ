/*
* Copyright (c) 2015, Freescale Semiconductor, Inc.
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
*/

#ifndef _BOARD_H_
#define _BOARD_H_

#include "clock_config.h"
#include "fsl_gpio.h"
#ifndef DEBUG_UART
#define DEBUG_UART				4
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief The board name */
#define BOARD_NAME "FRDM-K66F"

 // CanhLT - 23/12 -- chaunm - disable debug console
// #define DEBUG_CONSOLE_UART4
 
#ifndef DEBUG_UART
#define DEBUG_UART 4
#endif
/*! @brief The UART to use for debug messages. */
#if (DEBUG_UART == 0)
#define BOARD_DEBUG_UART_TYPE DEBUG_CONSOLE_DEVICE_TYPE_UART
#define BOARD_DEBUG_UART_BASEADDR (uint32_t) UART0
#define BOARD_DEBUG_UART_CLKSRC SYS_CLK
#define BOARD_DEBUG_UART_CLK_FREQ CLOCK_GetCoreSysClkFreq()
#define BOARD_UART_IRQ UART0_RX_TX_IRQn
#define BOARD_UART_IRQ_HANDLER UART0_RX_TX_IRQHandler
#elif (DEBUG_UART == 4)
#define BOARD_DEBUG_UART_TYPE DEBUG_CONSOLE_DEVICE_TYPE_UART
#define BOARD_DEBUG_UART_BASEADDR (uint32_t) UART4
#define BOARD_DEBUG_UART_CLKSRC UART4_CLK_SRC
#define BOARD_DEBUG_UART_CLK_FREQ CLOCK_GetBusClkFreq()
#define BOARD_UART_IRQ UART4_RX_TX_IRQn
#define BOARD_UART_IRQ_HANDLER UART4_RX_TX_IRQHandler
#endif
#ifndef BOARD_DEBUG_UART_BAUDRATE
#define BOARD_DEBUG_UART_BAUDRATE 115200
#endif /* BOARD_DEBUG_UART_BAUDRATE */

#define BOARD_ACCEL_I2C_BASE I2C0

/*! @brief The CAN instance used for board. */
#define BOARD_CAN_BASEADDR CAN0

/*! @brief The i2c instance used for i2c connection by default */
#define BOARD_I2C_BASEADDR I2C1

/*! @brief The TPM instance/channel used for board */
#define BOARD_TPM_BASEADDR TPM2
#define BOARD_TPM_CHANNEL 1U

#define BOARD_FTM_BASEADDR FTM3
#define BOARD_FTM_X_CHANNEL 0U
#define BOARD_FTM_Y_CHANNEL 1U
#define BOARD_FTM_PERIOD_HZ 100
#define BOARD_FTM_IRQ_HANDLER FTM0_IRQHandler
#define BOARD_FTM_IRQ_VECTOR FTM0_IRQn

/*! @brief The bubble level demo information */
#define BOARD_FXOS8700_ADDR 0x1D
#define BOARD_ACCEL_ADDR BOARD_FXOS8700_ADDR
#define BOARD_ACCEL_BAUDRATE 100
#define BOARD_ACCEL_I2C_BASEADDR I2C0

/*! @brief The Enet instance used for board */
#define BOARD_ENET_BASEADDR ENET

/*! @brief The FlexBus instance used for board.*/
#define BOARD_FLEXBUS_BASEADDR FB

/*! @brief The SDHC instance/channel used for board. */
#define BOARD_SDHC_BASEADDR SDHC
#define BOARD_SDHC_CD_GPIO_IRQ_HANDLER PORTD_IRQHandler

/*! @brief The CMP instance/channel used for board. */
#define BOARD_CMP_BASEADDR CMP2
#define BOARD_CMP_CHANNEL 2U

/*! @brief The i2c instance used for board. */
#define BOARD_SAI_DEMO_I2C_BASEADDR I2C0

/*! @brief The rtc instance used for board. */
#define BOARD_RTC_FUNC_BASEADDR RTC

/*! @brief If connected the TWR_MEM, this is spi sd card */
#define BOARD_SDCARD_CARD_DETECTION_GPIO_PORT GPIOD
#define SDCARD_CARD_DETECTION_GPIO_PIN 10U
#define SDCARD_CARD_INSERTED 0U

/*! @brief Define the port interrupt number for the board switches */
#define BOARD_SW3_GPIO GPIOA
#define BOARD_SW3_PORT PORTA
#define BOARD_SW3_GPIO_PIN 10U
#define BOARD_SW3_IRQ PORTA_IRQn
#define BOARD_SW3_IRQ_HANDLER PORTA_IRQHandler
#define BOARD_SW3_NAME "SW3"

#define LLWU_SW_GPIO BOARD_SW3_GPIO
#define LLWU_SW_PORT BOARD_SW3_PORT
#define LLWU_SW_GPIO_PIN BOARD_SW3_GPIO_PIN
#define LLWU_SW_IRQ BOARD_SW3_IRQ
#define LLWU_SW_IRQ_HANDLER BOARD_SW3_IRQ_HANDLER
#define LLWU_SW_NAME BOARD_SW3_NAME

/* Board led color mapping */

#define LED_RUN_INIT(output)                                                 \
    GPIO_WritePinOutput(BOARD_INITLEDS_LED_RUN_GPIO, BOARD_INITLEDS_LED_RUN_GPIO_PIN, output); \
    BOARD_INITLEDS_LED_RUN_GPIO->PDDR |= (1U << BOARD_INITLEDS_LED_RUN_GPIO_PIN) /*!< Enable target LED_RUN */
#define LED_RUN_ON() \
    GPIO_ClearPinsOutput(BOARD_INITLEDS_LED_RUN_GPIO, 1U << BOARD_INITLEDS_LED_RUN_GPIO_PIN) /*!< Turn on target LED_RUN */
#define LED_RUN_OFF() \
    GPIO_SetPinsOutput(BOARD_INITLEDS_LED_RUN_GPIO, 1U << BOARD_INITLEDS_LED_RUN_GPIO_PIN) /*!< Turn off target LED_RUN */
#define LED_RUN_TOGGLE() \
    GPIO_TogglePinsOutput(BOARD_INITLEDS_LED_RUN_GPIO, 1U << BOARD_INITLEDS_LED_RUN_GPIO_PIN) /*!< Toggle on target LED_RUN */

#define LED_CON_GPRS_INIT(output)                                                   \
    GPIO_WritePinOutput(BOARD_INITLEDS_LED_CON_GPRS_GPIO, BOARD_INITLEDS_LED_CON_GPRS_GPIO_PIN, output); \
    BOARD_INITLEDS_LED_CON_GPRS_GPIO->PDDR |= (1U << BOARD_INITLEDS_LED_CON_GPRS_GPIO_PIN) /*!< Enable target LED_CON_GPRS */
#define LED_CON_GPRS_ON() \
    GPIO_ClearPinsOutput(BOARD_INITLEDS_LED_CON_GPRS_GPIO, 1U << BOARD_INITLEDS_LED_CON_GPRS_GPIO_PIN) /*!< Turn on target LED_CON_GPRS */
#define LED_CON_GPRS_OFF() \
    GPIO_SetPinsOutput(BOARD_INITLEDS_LED_CON_GPRS_GPIO, 1U << BOARD_INITLEDS_LED_CON_GPRS_GPIO_PIN) /*!< Turn off target LED_CON_GPRS */
#define LED_CON_GPRS_TOGGLE() \
    GPIO_TogglePinsOutput(BOARD_INITLEDS_LED_CON_GPRS_GPIO, 1U << BOARD_INITLEDS_LED_CON_GPRS_GPIO_PIN) /*!< Toggle on target LED_CON_GPRS */

#define LED_CON_ETH_INIT(output)                                                  \
    GPIO_WritePinOutput(BOARD_INITLEDS_LED_CON_ETH_GPIO, BOARD_INITLEDS_LED_CON_ETH_GPIO_PIN, output); \
    BOARD_INITLEDS_LED_CON_ETH_GPIO->PDDR |= (1U << BOARD_INITLEDS_LED_CON_ETH_GPIO_PIN) /*!< Enable target LED_CON_ETH */
#define LED_CON_ETH_ON() \
    GPIO_ClearPinsOutput(BOARD_INITLEDS_LED_CON_ETH_GPIO, 1U << BOARD_INITLEDS_LED_CON_ETH_GPIO_PIN) /*!< Turn on target LED_CON_ETH */
#define LED_CON_ETH_OFF() \
    GPIO_SetPinsOutput(BOARD_INITLEDS_LED_CON_ETH_GPIO, 1U << BOARD_INITLEDS_LED_CON_ETH_GPIO_PIN) /*!< Turn off target LED_CON_ETH */
#define LED_CON_ETH_TOGGLE() \
    GPIO_TogglePinsOutput(BOARD_INITLEDS_LED_CON_ETH_GPIO, 1U << BOARD_INITLEDS_LED_CON_ETH_GPIO_PIN) /*!< Toggle on target LED_CON_ETH */

#define LED_ALARM_INIT(output)                                                  \
    GPIO_WritePinOutput(BOARD_INITLEDS_LED_ALARM_GPIO, BOARD_INITLEDS_LED_ALARM_GPIO_PIN, output); \
    BOARD_INITLEDS_LED_ALARM_GPIO->PDDR |= (1U << BOARD_INITLEDS_LED_ALARM_GPIO_PIN) /*!< Enable target LED_ALARM */
#define LED_ALARM_ON() \
    GPIO_ClearPinsOutput(BOARD_INITLEDS_LED_ALARM_GPIO, 1U << BOARD_INITLEDS_LED_ALARM_GPIO_PIN) /*!< Turn on target LED_ALARM */
#define LED_ALARM_OFF() \
    GPIO_SetPinsOutput(BOARD_INITLEDS_LED_ALARM_GPIO, 1U << BOARD_INITLEDS_LED_ALARM_GPIO_PIN) /*!< Turn off target LED_ALARM */
#define LED_ALARM_TOGGLE() \
    GPIO_TogglePinsOutput(BOARD_INITLEDS_LED_ALARM_GPIO, 1U << BOARD_INITLEDS_LED_ALARM_GPIO_PIN) /*!< Toggle on target LED_ALARM */

#define LED_STATUS_INIT(output)                                                  \
    GPIO_WritePinOutput(BOARD_INITLEDS_LED_STATUS_GPIO, BOARD_INITLEDS_LED_STATUS_GPIO_PIN, output); \
    BOARD_INITLEDS_LED_STATUS_GPIO->PDDR |= (1U << BOARD_INITLEDS_LED_STATUS_GPIO_PIN) /*!< Enable target LED_STATUS */
#define LED_STATUS_ON() \
    GPIO_ClearPinsOutput(BOARD_INITLEDS_LED_STATUS_GPIO, 1U << BOARD_INITLEDS_LED_STATUS_GPIO_PIN) /*!< Turn on target LED_STATUS */
#define LED_STATUS_OFF() \
    GPIO_SetPinsOutput(BOARD_INITLEDS_LED_STATUS_GPIO, 1U << BOARD_INITLEDS_LED_STATUS_GPIO_PIN) /*!< Turn off target LED_STATUS */
#define LED_STATUS_TOGGLE() \
    GPIO_TogglePinsOutput(BOARD_INITLEDS_LED_STATUS_GPIO, 1U << BOARD_INITLEDS_LED_STATUS_GPIO_PIN) /*!< Toggle on target LED_STATUS */

#define IO_MUX_S1_INIT(output)                                                  \
    GPIO_WritePinOutput(BOARD_INITIOS_MUX_S1_GPIO, BOARD_INITIOS_MUX_S1_GPIO_PIN, output); \
    BOARD_INITIOS_MUX_S1_GPIO->PDDR |= (1U << BOARD_INITIOS_MUX_S1_GPIO_PIN) /*!< Enable target IO_MUX_S1 */
#define IO_MUX_S1_ON() \
    GPIO_ClearPinsOutput(BOARD_INITIOS_MUX_S1_GPIO, 1U << BOARD_INITIOS_MUX_S1_GPIO_PIN) /*!< Turn on target IO_MUX_S1 */
#define IO_MUX_S1_OFF() \
    GPIO_SetPinsOutput(BOARD_INITIOS_MUX_S1_GPIO, 1U << BOARD_INITIOS_MUX_S1_GPIO_PIN) /*!< Turn off target IO_MUX_S1 */

#define IO_MUX_S3_INIT(output)                                                  \
    GPIO_WritePinOutput(BOARD_INITIOS_MUX_S3_GPIO, BOARD_INITIOS_MUX_S3_GPIO_PIN, output); \
    BOARD_INITIOS_MUX_S3_GPIO->PDDR |= (1U << BOARD_INITIOS_MUX_S3_GPIO_PIN) /*!< Enable target IO_MUX_S3 */
#define IO_MUX_S3_ON() \
    GPIO_ClearPinsOutput(BOARD_INITIOS_MUX_S3_GPIO, 1U << BOARD_INITIOS_MUX_S3_GPIO_PIN) /*!< Turn on target IO_MUX_S3 */
#define IO_MUX_S3_OFF() \
    GPIO_SetPinsOutput(BOARD_INITIOS_MUX_S3_GPIO, 1U << BOARD_INITIOS_MUX_S3_GPIO_PIN) /*!< Turn off target IO_MUX_S3 */

#define IO_MUX_S2_INIT(output)                                                  \
    GPIO_WritePinOutput(BOARD_INITIOS_MUX_S2_GPIO, BOARD_INITIOS_MUX_S2_GPIO_PIN, output); \
    BOARD_INITIOS_MUX_S2_GPIO->PDDR |= (1U << BOARD_INITIOS_MUX_S2_GPIO_PIN) /*!< Enable target IO_MUX_S2 */
#define IO_MUX_S2_ON() \
    GPIO_ClearPinsOutput(BOARD_INITIOS_MUX_S2_GPIO, 1U << BOARD_INITIOS_MUX_S2_GPIO_PIN) /*!< Turn on target IO_MUX_S2 */
#define IO_MUX_S2_OFF() \
    GPIO_SetPinsOutput(BOARD_INITIOS_MUX_S2_GPIO, 1U << BOARD_INITIOS_MUX_S2_GPIO_PIN) /*!< Turn off target IO_MUX_S2 */

#define IO_MUX_S0_INIT(output)                                                  \
    GPIO_WritePinOutput(BOARD_INITIOS_MUX_S0_GPIO, BOARD_INITIOS_MUX_S0_GPIO_PIN, output); \
    BOARD_INITIOS_MUX_S0_GPIO->PDDR |= (1U << BOARD_INITIOS_MUX_S0_GPIO_PIN) /*!< Enable target IO_MUX_S0 */
#define IO_MUX_S0_ON() \
    GPIO_ClearPinsOutput(BOARD_INITIOS_MUX_S0_GPIO, 1U << BOARD_INITIOS_MUX_S0_GPIO_PIN) /*!< Turn on target IO_MUX_S0 */
#define IO_MUX_S0_OFF() \
    GPIO_SetPinsOutput(BOARD_INITIOS_MUX_S0_GPIO, 1U << BOARD_INITIOS_MUX_S0_GPIO_PIN) /*!< Turn off target IO_MUX_S0 */
//================================== OUTPUT PIN CONFIGURATION =============================================//
#define IO_WAITMCU_1_INIT(output)                                                  \
    GPIO_WritePinOutput(BOARD_INITIOS_WAIT_MCU1_GPIO, BOARD_INITIOS_WAIT_MCU1_GPIO_PIN, output); \
    BOARD_INITIOS_WAIT_MCU1_GPIO->PDDR |= (1U << BOARD_INITIOS_WAIT_MCU1_GPIO_PIN) /*!< Enable target IO_WAITMCU_1 */
#define IO_WAITMCU_1_OFF() \
    GPIO_ClearPinsOutput(BOARD_INITIOS_WAIT_MCU1_GPIO, 1U << BOARD_INITIOS_WAIT_MCU1_GPIO_PIN) /*!< Turn on target IO_WAITMCU_1 */
#define IO_WAITMCU_1_ON() \
    GPIO_SetPinsOutput(BOARD_INITIOS_WAIT_MCU1_GPIO, 1U << BOARD_INITIOS_WAIT_MCU1_GPIO_PIN) /*!< Turn off target IO_WAITMCU_1 */

#define IO_WAITMCU_2_INIT(output)                                                  \
    GPIO_WritePinOutput(BOARD_INITIOS_WAIT_MCU2_GPIO, BOARD_INITIOS_WAIT_MCU2_GPIO_PIN, output); \
    BOARD_INITIOS_WAIT_MCU2_GPIO->PDDR |= (1U << BOARD_INITIOS_WAIT_MCU2_GPIO_PIN) /*!< Enable target IO_WAITMCU_2 */
#define IO_WAITMCU_2_OFF() \
    GPIO_ClearPinsOutput(BOARD_INITIOS_WAIT_MCU2_GPIO, 1U << BOARD_INITIOS_WAIT_MCU2_GPIO_PIN) /*!< Turn on target IO_WAITMCU_2 */
#define IO_WAITMCU_2_ON() \
    GPIO_SetPinsOutput(BOARD_INITIOS_WAIT_MCU2_GPIO, 1U << BOARD_INITIOS_WAIT_MCU2_GPIO_PIN) /*!< Turn off target IO_WAITMCU_2 */

#define IO_WAITMCU_3_INIT(output)                                                  \
    GPIO_WritePinOutput(BOARD_INITIOS_WAIT_MCU3_GPIO, BOARD_INITIOS_WAIT_MCU3_GPIO_PIN, output); \
    BOARD_INITIOS_WAIT_MCU3_GPIO->PDDR |= (1U << BOARD_INITIOS_WAIT_MCU3_GPIO_PIN) /*!< Enable target IO_WAITMCU_3 */
#define IO_WAITMCU_3_OFF() \
    GPIO_ClearPinsOutput(BOARD_INITIOS_WAIT_MCU3_GPIO, 1U << BOARD_INITIOS_WAIT_MCU3_GPIO_PIN) /*!< Turn on target IO_WAITMCU_3 */
#define IO_WAITMCU_3_ON() \
    GPIO_SetPinsOutput(BOARD_INITIOS_WAIT_MCU3_GPIO, 1U << BOARD_INITIOS_WAIT_MCU3_GPIO_PIN) /*!< Turn off target IO_WAITMCU_3 */

#define IO_WAITMCU_4_INIT(output)                                                  \
    GPIO_WritePinOutput(BOARD_INITIOS_WAIT_MCU4_GPIO, BOARD_INITIOS_WAIT_MCU4_GPIO_PIN, output); \
    BOARD_INITIOS_WAIT_MCU4_GPIO->PDDR |= (1U << BOARD_INITIOS_WAIT_MCU4_GPIO_PIN) /*!< Enable target IO_WAITMCU_4 */
#define IO_WAITMCU_4_OFF() \
    GPIO_ClearPinsOutput(BOARD_INITIOS_WAIT_MCU4_GPIO, 1U << BOARD_INITIOS_WAIT_MCU4_GPIO_PIN) /*!< Turn on target IO_WAITMCU_4 */
#define IO_WAITMCU_4_ON() \
    GPIO_SetPinsOutput(BOARD_INITIOS_WAIT_MCU4_GPIO, 1U << BOARD_INITIOS_WAIT_MCU4_GPIO_PIN) /*!< Turn off target IO_WAITMCU_4 */

#define IO_WAITMCU_5_INIT(output)                                                  \
    GPIO_WritePinOutput(BOARD_INITIOS_WAIT_MCU5_GPIO, BOARD_INITIOS_WAIT_MCU5_GPIO_PIN, output); \
    BOARD_INITIOS_WAIT_MCU5_GPIO->PDDR |= (1U << BOARD_INITIOS_WAIT_MCU5_GPIO_PIN) /*!< Enable target IO_WAITMCU_5 */
#define IO_WAITMCU_5_OFF() \
    GPIO_ClearPinsOutput(BOARD_INITIOS_WAIT_MCU5_GPIO, 1U << BOARD_INITIOS_WAIT_MCU5_GPIO_PIN) /*!< Turn on target IO_WAITMCU_5 */
#define IO_WAITMCU_5_ON() \
    GPIO_SetPinsOutput(BOARD_INITIOS_WAIT_MCU5_GPIO, 1U << BOARD_INITIOS_WAIT_MCU5_GPIO_PIN) /*!< Turn off target IO_WAITMCU_5 */

#define IO_OPENDOOR_MCU_INIT(output)                                                  \
    GPIO_WritePinOutput(BOARD_INITIOS_OPENDOOR_MCU_GPIO, BOARD_INITIOS_OPENDOOR_MCU_GPIO_PIN, output); \
    BOARD_INITIOS_OPENDOOR_MCU_GPIO->PDDR |= (1U << BOARD_INITIOS_OPENDOOR_MCU_GPIO_PIN) /*!< Enable target IO_OPENDOOR_MCU */
#define IO_OPENDOOR_MCU_OFF() \
    GPIO_ClearPinsOutput(BOARD_INITIOS_OPENDOOR_MCU_GPIO, 1U << BOARD_INITIOS_OPENDOOR_MCU_GPIO_PIN) /*!< Turn on target IO_OPENDOOR_MCU */
#define IO_OPENDOOR_MCU_ON() \
    GPIO_SetPinsOutput(BOARD_INITIOS_OPENDOOR_MCU_GPIO, 1U << BOARD_INITIOS_OPENDOOR_MCU_GPIO_PIN) /*!< Turn off target IO_OPENDOOR_MCU */

#define IO_ALARM_LIGHT_INIT(output)                                                  \
    GPIO_WritePinOutput(BOARD_INITIOS_ALARM_LIGHT_GPIO, BOARD_INITIOS_ALARM_LIGHT_GPIO_PIN, output); \
    BOARD_INITIOS_ALARM_LIGHT_GPIO->PDDR |= (1U << BOARD_INITIOS_ALARM_LIGHT_GPIO_PIN) /*!< Enable target IO_ALARM_LIGHT */
#define IO_ALARM_LIGHT_OFF() \
    GPIO_ClearPinsOutput(BOARD_INITIOS_ALARM_LIGHT_GPIO, 1U << BOARD_INITIOS_ALARM_LIGHT_GPIO_PIN) /*!< Turn on target IO_ALARM_LIGHT */
#define IO_ALARM_LIGHT_ON() \
    GPIO_SetPinsOutput(BOARD_INITIOS_ALARM_LIGHT_GPIO, 1U << BOARD_INITIOS_ALARM_LIGHT_GPIO_PIN) /*!< Turn off target IO_ALARM_LIGHT */

#define IO_ALARM_SPEAKER_INIT(output)                                                  \
    GPIO_WritePinOutput(BOARD_INITIOS_ALARM_SPEAKER_GPIO, BOARD_INITIOS_ALARM_SPEAKER_GPIO_PIN, output); \
    BOARD_INITIOS_ALARM_SPEAKER_GPIO->PDDR |= (1U << BOARD_INITIOS_ALARM_SPEAKER_GPIO_PIN) /*!< Enable target IO_ALARM_SPEAKER */
#define IO_ALARM_SPEAKER_OFF() \
    GPIO_ClearPinsOutput(BOARD_INITIOS_ALARM_SPEAKER_GPIO, 1U << BOARD_INITIOS_ALARM_SPEAKER_GPIO_PIN) /*!< Turn on target IO_ALARM_SPEAKER */
#define IO_ALARM_SPEAKER_ON() \
    GPIO_SetPinsOutput(BOARD_INITIOS_ALARM_SPEAKER_GPIO, 1U << BOARD_INITIOS_ALARM_SPEAKER_GPIO_PIN) /*!< Turn off target IO_ALARM_SPEAKER */

//================================== OUTPUT PIN CONFIGURATION =============================================//
 // CanhLT - 23/12
//================================== GPRS PIN CONFIGURATION =============================================//
#define GPRS_EN_INIT(output)                                                 \
    GPIO_WritePinOutput(BOARD_GPRS_EN_GPIO, BOARD_GPRS_EN_GPIO_PIN, output); \
    BOARD_GPRS_EN_GPIO->PDDR |= (1U << BOARD_GPRS_EN_GPIO_PIN) /*!< Enable target PWR_KEY */
#define GPRS_EN_ON() \
    GPIO_ClearPinsOutput(BOARD_GPRS_EN_GPIO, 1U << BOARD_GPRS_EN_GPIO_PIN) /*!< Turn on target PWR_KEY */
#define GPRS_EN_OFF() \
    GPIO_SetPinsOutput(BOARD_GPRS_EN_GPIO, 1U << BOARD_GPRS_EN_GPIO_PIN) /*!< Turn off target PWR_KEY */

#define GPRS_PWR_INIT(output)                                                 \
    GPIO_WritePinOutput(BOARD_GPRS_PWR_GPIO, BOARD_GPRS_PWR_GPIO_PIN, output); \
    BOARD_GPRS_PWR_GPIO->PDDR |= (1U << BOARD_GPRS_PWR_GPIO_PIN) /*!< Enable target PWR_ON */
#define GPRS_PWR_OFF() \
    GPIO_ClearPinsOutput(BOARD_GPRS_PWR_GPIO, 1U << BOARD_GPRS_PWR_GPIO_PIN) /*!< Turn on target PWR_ON */
#define GPRS_PWR_ON() \
    GPIO_SetPinsOutput(BOARD_GPRS_PWR_GPIO, 1U << BOARD_GPRS_PWR_GPIO_PIN) /*!< Turn off target PWR_ON */


#define IO_DI1_READ() \
	GPIO_ReadPinInput(BOARD_INITIOS_DI1_GPIO, BOARD_INITIOS_DI1_GPIO_PIN)
#define IO_DI2_READ() \
	GPIO_ReadPinInput(BOARD_INITIOS_DI2_GPIO, BOARD_INITIOS_DI2_GPIO_PIN)

#define BUTTON_UP_READ() \
	GPIO_ReadPinInput(BOARD_INITBUTTONS_BUTTON_UP_GPIO, BOARD_INITBUTTONS_BUTTON_UP_GPIO_PIN)
#define BUTTON_DOWN_READ() \
	GPIO_ReadPinInput(BOARD_INITBUTTONS_BUTTON_DOWN_GPIO, BOARD_INITBUTTONS_BUTTON_DOWN_GPIO_PIN)
#define BUTTON_ESC_READ() \
	GPIO_ReadPinInput(BOARD_INITBUTTONS_BUTTON_ESC_GPIO, BOARD_INITBUTTONS_BUTTON_ESC_GPIO_PIN)
#define BUTTON_ENTER_READ() \
	GPIO_ReadPinInput(BOARD_INITBUTTONS_BUTTON_ENTER_GPIO, BOARD_INITBUTTONS_BUTTON_ENTER_GPIO_PIN)

/*! @brief Define the port interrupt number for the usb id gpio pin */
#define BOARD_ID_GPIO GPIOE
#define BOARD_ID_PORT PORTE
#define BOARD_ID_GPIO_PIN 10U
#define BOARD_ID_IRQ PORTE_IRQn

/* SDHC base address, clock and card detection pin */
#define BOARD_SDHC_BASEADDR SDHC
#define BOARD_SDHC_CLKSRC kCLOCK_CoreSysClk
#define BOARD_SDHC_IRQ SDHC_IRQn
#define BOARD_SDHC_CD_GPIO_BASE GPIOD
#define BOARD_SDHC_CD_GPIO_PIN 10U
#define BOARD_SDHC_CD_PORT_BASE PORTD
#define BOARD_SDHC_CD_PORT_IRQ PORTD_IRQn
#define BOARD_SDHC_CD_PORT_IRQ_HANDLER PORTD_IRQHandler
#define BOARD_SDHC_CD_LOGIC_RISING

/* ERPC DSPI configuration */
#define ERPC_BOARD_DSPI_BASEADDR SPI0
#define ERPC_BOARD_DSPI_BAUDRATE 500000U
#define ERPC_BOARD_DSPI_CLKSRC DSPI0_CLK_SRC
#define ERPC_BOARD_DSPI_INT_GPIO GPIOB
#define ERPC_BOARD_DSPI_INT_PORT PORTB
#define ERPC_BOARD_DSPI_INT_PIN 2U
#define ERPC_BOARD_DSPI_INT_PIN_IRQ PORTB_IRQn
#define ERPC_BOARD_DSPI_INT_PIN_IRQ_HANDLER PORTB_IRQHandler

/* DAC base address */
#define BOARD_DAC_BASEADDR DAC0

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/*******************************************************************************
 * API
 ******************************************************************************/

void BOARD_InitDebugConsole(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* _BOARD_H_ */
