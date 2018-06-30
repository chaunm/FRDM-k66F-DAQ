/**
 * @file lan8720_driver.h
 * @brief LAN8720 Ethernet PHY transceiver
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

#ifndef _LAN8720_DRIVER_H
#define _LAN8720_DRIVER_H

//Dependencies
#include "core/nic.h"

//PHY address
#ifndef LAN8720_PHY_ADDR
   #define LAN8720_PHY_ADDR 0
#elif (LAN8720_PHY_ADDR < 0 || LAN8720_PHY_ADDR > 31)
   #error LAN8720_PHY_ADDR parameter is not valid
#endif

//LAN8720 registers
#define LAN8720_PHY_REG_BMCR        0x00
#define LAN8720_PHY_REG_BMSR        0x01
#define LAN8720_PHY_REG_PHYIDR1     0x02
#define LAN8720_PHY_REG_PHYIDR2     0x03
#define LAN8720_PHY_REG_ANAR        0x04
#define LAN8720_PHY_REG_ANLPAR      0x05
#define LAN8720_PHY_REG_ANER        0x06
#define LAN8720_PHY_REG_SRR         0x10
#define LAN8720_PHY_REG_MCSR        0x11
#define LAN8720_PHY_REG_SMR         0x12
#define LAN8720_PHY_REG_SECR        0x1A
#define LAN8720_PHY_REG_SCSIR       0x1B
#define LAN8720_PHY_REG_SITCR       0x1C
#define LAN8720_PHY_REG_ISR         0x1D
#define LAN8720_PHY_REG_IMR         0x1E
#define LAN8720_PHY_REG_PSCSR       0x1F

//BMCR register
#define BMCR_RESET                  (1 << 15)
#define BMCR_LOOPBACK               (1 << 14)
#define BMCR_SPEED_SEL              (1 << 13)
#define BMCR_AN_EN                  (1 << 12)
#define BMCR_POWER_DOWN             (1 << 11)
#define BMCR_ISOLATE                (1 << 10)
#define BMCR_RESTART_AN             (1 << 9)
#define BMCR_DUPLEX_MODE            (1 << 8)
#define BMCR_COL_TEST               (1 << 7)

//BMSR register
#define BMSR_100BT4                 (1 << 15)
#define BMSR_100BTX_FD              (1 << 14)
#define BMSR_100BTX                 (1 << 13)
#define BMSR_10BT_FD                (1 << 12)
#define BMSR_10BT                   (1 << 11)
#define BMSR_AN_COMPLETE            (1 << 5)
#define BMSR_REMOTE_FAULT           (1 << 4)
#define BMSR_AN_ABLE                (1 << 3)
#define BMSR_LINK_STATUS            (1 << 2)
#define BMSR_JABBER_DETECT          (1 << 1)
#define BMSR_EXTENDED_CAP           (1 << 0)

//ANAR register
#define ANAR_NP                     (1 << 15)
#define ANAR_RF                     (1 << 13)
#define ANAR_PAUSE1                 (1 << 11)
#define ANAR_PAUSE0                 (1 << 10)
#define ANAR_100BT4                 (1 << 9)
#define ANAR_100BTX_FD              (1 << 8)
#define ANAR_100BTX                 (1 << 7)
#define ANAR_10BT_FD                (1 << 6)
#define ANAR_10BT                   (1 << 5)
#define ANAR_SELECTOR4              (1 << 4)
#define ANAR_SELECTOR3              (1 << 3)
#define ANAR_SELECTOR2              (1 << 2)
#define ANAR_SELECTOR1              (1 << 1)
#define ANAR_SELECTOR0              (1 << 0)

//ANLPAR register
#define ANLPAR_NP                   (1 << 15)
#define ANLPAR_ACK                  (1 << 14)
#define ANLPAR_RF                   (1 << 13)
#define ANLPAR_PAUSE                (1 << 10)
#define ANLPAR_100BT4               (1 << 9)
#define ANLPAR_100BTX_FD            (1 << 8)
#define ANLPAR_100BTX               (1 << 7)
#define ANLPAR_10BT_FD              (1 << 6)
#define ANLPAR_10BT                 (1 << 5)
#define ANLPAR_SELECTOR4            (1 << 4)
#define ANLPAR_SELECTOR3            (1 << 3)
#define ANLPAR_SELECTOR2            (1 << 2)
#define ANLPAR_SELECTOR1            (1 << 1)
#define ANLPAR_SELECTOR0            (1 << 0)

//ANER register
#define ANER_PDF                    (1 << 4)
#define ANER_LP_NP_ABLE             (1 << 3)
#define ANER_NP_ABLE                (1 << 2)
#define ANER_PAGE_RX                (1 << 1)
#define ANER_LP_AN_ABLE             (1 << 0)

//SRR register
#define SRR_SILICON_REVISON3        (1 << 9)
#define SRR_SILICON_REVISON2        (1 << 8)
#define SRR_SILICON_REVISON1        (1 << 7)
#define SRR_SILICON_REVISON0        (1 << 6)

//MCSR register
#define MCSR_EDPWRDOWN              (1 << 13)
#define MCSR_LOWSQEN                (1 << 11)
#define MCSR_MDPREBP                (1 << 10)
#define MCSR_FARLOOPBACK            (1 << 9)
#define MCSR_ALTINT                 (1 << 6)
#define MCSR_PHYADBP                (1 << 3)
#define MCSR_FORCE_GOOD_LINK_STATUS (1 << 2)
#define MCSR_ENERGYON               (1 << 1)

//SMR register
#define SMR_MODE2                   (1 << 7)
#define SMR_MODE1                   (1 << 6)
#define SMR_MODE0                   (1 << 5)
#define SMR_PHYAD4                  (1 << 4)
#define SMR_PHYAD3                  (1 << 3)
#define SMR_PHYAD2                  (1 << 2)
#define SMR_PHYAD1                  (1 << 1)
#define SMR_PHYAD0                  (1 << 0)

//SCSIR register
#define SCSIR_AMDIXCTRL            (1 << 15)
#define SCSIR_CH_SELECT            (1 << 13)
#define SCSIR_SQEOFF               (1 << 11)
#define SCSIR_XPOL                 (1 << 4)

//ISR register
#define ISR_ENERGYON                (1 << 7)
#define ISR_AN_COMPLETE             (1 << 6)
#define ISR_REMOTE_FAULT            (1 << 5)
#define ISR_LINK_DOWN               (1 << 4)
#define ISR_AN_LP_ACK               (1 << 3)
#define ISR_PD_FAULT                (1 << 2)
#define ISR_AN_PAGE_RECEIVED        (1 << 1)

//IMR register
#define IMR_ENERGYON                (1 << 7)
#define IMR_AN_COMPLETE             (1 << 6)
#define IMR_REMOTE_FAULT            (1 << 5)
#define IMR_LINK_DOWN               (1 << 4)
#define IMR_AN_LP_ACK               (1 << 3)
#define IMR_PD_FAULT                (1 << 2)
#define IMR_AN_PAGE_RECEIVED        (1 << 1)

//PSCSR register
#define PSCSR_AUTODONE              (1 << 12)
#define PSCSR_GPO2                  (1 << 9)
#define PSCSR_GPO1                  (1 << 8)
#define PSCSR_GPO0                  (1 << 7)
#define PSCSR_ENABLE_4B5B           (1 << 6)
#define PSCSR_HCDSPEED2             (1 << 4)
#define PSCSR_HCDSPEED1             (1 << 3)
#define PSCSR_HCDSPEED0             (1 << 2)
#define PSCSR_SCRAMBLE_DISABLE      (1 << 0)

//Speed indication
#define PSCSR_HCDSPEED_MASK         (7 << 2)
#define PSCSR_HCDSPEED_10BT         (1 << 2)
#define PSCSR_HCDSPEED_100BTX       (2 << 2)
#define PSCSR_HCDSPEED_10BT_FD      (5 << 2)
#define PSCSR_HCDSPEED_100BTX_FD    (6 << 2)

//C++ guard
#ifdef __cplusplus
   extern "C" {
#endif

//LAN8720 Ethernet PHY driver
extern const PhyDriver lan8720PhyDriver;

//LAN8720 related functions
error_t lan8720Init(NetInterface *interface);

void lan8720Tick(NetInterface *interface);

void lan8720EnableIrq(NetInterface *interface);
void lan8720DisableIrq(NetInterface *interface);

void lan8720EventHandler(NetInterface *interface);

void lan8720WritePhyReg(NetInterface *interface, uint8_t address, uint16_t data);
uint16_t lan8720ReadPhyReg(NetInterface *interface, uint8_t address);

void lan8720DumpPhyReg(NetInterface *interface);

//C++ guard
#ifdef __cplusplus
   }
#endif

#endif
