#include "fsl_debug_console.h"
#include "net_config.h"
#include "core/net.h"
#include "drivers/mk6x_eth.h"
#include "drivers/ksz8081.h"
#include "debug.h"
#include "variables.h"
#include "ethernet.h"

static MacAddr macAddr;
#if (APP_USE_DHCP == DISABLED)
static Ipv4Addr ipv4Addr;
#endif
#if (APP_USE_DHCP == ENABLED)
DhcpClientSettings dhcpClientSettings;
DhcpClientCtx dhcpClientContext;
#endif


NetInterface* EthernetInit()
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
  interface = &netInterface[0];
  //Set interface name
  netSetInterfaceName(interface, "eth0");
  //Set host name
  netSetHostname(interface, "SiteMonitor");
  //Select the relevant network adapter
  netSetDriver(interface, &mk6xEthDriver);
  netSetPhyDriver(interface, &ksz8081PhyDriver);
  //Set host MAC address
  macStringToAddr(APP_MAC_ADDR, &macAddr);
  netSetMacAddr(interface, &macAddr);
  //Initialize network interface
  error = netConfigInterface(interface);
  //Any error to report?
  if(error)
  {
    //Debug message
    TRACE_ERROR("Failed to configure interface %s!\r\n", interface->name);
    return NULL;
  }
  
#if (IPV4_SUPPORT == ENABLED)
#if (APP_USE_DHCP == ENABLED)
  //Get default settings
  dhcpClientGetDefaultSettings(&dhcpClientSettings);
  //Set the network interface to be configured by DHCP
  dhcpClientSettings.interface = interface;
  //Disable rapid commit option
  dhcpClientSettings.rapidCommit = FALSE;
  
  //DHCP client initialization
  error = dhcpClientInit(&dhcpClientContext, &dhcpClientSettings);
  //Failed to initialize DHCP client?
  if(error)
  {
    //Debug message
    TRACE_ERROR("Failed to initialize DHCP client!\r\n");
  }
  
  //Start DHCP client
  error = dhcpClientStart(&dhcpClientContext);
  //Failed to start DHCP client?
  if(error)
  {
    //Debug message
    TRACE_ERROR("Failed to start DHCP client!\r\n");
  }
#else
  //Set IPv4 host address
  ipv4StringToAddr((const char*)sMenu_Variable.ucIP, &ipv4Addr);//"192.168.100.104"
  ipv4SetHostAddr(interface, ipv4Addr);
  
  //Set subnet mask
  ipv4StringToAddr((const char*)sMenu_Variable.ucSN, &ipv4Addr);//"255.255.255.0"
  ipv4SetSubnetMask(interface, ipv4Addr);
  
  //Set default gateway
  ipv4StringToAddr((const char*)sMenu_Variable.ucGW, &ipv4Addr);//"192.168.100.1"
  ipv4SetDefaultGateway(interface, ipv4Addr);
  
  //Set primary and secondary DNS servers
  ipv4StringToAddr(APP_IPV4_PRIMARY_DNS, &ipv4Addr);
  ipv4SetDnsServer(interface, 0, ipv4Addr);
  ipv4StringToAddr(APP_IPV4_SECONDARY_DNS, &ipv4Addr);
  ipv4SetDnsServer(interface, 1, ipv4Addr);
#endif
#endif
  return interface;
}