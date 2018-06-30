#ifndef __SNMPCONNECT_MANAGER_H
#define __SNMPCONNECT_MANAGER_H

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "board.h"
#include "fsl_gpio.h"
#include "fsl_uart.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "portmacro.h"
#include "task.h"
#include "debug.h"
#include "net_config.h"

#include "core/net.h"
#include "core/ping.h"
#include "core/ip.h"
#include "ipv4/ipv4.h"
#include "ipv4/icmp.h"
#include "ipv6/ipv6.h"
#include "ipv6/ipv6_misc.h"
#include "ipv6/icmpv6.h"
#include "core/socket.h"
#include "debug.h"
#include "variables.h"

#if (USERDEF_CLIENT_SNMP == ENABLED)
#include "snmp/snmp_agent.h"
#endif

/**
* @brief IP Connection context
**/
typedef enum connection_status_e
{
  ETHERNET_CONNECTED,
  GPRS_CONNECTED,
  DISCONNECTED
} connection_status_t;

typedef struct
{
  connection_status_t status;
  uint32_t pingTick;
  OsMutex mutex;                                        ///<Mutex preventing simultaneous access to SNMP agent context
}SNMPConnectManager;

#define PING_SEND_PERIOD        5

//=======================================
//Function declearation
//=======================================
void snmpConnectManagerTask (void *param);
connection_status_t snmpConnectCheckStatus (void);
void snmpConnectIncreaseTick(void);
void snmpConnectManagerSendPing (void);

#endif
