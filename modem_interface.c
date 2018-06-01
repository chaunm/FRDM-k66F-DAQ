#include "modem_interface.h"
#include "modem.h"
#include "uart_driver.h"
#include "ppp/ppp.h"
#include "debug.h"

PppSettings pppSettings;
PppContext pppContext;

NetInterface* ModemInterfaceInit()
{
  NetInterface* interface;
  error_t error;
  
  //TCP/IP stack initialization
  error = netInit();
  //Any error to report?
  if(error)
  {
    //Debug message
    TRACE_ERROR("Failed to initialize TCP/IP stack!\r\n");
    return NULL;
  }
  
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
  return interface;
}