/**
 * @file web_socket_transport.h
 * @brief WebSocket transport layer
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

#ifndef _WEB_SOCKET_TRANSPORT_H
#define _WEB_SOCKET_TRANSPORT_H

//Dependencies
#include "core/net.h"
#include "web_socket/web_socket.h"

//C++ guard
#ifdef __cplusplus
   extern "C" {
#endif

//WebSocket related functions
error_t webSocketOpenConnection(WebSocket *webSocket);

error_t webSocketEstablishConnection(WebSocket *webSocket,
   const IpAddr *serverIpAddr, uint16_t serverPort);

error_t webSocketShutdownConnection(WebSocket *webSocket);
void webSocketCloseConnection(WebSocket *webSocket);

error_t webSocketSendData(WebSocket *webSocket, const void *data,
   size_t length, size_t *written, uint_t flags);

error_t webSocketReceiveData(WebSocket *webSocket, void *data,
   size_t size, size_t *received, uint_t flags);

//C++ guard
#ifdef __cplusplus
   }
#endif

#endif
