/**
* @file ipConnection_manager.c
* @brief GPRS control routine
*
* @section License
* ^^(^____^)^^
*
**/

//Dependencies
#include "core/net.h"
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

NetInterface *activeNetInterface = NULL;
//========================================
//Function Implementation
//========================================
void snmpConnectManagerTask (void *param)
{
  NetInterface *netInterface= ethernetSnmpAgentContext.socket->interface;
  IpAddr ipaddr; 
  uint32_t timeout = 3000;
  uint32_t rtt_time;
  error_t status;
  uint16_t pingRetry = 0;
  bool pingRequested = FALSE;
  ipStringToAddr((const char*)sMenu_Variable.ucSIP, &ipaddr); 
  osDelayTask(5000); 
  for (;;)  {
    if (snmpConnectManager.pingTick > PING_SEND_PERIOD)
    {
      netInterface = ethernetSnmpAgentContext.socket->interface;
      if (netInterface->linkState == TRUE)
      {
        TRACE_INFO("Send ping to %s\r\n", sMenu_Variable.ucSIP);  
        status = ping(netInterface, &ipaddr, 32, 255, timeout, &rtt_time);
        snmpConnectManager.pingTick = 0;
        pingRequested = TRUE;
        if (status != NO_ERROR)
          TRACE_INFO("ethernet ping failed...\r\n");
      }
      else
      {
        if (snmpConnectManager.status == ETHERNET_CONNECTED)
        {
          activeNetInterface = NULL;
          snmpConnectManager.status = DISCONNECTED;
          ModemInterfaceSetState(MODEM_INTERFACE_STATE_CONNECTED);
        }          
      }
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
          activeNetInterface = NULL;
          ModemInterfaceSetState(MODEM_INTERFACE_STATE_CONNECTED);
        } else pingRetry++;
      }
      break;
    case GPRS_CONNECTED:
      if (pingRequested && (status == NO_ERROR))
      {
        pingRequested = false;
        snmpConnectManager.status = ETHERNET_CONNECTED;        
        activeNetInterface = &netInterface[0];
        MenuGetDeviceIpv4(&privateMibBase.siteInfoGroup.siteInfoIpAddress);
        ModemInterfaceSetState(MODEM_INTERFACE_STATE_DISCONNECTED);
        TRACE_INFO("Ethernet up, turn GPRS OFF\r\n");
      }
      else if (ModemInterfaceGetState() == MODEM_INTERFACE_STATE_DISCONNECTED)
      {
        snmpConnectManager.status = DISCONNECTED;
        activeNetInterface = NULL;
        ModemInterfaceSetState(MODEM_INTERFACE_STATE_CONNECTED);
        TRACE_INFO("Turn GPRS on... %d\r\n", ModemInterfaceGetState());
      }
      break;
    case DISCONNECTED:
      if (pingRequested && (status == NO_ERROR))
      {
        activeNetInterface = &netInterface[0];
        snmpConnectManager.status = ETHERNET_CONNECTED;
//        MenuGetDeviceIpv4(&privateMibBase.siteInfoGroup.siteInfoIpAddress);
        ModemInterfaceSetState(MODEM_INTERFACE_STATE_DISCONNECTED);
        TRACE_INFO("Ethernet up\r\n");
      }
      else if (ModemInterfaceGetState() == MODEM_INTERFACE_STATE_DISCONNECTED)
        ModemInterfaceSetState(MODEM_INTERFACE_STATE_CONNECTED);
      else if (ModemInterfaceGetState() == MODEM_INTERFACE_STATE_CONNECTED)       
      {
        snmpConnectManager.status = GPRS_CONNECTED;
        activeNetInterface = &netInterface[1];
        TRACE_INFO("ethernet down, GPRS up\r\n");
      }
      break;
    default:
      break;
    }
    pingRequested = FALSE;
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

NetInterface* interfaceManagerGetActiveInterface()
{
  return activeNetInterface;
}