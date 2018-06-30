/**
 * @file ipv4_routing.h
 * @brief IPv4 routing
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

#ifndef _IPV4_ROUTING_H
#define _IPV4_ROUTING_H

//Dependencies
#include "core/net.h"
#include "ipv4/ipv4.h"

//IPv4 routing support
#ifndef IPV4_ROUTING_SUPPORT
   #define IPV4_ROUTING_SUPPORT DISABLED
#elif (IPV4_ROUTING_SUPPORT != ENABLED && IPV4_ROUTING_SUPPORT != DISABLED)
   #error IPV4_ROUTING_SUPPORT parameter is not valid
#endif

//Size of the IPv4 routing table
#ifndef IPV4_ROUTING_TABLE_SIZE
   #define IPV4_ROUTING_TABLE_SIZE 8
#elif (IPV4_ROUTING_TABLE_SIZE < 1)
   #error IPV4_ROUTING_TABLE_SIZE parameter is not valid
#endif

//C++ guard
#ifdef __cplusplus
   extern "C" {
#endif


/**
 * @brief Routing table entry
 **/

typedef struct
{
   bool_t valid;            ///<Valid entry
   Ipv4Addr networkDest;    ///<Network destination
   Ipv4Addr networkMask;    ///<Subnet mask for this route
   NetInterface *interface; ///<Outgoing network interface
   Ipv4Addr nextHop;        ///<Next hop
   uint_t metric;           ///<Metric value
} Ipv4RoutingTableEntry;


//IPv4 routing related functions
error_t ipv4InitRouting(void);
error_t ipv4EnableRouting(NetInterface *interface, bool_t enable);

error_t ipv4AddRoute(Ipv4Addr networkDest, Ipv4Addr networkMask,
   NetInterface *interface, Ipv4Addr nextHop, uint_t metric);

error_t ipv4DeleteRoute(Ipv4Addr networkDest, Ipv4Addr networkMask);
error_t ipv4DeleteAllRoutes(void);

error_t ipv4ForwardPacket(NetInterface *srcInterface,
   const NetBuffer *ipPacket, size_t ipPacketOffset);

//C++ guard
#ifdef __cplusplus
   }
#endif

#endif
