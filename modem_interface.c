#include "modem_interface.h"
#include "modem.h"
#include "uart_driver.h"
#include "ppp/ppp.h"
#include "core/ping.h"
#include "debug.h"

#define PPP_PING_PERIOD         5
PppSettings pppSettings;
PppContext pppContext;
modem_interface_manage_t interfaceManage;

NetInterface* ModemInterfaceInit()
{
  NetInterface* interface;
  error_t error;
  
  //Configure the first Ethernet interface
  interface = &netInterface[1];
  //Get default PPP settings
  pppGetDefaultSettings(&pppSettings);
  //Select the underlying interface
  pppSettings.interface = interface;
  //Default async control character map
  pppSettings.accm = 0x00000000;
  //Allowed authentication protocols
  pppSettings.authProtocol = PPP_AUTH_PROTOCOL_PAP | PPP_AUTH_PROTOCOL_CHAP_MD5;
  
  //Initialize PPP
  error = pppInit(&pppContext, &pppSettings);
  //Any error to report?
  if(error)
  {
    //Debug message
    TRACE_ERROR("Failed to initialize PPP!\r\n");
  }
  //Set interface name
  netSetInterfaceName(interface, "ppp0");
  //Set host name
  netSetHostname(interface, "SiteMonitor");
  //Select the relevant UART driver
  netSetUartDriver(interface, &uartDriver);
  
  //Initialize network interface
  error = netConfigInterface(interface);
  //Any error to report?
  if(error)
  {
    //Debug message
    TRACE_ERROR("Failed to configure interface %s!\r\n", interface->name);
  }
  modemInitGPIO();  
  return interface;
}

error_t ModemInterfaceConnect()
{ 
  NetInterface* interface;
  error_t error = ERROR_FAILURE;      
  interface = &netInterface[1];
  while (error)
  {   
    modemTurnOn();
    osDelayTask(1000);
    error = modemInit(interface);
    if (error)
      TRACE_ERROR("Modem initialization failed\r\n");
    else 
      TRACE_ERROR("ppp modem initialzation succedd\r\n");
    if (!error)
        error = modemConnect(interface);
    if (error)
    {
      TRACE_ERROR("Modem connect failed\r\n");
      modemDisconnect(interface);
      osDelayTask(2000);
      modemTurnOff();
      osDelayTask(5000);     
    }
    else
    {
      osDelayTask(1000);
      TRACE_ERROR("Modem connected\r\n");
    }
    if (interfaceManage.setState == MODEM_INTERFACE_STATE_CONNECTED)
      break;
  }
  return error;
}

void ModemInterfaceDisconnect()
{
  NetInterface* interface;
  interface = &netInterface[1];
  modemDisconnect(interface);
  osDelayTask(2000);
  modemTurnOff();
  osDelayTask(5000);
}

void ModemInterfaceSetState(modem_interface_state_t state)
{
  interfaceManage.setState = state;
}

uint8_t ModemInterfaceGetState()
{
  return interfaceManage.currentState;
}

void ModemInterfaceTask(void* param)
{
  static uint8_t pingCount = 0;
  static uint8_t pingLostCount = 0;
  IpAddr ipaddr;
  error_t error, status;
  uint32_t rtt_time;
//  modemTurnOn();
  while (1)
  {
    if ((interfaceManage.setState == MODEM_INTERFACE_STATE_CONNECTED) && (interfaceManage.currentState == MODEM_INTERFACE_STATE_DISCONNECTED))
    {
      error = ModemInterfaceConnect();
      if (!error)
        interfaceManage.currentState = MODEM_INTERFACE_STATE_CONNECTED;
    }
    if ((interfaceManage.setState == MODEM_INTERFACE_STATE_DISCONNECTED) && (interfaceManage.currentState == MODEM_INTERFACE_STATE_CONNECTED))
    {
      ModemInterfaceDisconnect();
      interfaceManage.currentState = MODEM_INTERFACE_STATE_DISCONNECTED;;
    }
    
    if (interfaceManage.currentState == MODEM_INTERFACE_STATE_CONNECTED)
    {
      pingCount++;
      if (pingCount >= PPP_PING_PERIOD)
      {
        ipStringToAddr("8.8.8.8", &ipaddr); 
        status = ping(&netInterface[1], &ipaddr, 32, 255, 5000, &rtt_time);
        if (status != NO_ERROR)
        {
          TRACE_ERROR("ppp ping failed\r\n");
          pingLostCount++;
          if (pingLostCount > 3)
          {
            pingLostCount = 0;
            ModemInterfaceDisconnect();
            interfaceManage.currentState = MODEM_INTERFACE_STATE_DISCONNECTED;        
          }
        }
        else
          pingLostCount = 0;
        pingCount = 0;
      }
    }
    osDelayTask(1000);
  }
}