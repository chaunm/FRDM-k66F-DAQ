#ifndef __MODEM_PORTS_H__
#define __MODEM_PORTS_H__
#include "pin_mux.h"
#include "board.h"

#define MODEM_UART                              UART1
#define MODEM_UART_CLKSRC                       SYS_CLK
#define MODEM_UART_IRQn                         UART1_RX_TX_IRQn
#define MODEM_UART_IRQHandler                   UART1_RX_TX_IRQHandler
#define MODEM_UART_BAUDRATE                     172827      
#define MODEM_EN_INIT                           GPRS_EN_INIT
#define MODEM_PWR_INIT                          GPRS_PWR_INIT
#define MODEM_EN_OFF                            GPRS_EN_OFF
#define MODEM_EN_ON                             GPRS_EN_ON
#define MODEM_PWR_OFF                           GPRS_PWR_OFF
#define MODEM_PWR_ON                            GPRS_PWR_ON
#endif