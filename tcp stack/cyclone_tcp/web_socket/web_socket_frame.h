/**
 * @file web_socket_frame.h
 * @brief WebSocket frame parsing and formatting
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

#ifndef _WEB_SOCKET_FRAME_H
#define _WEB_SOCKET_FRAME_H

//Dependencies
#include "core/net.h"
#include "web_socket/web_socket.h"

//C++ guard
#ifdef __cplusplus
   extern "C" {
#endif

//WebSocket related functions
error_t webSocketFormatFrameHeader(WebSocket *webSocket,
   bool_t fin, WebSocketFrameType type, size_t payloadLen);

error_t webSocketParseFrameHeader(WebSocket *webSocket,
   const WebSocketFrame *frame, WebSocketFrameType *type);

error_t webSocketFormatCloseFrame(WebSocket *webSocket);

//C++ guard
#ifdef __cplusplus
   }
#endif

#endif
