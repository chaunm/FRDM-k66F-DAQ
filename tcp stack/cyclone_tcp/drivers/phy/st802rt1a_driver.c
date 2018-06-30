/**
 * @file st802rt1a_driver.c
 * @brief ST802RT1A Ethernet PHY transceiver
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
#include "drivers/phy/st802rt1a_driver.h"
#include "debug.h"


/**
 * @brief ST802RT1A Ethernet PHY driver
 **/

const PhyDriver st802rt1aPhyDriver =
{
   st802rt1aInit,
   st802rt1aTick,
   st802rt1aEnableIrq,
   st802rt1aDisableIrq,
   st802rt1aEventHandler,
};


/**
 * @brief ST802RT1A PHY transceiver initialization
 * @param[in] interface Underlying network interface
 * @return Error code
 **/

error_t st802rt1aInit(NetInterface *interface)
{
   //Debug message
   TRACE_INFO("Initializing ST802RT1A...\r\n");

   //Reset PHY transceiver
   st802rt1aWritePhyReg(interface, ST802RT1A_PHY_REG_RN00, RN00_SOFT_RESET);
   //Wait for the reset to complete
   while(st802rt1aReadPhyReg(interface, ST802RT1A_PHY_REG_RN00) & RN00_SOFT_RESET);

   //Dump PHY registers for debugging purpose
   st802rt1aDumpPhyReg(interface);

   //Force the TCP/IP stack to poll the link state at startup
   interface->phyEvent = TRUE;
   //Notify the TCP/IP stack of the event
   osSetEvent(&netEvent);

   //Successful initialization
   return NO_ERROR;
}


/**
 * @brief ST802RT1A timer handler
 * @param[in] interface Underlying network interface
 **/

void st802rt1aTick(NetInterface *interface)
{
   uint16_t value;
   bool_t linkState;

   //Read status register
   value = st802rt1aReadPhyReg(interface, ST802RT1A_PHY_REG_RN01);
   //Retrieve current link state
   linkState = (value & RN01_LINK_STATUS) ? TRUE : FALSE;

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

void st802rt1aEnableIrq(NetInterface *interface)
{
}


/**
 * @brief Disable interrupts
 * @param[in] interface Underlying network interface
 **/

void st802rt1aDisableIrq(NetInterface *interface)
{
}


/**
 * @brief ST802RT1A event handler
 * @param[in] interface Underlying network interface
 **/

void st802rt1aEventHandler(NetInterface *interface)
{
   uint16_t value;
   bool_t linkState;

   //Read status register
   value = st802rt1aReadPhyReg(interface, ST802RT1A_PHY_REG_RN01);
   //Retrieve current link state
   linkState = (value & RN01_LINK_STATUS) ? TRUE : FALSE;

   //Link is up?
   if(linkState && !interface->linkState)
   {
      //Read RN13 register
      value = st802rt1aReadPhyReg(interface, ST802RT1A_PHY_REG_RN13);

      //Check current operation mode
      switch(value & RN13_CMODE_MASK)
      {
      //10BASE-T
      case RN13_CMODE_10BT:
         interface->linkSpeed = NIC_LINK_SPEED_10MBPS;
         interface->duplexMode = NIC_HALF_DUPLEX_MODE;
         break;
      //10BASE-T full-duplex
      case RN13_CMODE_10BT_FD:
         interface->linkSpeed = NIC_LINK_SPEED_10MBPS;
         interface->duplexMode = NIC_FULL_DUPLEX_MODE;
         break;
      //100BASE-TX
      case RN13_CMODE_100BTX:
         interface->linkSpeed = NIC_LINK_SPEED_100MBPS;
         interface->duplexMode = NIC_HALF_DUPLEX_MODE;
         break;
      //100BASE-TX full-duplex
      case RN13_CMODE_100BTX_FD:
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

      //Process link state change event
      nicNotifyLinkChange(interface);
   }
   //Link is down?
   else if(!linkState && interface->linkState)
   {
      //Update link state
      interface->linkState = FALSE;

      //Process link state change event
      nicNotifyLinkChange(interface);
   }
}


/**
 * @brief Write PHY register
 * @param[in] interface Underlying network interface
 * @param[in] address PHY register Register address
 * @param[in] data Register value
 **/

void st802rt1aWritePhyReg(NetInterface *interface, uint8_t address, uint16_t data)
{
   uint8_t phyAddr;

   //Get the address of the PHY transceiver
   if(interface->phyAddr < 32)
      phyAddr = interface->phyAddr;
   else
      phyAddr = ST802RT1A_PHY_ADDR;

   //Write the specified PHY register
   interface->nicDriver->writePhyReg(phyAddr, address, data);
}


/**
 * @brief Read PHY register
 * @param[in] interface Underlying network interface
 * @param[in] address PHY register address
 * @return Register value
 **/

uint16_t st802rt1aReadPhyReg(NetInterface *interface, uint8_t address)
{
   uint8_t phyAddr;

   //Get the address of the PHY transceiver
   if(interface->phyAddr < 32)
      phyAddr = interface->phyAddr;
   else
      phyAddr = ST802RT1A_PHY_ADDR;

   //Read the specified PHY register
   return interface->nicDriver->readPhyReg(phyAddr, address);
}


/**
 * @brief Dump PHY registers for debugging purpose
 * @param[in] interface Underlying network interface
 **/

void st802rt1aDumpPhyReg(NetInterface *interface)
{
   uint8_t i;

   //Loop through PHY registers
   for(i = 0; i < 32; i++)
   {
      //Display current PHY register
      TRACE_DEBUG("%02" PRIu8 ": 0x%04" PRIX16 "\r\n", i, st802rt1aReadPhyReg(interface, i));
   }

   //Terminate with a line feed
   TRACE_DEBUG("\r\n");
}
