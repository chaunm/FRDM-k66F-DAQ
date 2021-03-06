/**
 * @file dp83640_driver.c
 * @brief DP83640 Ethernet PHY transceiver
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
#include "drivers/phy/dp83640_driver.h"
#include "debug.h"


/**
 * @brief DP83640 Ethernet PHY driver
 **/

const PhyDriver dp83640PhyDriver =
{
   dp83640Init,
   dp83640Tick,
   dp83640EnableIrq,
   dp83640DisableIrq,
   dp83640EventHandler,
};


/**
 * @brief DP83640 PHY transceiver initialization
 * @param[in] interface Underlying network interface
 * @return Error code
 **/

error_t dp83640Init(NetInterface *interface)
{
   uint16_t value;

   //Debug message
   TRACE_INFO("Initializing DP83640...\r\n");

   //Initialize external interrupt line driver
   if(interface->extIntDriver != NULL)
      interface->extIntDriver->init();

   //A software reset is accomplished by setting the RESET bit of the BMCR register
   dp83640WritePhyReg(interface, DP83640_PHY_REG_BMCR, BMCR_RESET);
   //Wait for the reset to complete
   while(dp83640ReadPhyReg(interface, DP83640_PHY_REG_BMCR) & BMCR_RESET);

   //Dump PHY registers for debugging purpose
   dp83640DumpPhyReg(interface);

   //Configure PWR_DOWN/INT pin as an interrupt output
   dp83640WritePhyReg(interface, DP83640_PHY_REG_MICR, MICR_INTEN | MICR_INT_OE);
   //The PHY will generate interrupts when link status changes are detected
   dp83640WritePhyReg(interface, DP83640_PHY_REG_MISR, MISR_LINK_INT_EN);

   //Select page 6
   dp83640WritePhyReg(interface, DP83640_PHY_REG_PAGESEL, 6);
   //Read PTP_COC register
   value = dp83640ReadPhyReg(interface, DP83640_PHY_REG_PTP_COC);

   //If the CLK_OUT pin is to be used as a 50 MHz RMII clock, the default PTP
   //clock output function must be disabled
   value &= ~PTP_COC_PTP_CLKOUT_EN;

   //Write PTP_COC register
   dp83640WritePhyReg(interface, DP83640_PHY_REG_PTP_COC, value);
   //Select page 0
   dp83640WritePhyReg(interface, DP83640_PHY_REG_PAGESEL, 0);

   //Force the TCP/IP stack to poll the link state at startup
   interface->phyEvent = TRUE;
   //Notify the TCP/IP stack of the event
   osSetEvent(&netEvent);

   //Successful initialization
   return NO_ERROR;
}


/**
 * @brief DP83640 timer handler
 * @param[in] interface Underlying network interface
 **/

void dp83640Tick(NetInterface *interface)
{
   uint16_t value;
   bool_t linkState;

   //No external interrupt line driver?
   if(interface->extIntDriver == NULL)
   {
      //Read basic status register
      value = dp83640ReadPhyReg(interface, DP83640_PHY_REG_BMSR);
      //Retrieve current link state
      linkState = (value & BMSR_LINK_STATUS) ? TRUE : FALSE;

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
}


/**
 * @brief Enable interrupts
 * @param[in] interface Underlying network interface
 **/

void dp83640EnableIrq(NetInterface *interface)
{
   //Enable PHY transceiver interrupts
   if(interface->extIntDriver != NULL)
      interface->extIntDriver->enableIrq();
}


/**
 * @brief Disable interrupts
 * @param[in] interface Underlying network interface
 **/

void dp83640DisableIrq(NetInterface *interface)
{
   //Disable PHY transceiver interrupts
   if(interface->extIntDriver != NULL)
      interface->extIntDriver->disableIrq();
}


/**
 * @brief DP83640 event handler
 * @param[in] interface Underlying network interface
 **/

void dp83640EventHandler(NetInterface *interface)
{
   uint16_t status;

   //Read status register to acknowledge the interrupt
   status = dp83640ReadPhyReg(interface, DP83640_PHY_REG_MISR);

   //Link status change?
   if(status & MISR_LINK_INT)
   {
      //Read PHY status register
      status = dp83640ReadPhyReg(interface, DP83640_PHY_REG_PHYSTS);

      //Link is up?
      if(status & PHYSTS_LINK_STATUS)
      {
         //Check current speed
         if(status & PHYSTS_SPEED_STATUS)
            interface->linkSpeed = NIC_LINK_SPEED_10MBPS;
         else
            interface->linkSpeed = NIC_LINK_SPEED_100MBPS;

         //Check duplex mode
         if(status & PHYSTS_DUPLEX_STATUS)
            interface->duplexMode = NIC_FULL_DUPLEX_MODE;
         else
            interface->duplexMode = NIC_HALF_DUPLEX_MODE;

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
}


/**
 * @brief Write PHY register
 * @param[in] interface Underlying network interface
 * @param[in] address PHY register address
 * @param[in] data Register value
 **/

void dp83640WritePhyReg(NetInterface *interface, uint8_t address, uint16_t data)
{
   uint8_t phyAddr;

   //Get the address of the PHY transceiver
   if(interface->phyAddr < 32)
      phyAddr = interface->phyAddr;
   else
      phyAddr = DP83640_PHY_ADDR;

   //Write the specified PHY register
   interface->nicDriver->writePhyReg(phyAddr, address, data);
}


/**
 * @brief Read PHY register
 * @param[in] interface Underlying network interface
 * @param[in] address PHY register address
 * @return Register value
 **/

uint16_t dp83640ReadPhyReg(NetInterface *interface, uint8_t address)
{
   uint8_t phyAddr;

   //Get the address of the PHY transceiver
   if(interface->phyAddr < 32)
      phyAddr = interface->phyAddr;
   else
      phyAddr = DP83640_PHY_ADDR;

   //Read the specified PHY register
   return interface->nicDriver->readPhyReg(phyAddr, address);
}


/**
 * @brief Dump PHY registers for debugging purpose
 * @param[in] interface Underlying network interface
 **/

void dp83640DumpPhyReg(NetInterface *interface)
{
   uint8_t i;

   //Loop through PHY registers
   for(i = 0; i < 32; i++)
   {
      //Display current PHY register
      TRACE_DEBUG("%02" PRIu8 ": 0x%04" PRIX16 "\r\n", i, dp83640ReadPhyReg(interface, i));
   }

   //Terminate with a line feed
   TRACE_DEBUG("\r\n");
}
