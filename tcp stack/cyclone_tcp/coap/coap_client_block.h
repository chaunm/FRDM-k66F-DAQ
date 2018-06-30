/**
 * @file coap_client_block.h
 * @brief CoAP block-wise transfer
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

#ifndef _COAP_CLIENT_BLOCK_H
#define _COAP_CLIENT_BLOCK_H

//Dependencies
#include "core/net.h"
#include "coap/coap_client.h"

//C++ guard
#ifdef __cplusplus
   extern "C" {
#endif

//CoAP client related functions
error_t coapClientSetTxBlockSize(CoapClientRequest *request, uint_t blockSize);
error_t coapClientSetRxBlockSize(CoapClientRequest *request, uint_t blockSize);

error_t coapClientWriteBody(CoapClientRequest *request, const void *data,
   size_t length, size_t *written, bool_t last);

error_t coapClientReadBody(CoapClientRequest *request, void *data,
   size_t size, size_t *received);

CoapBlockSize coapClientGetMaxBlockSize(void);

//C++ guard
#ifdef __cplusplus
   }
#endif

#endif
