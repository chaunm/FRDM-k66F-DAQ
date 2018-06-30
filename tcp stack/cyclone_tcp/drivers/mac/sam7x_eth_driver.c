/**
 * @file sam7x_eth_driver.c
 * @brief AT91SAM7X Ethernet MAC controller
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
#include <limits.h>
#include "at91sam7x256.h"
#include "core/net.h"
#include "drivers/mac/sam7x_eth_driver.h"
#include "debug.h"

//Underlying network interface
static NetInterface *nicDriverInterface;

//IAR EWARM compiler?
#if defined(__ICCARM__)

//TX buffer
#pragma data_alignment = 8
static uint8_t txBuffer[SAM7X_ETH_TX_BUFFER_COUNT][SAM7X_ETH_TX_BUFFER_SIZE];
//RX buffer
#pragma data_alignment = 8
static uint8_t rxBuffer[SAM7X_ETH_RX_BUFFER_COUNT][SAM7X_ETH_RX_BUFFER_SIZE];
//TX buffer descriptors
#pragma data_alignment = 4
static Sam7xTxBufferDesc txBufferDesc[SAM7X_ETH_TX_BUFFER_COUNT];
//RX buffer descriptors
#pragma data_alignment = 4
static Sam7xRxBufferDesc rxBufferDesc[SAM7X_ETH_RX_BUFFER_COUNT];

//Keil MDK-ARM or GCC compiler?
#else

//TX buffer
static uint8_t txBuffer[SAM7X_ETH_TX_BUFFER_COUNT][SAM7X_ETH_TX_BUFFER_SIZE]
   __attribute__((aligned(8)));
//RX buffer
static uint8_t rxBuffer[SAM7X_ETH_RX_BUFFER_COUNT][SAM7X_ETH_RX_BUFFER_SIZE]
   __attribute__((aligned(8)));
//TX buffer descriptors
static Sam7xTxBufferDesc txBufferDesc[SAM7X_ETH_TX_BUFFER_COUNT]
   __attribute__((aligned(4)));
//RX buffer descriptors
static Sam7xRxBufferDesc rxBufferDesc[SAM7X_ETH_RX_BUFFER_COUNT]
   __attribute__((aligned(4)));

#endif

//TX buffer index
static uint_t txBufferIndex;
//RX buffer index
static uint_t rxBufferIndex;


/**
 * @brief SAM7X Ethernet MAC driver
 **/

const NicDriver sam7xEthDriver =
{
   NIC_TYPE_ETHERNET,
   ETH_MTU,
   sam7xEthInit,
   sam7xEthTick,
   sam7xEthEnableIrq,
   sam7xEthDisableIrq,
   sam7xEthEventHandler,
   sam7xEthSendPacket,
   sam7xEthSetMulticastFilter,
   sam7xEthUpdateMacConfig,
   sam7xEthWritePhyReg,
   sam7xEthReadPhyReg,
   TRUE,
   TRUE,
   TRUE,
   FALSE
};


/**
 * @brief SAM7X Ethernet MAC initialization
 * @param[in] interface Underlying network interface
 * @return Error code
 **/

error_t sam7xEthInit(NetInterface *interface)
{
   error_t error;
   volatile uint32_t status;

   //Debug message
   TRACE_INFO("Initializing SAM7X Ethernet MAC...\r\n");

   //Save underlying network interface
   nicDriverInterface = interface;

   //Enable EMAC peripheral clock
   AT91C_BASE_PMC->PMC_PCER = (1 << AT91C_ID_EMAC);

   //GPIO configuration
   sam7xEthInitGpio(interface);

   //Configure MDC clock speed
   AT91C_BASE_EMAC->EMAC_NCFGR = AT91C_EMAC_CLK_HCLK_32;
   //Enable management port (MDC and MDIO)
   AT91C_BASE_EMAC->EMAC_NCR |= AT91C_EMAC_MPE;

   //PHY transceiver initialization
   error = interface->phyDriver->init(interface);
   //Failed to initialize PHY transceiver?
   if(error)
      return error;

   //Set the MAC address
   AT91C_BASE_EMAC->EMAC_SA1L = interface->macAddr.w[0] | (interface->macAddr.w[1] << 16);
   AT91C_BASE_EMAC->EMAC_SA1H = interface->macAddr.w[2];

   //Configure the receive filter
   AT91C_BASE_EMAC->EMAC_NCFGR |= AT91C_EMAC_UNI | AT91C_EMAC_MTI;

   //Initialize hash table
   AT91C_BASE_EMAC->EMAC_HRB = 0;
   AT91C_BASE_EMAC->EMAC_HRT = 0;

   //Initialize buffer descriptors
   sam7xEthInitBufferDesc(interface);

   //Clear transmit status register
   AT91C_BASE_EMAC->EMAC_TSR = AT91C_EMAC_UND | AT91C_EMAC_COMP | AT91C_EMAC_BEX |
      AT91C_EMAC_TGO | AT91C_EMAC_RLES | AT91C_EMAC_COL | AT91C_EMAC_UBR;
   //Clear receive status register
   AT91C_BASE_EMAC->EMAC_RSR = AT91C_EMAC_OVR | AT91C_EMAC_REC | AT91C_EMAC_BNA;

   //First disable all EMAC interrupts
   AT91C_BASE_EMAC->EMAC_IDR = 0xFFFFFFFF;
   //Only the desired ones are enabled
   AT91C_BASE_EMAC->EMAC_IER = AT91C_EMAC_ROVR | AT91C_EMAC_TCOMP | AT91C_EMAC_TXERR |
      AT91C_EMAC_RLEX | AT91C_EMAC_TUNDR | AT91C_EMAC_RXUBR | AT91C_EMAC_RCOMP;

   //Read EMAC ISR register to clear any pending interrupt
   status = AT91C_BASE_EMAC->EMAC_ISR;

   //Configure interrupt controller
   AT91C_BASE_AIC->AIC_SMR[AT91C_ID_EMAC] = AT91C_AIC_SRCTYPE_INT_HIGH_LEVEL | AT91C_AIC_PRIOR_LOWEST;
   AT91C_BASE_AIC->AIC_SVR[AT91C_ID_EMAC] = (uint32_t) emacIrqWrapper;

   //Clear EMAC interrupt flag
   AT91C_BASE_AIC->AIC_ICCR = (1 << AT91C_ID_EMAC);

   //Enable the EMAC to transmit and receive data
   AT91C_BASE_EMAC->EMAC_NCR |= AT91C_EMAC_TE | AT91C_EMAC_RE;

   //Accept any packets from the upper layer
   osSetEvent(&interface->nicTxEvent);

   //Successful initialization
   return NO_ERROR;
}


//SAM7-EX256 evaluation board?
#if defined(USE_SAM7_EX256)

/**
 * @brief GPIO configuration
 * @param[in] interface Underlying network interface
 **/

void sam7xEthInitGpio(NetInterface *interface)
{
   //Enable PIO peripheral clock
   AT91C_BASE_PMC->PMC_PCER = (1 << AT91C_ID_PIOB);

   //Disable pull-up resistors on MII pins
   AT91C_BASE_PIOB->PIO_PPUDR = AT91C_EMAC_MII_MASK;
   //Disable interrupts-on-change
   AT91C_BASE_PIOB->PIO_IDR = AT91C_EMAC_MII_MASK;
   //Assign MII pins to peripheral A function
   AT91C_BASE_PIOB->PIO_ASR = AT91C_EMAC_MII_MASK;
   //Disable the PIO from controlling the corresponding pins
   AT91C_BASE_PIOB->PIO_PDR = AT91C_EMAC_MII_MASK;

   //Select MII operation mode and enable transceiver clock
   AT91C_BASE_EMAC->EMAC_USRIO = AT91C_EMAC_CLKEN;
}

#endif


/**
 * @brief Initialize buffer descriptors
 * @param[in] interface Underlying network interface
 **/

void sam7xEthInitBufferDesc(NetInterface *interface)
{
   uint_t i;
   uint32_t address;

   //Initialize TX buffer descriptors
   for(i = 0; i < SAM7X_ETH_TX_BUFFER_COUNT; i++)
   {
      //Calculate the address of the current TX buffer
      address = (uint32_t) txBuffer[i];
      //Write the address to the descriptor entry
      txBufferDesc[i].address = address;
      //Initialize status field
      txBufferDesc[i].status = AT91C_EMAC_TX_USED;
   }

   //Mark the last descriptor entry with the wrap flag
   txBufferDesc[i - 1].status |= AT91C_EMAC_TX_WRAP;
   //Initialize TX buffer index
   txBufferIndex = 0;

   //Initialize RX buffer descriptors
   for(i = 0; i < SAM7X_ETH_RX_BUFFER_COUNT; i++)
   {
      //Calculate the address of the current RX buffer
      address = (uint32_t) rxBuffer[i];
      //Write the address to the descriptor entry
      rxBufferDesc[i].address = address & AT91C_EMAC_RX_ADDRESS;
      //Clear status field
      rxBufferDesc[i].status = 0;
   }

   //Mark the last descriptor entry with the wrap flag
   rxBufferDesc[i - 1].address |= AT91C_EMAC_RX_WRAP;
   //Initialize RX buffer index
   rxBufferIndex = 0;

   //Start location of the TX descriptor list
   AT91C_BASE_EMAC->EMAC_TBQP = (uint32_t) txBufferDesc;
   //Start location of the RX descriptor list
   AT91C_BASE_EMAC->EMAC_RBQP = (uint32_t) rxBufferDesc;
}


/**
 * @brief SAM7X Ethernet MAC timer handler
 *
 * This routine is periodically called by the TCP/IP stack to
 * handle periodic operations such as polling the link state
 *
 * @param[in] interface Underlying network interface
 **/

void sam7xEthTick(NetInterface *interface)
{
   //Handle periodic operations
   interface->phyDriver->tick(interface);
}


/**
 * @brief Enable interrupts
 * @param[in] interface Underlying network interface
 **/

void sam7xEthEnableIrq(NetInterface *interface)
{
   //Enable Ethernet MAC interrupts
   AT91C_BASE_AIC->AIC_IECR = (1 << AT91C_ID_EMAC);
   //Enable Ethernet PHY interrupts
   interface->phyDriver->enableIrq(interface);
}


/**
 * @brief Disable interrupts
 * @param[in] interface Underlying network interface
 **/

void sam7xEthDisableIrq(NetInterface *interface)
{
   //Disable Ethernet MAC interrupts
   AT91C_BASE_AIC->AIC_IDCR = (1 << AT91C_ID_EMAC);
   //Disable Ethernet PHY interrupts
   interface->phyDriver->disableIrq(interface);
}


/**
 * @brief SAM7X Ethernet MAC interrupt service routine
 **/

void sam7xEthIrqHandler(void)
{
   bool_t flag;
   volatile uint32_t isr;
   volatile uint32_t tsr;
   volatile uint32_t rsr;

   //Enter interrupt service routine
   osEnterIsr();

   //This flag will be set if a higher priority task must be woken
   flag = FALSE;

   //Each time the software reads EMAC_ISR, it has to check the contents of
   //EMAC_TSR, EMAC_RSR and EMAC_NSR (see SAM7X errata 41.3.3.2)
   isr = AT91C_BASE_EMAC->EMAC_ISR;
   tsr = AT91C_BASE_EMAC->EMAC_TSR;
   rsr = AT91C_BASE_EMAC->EMAC_RSR;

   //A packet has been transmitted?
   if(tsr & (AT91C_EMAC_UND | AT91C_EMAC_COMP | AT91C_EMAC_BEX |
      AT91C_EMAC_TGO | AT91C_EMAC_RLES | AT91C_EMAC_COL | AT91C_EMAC_UBR))
   {
      //Only clear TSR flags that are currently set
      AT91C_BASE_EMAC->EMAC_TSR = tsr;

      //Check whether the TX buffer is available for writing
      if(txBufferDesc[txBufferIndex].status & AT91C_EMAC_TX_USED)
      {
         //Notify the TCP/IP stack that the transmitter is ready to send
         flag |= osSetEventFromIsr(&nicDriverInterface->nicTxEvent);
      }
   }

   //A packet has been received?
   if(rsr & (AT91C_EMAC_OVR | AT91C_EMAC_REC | AT91C_EMAC_BNA))
   {
      //Set event flag
      nicDriverInterface->nicEvent = TRUE;
      //Notify the TCP/IP stack of the event
      flag |= osSetEventFromIsr(&netEvent);
   }

   //Write AIC_EOICR register before exiting
   AT91C_BASE_AIC->AIC_EOICR = 0;

   //Leave interrupt service routine
   osExitIsr(flag);
}


/**
 * @brief SAM7X Ethernet MAC event handler
 * @param[in] interface Underlying network interface
 **/

void sam7xEthEventHandler(NetInterface *interface)
{
   error_t error;
   uint32_t rsr;

   //Read receive status
   rsr = AT91C_BASE_EMAC->EMAC_RSR;

   //Packet received?
   if(rsr & (AT91C_EMAC_OVR | AT91C_EMAC_REC | AT91C_EMAC_BNA))
   {
      //Only clear RSR flags that are currently set
      AT91C_BASE_EMAC->EMAC_RSR = rsr;

      //Process all pending packets
      do
      {
         //Read incoming packet
         error = sam7xEthReceivePacket(interface);

         //No more data in the receive buffer?
      } while(error != ERROR_BUFFER_EMPTY);
   }
}


/**
 * @brief Send a packet
 * @param[in] interface Underlying network interface
 * @param[in] buffer Multi-part buffer containing the data to send
 * @param[in] offset Offset to the first data byte
 * @return Error code
 **/

error_t sam7xEthSendPacket(NetInterface *interface,
   const NetBuffer *buffer, size_t offset)
{
   size_t length;

   //Retrieve the length of the packet
   length = netBufferGetLength(buffer) - offset;

   //Check the frame length
   if(length > SAM7X_ETH_TX_BUFFER_SIZE)
   {
      //The transmitter can accept another packet
      osSetEvent(&interface->nicTxEvent);
      //Report an error
      return ERROR_INVALID_LENGTH;
   }

   //Make sure the current buffer is available for writing
   if(!(txBufferDesc[txBufferIndex].status & AT91C_EMAC_TX_USED))
      return ERROR_FAILURE;

   //Copy user data to the transmit buffer
   netBufferRead(txBuffer[txBufferIndex], buffer, offset, length);

   //Set the necessary flags in the descriptor entry
   if(txBufferIndex < (SAM7X_ETH_TX_BUFFER_COUNT - 1))
   {
      //Write the status word
      txBufferDesc[txBufferIndex].status =
         AT91C_EMAC_TX_LAST | (length & AT91C_EMAC_TX_LENGTH);

      //Point to the next buffer
      txBufferIndex++;
   }
   else
   {
      //Write the status word
      txBufferDesc[txBufferIndex].status = AT91C_EMAC_TX_WRAP |
         AT91C_EMAC_TX_LAST | (length & AT91C_EMAC_TX_LENGTH);

      //Wrap around
      txBufferIndex = 0;
   }

   //Set the TSTART bit to initiate transmission
   AT91C_BASE_EMAC->EMAC_NCR |= AT91C_EMAC_TSTART;

   //Check whether the next buffer is available for writing
   if(txBufferDesc[txBufferIndex].status & AT91C_EMAC_TX_USED)
   {
      //The transmitter can accept another packet
      osSetEvent(&interface->nicTxEvent);
   }

   //Successful processing
   return NO_ERROR;
}


/**
 * @brief Receive a packet
 * @param[in] interface Underlying network interface
 * @return Error code
 **/

error_t sam7xEthReceivePacket(NetInterface *interface)
{
   static uint8_t temp[ETH_MAX_FRAME_SIZE];
   error_t error;
   uint_t i;
   uint_t j;
   uint_t sofIndex;
   uint_t eofIndex;
   size_t n;
   size_t size;
   size_t length;

   //Initialize SOF and EOF indices
   sofIndex = UINT_MAX;
   eofIndex = UINT_MAX;

   //Search for SOF and EOF flags
   for(i = 0; i < SAM7X_ETH_RX_BUFFER_COUNT; i++)
   {
      //Point to the current entry
      j = rxBufferIndex + i;

      //Wrap around to the beginning of the buffer if necessary
      if(j >= SAM7X_ETH_RX_BUFFER_COUNT)
         j -= SAM7X_ETH_RX_BUFFER_COUNT;

      //No more entries to process?
      if(!(rxBufferDesc[j].address & EMAC_RX_OWNERSHIP))
      {
         //Stop processing
         break;
      }
      //A valid SOF has been found?
      if(rxBufferDesc[j].status & EMAC_RX_SOF)
      {
         //Save the position of the SOF
         sofIndex = i;
      }
      //A valid EOF has been found?
      if((rxBufferDesc[j].status & EMAC_RX_EOF) && sofIndex != UINT_MAX)
      {
         //Save the position of the EOF
         eofIndex = i;
         //Retrieve the length of the frame
         size = rxBufferDesc[j].status & EMAC_RX_LENGTH;
         //Limit the number of data to read
         size = MIN(size, ETH_MAX_FRAME_SIZE);
         //Stop processing since we have reached the end of the frame
         break;
      }
   }

   //Determine the number of entries to process
   if(eofIndex != UINT_MAX)
      j = eofIndex + 1;
   else if(sofIndex != UINT_MAX)
      j = sofIndex;
   else
      j = i;

   //Total number of bytes that have been copied from the receive buffer
   length = 0;

   //Process incoming frame
   for(i = 0; i < j; i++)
   {
      //Any data to copy from current buffer?
      if(eofIndex != UINT_MAX && i >= sofIndex && i <= eofIndex)
      {
         //Calculate the number of bytes to read at a time
         n = MIN(size, SAM7X_ETH_RX_BUFFER_SIZE);
         //Copy data from receive buffer
         memcpy(temp + length, rxBuffer[rxBufferIndex], n);
         //Update byte counters
         length += n;
         size -= n;
      }

      //Mark the current buffer as free
      rxBufferDesc[rxBufferIndex].address &= ~EMAC_RX_OWNERSHIP;

      //Point to the following entry
      rxBufferIndex++;

      //Wrap around to the beginning of the buffer if necessary
      if(rxBufferIndex >= SAM7X_ETH_RX_BUFFER_COUNT)
         rxBufferIndex = 0;
   }

   //Any packet to process?
   if(length > 0)
   {
      //Pass the packet to the upper layer
      nicProcessPacket(interface, temp, length);
      //Valid packet received
      error = NO_ERROR;
   }
   else
   {
      //No more data in the receive buffer
      error = ERROR_BUFFER_EMPTY;
   }

   //Return status code
   return error;
}


/**
 * @brief Configure multicast MAC address filtering
 * @param[in] interface Underlying network interface
 * @return Error code
 **/

error_t sam7xEthSetMulticastFilter(NetInterface *interface)
{
   uint_t i;
   uint_t k;
   uint8_t *p;
   uint32_t hashTable[2];
   MacFilterEntry *entry;

   //Debug message
   TRACE_DEBUG("Updating SAM7X hash table...\r\n");

   //Clear hash table
   hashTable[0] = 0;
   hashTable[1] = 0;

   //The MAC filter table contains the multicast MAC addresses
   //to accept when receiving an Ethernet frame
   for(i = 0; i < MAC_MULTICAST_FILTER_SIZE; i++)
   {
      //Point to the current entry
      entry = &interface->macMulticastFilter[i];

      //Valid entry?
      if(entry->refCount > 0)
      {
         //Point to the MAC address
         p = entry->addr.b;

         //Apply the hash function
         k = (p[0] >> 6) ^ p[0];
         k ^= (p[1] >> 4) ^ (p[1] << 2);
         k ^= (p[2] >> 2) ^ (p[2] << 4);
         k ^= (p[3] >> 6) ^ p[3];
         k ^= (p[4] >> 4) ^ (p[4] << 2);
         k ^= (p[5] >> 2) ^ (p[5] << 4);

         //The hash value is reduced to a 6-bit index
         k &= 0x3F;

         //Update hash table contents
         hashTable[k / 32] |= (1 << (k % 32));
      }
   }

   //Write the hash table
   AT91C_BASE_EMAC->EMAC_HRB = hashTable[0];
   AT91C_BASE_EMAC->EMAC_HRT = hashTable[1];

   //Debug message
   TRACE_DEBUG("  HRB = %08" PRIX32 "\r\n", AT91C_BASE_EMAC->EMAC_HRB);
   TRACE_DEBUG("  HRT = %08" PRIX32 "\r\n", AT91C_BASE_EMAC->EMAC_HRT);

   //Successful processing
   return NO_ERROR;
}


/**
 * @brief Adjust MAC configuration parameters for proper operation
 * @param[in] interface Underlying network interface
 * @return Error code
 **/

error_t sam7xEthUpdateMacConfig(NetInterface *interface)
{
   uint32_t config;

   //Read network configuration register
   config = AT91C_BASE_EMAC->EMAC_NCFGR;

   //10BASE-T or 100BASE-TX operation mode?
   if(interface->linkSpeed == NIC_LINK_SPEED_100MBPS)
      config |= AT91C_EMAC_SPD;
   else
      config &= ~AT91C_EMAC_SPD;

   //Half-duplex or full-duplex mode?
   if(interface->duplexMode == NIC_FULL_DUPLEX_MODE)
      config |= AT91C_EMAC_FD;
   else
      config &= ~AT91C_EMAC_FD;

   //Write configuration value back to NCFGR register
   AT91C_BASE_EMAC->EMAC_NCFGR = config;

   //Successful processing
   return NO_ERROR;
}


/**
 * @brief Write PHY register
 * @param[in] phyAddr PHY address
 * @param[in] regAddr Register address
 * @param[in] data Register value
 **/

void sam7xEthWritePhyReg(uint8_t phyAddr, uint8_t regAddr, uint16_t data)
{
   uint32_t value;

   //Set up a write operation
   value = AT91C_EMAC_SOF_01 | AT91C_EMAC_RW_01 | AT91C_EMAC_CODE_10;
   //PHY address
   value |= (phyAddr << 23) & AT91C_EMAC_PHYA;
   //Register address
   value |= (regAddr << 18) & AT91C_EMAC_REGA;
   //Register value
   value |= data & AT91C_EMAC_DATA;

   //Start a write operation
   AT91C_BASE_EMAC->EMAC_MAN = value;
   //Wait for the write to complete
   while(!(AT91C_BASE_EMAC->EMAC_NSR & AT91C_EMAC_IDLE));
}


/**
 * @brief Read PHY register
 * @param[in] phyAddr PHY address
 * @param[in] regAddr Register address
 * @return Register value
 **/

uint16_t sam7xEthReadPhyReg(uint8_t phyAddr, uint8_t regAddr)
{
   uint32_t value;

   //Set up a read operation
   value = AT91C_EMAC_SOF_01 | AT91C_EMAC_RW_10 | AT91C_EMAC_CODE_10;
   //PHY address
   value |= (phyAddr << 23) & AT91C_EMAC_PHYA;
   //Register address
   value |= (regAddr << 18) & AT91C_EMAC_REGA;

   //Start a read operation
   AT91C_BASE_EMAC->EMAC_MAN = value;
   //Wait for the read to complete
   while(!(AT91C_BASE_EMAC->EMAC_NSR & AT91C_EMAC_IDLE));

   //Return PHY register contents
   return AT91C_BASE_EMAC->EMAC_MAN & AT91C_EMAC_DATA;
}
