/**
 * @file net_config.h
 * @brief CycloneTCP configuration file
 *
 * @section License
 *
 * Copyright (C) 2010-2016 Oryx Embedded SARL. All rights reserved.
 *
 * This file is part of CycloneTCP Open.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 * @author Oryx Embedded SARL (www.oryx-embedded.com)
 * @version 1.7.4
 **/

#ifndef _NET_CONFIG_H
#define _NET_CONFIG_H

//Trace level for TCP/IP stack debugging
#define MEM_TRACE_LEVEL          4
#define NIC_TRACE_LEVEL          4
#define ETH_TRACE_LEVEL          2
#define ARP_TRACE_LEVEL          2
#define IP_TRACE_LEVEL           2
#define IPV4_TRACE_LEVEL         2
#define IPV6_TRACE_LEVEL         2
#define ICMP_TRACE_LEVEL         2
#define IGMP_TRACE_LEVEL         2
#define ICMPV6_TRACE_LEVEL       2
#define MLD_TRACE_LEVEL          2
#define NDP_TRACE_LEVEL          2
#define UDP_TRACE_LEVEL          2
#define TCP_TRACE_LEVEL          2
#define SOCKET_TRACE_LEVEL       2
#define RAW_SOCKET_TRACE_LEVEL   2
#define BSD_SOCKET_TRACE_LEVEL   2
#define SLAAC_TRACE_LEVEL        4
#define DHCP_TRACE_LEVEL         4
#define DHCPV6_TRACE_LEVEL       4
#define DNS_TRACE_LEVEL          4
#define MDNS_TRACE_LEVEL         4
#define NBNS_TRACE_LEVEL         2
#define LLMNR_TRACE_LEVEL        4
#define FTP_TRACE_LEVEL          5
#define HTTP_TRACE_LEVEL         4
#define SMTP_TRACE_LEVEL         5
#define SNTP_TRACE_LEVEL         4
#define STD_SERVICES_TRACE_LEVEL 5
#define SNMP_TRACE_LEVEL 5
#define GPRS_TRACE_LEVEL 5

//Number of network adapters
#define NET_INTERFACE_COUNT 2

//Size of the multicast MAC filter
#define MAC_MULTICAST_FILTER_SIZE 12

//IPv4 support
#define IPV4_SUPPORT ENABLED
//Size of the IPv4 multicast filter
#define IPV4_MULTICAST_FILTER_SIZE 4

//IPv4 fragmentation support
#define IPV4_FRAG_SUPPORT ENABLED
//Maximum number of fragmented packets the host will accept
//and hold in the reassembly queue simultaneously
#define IPV4_MAX_FRAG_DATAGRAMS 4
//Maximum datagram size the host will accept when reassembling fragments
#define IPV4_MAX_FRAG_DATAGRAM_SIZE 8192

//Size of ARP cache
#define ARP_CACHE_SIZE 8
//Maximum number of packets waiting for address resolution to complete
#define ARP_MAX_PENDING_PACKETS 2

//IGMP support
#define IGMP_SUPPORT ENABLED

//IPv6 support
#define IPV6_SUPPORT DISABLED
//Size of the IPv6 multicast filter
#define IPV6_MULTICAST_FILTER_SIZE 8

//IPv6 fragmentation support
#define IPV6_FRAG_SUPPORT ENABLED
//Maximum number of fragmented packets the host will accept
//and hold in the reassembly queue simultaneously
#define IPV6_MAX_FRAG_DATAGRAMS 4
//Maximum datagram size the host will accept when reassembling fragments
#define IPV6_MAX_FRAG_DATAGRAM_SIZE 8192

//MLD support
#define MLD_SUPPORT ENABLED

//Neighbor cache size
#define NDP_NEIGHBOR_CACHE_SIZE 8
//Destination cache size
#define NDP_DEST_CACHE_SIZE 8
//Maximum number of packets waiting for address resolution to complete
#define NDP_MAX_PENDING_PACKETS 2

//TCP support
#define TCP_SUPPORT ENABLED
//Default buffer size for transmission
#define TCP_DEFAULT_TX_BUFFER_SIZE (1430*2)
//Default buffer size for reception
#define TCP_DEFAULT_RX_BUFFER_SIZE (1430*2)
//Default SYN queue size for listening sockets
#define TCP_DEFAULT_SYN_QUEUE_SIZE 4
//Maximum number of retransmissions
#define TCP_MAX_RETRIES 5
//Selective acknowledgment support
#define TCP_SACK_SUPPORT DISABLED

//UDP support
#define UDP_SUPPORT ENABLED
//Receive queue depth for connectionless sockets
#define UDP_RX_QUEUE_SIZE 4

//Raw socket support
#define RAW_SOCKET_SUPPORT ENABLED
//Receive queue depth for raw sockets
#define RAW_SOCKET_RX_QUEUE_SIZE 4

//Number of sockets that can be opened simultaneously
#define SOCKET_MAX_COUNT 10

//PPP support
#define PPP_SUPPORT ENABLED
//PAP authentication support
#define PAP_SUPPORT ENABLED
//CHAP authentication support
#define CHAP_SUPPORT ENABLED

//Server Side Includes support
#define HTTP_SERVER_SSI_SUPPORT ENABLED
//HTTP Server user-defined
#define USERDEF_SERVER_HTTP DISABLED
//SNMP client user-defined
#define USERDEF_CLIENT_SNMP ENABLED
//SNMP agent support
#define SNMP_AGENT_SUPPORT ENABLED
//SNMPv3 support
#define SNMP_V3_SUPPORT DISABLED
//MIB-II module support
#define MIB2_SUPPORT ENABLED
//Netmem pool support
#define NET_MEM_POOL_SUPPORT DISABLED
//Netmem pool support
#define DNS_CLIENT_SUPPORT ENABLED
//SNMP stack size user-defined
#define SNMP_CLIENT_STACK_SIZE 400
   
/********** USER DEFINITION ************/
//FTP client user-defined
#define USERDEF_CLIENT_FTP      DISABLED
//SW TIMER user-defined
#define USERDEF_SW_TIMER        ENABLED
//ADC Task user-defined
#define USERDEF_ADC_TASK        ENABLED
//USER INTERFACE user-defined
#define USERDEF_USER_INTERFACE  ENABLED
//IO INTERFACE user-defined
#define USERDEF_IO_INTERFACE    ENABLED
//GPRS user-defined
#define USERDEF_GPRS            DISABLED
//PPP user-defined
#define USERDEF_PPP             ENABLED
//MQTT CLIENT user-defined
#define USERDEF_MQTT_CLIENT     ENABLED
//Connection manager user-defined
#define USERDEF_SNMPCONNECT_MANAGER ENABLED

// chaunm
#define USERDEF_CHAUNM_TEST          DISABLED //enable to use specific network configuration for testing purpose
#if (USERDEF_CHAUNM_TEST == ENABLED)
    #define USERDEF_CHAUNM_TEST_GPRS            DISABLED
    #define USERDEF_CHAUNM_TEST_DOOR            DISABLED
    #define USERDEF_CHAUNM_TEST_GEN             DISABLED
    #define USERDEF_NO_TRAP_INFO_UPDATE_TEST    DISABLED
#else
    #define USERDEF_CHAUNM_TEST_GPRS            DISABLED
    #define USERDEF_CHAUNM_TEST_DOOR            DISABLED
    #define USERDEF_CHAUNM_TEST_GEN             DISABLED
    #define USERDEF_NO_TRAP_INFO_UPDATE_TEST    DISABLED
#endif
//#define USERDEF_DEBUG_USING ENABLE
   
#endif
