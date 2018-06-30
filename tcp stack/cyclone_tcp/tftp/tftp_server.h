/**
 * @file tftp_server.h
 * @brief TFTP server
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

#ifndef _TFTP_SERVER_H
#define _TFTP_SERVER_H

//Dependencies
#include "core/net.h"
#include "tftp/tftp_common.h"

//TFTP server support
#ifndef TFTP_SERVER_SUPPORT
   #define TFTP_SERVER_SUPPORT ENABLED
#elif (TFTP_SERVER_SUPPORT != ENABLED && TFTP_SERVER_SUPPORT != DISABLED)
   #error TFTP_SERVER_SUPPORT parameter is not valid
#endif

//Stack size required to run the TFTP server
#ifndef TFTP_SERVER_STACK_SIZE
   #define TFTP_SERVER_STACK_SIZE 650
#elif (TFTP_SERVER_STACK_SIZE < 1)
   #error TFTP_SERVER_STACK_SIZE parameter is not valid
#endif

//Priority at which the TFTP server should run
#ifndef TFTP_SERVER_PRIORITY
   #define TFTP_SERVER_PRIORITY OS_TASK_PRIORITY_NORMAL
#endif

//Maximum number of simultaneous connections
#ifndef TFTP_SERVER_MAX_CONNECTIONS
   #define TFTP_SERVER_MAX_CONNECTIONS 2
#elif (TFTP_SERVER_MAX_CONNECTIONS < 1)
   #error TFTP_SERVER_MAX_CONNECTIONS parameter is not valid
#endif

//TFTP server tick interval
#ifndef TFTP_SERVER_TICK_INTERVAL
   #define TFTP_SERVER_TICK_INTERVAL 500
#elif (TFTP_SERVER_TICK_INTERVAL < 100)
   #error TFTP_SERVER_TICK_INTERVAL parameter is not valid
#endif

//Maximum number of retransmissions of packets
#ifndef TFTP_SERVER_MAX_RETRIES
   #define TFTP_SERVER_MAX_RETRIES 5
#elif (TFTP_SERVER_MAX_RETRIES < 1)
   #error TFTP_SERVER_MAX_RETRIES parameter is not valid
#endif

//Retransmission timeout
#ifndef TFTP_SERVER_TIMEOUT
   #define TFTP_SERVER_TIMEOUT 5000
#elif (TFTP_SERVER_TIMEOUT < 1000)
   #error TFTP_SERVER_TIMEOUT parameter is not valid
#endif

//Additional delay before closing the connection (when sending the final ACK)
#ifndef TFTP_SERVER_FINAL_DELAY
   #define TFTP_SERVER_FINAL_DELAY 10000
#elif (TFTP_SERVER_FINAL_DELAY < 1000)
   #error TFTP_SERVER_FINAL_DELAY parameter is not valid
#endif

//Block size
#ifndef TFTP_SERVER_BLOCK_SIZE
   #define TFTP_SERVER_BLOCK_SIZE 512
#elif (TFTP_SERVER_BLOCK_SIZE < 512)
   #error TFTP_SERVER_BLOCK_SIZE parameter is not valid
#endif

//Maximum size of TFTP packets
#define TFTP_SERVER_MAX_PACKET_SIZE (sizeof(TftpDataPacket) + TFTP_SERVER_BLOCK_SIZE)

//Forward declaration of TftpClientConnection structure
struct _TftpClientConnection;
#define TftpClientConnection struct _TftpClientConnection

//Forward declaration of TftpServerContext structure
struct _TftpServerContext;
#define TftpServerContext struct _TftpServerContext

//C++ guard
#ifdef __cplusplus
   extern "C" {
#endif


/**
 * @brief TFTP connection state
 **/

typedef enum
{
   TFTP_STATE_CLOSED         = 0,
   TFTP_STATE_OPEN           = 1,
   TFTP_STATE_READING        = 2,
   TFTP_STATE_WRITING        = 3,
   TFTP_STATE_READ_COMPLETE  = 4,
   TFTP_STATE_WRITE_COMPLETE = 5
} TftpConnectionState;


/**
 * @brief Open file callback function
 **/

typedef void *(*TftpServerOpenFileCallback)(const char_t *filename,
   const char_t *mode, bool_t writeAccess);


/**
 * @brief Write file callback function
 **/

typedef error_t (*TftpServerWriteFileCallback)(void *file,
   size_t offset, const uint8_t *data, size_t length);


/**
 * @brief Read file callback function
 **/

typedef error_t (*TftpServerReadFileCallback)(void *file,
   size_t offset, uint8_t *data, size_t size, size_t *length);


/**
 * @brief Close file callback function
 **/

typedef void (*TftpServerCloseFileCallback)(void *file);


/**
 * @brief TFTP server settings
 **/

typedef struct
{
   NetInterface *interface;                       ///<Underlying network interface
   uint16_t port;                                 ///<TFTP port number
   TftpServerOpenFileCallback openFileCallback;   ///<Open file callback function
   TftpServerWriteFileCallback writeFileCallback; ///<Write file callback function
   TftpServerReadFileCallback readFileCallback;   ///<Read file callback function
   TftpServerCloseFileCallback closeFileCallback; ///<Close file callback function
} TftpServerSettings;


/**
 * @brief TFTP client connection
 **/

struct _TftpClientConnection
{
   TftpServerSettings *settings;                ///<User settings
   TftpConnectionState state;                   ///<Connection state
   Socket *socket;                              ///<Underlying socket
   void *file;                                  ///<File pointer
   uint16_t block;                              ///<Block number
   systime_t timestamp;                         ///<Time stamp to manage retransmissions
   uint_t retransmitCount;                      ///<Retransmission counter
   uint8_t packet[TFTP_SERVER_MAX_PACKET_SIZE]; ///<Outgoing TFTP packet
   size_t packetLen;                            ///<Length of the outgoing packet
};


/**
 * @brief TFTP server context
 **/

struct _TftpServerContext
{
   TftpServerSettings settings;                                  ///<User settings
   OsEvent event;                                                ///<Event object used to poll the sockets
   Socket *socket;                                               ///<Listening socket
   TftpClientConnection connection[TFTP_SERVER_MAX_CONNECTIONS]; ///<Client connections
   SocketEventDesc eventDesc[TFTP_SERVER_MAX_CONNECTIONS + 1];   ///<The events the application is interested in
   uint8_t packet[TFTP_SERVER_MAX_PACKET_SIZE];                  ///<Incoming TFTP packet
};


//TFTP server related functions
void tftpServerGetDefaultSettings(TftpServerSettings *settings);
error_t tftpServerInit(TftpServerContext *context, const TftpServerSettings *settings);
error_t tftpServerStart(TftpServerContext *context);

void tftpServerTask(TftpServerContext *context);

//C++ guard
#ifdef __cplusplus
   }
#endif

#endif
