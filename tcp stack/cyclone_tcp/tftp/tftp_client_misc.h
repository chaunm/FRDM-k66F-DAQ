/**
 * @file tftp_client_misc.h
 * @brief Helper functions for TFTP client
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

#ifndef _TFTP_CLIENT_MISC_H
#define _TFTP_CLIENT_MISC_H

//Dependencies
#include "core/net.h"
#include "tftp/tftp_client.h"

//C++ guard
#ifdef __cplusplus
   extern "C" {
#endif

//TFTP client related functions
error_t tftpClientOpenConnection(TftpClientContext *context);
void tftpClientCloseConnection(TftpClientContext *context);

error_t tftpClientProcessEvents(TftpClientContext *context);

void tftpClientProcessPacket(TftpClientContext *context,
   const IpAddr *srcIpAddr, uint16_t srcPort);

void tftpClientProcessDataPacket(TftpClientContext *context,
   uint16_t srcPort, const TftpDataPacket *dataPacket, size_t length);

void tftpClientProcessAckPacket(TftpClientContext *context,
   uint16_t srcPort, const TftpAckPacket *ackPacket, size_t length);

void tftpClientProcessErrorPacket(TftpClientContext *context,
   uint16_t srcPort, const TftpErrorPacket *errorPacket, size_t length);

error_t tftpClientSendRrqPacket(TftpClientContext *context,
   const char_t *filename, const char_t *mode);

error_t tftpClientSendWrqPacket(TftpClientContext *context,
   const char_t *filename, const char_t *mode);

error_t tftpClientSendDataPacket(TftpClientContext *context);
error_t tftpClientSendAckPacket(TftpClientContext *context);

error_t tftpClientSendErrorPacket(TftpClientContext *context,
   uint16_t errorCode, const char_t *errorMsg);

error_t tftpClientRetransmitPacket(TftpClientContext *context);

//C++ guard
#ifdef __cplusplus
   }
#endif

#endif
