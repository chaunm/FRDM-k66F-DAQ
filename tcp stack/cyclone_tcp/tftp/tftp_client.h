/**
 * @file tftp_client.h
 * @brief TFTP client
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

#ifndef _TFTP_CLIENT_H
#define _TFTP_CLIENT_H

//Dependencies
#include "core/net.h"
#include "tftp/tftp_common.h"

//TFTP client support
#ifndef TFTP_CLIENT_SUPPORT
   #define TFTP_CLIENT_SUPPORT ENABLED
#elif (TFTP_CLIENT_SUPPORT != ENABLED && TFTP_CLIENT_SUPPORT != DISABLED)
   #error TFTP_CLIENT_SUPPORT parameter is not valid
#endif

//TFTP client tick interval
#ifndef TFTP_CLIENT_TICK_INTERVAL
   #define TFTP_CLIENT_TICK_INTERVAL 500
#elif (TFTP_CLIENT_TICK_INTERVAL < 100)
   #error TFTP_CLIENT_TICK_INTERVAL parameter is not valid
#endif

//Maximum number of retransmissions of packets
#ifndef TFTP_CLIENT_MAX_RETRIES
   #define TFTP_CLIENT_MAX_RETRIES 5
#elif (TFTP_CLIENT_MAX_RETRIES < 1)
   #error TFTP_CLIENT_MAX_RETRIES parameter is not valid
#endif

//Retransmission timeout
#ifndef TFTP_CLIENT_TIMEOUT
   #define TFTP_CLIENT_TIMEOUT 5000
#elif (TFTP_CLIENT_TIMEOUT < 1000)
   #error TFTP_CLIENT_TIMEOUT parameter is not valid
#endif

//Additional delay before closing the connection (when sending the final ACK)
#ifndef TFTP_CLIENT_FINAL_DELAY
   #define TFTP_CLIENT_FINAL_DELAY 10000
#elif (TFTP_CLIENT_FINAL_DELAY < 1000)
   #error TFTP_CLIENT_FINAL_DELAY parameter is not valid
#endif

//Block size
#ifndef TFTP_CLIENT_BLOCK_SIZE
   #define TFTP_CLIENT_BLOCK_SIZE 512
#elif (TFTP_CLIENT_BLOCK_SIZE < 512)
   #error TFTP_CLIENT_BLOCK_SIZE parameter is not valid
#endif

//Maximum size of TFTP packets
#define TFTP_CLIENT_MAX_PACKET_SIZE (sizeof(TftpDataPacket) + TFTP_CLIENT_BLOCK_SIZE)

//C++ guard
#ifdef __cplusplus
   extern "C" {
#endif


/**
 * @brief File access modes
 **/

typedef enum
{
   TFTP_FILE_MODE_READ     = 0,
   TFTP_FILE_MODE_WRITE    = 1,
   TFTP_FILE_MODE_OCTET    = 0,
   TFTP_FILE_MODE_NETASCII = 2
} TftpClientFileMode;


/**
 * @brief TFTP client state
 **/

typedef enum
{
   TFTP_CLIENT_STATE_CLOSED    = 0,
   TFTP_CLIENT_STATE_RRQ       = 1,
   TFTP_CLIENT_STATE_WRQ       = 2,
   TFTP_CLIENT_STATE_DATA      = 3,
   TFTP_CLIENT_STATE_ACK       = 4,
   TFTP_CLIENT_STATE_LAST_DATA = 5,
   TFTP_CLIENT_STATE_COMPLETE  = 6,
   TFTP_CLIENT_STATE_ERROR     = 7
} TftpClientState;


/**
 * @brief TFTP client context
 **/

typedef struct
{
   NetInterface *interface;                        ///<Underlying network interface
   IpAddr serverIpAddr;
   uint16_t serverPort;
   uint16_t serverTid;
   Socket *socket;                                 ///<Underlying UDP socket
   TftpClientState state;                          ///<TFTP client state
   uint16_t block;                                 ///<Block number
   systime_t timestamp;                            ///<Time stamp to manage retransmissions
   uint_t retransmitCount;                         ///<Retransmission counter
   uint8_t inPacket[TFTP_CLIENT_MAX_PACKET_SIZE];  ///<Incoming TFTP packet
   size_t inPacketLen;                             ///<Length of the outgoing packet
   size_t inDataLen;
   size_t inDataPos;
   uint8_t outPacket[TFTP_CLIENT_MAX_PACKET_SIZE]; ///<Outgoing TFTP packet
   size_t outPacketLen;                            ///<Length of the outgoing packet
   size_t outDataLen;
} TftpClientContext;


//TFTP client related functions
void tftpClientInit(TftpClientContext *context);

error_t tftpClientBindToInterface(TftpClientContext *context,
   NetInterface *interface);

error_t tftpClientConnect(TftpClientContext *context,
   const IpAddr *serverIpAddr, uint16_t serverPort);

error_t tftpClientOpenFile(TftpClientContext *context,
   const char_t *filename, uint_t mode);

error_t tftpClientWriteFile(TftpClientContext *context,
   const void *data, size_t length, size_t *written, uint_t flags);

error_t tftpClientReadFile(TftpClientContext *context,
   void *data, size_t size, size_t *received, uint_t flags);

error_t tftpClientFlushFile(TftpClientContext *context);
void tftpClientCloseFile(TftpClientContext *context);

//C++ guard
#ifdef __cplusplus
   }
#endif

#endif
