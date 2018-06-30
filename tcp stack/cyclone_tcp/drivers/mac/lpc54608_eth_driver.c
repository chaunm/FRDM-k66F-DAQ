/**
 * @file lpc54608_eth_driver.c
 * @brief LPC54608 Ethernet MAC controller
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
#include "fsl_device_registers.h"
#include "fsl_power.h"
#include "fsl_reset.h"
#include "fsl_clock.h"
#include "fsl_iocon.h"
#include "fsl_gpio.h"
#include "core/net.h"
#include "drivers/mac/lpc54608_eth_driver.h"
#include "debug.h"

//Underlying network interface
static NetInterface *nicDriverInterface;

//IAR EWARM compiler?
#if defined(__ICCARM__)

//Transmit buffer
#pragma data_alignment = 4
static uint8_t txBuffer[LPC54608_ETH_TX_BUFFER_COUNT][LPC54608_ETH_TX_BUFFER_SIZE];
//Receive buffer
#pragma data_alignment = 4
static uint8_t rxBuffer[LPC54608_ETH_RX_BUFFER_COUNT][LPC54608_ETH_RX_BUFFER_SIZE];
//Transmit DMA descriptors
#pragma data_alignment = 4
static Lpc54608TxDmaDesc txDmaDesc[LPC54608_ETH_TX_BUFFER_COUNT];
//Receive DMA descriptors
#pragma data_alignment = 4
static Lpc54608RxDmaDesc rxDmaDesc[LPC54608_ETH_RX_BUFFER_COUNT];

//Keil MDK-ARM or GCC compiler?
#else

//Transmit buffer
static uint8_t txBuffer[LPC54608_ETH_TX_BUFFER_COUNT][LPC54608_ETH_TX_BUFFER_SIZE]
   __attribute__((aligned(4)));
//Receive buffer
static uint8_t rxBuffer[LPC54608_ETH_RX_BUFFER_COUNT][LPC54608_ETH_RX_BUFFER_SIZE]
   __attribute__((aligned(4)));
//Transmit DMA descriptors
static Lpc54608TxDmaDesc txDmaDesc[LPC54608_ETH_TX_BUFFER_COUNT]
   __attribute__((aligned(4)));
//Receive DMA descriptors
static Lpc54608RxDmaDesc rxDmaDesc[LPC54608_ETH_RX_BUFFER_COUNT]
   __attribute__((aligned(4)));

#endif

//Current transmit descriptor
static uint_t txIndex;
//Current receive descriptor
static uint_t rxIndex;


/**
 * @brief LPC54608 Ethernet MAC driver
 **/

const NicDriver lpc54608EthDriver =
{
   NIC_TYPE_ETHERNET,
   ETH_MTU,
   lpc54608EthInit,
   lpc54608EthTick,
   lpc54608EthEnableIrq,
   lpc54608EthDisableIrq,
   lpc54608EthEventHandler,
   lpc54608EthSendPacket,
   lpc54608EthSetMulticastFilter,
   lpc54608EthUpdateMacConfig,
   lpc54608EthWritePhyReg,
   lpc54608EthReadPhyReg,
   TRUE,
   TRUE,
   TRUE,
   FALSE
};


/**
 * @brief LPC54608 Ethernet MAC initialization
 * @param[in] interface Underlying network interface
 * @return Error code
 **/

error_t lpc54608EthInit(NetInterface *interface)
{
   error_t error;

   //Debug message
   TRACE_INFO("Initializing LPC54608 Ethernet MAC...\r\n");

   //Save underlying network interface
   nicDriverInterface = interface;

   //Enable ENET peripheral clock
   CLOCK_EnableClock(kCLOCK_Eth);
   //Reset ENET module
   RESET_PeripheralReset(kETH_RST_SHIFT_RSTn);

   //GPIO configuration
   lpc54608EthInitGpio(interface);

   //Perform a software reset
   ENET->DMA_MODE |= ENET_DMA_MODE_SWR_MASK;
   //Wait for the reset to complete
   while(ENET->DMA_MODE & ENET_DMA_MODE_SWR_MASK);

   //Adjust MDC clock range depending on CSR frequency
   ENET->MAC_MDIO_ADDR = ENET_MAC_MDIO_ADDR_CR(4);

   //PHY transceiver initialization
   error = interface->phyDriver->init(interface);
   //Failed to initialize PHY transceiver?
   if(error)
      return error;

   //Use default MAC configuration
   ENET->MAC_CONFIG = ENET_MAC_CONFIG_PS_MASK | ENET_MAC_CONFIG_DO_MASK;

   //Set the MAC address
   ENET->MAC_ADDR_LOW = interface->macAddr.w[0] | (interface->macAddr.w[1] << 16);
   ENET->MAC_ADDR_HIGH = interface->macAddr.w[2];

   //Configure the receive filter
   ENET->MAC_FRAME_FILTER = 0;

   //Disable flow control
   ENET->MAC_TX_FLOW_CTRL_Q[0] = 0;
   ENET->MAC_RX_FLOW_CTRL = 0;

   //Enable the first RX queue
   ENET->MAC_RXQ_CTRL[0] = ENET_MAC_RXQ_CTRL_RXQ0EN(1);

   //Configure DMA operating mode
   ENET->DMA_MODE = ENET_DMA_MODE_PR(0);
   //Configure system bus mode
   ENET->DMA_SYSBUS_MODE |= ENET_DMA_SYSBUS_MODE_AAL_MASK;

   //The DMA takes the descriptor table as contiguous
   ENET->DMA_CH[0].DMA_CHX_CTRL = ENET_DMA_CH_DMA_CHX_CTRL_DSL(0);
   //Configure TX features
   ENET->DMA_CH[0].DMA_CHX_TX_CTRL = ENET_DMA_CH_DMA_CHX_TX_CTRL_TxPBL(1);

   //Configure RX features
   ENET->DMA_CH[0].DMA_CHX_RX_CTRL = ENET_DMA_CH_DMA_CHX_RX_CTRL_RxPBL(1) |
      ENET_DMA_CH_DMA_CHX_RX_CTRL_RBSZ(LPC54608_ETH_RX_BUFFER_SIZE / 4);

   //Enable store and forward mode for transmission
   ENET->MTL_QUEUE[0].MTL_TXQX_OP_MODE |= ENET_MTL_QUEUE_MTL_TXQX_OP_MODE_TQS(7) |
      ENET_MTL_QUEUE_MTL_TXQX_OP_MODE_TXQEN(2) |
      ENET_MTL_QUEUE_MTL_TXQX_OP_MODE_TSF_MASK;

   //Enable store and forward mode for reception
   ENET->MTL_QUEUE[0].MTL_RXQX_OP_MODE |= ENET_MTL_QUEUE_MTL_RXQX_OP_MODE_RQS(7) |
      ENET_MTL_QUEUE_MTL_RXQX_OP_MODE_RSF_MASK;

   //Initialize DMA descriptor lists
   lpc54608EthInitDmaDesc(interface);

   //Disable MAC interrupts
   ENET->MAC_INTR_EN = 0;

   //Enable the desired DMA interrupts
   ENET->DMA_CH[0].DMA_CHX_INT_EN = ENET_DMA_CH_DMA_CHX_INT_EN_NIE_MASK |
      ENET_DMA_CH_DMA_CHX_INT_EN_RIE_MASK | ENET_DMA_CH_DMA_CHX_INT_EN_TIE_MASK;

   //Set priority grouping (3 bits for pre-emption priority, no bits for subpriority)
   NVIC_SetPriorityGrouping(LPC54608_ETH_IRQ_PRIORITY_GROUPING);

   //Configure Ethernet interrupt priority
   NVIC_SetPriority(ETHERNET_IRQn, NVIC_EncodePriority(LPC54608_ETH_IRQ_PRIORITY_GROUPING,
      LPC54608_ETH_IRQ_GROUP_PRIORITY, LPC54608_ETH_IRQ_SUB_PRIORITY));

   //Enable MAC transmission and reception
   ENET->MAC_CONFIG |= ENET_MAC_CONFIG_TE_MASK | ENET_MAC_CONFIG_RE_MASK;

   //Enable DMA transmission and reception
   ENET->DMA_CH[0].DMA_CHX_TX_CTRL |= ENET_DMA_CH_DMA_CHX_TX_CTRL_ST_MASK;
   ENET->DMA_CH[0].DMA_CHX_RX_CTRL |= ENET_DMA_CH_DMA_CHX_RX_CTRL_SR_MASK;

   //Accept any packets from the upper layer
   osSetEvent(&interface->nicTxEvent);

   //Successful initialization
   return NO_ERROR;
}


//LPCXpresso54608 evaluation board?
#if defined(USE_LPCXPRESSO_54608)

/**
 * @brief GPIO configuration
 * @param[in] interface Underlying network interface
 **/

void lpc54608EthInitGpio(NetInterface *interface)
{
   gpio_pin_config_t pinConfig;

   //Select RMII interface mode
   SYSCON->ETHPHYSEL |= SYSCON_ETHPHYSEL_PHY_SEL_MASK;

   //Enable IOCON clock
   CLOCK_EnableClock(kCLOCK_Iocon);

   //Enable GPIO clocks
   CLOCK_EnableClock(kCLOCK_Gpio0);
   CLOCK_EnableClock(kCLOCK_Gpio4);

   //Configure ENET_TXD1 (PA0_17)
   IOCON_PinMuxSet(IOCON, 0, 17, IOCON_FUNC7 | IOCON_MODE_INACT |
      IOCON_DIGITAL_EN | IOCON_INPFILT_OFF);

   //Configure ENET_TXD0 (PA4_8)
   IOCON_PinMuxSet(IOCON, 4, 8, IOCON_FUNC1 | IOCON_MODE_INACT |
      IOCON_DIGITAL_EN | IOCON_INPFILT_OFF);

   //Configure ENET_RX_DV (PA4_10)
   IOCON_PinMuxSet(IOCON, 4, 10, IOCON_FUNC1 | IOCON_MODE_INACT |
      IOCON_DIGITAL_EN | IOCON_INPFILT_OFF);

   //Configure ENET_RXD0 (PA4_11)
   IOCON_PinMuxSet(IOCON, 4, 11, IOCON_FUNC1 | IOCON_MODE_INACT |
      IOCON_DIGITAL_EN | IOCON_INPFILT_OFF);

   //Configure ENET_RXD1 (PA4_12)
   IOCON_PinMuxSet(IOCON, 4, 12, IOCON_FUNC1 | IOCON_MODE_INACT |
      IOCON_DIGITAL_EN | IOCON_INPFILT_OFF);

   //Configure ENET_TX_EN (PA4_13)
   IOCON_PinMuxSet(IOCON, 4, 13, IOCON_FUNC1 | IOCON_MODE_INACT |
      IOCON_DIGITAL_EN | IOCON_INPFILT_OFF);

   //Configure ENET_RX_CLK (PA4_14)
   IOCON_PinMuxSet(IOCON, 4, 14, IOCON_FUNC1 | IOCON_MODE_INACT |
      IOCON_DIGITAL_EN | IOCON_INPFILT_OFF);

   //Configure ENET_MDC (PA4_15)
   IOCON_PinMuxSet(IOCON, 4, 15, IOCON_FUNC1 | IOCON_MODE_INACT |
      IOCON_DIGITAL_EN | IOCON_INPFILT_OFF);

   //Configure ENET_MDIO (PA4_16)
   IOCON_PinMuxSet(IOCON, 4, 16, IOCON_FUNC1 | IOCON_MODE_PULLUP |
      IOCON_DIGITAL_EN | IOCON_INPFILT_OFF);

   //Configure ENET_RST as an output
   pinConfig.pinDirection = kGPIO_DigitalOutput;
   pinConfig.outputLogic = 0;
   GPIO_PinInit(GPIO, 2, 26, &pinConfig);

   //Reset PHY transceiver (hard reset)
   GPIO_WritePinOutput(GPIO, 2, 26, 0);
   sleep(10);
   GPIO_WritePinOutput(GPIO, 2, 26, 1);
   sleep(10);
}

#endif


/**
 * @brief Initialize DMA descriptor lists
 * @param[in] interface Underlying network interface
 **/

void lpc54608EthInitDmaDesc(NetInterface *interface)
{
   uint_t i;

   //Initialize TX DMA descriptor list
   for(i = 0; i < LPC54608_ETH_TX_BUFFER_COUNT; i++)
   {
      //The descriptor is initially owned by the application
      txDmaDesc[i].tdes0 = 0;
      txDmaDesc[i].tdes1 = 0;
      txDmaDesc[i].tdes2 = 0;
      txDmaDesc[i].tdes3 = 0;
   }

   //Initialize TX descriptor index
   txIndex = 0;

   //Initialize RX DMA descriptor list
   for(i = 0; i < LPC54608_ETH_RX_BUFFER_COUNT; i++)
   {
      //The descriptor is initially owned by the DMA
      rxDmaDesc[i].rdes0 = (uint32_t) rxBuffer[i];
      rxDmaDesc[i].rdes1 = 0;
      rxDmaDesc[i].rdes2 = 0;
      rxDmaDesc[i].rdes3 = ENET_RDES3_OWN | ENET_RDES3_IOC | ENET_RDES3_BUF1V;
   }

   //Initialize RX descriptor index
   rxIndex = 0;

   //Start location of the TX descriptor list
   ENET->DMA_CH[0].DMA_CHX_TXDESC_LIST_ADDR = (uint32_t) &txDmaDesc[0];
   //Length of the transmit descriptor ring
   ENET->DMA_CH[0].DMA_CHX_TXDESC_RING_LENGTH = LPC54608_ETH_TX_BUFFER_COUNT - 1;

   //Start location of the RX descriptor list
   ENET->DMA_CH[0].DMA_CHX_RXDESC_LIST_ADDR = (uint32_t) &rxDmaDesc[0];
   //Length of the receive descriptor ring
   ENET->DMA_CH[0].DMA_CHX_RXDESC_RING_LENGTH = LPC54608_ETH_RX_BUFFER_COUNT - 1;
}


/**
 * @brief LPC54608 Ethernet MAC timer handler
 *
 * This routine is periodically called by the TCP/IP stack to
 * handle periodic operations such as polling the link state
 *
 * @param[in] interface Underlying network interface
 **/

void lpc54608EthTick(NetInterface *interface)
{
   //Handle periodic operations
   interface->phyDriver->tick(interface);
}


/**
 * @brief Enable interrupts
 * @param[in] interface Underlying network interface
 **/

void lpc54608EthEnableIrq(NetInterface *interface)
{
   //Enable Ethernet MAC interrupts
   NVIC_EnableIRQ(ETHERNET_IRQn);
   //Enable Ethernet PHY interrupts
   interface->phyDriver->enableIrq(interface);
}


/**
 * @brief Disable interrupts
 * @param[in] interface Underlying network interface
 **/

void lpc54608EthDisableIrq(NetInterface *interface)
{
   //Disable Ethernet MAC interrupts
   NVIC_DisableIRQ(ETHERNET_IRQn);
   //Disable Ethernet PHY interrupts
   interface->phyDriver->disableIrq(interface);
}


/**
 * @brief LPC54608 Ethernet MAC interrupt service routine
 **/

void ETHERNET_IRQHandler(void)
{
   bool_t flag;
   uint32_t status;

   //Enter interrupt service routine
   osEnterIsr();

   //This flag will be set if a higher priority task must be woken
   flag = FALSE;

   //Read DMA status register
   status = ENET->DMA_CH[0].DMA_CHX_STAT;

   //A packet has been transmitted?
   if(status & ENET_DMA_CH_DMA_CHX_STAT_TI_MASK)
   {
      //Clear TI interrupt flag
      ENET->DMA_CH[0].DMA_CHX_STAT = ENET_DMA_CH_DMA_CHX_STAT_TI_MASK;

      //Check whether the TX buffer is available for writing
      if(!(txDmaDesc[txIndex].tdes3 & ENET_TDES3_OWN))
      {
         //Notify the TCP/IP stack that the transmitter is ready to send
         flag |= osSetEventFromIsr(&nicDriverInterface->nicTxEvent);
      }
   }

   //A packet has been received?
   if(status & ENET_DMA_CH_DMA_CHX_STAT_RI_MASK)
   {
      //Disable RIE interrupt
      ENET->DMA_CH[0].DMA_CHX_INT_EN &= ~ENET_DMA_CH_DMA_CHX_INT_EN_RIE_MASK;

      //Set event flag
      nicDriverInterface->nicEvent = TRUE;
      //Notify the TCP/IP stack of the event
      flag |= osSetEventFromIsr(&netEvent);
   }

   //Clear NIS interrupt flag
   ENET->DMA_CH[0].DMA_CHX_STAT = ENET_DMA_CH_DMA_CHX_STAT_NIS_MASK;

   //Leave interrupt service routine
   osExitIsr(flag);
}


/**
 * @brief LPC54608 Ethernet MAC event handler
 * @param[in] interface Underlying network interface
 **/

void lpc54608EthEventHandler(NetInterface *interface)
{
   error_t error;

   //Packet received?
   if(ENET->DMA_CH[0].DMA_CHX_STAT & ENET_DMA_CH_DMA_CHX_STAT_RI_MASK)
   {
      //Clear interrupt flag
      ENET->DMA_CH[0].DMA_CHX_STAT = ENET_DMA_CH_DMA_CHX_STAT_RI_MASK;

      //Process all pending packets
      do
      {
         //Read incoming packet
         error = lpc54608EthReceivePacket(interface);

         //No more data in the receive buffer?
      } while(error != ERROR_BUFFER_EMPTY);
   }

   //Re-enable DMA interrupts
   ENET->DMA_CH[0].DMA_CHX_INT_EN |= ENET_DMA_CH_DMA_CHX_INT_EN_NIE_MASK |
      ENET_DMA_CH_DMA_CHX_INT_EN_RIE_MASK | ENET_DMA_CH_DMA_CHX_INT_EN_TIE_MASK;
}


/**
 * @brief Send a packet
 * @param[in] interface Underlying network interface
 * @param[in] buffer Multi-part buffer containing the data to send
 * @param[in] offset Offset to the first data byte
 * @return Error code
 **/

error_t lpc54608EthSendPacket(NetInterface *interface,
   const NetBuffer *buffer, size_t offset)
{
   size_t length;

   //Retrieve the length of the packet
   length = netBufferGetLength(buffer) - offset;

   //Check the frame length
   if(length > LPC54608_ETH_TX_BUFFER_SIZE)
   {
      //The transmitter can accept another packet
      osSetEvent(&interface->nicTxEvent);
      //Report an error
      return ERROR_INVALID_LENGTH;
   }

   //Make sure the current buffer is available for writing
   if(txDmaDesc[txIndex].tdes3 & ENET_TDES3_OWN)
      return ERROR_FAILURE;

   //Copy user data to the transmit buffer
   netBufferRead(txBuffer[txIndex], buffer, offset, length);

   //Set the start address of the buffer
   txDmaDesc[txIndex].tdes0 = (uint32_t) txBuffer[txIndex];
   //Write the number of bytes to send
   txDmaDesc[txIndex].tdes2 = ENET_TDES2_IOC | (length & ENET_TDES2_B1L);
   //Give the ownership of the descriptor to the DMA
   txDmaDesc[txIndex].tdes3 = ENET_TDES3_OWN | ENET_TDES3_FD | ENET_TDES3_LD;

   //Clear TBU flag to resume processing
   ENET->DMA_CH[0].DMA_CHX_STAT = ENET_DMA_CH_DMA_CHX_STAT_TBU_MASK;
   //Instruct the DMA to poll the transmit descriptor list
   ENET->DMA_CH[0].DMA_CHX_TXDESC_TAIL_PTR = 0;

   //Increment index and wrap around if necessary
   if(++txIndex >= LPC54608_ETH_TX_BUFFER_COUNT)
      txIndex = 0;

   //Check whether the next buffer is available for writing
   if(!(txDmaDesc[txIndex].tdes3 & ENET_TDES3_OWN))
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

error_t lpc54608EthReceivePacket(NetInterface *interface)
{
   error_t error;
   size_t n;

   //The current buffer is available for reading?
   if(!(rxDmaDesc[rxIndex].rdes3 & ENET_RDES3_OWN))
   {
      //FD and LD flags should be set
      if((rxDmaDesc[rxIndex].rdes3 & ENET_RDES3_FD) &&
         (rxDmaDesc[rxIndex].rdes3 & ENET_RDES3_LD))
      {
         //Make sure no error occurred
         if(!(rxDmaDesc[rxIndex].rdes3 & ENET_RDES3_ES))
         {
            //Retrieve the length of the frame
            n = rxDmaDesc[rxIndex].rdes3 & ENET_RDES3_PL;
            //Limit the number of data to read
            n = MIN(n, LPC54608_ETH_RX_BUFFER_SIZE);

            //Pass the packet to the upper layer
            nicProcessPacket(interface, rxBuffer[rxIndex], n);

            //Valid packet received
            error = NO_ERROR;
         }
         else
         {
            //The received packet contains an error
            error = ERROR_INVALID_PACKET;
         }
      }
      else
      {
         //The packet is not valid
         error = ERROR_INVALID_PACKET;
      }

      //Set the start address of the buffer
      rxDmaDesc[rxIndex].rdes0 = (uint32_t) rxBuffer[rxIndex];
      //Give the ownership of the descriptor back to the DMA
      rxDmaDesc[rxIndex].rdes3 = ENET_RDES3_OWN | ENET_RDES3_IOC | ENET_RDES3_BUF1V;

      //Increment index and wrap around if necessary
      if(++rxIndex >= LPC54608_ETH_RX_BUFFER_COUNT)
         rxIndex = 0;
   }
   else
   {
      //No more data in the receive buffer
      error = ERROR_BUFFER_EMPTY;
   }

   //Clear RBU flag to resume processing
   ENET->DMA_CH[0].DMA_CHX_STAT = ENET_DMA_CH_DMA_CHX_STAT_RBU_MASK;
   //Instruct the DMA to poll the receive descriptor list
   ENET->DMA_CH[0].DMA_CHX_RXDESC_TAIL_PTR = 0;

   //Return status code
   return error;
}


/**
 * @brief Configure multicast MAC address filtering
 * @param[in] interface Underlying network interface
 * @return Error code
 **/

error_t lpc54608EthSetMulticastFilter(NetInterface *interface)
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
      ENET->MAC_FRAME_FILTER |= ENET_MAC_FRAME_FILTER_PM_MASK;
   else
      ENET->MAC_FRAME_FILTER &= ~ENET_MAC_FRAME_FILTER_PM_MASK;

   //Successful processing
   return NO_ERROR;
}


/**
 * @brief Adjust MAC configuration parameters for proper operation
 * @param[in] interface Underlying network interface
 * @return Error code
 **/

error_t lpc54608EthUpdateMacConfig(NetInterface *interface)
{
   uint32_t config;

   //Read current MAC configuration
   config = ENET->MAC_CONFIG;

   //10BASE-T or 100BASE-TX operation mode?
   if(interface->linkSpeed == NIC_LINK_SPEED_100MBPS)
      config |= ENET_MAC_CONFIG_FES_MASK;
   else
      config &= ~ENET_MAC_CONFIG_FES_MASK;

   //Half-duplex or full-duplex mode?
   if(interface->duplexMode == NIC_FULL_DUPLEX_MODE)
      config |= ENET_MAC_CONFIG_DM_MASK;
   else
      config &= ~ENET_MAC_CONFIG_DM_MASK;

   //Update MAC configuration register
   ENET->MAC_CONFIG = config;

   //Successful processing
   return NO_ERROR;
}


/**
 * @brief Write PHY register
 * @param[in] phyAddr PHY address
 * @param[in] regAddr Register address
 * @param[in] data Register value
 **/

void lpc54608EthWritePhyReg(uint8_t phyAddr, uint8_t regAddr, uint16_t data)
{
   uint32_t value;

   //Take care not to alter MDC clock configuration
   value = ENET->MAC_MDIO_ADDR & ENET_MAC_MDIO_ADDR_CR_MASK;
   //Set up a write operation
   value |= ENET_MAC_MDIO_ADDR_MOC(1) | ENET_MAC_MDIO_ADDR_MB_MASK;
   //PHY address
   value |= ENET_MAC_MDIO_ADDR_PA(phyAddr);
   //Register address
   value |= ENET_MAC_MDIO_ADDR_RDA(regAddr);

   //Data to be written in the PHY register
   ENET->MAC_MDIO_DATA = data & ENET_MAC_MDIO_DATA_MD_MASK;

   //Start a write operation
   ENET->MAC_MDIO_ADDR = value;
   //Wait for the write to complete
   while(ENET->MAC_MDIO_ADDR & ENET_MAC_MDIO_ADDR_MB_MASK);
}


/**
 * @brief Read PHY register
 * @param[in] phyAddr PHY address
 * @param[in] regAddr Register address
 * @return Register value
 **/

uint16_t lpc54608EthReadPhyReg(uint8_t phyAddr, uint8_t regAddr)
{
   uint32_t value;

   //Take care not to alter MDC clock configuration
   value = ENET->MAC_MDIO_ADDR & ENET_MAC_MDIO_ADDR_CR_MASK;
   //Set up a read operation
   value |= ENET_MAC_MDIO_ADDR_MOC(3) | ENET_MAC_MDIO_ADDR_MB_MASK;
   //PHY address
   value |= ENET_MAC_MDIO_ADDR_PA(phyAddr);
   //Register address
   value |= ENET_MAC_MDIO_ADDR_RDA(regAddr);

   //Start a read operation
   ENET->MAC_MDIO_ADDR = value;
   //Wait for the read to complete
   while(ENET->MAC_MDIO_ADDR & ENET_MAC_MDIO_ADDR_MB_MASK);

   //Return PHY register contents
   return ENET->MAC_MDIO_DATA & ENET_MAC_MDIO_DATA_MD_MASK;
}
