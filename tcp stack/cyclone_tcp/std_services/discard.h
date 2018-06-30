/**
 * @file discard.h
 * @brief Discard protocol
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

#ifndef _DISCARD_H
#define _DISCARD_H

//Dependencies
#include "core/net.h"
#include "core/socket.h"

//Stack size required to run the discard service
#ifndef DISCARD_SERVICE_STACK_SIZE
   #define DISCARD_SERVICE_STACK_SIZE 600
#elif (DISCARD_SERVICE_STACK_SIZE < 1)
   #error DISCARD_SERVICE_STACK_SIZE parameter is not valid
#endif

//Priority at which the discard service should run
#ifndef DISCARD_SERVICE_PRIORITY
   #define DISCARD_SERVICE_PRIORITY OS_TASK_PRIORITY_NORMAL
#endif

//Size of the buffer for input/output operations
#ifndef DISCARD_BUFFER_SIZE
   #define DISCARD_BUFFER_SIZE 1500
#elif (DISCARD_BUFFER_SIZE < 1)
   #error DISCARD_BUFFER_SIZE parameter is not valid
#endif

//Maximum time the TCP discard server will wait before closing the connection
#ifndef DISCARD_TIMEOUT
   #define DISCARD_TIMEOUT 20000
#elif (DISCARD_TIMEOUT < 1)
   #error DISCARD_TIMEOUT parameter is not valid
#endif

//Discard service port
#define DISCARD_PORT 9

//C++ guard
#ifdef __cplusplus
   extern "C" {
#endif


/**
 * @brief Discard service context
 **/

typedef struct
{
   Socket *socket;
   char_t buffer[DISCARD_BUFFER_SIZE];
} DiscardServiceContext;


//TCP discard service related functions
error_t tcpDiscardStart(void);
void tcpDiscardListenerTask(void *param);
void tcpDiscardConnectionTask(void *param);

//UDP discard service related functions
error_t udpDiscardStart(void);
void udpDiscardTask(void *param);

//C++ guard
#ifdef __cplusplus
   }
#endif

#endif
