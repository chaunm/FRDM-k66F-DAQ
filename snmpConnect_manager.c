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
#include "private_mib_module.h"
#include "menu.h"

//========================================
//Global Variable
//========================================
#if (USERDEF_CLIENT_SNMP == ENABLED)
extern SnmpAgentSettings snmpAgentSettings;
extern SnmpAgentContext snmpAgentContext;
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
  NetInterface *netInterface= snmpAgentContext.socket->interface;
  IpAddr ipaddr; //= //&snmpAgentContext.remoteIpAddr;
  uint32_t timeout = 1000/ portTICK_PERIOD_MS;
  uint32_t rtt_time;
  error_t status;
  uint16_t pingRetry = 0;
  bool pingRequested = false;
  char ipaddrStr[24];
  TRACE_INFO("SNMP connect manager task started\r\n");
  ipStringToAddr((const char*)sMenu_Variable.ucSIP, &ipaddr); 
  //ipStringToAddr("8.8.8.8", &ipaddr); 
  for (;;)  {
    if (snmpConnectManager.pingTick > PING_SEND_PERIOD)
    {
      TRACE_INFO("Send ping to %s\r\n", sMenu_Variable.ucSIP);      
      netInterface = snmpAgentContext.socket->interface;
      status = ping(netInterface, &ipaddr, 32, 255, timeout, &rtt_time);
      snmpConnectManager.pingTick = 0;
      pingRequested = true;
    }    
    switch (snmpConnectManager.status)
    {
    case ETHERNET_CONNECTED:
      if (pingRequested & (status != NO_ERROR))
      {
        TRACE_INFO("ping failed...\r\n");
        pingRequested = false;
        if (pingRetry > 1)
        {
          pingRetry = 0;
          if (gprsCheckStatus() == GPRS_POWER_OFF)
          {
            gprs_turnon();
            snmpConnectManager.status = DISCONNECTED;
          }
          else if (gprsCheckStatus() == GPRS_DISCONNECTED)
          {
            gprs_connect();
            snmpConnectManager.status = DISCONNECTED;
          }
          else if (gprsCheckStatus() == TRANSPARENT_DATA_MODE)
          {
            snmpConnectManager.status = GPRS_CONNECTED;
            privateMibBase.siteInfoGroup.siteInfoIpAddress = gprsGetIpAddr();
            TRACE_INFO("ethernet down, GPRS up\r\n");
          }
        } else pingRetry++;
      }
      break;
    case GPRS_CONNECTED:
      if (pingRequested & (status == NO_ERROR))
      {
        pingRequested = false;
        snmpConnectManager.status = ETHERNET_CONNECTED;
        gprs_disconnect();
        MenuGetDeviceIpv4(&privateMibBase.siteInfoGroup.siteInfoIpAddress);
        TRACE_INFO("Ethernet up, turn GPRS OFF\r\n");
      }
      else if (gprsCheckStatus() == GPRS_POWER_OFF)
      {
        gprs_turnon();
        TRACE_INFO("Turn GPRS on... %d\r\n", gprsCheckStatus());
      }
      else if (gprsCheckStatus() == GPRS_DISCONNECTED)
        gprs_connect();
      break;
    case DISCONNECTED:
      if (status == NO_ERROR)
      {
        snmpConnectManager.status = ETHERNET_CONNECTED;
        MenuGetDeviceIpv4(&privateMibBase.siteInfoGroup.siteInfoIpAddress);
        TRACE_INFO("Ethernet up\r\n");
      }
      else if (gprsCheckStatus() == GPRS_POWER_OFF)
        gprs_turnon();
      else if (gprsCheckStatus() == GPRS_DISCONNECTED)
        gprs_connect();
      else if (gprsCheckStatus() == TRANSPARENT_DATA_MODE)
      {
        snmpConnectManager.status = GPRS_CONNECTED;
        privateMibBase.siteInfoGroup.siteInfoIpAddress = gprsGetIpAddr();
        TRACE_INFO("ethernet down, GPRS up\r\n");
      }
      break;
    default:
      break;
    }
    osDelayTask(100);
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

void snmpConnectManagerSendPing (void)
{
  NetInterface *netInterface= snmpAgentContext.socket->interface;
  IpAddr ipaddr; //= //&snmpAgentContext.remoteIpAddr;
  uint32_t timeout = 1000/ portTICK_PERIOD_MS;
  uint32_t rtt_time;
  error_t status;
  uint16_t pingRetry = 0;
  bool pingRequested = false;
  char ipaddrStr[24];
  
  ipStringToAddr( "192.168.1.200",&ipaddr);
  
  TRACE_INFO("Send ping through Trap to %s...\r\n", ipAddrToString(&ipaddr, NULL));
  
  netInterface = snmpAgentContext.socket->interface;
  status = ping(netInterface, &ipaddr, 64, 128, timeout, &rtt_time);
  pingRequested = true;
}