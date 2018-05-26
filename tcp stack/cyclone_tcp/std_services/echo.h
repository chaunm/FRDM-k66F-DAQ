/**
 * @file echo.h
 * @brief Echo protocol
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

#ifndef _ECHO_H
#define _ECHO_H

//Dependencies
#include "core/net.h"
#include "core/socket.h"

//Stack size required to run the echo service
#ifndef ECHO_SERVICE_STACK_SIZE
   #define ECHO_SERVICE_STACK_SIZE 600
#elif (ECHO_SERVICE_STACK_SIZE < 1)
   #error ECHO_SERVICE_STACK_SIZE parameter is not valid
#endif

//Priority at which the echo service should run
#ifndef ECHO_SERVICE_PRIORITY
   #define ECHO_SERVICE_PRIORITY OS_TASK_PRIORITY_NORMAL
#endif

//Size of the buffer for input/output operations
#ifndef ECHO_BUFFER_SIZE
   #define ECHO_BUFFER_SIZE 1500
#elif (ECHO_BUFFER_SIZE < 1)
   #error ECHO_BUFFER_SIZE parameter is not valid
#endif

//Maximum time the TCP echo server will wait before closing the connection
#ifndef ECHO_TIMEOUT
   #define ECHO_TIMEOUT 20000
#elif (ECHO_TIMEOUT < 1)
   #error ECHO_TIMEOUT parameter is not valid
#endif

//Echo service port
#define ECHO_PORT 7

//C++ guard
#ifdef __cplusplus
   extern "C" {
#endif


/**
 * @brief Echo service context
 **/

typedef struct
{
   Socket *socket;
   char_t buffer[ECHO_BUFFER_SIZE];
} EchoServiceContext;


//TCP echo service related functions
error_t tcpEchoStart(void);
void tcpEchoListenerTask(void *param);
void tcpEchoConnectionTask(void *param);

//UDP echo service related functions
error_t udpEchoStart(void);
void udpEchoTask(void *param);

//C++ guard
#ifdef __cplusplus
   }
#endif

#endif
