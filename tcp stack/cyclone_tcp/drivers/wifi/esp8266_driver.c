/**
 * @file esp8266_driver.c
 * @brief ESP8266 Wi-Fi controller
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

//Switch to the appropriate trace level
#define TRACE_LEVEL NIC_TRACE_LEVEL

//Dependencies
#include "espressif/esp_wifi.h"
#include "espressif/esp_system.h"
#include "lwip/pbuf.h"
#include "core/net.h"
#include "drivers/wifi/esp8266_driver.h"
#include "debug.h"

//Underlying network interface
static NetInterface *esp8266WifiStaInterface = NULL;
static NetInterface *esp8266WifiApInterface = NULL;


/**
 * @brief ESP8266 Wi-Fi driver (STA mode)
 **/

const NicDriver esp8266WifiStaDriver =
{
   NIC_TYPE_ETHERNET,
   ETH_MTU,
   esp8266WifiInit,
   esp8266WifiTick,
   esp8266WifiEnableIrq,
   esp8266WifiDisableIrq,
   esp8266WifiEventHandler,
   esp8266WifiSendPacket,
   esp8266WifiSetMulticastFilter,
   NULL,
   NULL,
   NULL,
   TRUE,
   TRUE,
   TRUE,
   TRUE
};


/**
 * @brief ESP8266 Wi-Fi driver (AP mode)
 **/

const NicDriver esp8266WifiApDriver =
{
   NIC_TYPE_ETHERNET,
   ETH_MTU,
   esp8266WifiInit,
   esp8266WifiTick,
   esp8266WifiEnableIrq,
   esp8266WifiDisableIrq,
   esp8266WifiEventHandler,
   esp8266WifiSendPacket,
   esp8266WifiSetMulticastFilter,
   NULL,
   NULL,
   NULL,
   TRUE,
   TRUE,
   TRUE,
   TRUE
};


/**
 * @brief ESP8266_WIFI initialization
 * @param[in] interface Underlying network interface
 * @return Error code
 **/

error_t esp8266WifiInit(NetInterface *interface)
{
   bool_t ret;

   //STA or AP mode?
   if(interface->nicDriver == &esp8266WifiStaDriver)
   {
      //Debug message
      TRACE_INFO("Initializing ESP8266 Wi-Fi (STA mode)...\r\n");

      //Save underlying network interface (STA mode)
      esp8266WifiStaInterface = interface;

      //Optionally set the MAC address
      if(macCompAddr(&interface->macAddr, &MAC_UNSPECIFIED_ADDR))
      {
         //Use the factory preprogrammed station address
         ret = sdk_wifi_get_macaddr(STATION_IF, interface->macAddr.b);

         //Check status code
         if(ret)
         {
            //Generate the 64-bit interface identifier
            macAddrToEui64(&interface->macAddr, &interface->eui64);
         }
      }
      else
      {
         //Override the factory preprogrammed address
         ret = sdk_wifi_set_macaddr(STATION_IF, interface->macAddr.b);
      }
   }
   else
   {
      //Debug message
      TRACE_INFO("Initializing ESP8266 Wi-Fi (AP mode)...\r\n");

      //Save underlying network interface (AP mode)
      esp8266WifiApInterface = interface;

      //Optionally set the MAC address
      if(macCompAddr(&interface->macAddr, &MAC_UNSPECIFIED_ADDR))
      {
         //Use the factory preprogrammed station address
         ret = sdk_wifi_get_macaddr(SOFTAP_IF, interface->macAddr.b);

         //Check status code
         if(ret)
         {
            //Generate the 64-bit interface identifier
            macAddrToEui64(&interface->macAddr, &interface->eui64);
         }
      }
      else
      {
         //Override the factory preprogrammed address
         ret = sdk_wifi_set_macaddr(SOFTAP_IF, interface->macAddr.b);
      }
   }

   //ESP8266 Wi-Fi is now ready to send
   osSetEvent(&interface->nicTxEvent);

   //Return status code
   if(ret)
      return NO_ERROR;
   else
      return ERROR_FAILURE;
}


/**
 * @brief ESP8266 Wi-Fi timer handler
 *
 * This routine is periodically called by the TCP/IP stack to
 * handle periodic operations such as polling the link state
 *
 * @param[in] interface Underlying network interface
 **/

void esp8266WifiTick(NetInterface *interface)
{
}


/**
 * @brief Enable interrupts
 * @param[in] interface Underlying network interface
 **/

void esp8266WifiEnableIrq(NetInterface *interface)
{
}


/**
 * @brief Disable interrupts
 * @param[in] interface Underlying network interface
 **/

void esp8266WifiDisableIrq(NetInterface *interface)
{
}


/**
 * @brief ESP8266 Wi-Fi event handler
 * @param[in] interface Underlying network interface
 **/

void esp8266WifiEventHandler(NetInterface *interface)
{
}


/**
 * @brief Send a packet
 * @param[in] interface Underlying network interface
 * @param[in] buffer Multi-part buffer containing the data to send
 * @param[in] offset Offset to the first data byte
 * @return Error code
 **/

error_t esp8266WifiSendPacket(NetInterface *interface,
   const NetBuffer *buffer, size_t offset)
{
   int_t ret;
   size_t length;
   struct netif *netif;
   struct pbuf *p;

   //STA or AP mode?
   if(interface == esp8266WifiStaInterface)
      netif = sdk_system_get_netif(STATION_IF);
   else
      netif = sdk_system_get_netif(SOFTAP_IF);

   //Sanity check
   if(netif != NULL)
   {
      //Retrieve the length of the packet
      length = netBufferGetLength(buffer) - offset;

      //Allocate a buffer
      p = pbuf_alloc(PBUF_RAW_TX, length, PBUF_RAM);

      //Successful memory allocation?
      if(p != NULL)
      {
         //Copy user data
         netBufferRead(p->payload, buffer, offset, length);

         //Send packet
         ret = sdk_ieee80211_output_pbuf(netif, p);

         //Release buffer
         pbuf_free(p);
      }
   }

   //The transmitter can accept another packet
   osSetEvent(&interface->nicTxEvent);

   //Return status code
   if(!ret)
      return NO_ERROR;
   else
      return ERROR_FAILURE;
}


/**
 * @brief Configure multicast MAC address filtering
 * @param[in] interface Underlying network interface
 * @return Error code
 **/

error_t esp8266WifiSetMulticastFilter(NetInterface *interface)
{
   //Not implemented
   return NO_ERROR;
}


/**
 * @brief Process link-up event
 * @param[in] netif Underlying network interface
 **/

void netif_set_up(struct netif *netif)
{
   NetInterface *interface;

   //Check the interface where the event is occurring
   if(netif == sdk_system_get_netif(STATION_IF))
      interface = esp8266WifiStaInterface;
   else if(netif == sdk_system_get_netif(SOFTAP_IF))
      interface = esp8266WifiApInterface;
   else
      interface = NULL;

   //Valid interface?
   if(interface != NULL)
   {
      //The link is up
      interface->linkState = TRUE;

      //Get exclusive access
      osAcquireMutex(&netMutex);
      //Process link state change event
      nicNotifyLinkChange(interface);
      //Release exclusive access
      osReleaseMutex(&netMutex);
   }
}


/**
 * @brief Process link-down event
 * @param[in] netif Underlying network interface
 **/

void netif_set_down(struct netif *netif)
{
   NetInterface *interface;

   //Check the interface where the event is occurring
   if(netif == sdk_system_get_netif(STATION_IF))
      interface = esp8266WifiStaInterface;
   else if(netif == sdk_system_get_netif(SOFTAP_IF))
      interface = esp8266WifiApInterface;
   else
      interface = NULL;

   //Valid interface?
   if(interface != NULL)
   {
      //The link is down
      interface->linkState = FALSE;

      //Get exclusive access
      osAcquireMutex(&netMutex);
      //Process link state change event
      nicNotifyLinkChange(interface);
      //Release exclusive access
      osReleaseMutex(&netMutex);
   }
}


/**
 * @brief Process incoming packets
 * @param[in] netif Underlying network interface
 * @param[in] p Pointer to the buffer allocated by the Wi-Fi driver
 **/

void ethernetif_input(struct netif *netif, struct pbuf *p)
{
   NetInterface *interface;

   //Check the interface where the event is occurring
   if(netif == sdk_system_get_netif(STATION_IF))
      interface = esp8266WifiStaInterface;
   else if(netif == sdk_system_get_netif(SOFTAP_IF))
      interface = esp8266WifiApInterface;
   else
      interface = NULL;

   //Valid buffer?
   if(p != NULL)
   {
      //Valid interface?
      if(interface != NULL)
      {
         //Get exclusive access
         osAcquireMutex(&netMutex);
         //Pass the packet to the upper layer
         nicProcessPacket(interface, p->payload, p->len);
         //Release exclusive access
         osReleaseMutex(&netMutex);
      }

      //Release buffer
      pbuf_free(p);
   }
}
