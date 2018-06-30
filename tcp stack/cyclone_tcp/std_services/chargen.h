/**
 * @file chargen.h
 * @brief Character generator protocol
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

#ifndef _CHARGEN_H
#define _CHARGEN_H

//Dependencies
#include "core/net.h"
#include "core/socket.h"

//Stack size required to run the chargen service
#ifndef CHARGEN_SERVICE_STACK_SIZE
   #define CHARGEN_SERVICE_STACK_SIZE 600
#elif (CHARGEN_SERVICE_STACK_SIZE < 1)
   #error CHARGEN_SERVICE_STACK_SIZE parameter is not valid
#endif

//Priority at which the chargen service should run
#ifndef CHARGEN_SERVICE_PRIORITY
   #define CHARGEN_SERVICE_PRIORITY OS_TASK_PRIORITY_NORMAL
#endif

//Size of the buffer for input/output operations
#ifndef CHARGEN_BUFFER_SIZE
   #define CHARGEN_BUFFER_SIZE 1500
#elif (CHARGEN_BUFFER_SIZE < 1)
   #error CHARGEN_BUFFER_SIZE parameter is not valid
#endif

//Maximum time the TCP chargen server will wait before closing the connection
#ifndef CHARGEN_TIMEOUT
   #define CHARGEN_TIMEOUT 20000
#elif (CHARGEN_TIMEOUT < 1)
   #error CHARGEN_TIMEOUT parameter is not valid
#endif

//Chargen service port
#define CHARGEN_PORT 19

//C++ guard
#ifdef __cplusplus
   extern "C" {
#endif


/**
 * @brief Chargen service context
 **/

typedef struct
{
   Socket *socket;
   char_t buffer[CHARGEN_BUFFER_SIZE];
} ChargenServiceContext;


//TCP chargen service related functions
error_t tcpChargenStart(void);
void tcpChargenListenerTask(void *param);
void tcpChargenConnectionTask(void *param);

//UDP chargen service related functions
error_t udpChargenStart(void);
void udpChargenTask(void *param);

//C++ guard
#ifdef __cplusplus
   }
#endif

#endif
