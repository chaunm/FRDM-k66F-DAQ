/**
 * @file mk6x_eth_driver.h
 * @brief Freescale Kinetis K60/K64/K65/K66 Ethernet MAC controller
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

#ifndef _MK6X_ETH_DRIVER_H
#define _MK6X_ETH_DRIVER_H

//Number of TX buffers
#ifndef MK6X_ETH_TX_BUFFER_COUNT
   #define MK6X_ETH_TX_BUFFER_COUNT 3
#elif (MK6X_ETH_TX_BUFFER_COUNT < 1)
   #error MK6X_ETH_TX_BUFFER_COUNT parameter is not valid
#endif

//TX buffer size
#ifndef MK6X_ETH_TX_BUFFER_SIZE
   #define MK6X_ETH_TX_BUFFER_SIZE 1536
#elif (MK6X_ETH_TX_BUFFER_SIZE != 1536)
   #error MK6X_ETH_TX_BUFFER_SIZE parameter is not valid
#endif

//Number of RX buffers
#ifndef MK6X_ETH_RX_BUFFER_COUNT
   #define MK6X_ETH_RX_BUFFER_COUNT 6
#elif (MK6X_ETH_RX_BUFFER_COUNT < 1)
   #error MK6X_ETH_RX_BUFFER_COUNT parameter is not valid
#endif

//RX buffer size
#ifndef MK6X_ETH_RX_BUFFER_SIZE
   #define MK6X_ETH_RX_BUFFER_SIZE 1536
#elif (MK6X_ETH_RX_BUFFER_SIZE != 1536)
   #error MK6X_ETH_RX_BUFFER_SIZE parameter is not valid
#endif

//Interrupt priority grouping
#ifndef MK6X_ETH_IRQ_PRIORITY_GROUPING
   #define MK6X_ETH_IRQ_PRIORITY_GROUPING 3
#elif (MK6X_ETH_IRQ_PRIORITY_GROUPING < 0)
   #error MK6X_ETH_IRQ_PRIORITY_GROUPING parameter is not valid
#endif

//Ethernet interrupt group priority
#ifndef MK6X_ETH_IRQ_GROUP_PRIORITY
   #define MK6X_ETH_IRQ_GROUP_PRIORITY 12
#elif (MK6X_ETH_IRQ_GROUP_PRIORITY < 0)
   #error MK6X_ETH_IRQ_GROUP_PRIORITY parameter is not valid
#endif

//Ethernet interrupt subpriority
#ifndef MK6X_ETH_IRQ_SUB_PRIORITY
   #define MK6X_ETH_IRQ_SUB_PRIORITY 0
#elif (MK6X_ETH_IRQ_SUB_PRIORITY < 0)
   #error MK6X_ETH_IRQ_SUB_PRIORITY parameter is not valid
#endif

//Legacy definitions
#ifndef MPU
   #define MPU SYSMPU
#endif

#ifndef MPU_CESR_VLD_MASK
   #define MPU_CESR_VLD_MASK SYSMPU_CESR_VLD_MASK
#endif

//Enhanced transmit buffer descriptor
#define ENET_TBD0_R                0x8000
#define ENET_TBD0_TO1              0x4000
#define ENET_TBD0_W                0x2000
#define ENET_TBD0_TO2              0x1000
#define ENET_TBD0_L                0x0800
#define ENET_TBD0_TC               0x0400
#define ENET_TBD1_DATA_LENGTH      0xFFFF
#define ENET_TBD2_DATA_POINTER_H   0xFFFF
#define ENET_TBD3_DATA_POINTER_L   0xFFFF
#define ENET_TBD4_INT              0x4000
#define ENET_TBD4_TS               0x2000
#define ENET_TBD4_PINS             0x1000
#define ENET_TBD4_IINS             0x0800
#define ENET_TBD5_TXE              0x8000
#define ENET_TBD5_UE               0x2000
#define ENET_TBD5_EE               0x1000
#define ENET_TBD5_FE               0x0800
#define ENET_TBD5_LCE              0x0400
#define ENET_TBD5_OE               0x0200
#define ENET_TBD5_TSE              0x0100
#define ENET_TBD8_BDU              0x8000
#define ENET_TBD10_TIMESTAMP_H     0xFFFF
#define ENET_TBD11_TIMESTAMP_L     0xFFFF

//Enhanced receive buffer descriptor
#define ENET_RBD0_E                0x8000
#define ENET_RBD0_RO1              0x4000
#define ENET_RBD0_W                0x2000
#define ENET_RBD0_RO2              0x1000
#define ENET_RBD0_L                0x0800
#define ENET_RBD0_M                0x0100
#define ENET_RBD0_BC               0x0080
#define ENET_RBD0_MC               0x0040
#define ENET_RBD0_LG               0x0020
#define ENET_RBD0_NO               0x0010
#define ENET_RBD0_CR               0x0004
#define ENET_RBD0_OV               0x0002
#define ENET_RBD0_TR               0x0001
#define ENET_RBD1_DATA_LENGTH      0xFFFF
#define ENET_RBD2_DATA_POINTER_H   0xFFFF
#define ENET_RBD3_DATA_POINTER_L   0xFFFF
#define ENET_RBD4_ME               0x8000
#define ENET_RBD4_PE               0x0400
#define ENET_RBD4_CE               0x0200
#define ENET_RBD4_UC               0x0100
#define ENET_RBD4_INT              0x0080
#define ENET_RBD5_VPCP             0xE000
#define ENET_RBD5_ICE              0x0020
#define ENET_RBD5_PCR              0x0010
#define ENET_RBD5_VLAN             0x0004
#define ENET_RBD5_IPV6             0x0002
#define ENET_RBD5_FRAG             0x0001
#define ENET_RBD6_HEADER_LENGTH    0xF800
#define ENET_RBD6_PROTOCOL_TYPE    0x00FF
#define ENET_RBD7_PAYLOAD_CHECKSUM 0xFFFF
#define ENET_RBD8_BDU              0x8000
#define ENET_RBD10_TIMESTAMP_H     0xFFFF
#define ENET_RBD11_TIMESTAMP_L     0xFFFF

//C++ guard
#ifdef __cplusplus
   extern "C" {
#endif

//Kinetis K6x Ethernet MAC driver
extern const NicDriver mk6xEthDriver;

//Kinetis K6x Ethernet MAC related functions
error_t mk6xEthInit(NetInterface *interface);
void mk6xEthInitGpio(NetInterface *interface);
void mk6xEthInitBufferDesc(NetInterface *interface);

void mk6xEthTick(NetInterface *interface);

void mk6xEthEnableIrq(NetInterface *interface);
void mk6xEthDisableIrq(NetInterface *interface);
void mk6xEthEventHandler(NetInterface *interface);

error_t mk6xEthSendPacket(NetInterface *interface,
   const NetBuffer *buffer, size_t offset);

error_t mk6xEthReceivePacket(NetInterface *interface);

error_t mk6xEthSetMulticastFilter(NetInterface *interface);
error_t mk6xEthUpdateMacConfig(NetInterface *interface);

void mk6xEthWritePhyReg(uint8_t phyAddr, uint8_t regAddr, uint16_t data);
uint16_t mk6xEthReadPhyReg(uint8_t phyAddr, uint8_t regAddr);

uint32_t mk6xEthCalcCrc(const void *data, size_t length);

//C++ guard
#ifdef __cplusplus
   }
#endif

#endif
