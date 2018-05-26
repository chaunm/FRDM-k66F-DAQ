/**
* @file uart_driver.c
* @brief UART driver
*
* @section License
*
* Copyright (C) 2010-2018 Oryx Embedded SARL. All rights reserved.
*
* This file is part of CycloneTCP Eval.
*
* This software is provided in source form for a short-term evaluation only. The
* evaluation license expires 90 days after the date you first download the software.
*
* If you plan to use this software in a commercial product, you are required to
* purchase a commercial license from Oryx Embedded SARL.
*
* After the 90-day evaluation period, you agree to either purchase a commercial
* license or delete all copies of this software. If you wish to extend the
* evaluation period, you must contact sales@oryx-embedded.com.
*
* This evaluation software is provided "as is" without warranty of any kind.
* Technical support is available as an option during the evaluation period.
*
* @author Oryx Embedded SARL (www.oryx-embedded.com)
* @version 1.8.2
**/

//Dependencies
#include <stdio.h>
#include "fsl_uart.h"
#include "core/net.h"
#include "ppp/ppp_hdlc.h"
#include "uart_driver.h"
#include "modem_ports.h"
#include "debug.h"
#include "fsl_debug_console.h"

//Disable hardware flow control
#define APP_UART_HW_FLOW_CTRL DISABLE


/**
* @brief UART driver
**/

const UartDriver uartDriver =
{
  uartInit,
  uartEnableIrq,
  uartDisableIrq,
  uartStartTx
};


/**
* @brief UART configuration
* @return Error code
**/

error_t uartInit(void)
{
  uart_config_t config;
  //Debug message
  TRACE_INFO("Initializing UART for ppp modem...\r\n");
  // configure UART RX - TX pin if neccessary
  // in this project, IO has been configure in BOARD_InitUARTs() - pin_mux.c.
#if (APP_UART_HW_FLOW_CTRL == ENABLED)
    
#else
   // init UART for controlling the modem 
  UART_GetDefaultConfig(&config);
  config.baudRate_Bps = MODEM_UART_BAUDRATE;
  config.enableTx = true;
  config.enableRx = true;
  UART_Init(MODEM_UART, &config, CLOCK_GetFreq(MODEM_UART_CLKSRC));
#endif
  
  //Configure USART1 interrupts
  // configure TXE and RXNE interrup - not enable at this point
  UART_EnableInterrupts(MODEM_UART, kUART_RxDataRegFullInterruptEnable | kUART_RxOverrunInterruptEnable | kUART_TxDataRegEmptyInterruptEnable);
  NVIC_SetPriorityGrouping(3);
  //Configure interrupt priority
  NVIC_SetPriority(MODEM_UART_IRQn, NVIC_EncodePriority(3, 12, 0));
  //Enable USART if neccessary
  //Successful processing
  return NO_ERROR;
}


/**
* @brief Enable UART interrupts
**/

void uartEnableIrq(void)
{
  //Enable modem UART interrupts
  EnableIRQ(MODEM_UART_IRQn);
}


/**
* @brief Disable UART interrupts
**/

void uartDisableIrq(void)
{
  //Disable USART1 interrupt
  DisableIRQ(MODEM_UART_IRQn);
}


/**
* @brief Start transmission
**/

void uartStartTx(void)
{
  //Enable TXE interrupt
  UART_EnableInterrupts(MODEM_UART, kUART_TxDataRegEmptyInterruptEnable);
}


/**
* @brief UART interrupt handler
**/

void MODEM_UART_IRQHandler(void)
{
  int_t c;
  bool_t flag;
  NetInterface *interface;
  
  //Enter interrupt service routine
  osEnterIsr();
  
  //This flag will be set if a higher priority task must be woken
  flag = FALSE;
  
  //Point to the PPP network interface
  interface = &netInterface[0];
  
  //TXE interrupt?
  if(kUART_TxDataRegEmptyFlag & UART_GetStatusFlags(MODEM_UART))
  {
    //Get next character
    flag |= pppHdlcDriverReadTxQueue(interface, &c);
    
    //Valid character read?
    if(c != EOF)
    {
      //Send data byte
      MODEM_UART->D = c;
    }
    else
    {
      //Disable TXE interrupt
     UART_DisableInterrupts(MODEM_UART, kUART_TxDataRegEmptyInterruptEnable);
    }
  }
  
  //RXNE interrupt?
  if(kUART_RxDataRegFullFlag & UART_GetStatusFlags(MODEM_UART))
  {
    //Read data byte
    c = UART_ReadByte(MODEM_UART);
    //Process incoming character
    flag |= pppHdlcDriverWriteRxQueue(interface, c);
  }
  
  //ORE interrupt?
  if(kUART_RxOverrunFlag & UART_GetStatusFlags(MODEM_UART))
  {
    //Clear ORE interrupt flag
    UART_ClearStatusFlags(MODEM_UART, kUART_RxOverrunFlag);
  }
  
  //Leave interrupt service routine
  osExitIsr(flag);
}
