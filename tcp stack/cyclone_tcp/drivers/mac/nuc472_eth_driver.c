/**
 * @file nuc472_eth_driver.c
 * @brief Nuvoton NUC472 Ethernet MAC controller
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
#include "nuc472_442.h"
#include "core/net.h"
#include "drivers/mac/nuc472_eth_driver.h"
#include "debug.h"

//Underlying network interface
static NetInterface *nicDriverInterface;

//IAR EWARM compiler?
#if defined(__ICCARM__)

//Transmit buffer
#pragma data_alignment = 4
static uint8_t txBuffer[NUC472_ETH_TX_BUFFER_COUNT][NUC472_ETH_TX_BUFFER_SIZE];
//Receive buffer
#pragma data_alignment = 4
static uint8_t rxBuffer[NUC472_ETH_RX_BUFFER_COUNT][NUC472_ETH_RX_BUFFER_SIZE];
//Transmit DMA descriptors
#pragma data_alignment = 4
static Nuc472TxDmaDesc txDmaDesc[NUC472_ETH_TX_BUFFER_COUNT];
//Receive DMA descriptors
#pragma data_alignment = 4
static Nuc472RxDmaDesc rxDmaDesc[NUC472_ETH_RX_BUFFER_COUNT];

//Keil MDK-ARM or GCC compiler?
#else

//Transmit buffer
static uint8_t txBuffer[NUC472_ETH_TX_BUFFER_COUNT][NUC472_ETH_TX_BUFFER_SIZE]
   __attribute__((aligned(4)));
//Receive buffer
static uint8_t rxBuffer[NUC472_ETH_RX_BUFFER_COUNT][NUC472_ETH_RX_BUFFER_SIZE]
   __attribute__((aligned(4)));
//Transmit DMA descriptors
static Nuc472TxDmaDesc txDmaDesc[NUC472_ETH_TX_BUFFER_COUNT]
   __attribute__((aligned(4)));
//Receive DMA descriptors
static Nuc472RxDmaDesc rxDmaDesc[NUC472_ETH_RX_BUFFER_COUNT]
   __attribute__((aligned(4)));

#endif

//Current transmit descriptor
static uint_t txIndex;
//Current receive descriptor
static uint_t rxIndex;


/**
 * @brief NUC472 Ethernet MAC driver
 **/

const NicDriver nuc472EthDriver =
{
   NIC_TYPE_ETHERNET,
   ETH_MTU,
   nuc472EthInit,
   nuc472EthTick,
   nuc472EthEnableIrq,
   nuc472EthDisableIrq,
   nuc472EthEventHandler,
   nuc472EthSendPacket,
   nuc472EthSetMulticastFilter,
   nuc472EthUpdateMacConfig,
   nuc472EthWritePhyReg,
   nuc472EthReadPhyReg,
   TRUE,
   TRUE,
   TRUE,
   FALSE
};


/**
 * @brief NUC472 Ethernet MAC initialization
 * @param[in] interface Underlying network interface
 * @return Error code
 **/

error_t nuc472EthInit(NetInterface *interface)
{
   error_t error;

   //Debug message
   TRACE_INFO("Initializing NUC472 Ethernet MAC...\r\n");

   //Save underlying network interface
   nicDriverInterface = interface;

   //Enable EMAC clock
   CLK_EnableModuleClock(EMAC_MODULE);
   //Select MDC clock frequency
   CLK_SetModuleClock(EMAC_MODULE, 0, CLK_CLKDIV3_EMAC(100));

   //Perform a software reset
   EMAC->CTL |= EMAC_CTL_RST_Msk;
   //Wait for the reset to complete
   while(EMAC->CTL & EMAC_CTL_RST_Msk);

   //GPIO configuration
   nuc472EthInitGpio(interface);

   //PHY transceiver initialization
   error = interface->phyDriver->init(interface);
   //Failed to initialize PHY transceiver?
   if(error)
      return error;

   //Set the upper 32 bits of the source MAC address
   EMAC->CAM0M = interface->macAddr.b[3] |
      (interface->macAddr.b[2] << 8) |
      (interface->macAddr.b[1] << 16) |
      (interface->macAddr.b[0] << 24);

   //Set the lower 16 bits of the source MAC address
   EMAC->CAM0L = (interface->macAddr.b[5] << 16) |
      (interface->macAddr.b[4] << 24);

   //Enable the corresponding CAM entry
   EMAC->CAMEN = EMAC_CAMEN_CAMxEN_Msk << 0;
   //Accept broadcast and multicast packets
   EMAC->CAMCTL = EMAC_CAMCTL_CMPEN_Msk | EMAC_CAMCTL_ABP_Msk;

   //Maximum frame length that can be accepted
   EMAC->MRFL = NUC472_ETH_RX_BUFFER_SIZE;

   //Initialize DMA descriptor lists
   nuc472EthInitDmaDesc(interface);

   //Enable the desired MAC interrupts
   EMAC->INTEN = EMAC_INTEN_TXCPIEN_Msk | EMAC_INTEN_TXIEN_Msk |
      EMAC_INTEN_RXGDIEN_Msk | EMAC_INTEN_RXIEN_Msk;

   //Set priority grouping (4 bits for pre-emption priority, no bits for subpriority)
   NVIC_SetPriorityGrouping(NUC472_ETH_IRQ_PRIORITY_GROUPING);

   //Configure EMAC transmit interrupt priority
   NVIC_SetPriority(EMAC_TX_IRQn, NVIC_EncodePriority(NUC472_ETH_IRQ_PRIORITY_GROUPING,
      NUC472_ETH_IRQ_GROUP_PRIORITY, NUC472_ETH_IRQ_SUB_PRIORITY));

   //Configure EMAC receive interrupt priority
   NVIC_SetPriority(EMAC_RX_IRQn, NVIC_EncodePriority(NUC472_ETH_IRQ_PRIORITY_GROUPING,
      NUC472_ETH_IRQ_GROUP_PRIORITY, NUC472_ETH_IRQ_SUB_PRIORITY));

   //Enable transmission and reception
   EMAC->CTL |= EMAC_CTL_TXON_Msk | EMAC_CTL_RXON_Msk;

   //Accept any packets from the upper layer
   osSetEvent(&interface->nicTxEvent);

   //Successful initialization
   return NO_ERROR;
}


//NuTiny-SDK-NUC472 or NuMaker-PFM-NUC472 evaluation board?
#if defined(USE_NUTINY_SDK_NUC472) || defined(USE_NUMAKER_PFM_NUC472)

/**
 * @brief GPIO configuration
 * @param[in] interface Underlying network interface
 **/

void nuc472EthInitGpio(NetInterface *interface)
{
   //Select RMII interface mode
   EMAC->CTL |= EMAC_CTL_RMIIEN_Msk | EMAC_CTL_RMIIRXCTL_Msk;

   //Configure EMAC_REFCLK (PC.0), EMAC_RXERR (PC.1), EMAC_RXDV (PC.2),
   //EMAC_RXD1 (PC.3), EMAC_RXD0 (PC.4), EMAC_TXD0 (PC.6) and EMAC_TXD1 (PC.7)
    SYS->GPC_MFPL = SYS_GPC_MFPL_PC0MFP_EMAC_REFCLK |
      SYS_GPC_MFPL_PC1MFP_EMAC_MII_RXERR |
      SYS_GPC_MFPL_PC2MFP_EMAC_MII_RXDV |
      SYS_GPC_MFPL_PC3MFP_EMAC_MII_RXD1 |
      SYS_GPC_MFPL_PC4MFP_EMAC_MII_RXD0 |
      SYS_GPC_MFPL_PC6MFP_EMAC_MII_TXD0 |
      SYS_GPC_MFPL_PC7MFP_EMAC_MII_TXD1;

   //Configure EMAC_TXEN (PC.8)
   SYS->GPC_MFPH = SYS_GPC_MFPH_PC8MFP_EMAC_MII_TXEN;

   //Enable high slew rate on RMII pins
   PC->SLEWCTL |= GPIO_SLEWCTL_HSREN0_Msk |
      GPIO_SLEWCTL_HSREN1_Msk |
      GPIO_SLEWCTL_HSREN2_Msk |
      GPIO_SLEWCTL_HSREN3_Msk |
      GPIO_SLEWCTL_HSREN4_Msk |
      GPIO_SLEWCTL_HSREN6_Msk |
      GPIO_SLEWCTL_HSREN7_Msk |
      GPIO_SLEWCTL_HSREN8_Msk;

   //Configure EMAC_MDC (PB.14) and EMAC_MDIO (PB.15)
   SYS->GPB_MFPH = SYS_GPB_MFPH_PB14MFP_EMAC_MII_MDC |
      SYS_GPB_MFPH_PB15MFP_EMAC_MII_MDIO;
}

#endif


/**
 * @brief Initialize DMA descriptor lists
 * @param[in] interface Underlying network interface
 **/

void nuc472EthInitDmaDesc(NetInterface *interface)
{
   uint_t i;

   //Initialize TX DMA descriptor list
   for(i = 0; i < NUC472_ETH_TX_BUFFER_COUNT; i++)
   {
      //The descriptor is initially owned by the CPU
      txDmaDesc[i].txdes0 = 0;
      //Transmit buffer address
      txDmaDesc[i].txdes1 = (uint32_t) txBuffer[i];
      //Transmit frame status
      txDmaDesc[i].txdes2 = 0;
      //Next descriptor address
      txDmaDesc[i].txdes3 = (uint32_t) &txDmaDesc[i + 1];
   }

   //The last descriptor is chained to the first entry
   txDmaDesc[i - 1].txdes3 = (uint32_t) &txDmaDesc[0];
   //Initialize TX descriptor index
   txIndex = 0;

   //Initialize RX DMA descriptor list
   for(i = 0; i < NUC472_ETH_RX_BUFFER_COUNT; i++)
   {
      //The descriptor is initially owned by the DMA
      rxDmaDesc[i].rxdes0 = EMAC_RXDES0_OWNER;
      //Receive buffer address
      rxDmaDesc[i].rxdes1 = (uint32_t) rxBuffer[i];
      //Reserved field
      rxDmaDesc[i].rxdes2 = 0;
      //Next descriptor address
      rxDmaDesc[i].rxdes3 = (uint32_t) &rxDmaDesc[i + 1];
   }

   //The last descriptor is chained to the first entry
   rxDmaDesc[i - 1].rxdes3 = (uint32_t) &rxDmaDesc[0];
   //Initialize RX descriptor index
   rxIndex = 0;

   //Start address of the TX descriptor list
   EMAC->TXDSA = (uint32_t) txDmaDesc;
   //Start address of the RX descriptor list
   EMAC->RXDSA = (uint32_t) rxDmaDesc;
}


/**
 * @brief NUC472 Ethernet MAC timer handler
 *
 * This routine is periodically called by the TCP/IP stack to
 * handle periodic operations such as polling the link state
 *
 * @param[in] interface Underlying network interface
 **/

void nuc472EthTick(NetInterface *interface)
{
   //Handle periodic operations
   interface->phyDriver->tick(interface);
}


/**
 * @brief Enable interrupts
 * @param[in] interface Underlying network interface
 **/

void nuc472EthEnableIrq(NetInterface *interface)
{
   //Enable Ethernet MAC interrupts
   NVIC_EnableIRQ(EMAC_TX_IRQn);
   NVIC_EnableIRQ(EMAC_RX_IRQn);

   //Enable Ethernet PHY interrupts
   interface->phyDriver->enableIrq(interface);
}


/**
 * @brief Disable interrupts
 * @param[in] interface Underlying network interface
 **/

void nuc472EthDisableIrq(NetInterface *interface)
{
   //Disable Ethernet MAC interrupts
   NVIC_DisableIRQ(EMAC_TX_IRQn);
   NVIC_DisableIRQ(EMAC_RX_IRQn);

   //Disable Ethernet PHY interrupts
   interface->phyDriver->disableIrq(interface);
}


/**
 * @brief Ethernet MAC transmit interrupt
 **/

void EMAC_TX_IRQHandler(void)
{
   bool_t flag;

   //Enter interrupt service routine
   osEnterIsr();

   //This flag will be set if a higher priority task must be woken
   flag = FALSE;

   //A packet has been transmitted?
   if(EMAC->INTSTS & EMAC_INTSTS_TXCPIF_Msk)
   {
      //Clear TXCPIF interrupt flag
      EMAC->INTSTS = EMAC_INTSTS_TXCPIF_Msk;

      //Check whether the TX buffer is available for writing
      if(!(txDmaDesc[txIndex].txdes0 & EMAC_TXDES0_OWNER))
      {
         //Notify the TCP/IP stack that the transmitter is ready to send
         flag |= osSetEventFromIsr(&nicDriverInterface->nicTxEvent);
      }
   }

   //Leave interrupt service routine
   osExitIsr(flag);
}


/**
 * @brief Ethernet MAC receive interrupt
 **/

void EMAC_RX_IRQHandler(void)
{
   bool_t flag;

   //Enter interrupt service routine
   osEnterIsr();

   //This flag will be set if a higher priority task must be woken
   flag = FALSE;

   //A packet has been received?
   if(EMAC->INTSTS & EMAC_INTSTS_RXGDIF_Msk)
   {
      //Disable receive interrupts
      EMAC->INTEN &= ~EMAC_INTEN_RXIEN_Msk;

      //Set event flag
      nicDriverInterface->nicEvent = TRUE;
      //Notify the TCP/IP stack of the event
      flag |= osSetEventFromIsr(&netEvent);
   }

   //Leave interrupt service routine
   osExitIsr(flag);
}


/**
 * @brief NUC472 Ethernet MAC event handler
 * @param[in] interface Underlying network interface
 **/

void nuc472EthEventHandler(NetInterface *interface)
{
   error_t error;

   //Packet received?
   if(EMAC->INTSTS & EMAC_INTSTS_RXGDIF_Msk)
   {
      //Clear interrupt flag
      EMAC->INTSTS = EMAC_INTSTS_RXGDIF_Msk;

      //Process all pending packets
      do
      {
         //Read incoming packet
         error = nuc472EthReceivePacket(interface);

         //No more data in the receive buffer?
      } while(error != ERROR_BUFFER_EMPTY);
   }

   //Re-enable DMA interrupts
   EMAC->INTEN = EMAC_INTEN_TXCPIEN_Msk | EMAC_INTEN_TXIEN_Msk |
      EMAC_INTEN_RXGDIEN_Msk | EMAC_INTEN_RXIEN_Msk;
}


/**
 * @brief Send a packet
 * @param[in] interface Underlying network interface
 * @param[in] buffer Multi-part buffer containing the data to send
 * @param[in] offset Offset to the first data byte
 * @return Error code
 **/

error_t nuc472EthSendPacket(NetInterface *interface,
   const NetBuffer *buffer, size_t offset)
{
   size_t length;
   uint_t txNextIndex;

   //Retrieve the length of the packet
   length = netBufferGetLength(buffer) - offset;

   //Check the frame length
   if(length > NUC472_ETH_TX_BUFFER_SIZE)
   {
      //The transmitter can accept another packet
      osSetEvent(&interface->nicTxEvent);
      //Report an error
      return ERROR_INVALID_LENGTH;
   }

   //Make sure the current buffer is available for writing
   if(txDmaDesc[txIndex].txdes0 & EMAC_TXDES0_OWNER)
      return ERROR_FAILURE;

   //Copy user data to the transmit buffer
   netBufferRead((uint8_t *) txBuffer[txIndex], buffer, offset, length);

   //Calculate the index of the next descriptor
   txNextIndex = txIndex + 1;

   //Wrap around if necessary
   if(txNextIndex >= NUC472_ETH_TX_BUFFER_COUNT)
      txNextIndex = 0;

   //Set the start address of the buffer
   txDmaDesc[txIndex].txdes1 = (uint32_t) txBuffer[txIndex];
   //Write the number of bytes to send
   txDmaDesc[txIndex].txdes2 = length & EMAC_TXDES2_TBC;
   //Set the address of the next descriptor
   txDmaDesc[txIndex].txdes3 = (uint32_t) &txDmaDesc[txNextIndex];

   //Give the ownership of the descriptor to the DMA
   txDmaDesc[txIndex].txdes0 = EMAC_TXDES0_OWNER | EMAC_TXDES0_INTEN |
      EMAC_TXDES0_CRCAPP | EMAC_TXDES0_PADEN;

   //Instruct the DMA to poll the transmit descriptor list
   EMAC->TXST = 0;

   //Point to the next register
   txIndex = txNextIndex;

   //Check whether the next buffer is available for writing
   if(!(txDmaDesc[txIndex].txdes0 & EMAC_TXDES0_OWNER))
   {
      //The transmitter can accept another packet
      osSetEvent(&interface->nicTxEvent);
   }

   //Data successfully written
   return NO_ERROR;
}


/**
 * @brief Receive a packet
 * @param[in] interface Underlying network interface
 * @return Error code
 **/

error_t nuc472EthReceivePacket(NetInterface *interface)
{
   error_t error;
   size_t n;
   uint_t rxNextIndex;

   //The current buffer is available for reading?
   if(!(rxDmaDesc[rxIndex].rxdes0 & EMAC_RXDES0_OWNER))
   {
      //Valid frame received?
      if(rxDmaDesc[rxIndex].rxdes0 & EMAC_RXDES0_RXGDIF)
      {
         //Retrieve the length of the frame
         n = rxDmaDesc[rxIndex].rxdes0 & EMAC_RXDES0_RBC;
         //Limit the number of data to read
         n = MIN(n, NUC472_ETH_RX_BUFFER_SIZE);

         //Pass the packet to the upper layer
         nicProcessPacket(interface, rxBuffer[rxIndex], n);

         //Valid packet received
         error = NO_ERROR;
      }
      else
      {
         //The packet is not valid
         error = ERROR_INVALID_PACKET;
      }

      //Calculate the index of the next descriptor
      rxNextIndex = rxIndex + 1;

      //Wrap around if necessary
      if(rxNextIndex >= NUC472_ETH_RX_BUFFER_COUNT)
         rxNextIndex = 0;

      //Set the start address of the buffer
      rxDmaDesc[rxIndex].rxdes1 = (uint32_t) rxBuffer[rxIndex];
      //Set the address of the next descriptor
      rxDmaDesc[rxIndex].rxdes3 = (uint32_t) &rxDmaDesc[rxNextIndex];
      //Give the ownership of the descriptor back to the DMA
      rxDmaDesc[rxIndex].rxdes0 = EMAC_RXDES0_OWNER;

      //Point to the next register
      rxIndex = rxNextIndex;
   }
   else
   {
      //No more data in the receive buffer
      error = ERROR_BUFFER_EMPTY;
   }

   //Instruct the DMA to poll the receive descriptor list
   EMAC->RXST = 0;

   //Return status code
   return error;
}


/**
 * @brief Configure multicast MAC address filtering
 * @param[in] interface Underlying network interface
 * @return Error code
 **/

error_t nuc472EthSetMulticastFilter(NetInterface *interface)
{
   uint_t i;
   bool_t acceptMulticast;

   //This flag will be set if multicast addresses should be accepted
   acceptMulticast = FALSE;

   //The MAC filter table contains the multicast MAC addresses
   //to accept when receiving an Ethernet frame
   for(i = 0; i < MAC_MULTICAST_FILTER_SIZE; i++)
   {
      //Valid entry?
      if(interface->macMulticastFilter[i].refCount > 0)
      {
         //Accept multicast addresses
         acceptMulticast = TRUE;
         //We are done
         break;
      }
   }

   //Enable the reception of multicast frames if necessary
   if(acceptMulticast)
      EMAC->CAMCTL |= EMAC_CAMCTL_AMP_Msk;
   else
      EMAC->CAMCTL &= ~EMAC_CAMCTL_AMP_Msk;

   //Successful processing
   return NO_ERROR;
}


/**
 * @brief Adjust MAC configuration parameters for proper operation
 * @param[in] interface Underlying network interface
 * @return Error code
 **/

error_t nuc472EthUpdateMacConfig(NetInterface *interface)
{
   uint32_t config;

   //Read MAC control register
   config = EMAC->CTL;

   //10BASE-T or 100BASE-TX operation mode?
   if(interface->linkSpeed == NIC_LINK_SPEED_100MBPS)
      config |= EMAC_CTL_OPMODE_Msk;
   else
      config &= ~EMAC_CTL_OPMODE_Msk;

   //Half-duplex or full-duplex mode?
   if(interface->duplexMode == NIC_FULL_DUPLEX_MODE)
      config |= EMAC_CTL_FUDUP_Msk;
   else
      config &= ~EMAC_CTL_FUDUP_Msk;

   //Update MAC control register
   EMAC->CTL = config;

   //Successful processing
   return NO_ERROR;
}


/**
 * @brief Write PHY register
 * @param[in] phyAddr PHY address
 * @param[in] regAddr Register address
 * @param[in] data Register value
 **/

void nuc472EthWritePhyReg(uint8_t phyAddr, uint8_t regAddr, uint16_t data)
{
   uint32_t value;

   //Set up a write operation
   value = EMAC_MIIMCTL_MDCON_Msk | EMAC_MIIMCTL_BUSY_Msk | EMAC_MIIMCTL_WRITE_Msk;
   //PHY address
   value |= (phyAddr << EMAC_MIIMCTL_PHYADDR_Pos) & EMAC_MIIMCTL_PHYADDR_Msk;
   //Register address
   value |= (regAddr << EMAC_MIIMCTL_PHYREG_Pos) & EMAC_MIIMCTL_PHYREG_Msk;

   //Data to be written in the PHY register
   EMAC->MIIMDAT = data & EMAC_MIIMDAT_DATA_Msk;

   //Start a write operation
   EMAC->MIIMCTL = value;
   //Wait for the write to complete
   while(EMAC->MIIMCTL & EMAC_MIIMCTL_BUSY_Msk);
}


/**
 * @brief Read PHY register
 * @param[in] phyAddr PHY address
 * @param[in] regAddr Register address
 * @return Register value
 **/

uint16_t nuc472EthReadPhyReg(uint8_t phyAddr, uint8_t regAddr)
{
   uint32_t value;

   //Set up a read operation
   value = EMAC_MIIMCTL_MDCON_Msk | EMAC_MIIMCTL_BUSY_Msk;
   //PHY address
   value |= (phyAddr << EMAC_MIIMCTL_PHYADDR_Pos) & EMAC_MIIMCTL_PHYADDR_Msk;
   //Register address
   value |= (regAddr << EMAC_MIIMCTL_PHYREG_Pos) & EMAC_MIIMCTL_PHYREG_Msk;

   //Start a read operation
   EMAC->MIIMCTL = value;
   //Wait for the read to complete
   while(EMAC->MIIMCTL & EMAC_MIIMCTL_BUSY_Msk);

   //Return PHY register contents
   return EMAC->MIIMDAT & EMAC_MIIMDAT_DATA_Msk;
}
