/**
 * @file ip101_driver.c
 * @brief IC+ IP101 Ethernet PHY transceiver
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
#include "core/net.h"
#include "drivers/phy/ip101_driver.h"
#include "debug.h"


/**
 * @brief IP101 Ethernet PHY driver
 **/

const PhyDriver ip101PhyDriver =
{
   ip101Init,
   ip101Tick,
   ip101EnableIrq,
   ip101DisableIrq,
   ip101EventHandler,
};


/**
 * @brief IP101 PHY transceiver initialization
 * @param[in] interface Underlying network interface
 * @return Error code
 **/

error_t ip101Init(NetInterface *interface)
{
   //Debug message
   TRACE_INFO("Initializing IP101...\r\n");

   //Reset PHY transceiver
   ip101WritePhyReg(interface, IP101_PHY_REG_BMCR, BMCR_RESET);
   //Wait for the reset to complete
   while(ip101ReadPhyReg(interface, IP101_PHY_REG_BMCR) & BMCR_RESET);

   //Dump PHY registers for debugging purpose
   ip101DumpPhyReg(interface);

   //Force the TCP/IP stack to poll the link state at startup
   interface->phyEvent = TRUE;
   //Notify the TCP/IP stack of the event
   osSetEvent(&netEvent);

   //Successful initialization
   return NO_ERROR;
}


/**
 * @brief IP101 timer handler
 * @param[in] interface Underlying network interface
 **/

void ip101Tick(NetInterface *interface)
{
   uint16_t value;
   bool_t linkState;

   //Read basic status register
   value = ip101ReadPhyReg(interface, IP101_PHY_REG_PHYMCSSR);
   //Retrieve current link state
   linkState = (value & PHYMCSSR_LINK_UP) ? TRUE : FALSE;

   //Link up event?
   if(linkState && !interface->linkState)
   {
      //Set event flag
      interface->phyEvent = TRUE;
      //Notify the TCP/IP stack of the event
      osSetEvent(&netEvent);
   }
   //Link down event?
   else if(!linkState && interface->linkState)
   {
      //Set event flag
      interface->phyEvent = TRUE;
      //Notify the TCP/IP stack of the event
      osSetEvent(&netEvent);
   }
}


/**
 * @brief Enable interrupts
 * @param[in] interface Underlying network interface
 **/

void ip101EnableIrq(NetInterface *interface)
{
}


/**
 * @brief Disable interrupts
 * @param[in] interface Underlying network interface
 **/

void ip101DisableIrq(NetInterface *interface)
{
}


/**
 * @brief IP101 event handler
 * @param[in] interface Underlying network interface
 **/

void ip101EventHandler(NetInterface *interface)
{
   uint16_t value;

   //Read PHY status register
   value = ip101ReadPhyReg(interface, IP101_PHY_REG_PHYMCSSR);

   //Link is up?
   if(value & PHYMCSSR_LINK_UP)
   {
      //Check current operation mode
      switch(value & PHYMCSSR_OP_MODE_MASK)
      {
      //10BASE-T
      case PHYMCSSR_OP_MODE_10M_HD:
         interface->linkSpeed = NIC_LINK_SPEED_10MBPS;
         interface->duplexMode = NIC_HALF_DUPLEX_MODE;
         break;
      //10BASE-T full-duplex
      case PHYMCSSR_OP_MODE_10M_FD:
         interface->linkSpeed = NIC_LINK_SPEED_10MBPS;
         interface->duplexMode = NIC_FULL_DUPLEX_MODE;
         break;
      //100BASE-TX
      case PHYMCSSR_OP_MODE_100M_HD:
         interface->linkSpeed = NIC_LINK_SPEED_100MBPS;
         interface->duplexMode = NIC_HALF_DUPLEX_MODE;
         break;
      //100BASE-TX full-duplex
      case PHYMCSSR_OP_MODE_100_FD:
         interface->linkSpeed = NIC_LINK_SPEED_100MBPS;
         interface->duplexMode = NIC_FULL_DUPLEX_MODE;
         break;
      //Unknown operation mode
      default:
         //Debug message
         TRACE_WARNING("Invalid Duplex mode\r\n");
         break;
      }

      //Update link state
      interface->linkState = TRUE;

      //Adjust MAC configuration parameters for proper operation
      interface->nicDriver->updateMacConfig(interface);
   }
   else
   {
      //Update link state
      interface->linkState = FALSE;
   }

   //Process link state change event
   nicNotifyLinkChange(interface);
}


/**
 * @brief Write PHY register
 * @param[in] interface Underlying network interface
 * @param[in] address PHY register address
 * @param[in] data Register value
 **/

void ip101WritePhyReg(NetInterface *interface, uint8_t address, uint16_t data)
{
   uint8_t phyAddr;

   //Get the address of the PHY transceiver
   if(interface->phyAddr < 32)
      phyAddr = interface->phyAddr;
   else
      phyAddr = IP101_PHY_ADDR;

   //Write the specified PHY register
   interface->nicDriver->writePhyReg(phyAddr, address, data);
}


/**
 * @brief Read PHY register
 * @param[in] interface Underlying network interface
 * @param[in] address PHY register address
 * @return Register value
 **/

uint16_t ip101ReadPhyReg(NetInterface *interface, uint8_t address)
{
   uint8_t phyAddr;

   //Get the address of the PHY transceiver
   if(interface->phyAddr < 32)
      phyAddr = interface->phyAddr;
   else
      phyAddr = IP101_PHY_ADDR;

   //Read the specified PHY register
   return interface->nicDriver->readPhyReg(phyAddr, address);
}


/**
 * @brief Dump PHY registers for debugging purpose
 * @param[in] interface Underlying network interface
 **/

void ip101DumpPhyReg(NetInterface *interface)
{
   uint8_t i;

   //Loop through PHY registers
   for(i = 0; i < 32; i++)
   {
      //Display current PHY register
      TRACE_DEBUG("%02" PRIu8 ": 0x%04" PRIX16 "\r\n", i, ip101ReadPhyReg(interface, i));
   }

   //Terminate with a line feed
   TRACE_DEBUG("\r\n");
}
