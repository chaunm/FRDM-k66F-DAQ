/**
 * @file lan9303_driver.c
 * @brief LAN9303 Ethernet switch
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
#include "drivers/switch/lan9303_driver.h"
#include "debug.h"


/**
 * @brief LAN9303 Ethernet switch driver
 **/

const PhyDriver lan9303PhyDriver =
{
   lan9303Init,
   lan9303Tick,
   lan9303EnableIrq,
   lan9303DisableIrq,
   lan9303EventHandler,
};


/**
 * @brief LAN9303 Ethernet switch initialization
 * @param[in] interface Underlying network interface
 * @return Error code
 **/

error_t lan9303Init(NetInterface *interface)
{
   uint_t port;

   //Debug message
   TRACE_INFO("Initializing LAN9303...\r\n");

   //Loop through ports
   for(port = LAN9303_PORT1; port <= LAN9303_PORT2; port++)
   {
      //Debug message
      TRACE_INFO("Port %u:\r\n", port);
      //Dump PHY registers for debugging purpose
      lan9303DumpPhyReg(interface, port);
   }

   //Force the TCP/IP stack to poll the link state at startup
   interface->phyEvent = TRUE;
   //Notify the TCP/IP stack of the event
   osSetEvent(&netEvent);

   //Successful initialization
   return NO_ERROR;
}


/**
 * @brief Get link state
 * @param[in] interface Underlying network interface
 * @param[in] port Port number
 * @return Link state
 **/

bool_t lan9303GetLinkState(NetInterface *interface, uint8_t port)
{
   uint16_t status;
   bool_t linkState;

   //Check port number
   if(port >= LAN9303_PORT1 && port <= LAN9303_PORT2)
   {
      //Get exclusive access
      osAcquireMutex(&netMutex);
      //Read status register
      status = lan9303ReadPhyReg(interface, port, LAN9303_PHY_REG_BMSR);
      //Release exclusive access
      osReleaseMutex(&netMutex);

      //Retrieve current link state
      linkState = (status & BMSR_LINK_STATUS) ? TRUE : FALSE;
   }
   else
   {
      //The specified port number is not valid
      linkState = FALSE;
   }

   //Return link status
   return linkState;
}


/**
 * @brief LAN9303 timer handler
 * @param[in] interface Underlying network interface
 **/

void lan9303Tick(NetInterface *interface)
{
   uint_t port;
   uint16_t status;
   bool_t linkState;

   //Initialize link state
   linkState = FALSE;

   //Loop through ports
   for(port = LAN9303_PORT1; port <= LAN9303_PORT2; port++)
   {
      //Read status register
      status = lan9303ReadPhyReg(interface, port, LAN9303_PHY_REG_BMSR);

      //Retrieve current link state
      if(status & BMSR_LINK_STATUS)
         linkState = TRUE;
   }

   //Link up event?
   if(linkState)
   {
      if(!interface->linkState)
      {
         //Set event flag
         interface->phyEvent = TRUE;
         //Notify the TCP/IP stack of the event
         osSetEvent(&netEvent);
      }
   }
   //Link down event?
   else
   {
      if(interface->linkState)
      {
         //Set event flag
         interface->phyEvent = TRUE;
         //Notify the TCP/IP stack of the event
         osSetEvent(&netEvent);
      }
   }
}


/**
 * @brief Enable interrupts
 * @param[in] interface Underlying network interface
 **/

void lan9303EnableIrq(NetInterface *interface)
{
}


/**
 * @brief Disable interrupts
 * @param[in] interface Underlying network interface
 **/

void lan9303DisableIrq(NetInterface *interface)
{
}


/**
 * @brief LAN9303 event handler
 * @param[in] interface Underlying network interface
 **/

void lan9303EventHandler(NetInterface *interface)
{
   uint_t port;
   uint16_t status;
   bool_t linkState;

   //Initialize link state
   linkState = FALSE;

   //Loop through ports
   for(port = LAN9303_PORT1; port <= LAN9303_PORT2; port++)
   {
      //Read status register
      status = lan9303ReadPhyReg(interface, port, LAN9303_PHY_REG_BMSR);

      //Retrieve current link state
      if(status & BMSR_LINK_STATUS)
         linkState = TRUE;
   }

   //Link up event?
   if(linkState)
   {
      //Set current speed
      interface->linkSpeed = NIC_LINK_SPEED_100MBPS;
      //Set duplex mode
      interface->duplexMode = NIC_FULL_DUPLEX_MODE;

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
 * @param[in] port Port number
 * @param[in] address PHY register address
 * @param[in] data Register value
 **/

void lan9303WritePhyReg(NetInterface *interface,
   uint8_t port, uint8_t address, uint16_t data)
{
   //Write the specified PHY register
   interface->nicDriver->writePhyReg(port, address, data);
}


/**
 * @brief Read PHY register
 * @param[in] interface Underlying network interface
 * @param[in] port Port number
 * @param[in] address PHY register address
 * @return Register value
 **/

uint16_t lan9303ReadPhyReg(NetInterface *interface,
   uint8_t port, uint8_t address)
{
   //Read the specified PHY register
   return interface->nicDriver->readPhyReg(port, address);
}


/**
 * @brief Dump PHY registers for debugging purpose
 * @param[in] interface Underlying network interface
 * @param[in] port Port number
 **/

void lan9303DumpPhyReg(NetInterface *interface, uint8_t port)
{
   uint8_t i;

   //Loop through PHY registers
   for(i = 0; i < 32; i++)
   {
      //Display current PHY register
      TRACE_DEBUG("%02" PRIu8 ": 0x%04" PRIX16 "\r\n", i,
         lan9303ReadPhyReg(interface, port, i));
   }

   //Terminate with a line feed
   TRACE_DEBUG("\r\n");
}


/**
 * @brief Write SMI register
 * @param[in] interface Underlying network interface
 * @param[in] address System register address
 * @param[in] data Register value
 **/

void lan9303WriteSmiReg(NetInterface *interface, uint16_t address,
   uint32_t data)
{
   uint8_t phyAddr;
   uint8_t regAddr;

   //PHY address bit 4 is 1 for SMI commands. PHY address 3:0 form
   //system register address bits 9:6
   phyAddr = 0x10 | ((address >> 6) & 0x0F);

   //Register address field forms register address bits 5:1
   regAddr = (address >> 1) & 0x1F;

   //Get exclusive access
   osAcquireMutex(&netMutex);

   //Write the low word of the SMI register
   interface->nicDriver->writePhyReg(phyAddr, regAddr, data & 0xFFFF);
   //Write the high word of the SMI register
   interface->nicDriver->writePhyReg(phyAddr, regAddr + 1, (data >> 16) & 0xFFFF);

   //Release exclusive access
   osReleaseMutex(&netMutex);
}


/**
 * @brief Read SMI register
 * @param[in] interface Underlying network interface
 * @param[in] address System register address
 * @return Register value
 **/

uint32_t lan9303ReadSmiReg(NetInterface *interface, uint16_t address)
{
   uint8_t phyAddr;
   uint8_t regAddr;
   uint32_t data;

   //PHY address bit 4 is 1 for SMI commands. PHY address 3:0 form
   //system register address bits 9:6
   phyAddr = 0x10 | ((address >> 6) & 0x0F);

   //Register address field forms register address bits 5:1
   regAddr = (address >> 1) & 0x1F;

   //Get exclusive access
   osAcquireMutex(&netMutex);

   //Read the low word of the SMI register
   data = interface->nicDriver->readPhyReg(phyAddr, regAddr);
   //Read the high word of the SMI register
   data |= interface->nicDriver->readPhyReg(phyAddr, regAddr + 1) << 16;

   //Release exclusive access
   osReleaseMutex(&netMutex);

   //Return register value
   return data;
}


/**
 * @brief Dump SMI registers for debugging purpose
 * @param[in] interface Underlying network interface
 **/

void lan9303DumpSmiReg(NetInterface *interface)
{
   uint16_t i;

   //Loop through SMI registers
   for(i = 80; i < 512; i += 4)
   {
      //Display current SMI register
      TRACE_DEBUG("0x%03" PRIX16 ": 0x%08" PRIX32 "\r\n", i,
         lan9303ReadSmiReg(interface, i));
   }

   //Terminate with a line feed
   TRACE_DEBUG("\r\n");
}
