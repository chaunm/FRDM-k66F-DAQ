/**
 * @file tftp_server_misc.h
 * @brief Helper functions for TFTP server
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

#ifndef _TFTP_SERVER_MISC_H
#define _TFTP_SERVER_MISC_H

//Dependencies
#include "core/net.h"
#include "tftp/tftp_server.h"

//C++ guard
#ifdef __cplusplus
   extern "C" {
#endif

//TFTP server related functions
void tftpServerTick(TftpServerContext *context);

TftpClientConnection *tftpServerOpenConnection(TftpServerContext *context,
   const IpAddr *clientIpAddr, uint16_t clientPort);

void tftpServerCloseConnection(TftpClientConnection *connection);

void tftpServerAcceptRequest(TftpServerContext *context);

void tftpServerProcessPacket(TftpServerContext *context,
   TftpClientConnection *connection);

void tftpServerProcessRrqPacket(TftpServerContext *context, const IpAddr *clientIpAddr,
   uint16_t clientPort, const TftpRrqPacket *rrqPacket, size_t length);

void tftpServerProcessWrqPacket(TftpServerContext *context, const IpAddr *clientIpAddr,
   uint16_t clientPort, const TftpWrqPacket *wrqPacket, size_t length);

void tftpServerProcessDataPacket(TftpClientConnection *connection,
   const TftpDataPacket *dataPacket, size_t length);

void tftpServerProcessAckPacket(TftpClientConnection *connection,
   const TftpAckPacket *ackPacket, size_t length);

void tftpServerProcessErrorPacket(TftpClientConnection *connection,
   const TftpErrorPacket *errorPacket, size_t length);

error_t tftpServerSendDataPacket(TftpClientConnection *connection);
error_t tftpServerSendAckPacket(TftpClientConnection *connection);

error_t tftpServerSendErrorPacket(TftpClientConnection *connection,
   uint16_t errorCode, const char_t *errorMsg);

error_t tftpServerRetransmitPacket(TftpClientConnection *connection);

//C++ guard
#ifdef __cplusplus
   }
#endif

#endif
