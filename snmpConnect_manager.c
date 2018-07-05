/**
* @file ipConnection_manager.c
* @brief GPRS control routine
*
* @section License
* ^^(^____^)^^
*
**/

//Dependencies
#include "snmpConnect_manager.h"
#include "modem_interface.h"
#include "private_mib_module.h"
#include "menu.h"

//========================================
//Global Variable
//========================================
#if (USERDEF_CLIENT_SNMP == ENABLED)
extern SnmpAgentContext ethernetSnmpAgentContext;
extern sMenu_Variable_Struct	sMenu_Variable;
#endif

SNMPConnectManager  snmpConnectManager = {
  .status = DISCONNECTED,
  .pingTick  = PING_SEND_PERIOD
};

//========================================
//Function Implementation
//========================================
void snmpConnectManagerTask (void *param)
{
  NetInterface *netInterface= ethernetSnmpAgentContext.socket->interface;
  IpAddr ipaddr; //= //&ethernetSnmpAgentContext.remoteIpAddr;
  uint32_t timeout = 3000;
  uint32_t rtt_time;
  error_t status;
  uint16_t pingRetry = 0;
  bool pingRequested = false;
//  TRACE_INFO("SNMP connect manager task started\r\n");
  ipStringToAddr((const char*)sMenu_Variable.ucSIP, &ipaddr); 
//  ipStringToAddr("192.168.1.161", &ipaddr); 
  for (;;)  {
    if (snmpConnectManager.pingTick > PING_SEND_PERIOD)
    {
      TRACE_INFO("Send ping to %s\r\n", sMenu_Variable.ucSIP);      
//      TRACE_INFO("Send ping to 8.8.8.8\r\n");      
      netInterface = ethernetSnmpAgentContext.socket->interface;
      status = ping(netInterface, &ipaddr, 32, 255, timeout, &rtt_time);
      snmpConnectManager.pingTick = 0;
      pingRequested = true;
      if (status != NO_ERROR)
        TRACE_INFO("ethernet ping failed...\r\n");
    }    
    switch (snmpConnectManager.status)
    {
    case ETHERNET_CONNECTED:
      if (pingRequested && (status != NO_ERROR))
      {
        pingRequested = false;
        if (pingRetry > 1)
        {
          pingRetry = 0;
          // establish ppp connection
          snmpConnectManager.status = DISCONNECTED;
          ModemInterfaceSetState(MODEM_INTERFACE_STATE_CONNECTED);
        } else pingRetry++;
      }
      break;
    case GPRS_CONNECTED:
      if (pingRequested && (status == NO_ERROR))
      {
        pingRequested = false;
        snmpConnectManager.status = ETHERNET_CONNECTED;        
        MenuGetDeviceIpv4(&privateMibBase.siteInfoGroup.siteInfoIpAddress);
        ModemInterfaceSetState(MODEM_INTERFACE_STATE_DISCONNECTED);
        TRACE_INFO("Ethernet up, turn GPRS OFF\r\n");
      }
      else if (ModemInterfaceGetState() == MODEM_INTERFACE_STATE_DISCONNECTED)
      {
        ModemInterfaceSetState(MODEM_INTERFACE_STATE_CONNECTED);
        TRACE_INFO("Turn GPRS on... %d\r\n", ModemInterfaceGetState());
      }
      break;
    case DISCONNECTED:
      if (status == NO_ERROR)
      {
        snmpConnectManager.status = ETHERNET_CONNECTED;
        MenuGetDeviceIpv4(&privateMibBase.siteInfoGroup.siteInfoIpAddress);
        TRACE_INFO("Ethernet up\r\n");
      }
      else if (ModemInterfaceGetState() == MODEM_INTERFACE_STATE_DISCONNECTED)
        ModemInterfaceSetState(MODEM_INTERFACE_STATE_CONNECTED);
      else if (ModemInterfaceGetState() == MODEM_INTERFACE_STATE_CONNECTED)       
      {
        snmpConnectManager.status = GPRS_CONNECTED;
        // get ip addr
        //privateMibBase.siteInfoGroup.siteInfoIpAddress = gprsGetIpAddr();
        TRACE_INFO("ethernet down, GPRS up\r\n");
      }
      break;
    default:
      break;
    }
    osDelayTask(1000);
  }
}


connection_status_t snmpConnectCheckStatus (void)
{
  return snmpConnectManager.status;
}

void snmpConnectIncreaseTick (void)
{
  snmpConnectManager.pingTick ++;
}
