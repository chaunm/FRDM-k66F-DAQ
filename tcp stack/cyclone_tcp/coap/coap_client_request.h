/**
 * @file coap_client_request.h
 * @brief CoAP request handling
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

#ifndef _COAP_CLIENT_REQUEST_H
#define _COAP_CLIENT_REQUEST_H

//Dependencies
#include "core/net.h"
#include "coap/coap_client.h"
#include "coap/coap_option.h"

//C++ guard
#ifdef __cplusplus
   extern "C" {
#endif


/**
 * @brief CoAP request states
 **/

typedef enum
{
   COAP_REQ_STATE_UNUSED    = 0,
   COAP_REQ_STATE_INIT      = 1,
   COAP_REQ_STATE_TRANSMIT  = 2,
   COAP_REQ_STATE_RECEIVE   = 3,
   COAP_REQ_STATE_SEPARATE  = 4,
   COAP_REQ_STATE_OBSERVE   = 5,
   COAP_REQ_STATE_DONE      = 6,
   COAP_REQ_STATE_RESET     = 7,
   COAP_REQ_STATE_TIMEOUT   = 8,
   COAP_REQ_STATE_CANCELED  = 9
} CoapRequestState;


/**
 * @brief Request status
 **/

typedef enum
{
   COAP_REQUEST_STATUS_SUCCESS  = 0,
   COAP_REQUEST_STATUS_FAILURE  = 1,
   COAP_REQUEST_STATUS_RESET    = 2,
   COAP_REQUEST_STATUS_TIMEOUT  = 3,
   COAP_REQUEST_STATUS_CANCELED = 4
} CoapRequestStatus;


/**
 * @brief Request completed callback
 **/

typedef error_t (*CoapRequestCallback)(CoapClientContext *context,
   CoapClientRequest *request, CoapRequestStatus status, void *param);


/**
 * @brief CoAP request
 **/

struct _CoapClientRequest
{
   CoapRequestState state;        ///<CoAP request state
   CoapClientContext *context;    ///<CoAP client context
   systime_t startTime;           ///<Request start time
   systime_t timeout;             ///<Request timeout
#if (COAP_CLIENT_BLOCK_SUPPORT == ENABLED)
   CoapBlockSize txBlockSzx;       ///<TX block size
   CoapBlockSize rxBlockSzx;       ///<RX block size
#endif
   systime_t retransmitStartTime; ///<Time at which the last message was sent
   systime_t retransmitTimeout;   ///<Retransmission timeout
   uint8_t retransmitCount;       ///<Retransmission counter
   uint32_t observeSeqNum;        ///<Sequence number for reordering detection
   CoapMessage message;           ///<CoAP request message
   CoapRequestCallback callback;  ///<Callback function to invoke when the request completes
   void *param;                   ///<Callback function parameter
};


//CoAP client related functions
CoapClientRequest *coapClientCreateRequest(CoapClientContext *context);

error_t coapClientSetRequestTimeout(CoapClientRequest *request,
   systime_t timeout);

error_t coapClientSendRequest(CoapClientRequest *request,
   CoapRequestCallback callback, void *param);

error_t coapClientCancelRequest(CoapClientRequest *request);
void coapClientDeleteRequest(CoapClientRequest *request);

CoapMessage *coapClientGetRequestMessage(CoapClientRequest *request);
CoapMessage *coapClientGetResponseMessage(CoapClientRequest *request);

error_t coapClientSetType(CoapMessage *message, CoapMessageType type);
error_t coapClientGetType(const CoapMessage *message, CoapMessageType *type);

error_t coapClientSetMethodCode(CoapMessage *message, CoapCode code);
error_t coapClientGetMethodCode(const CoapMessage *message, CoapCode *code);

error_t coapClientGetResponseCode(const CoapMessage *message, CoapCode *code);

error_t coapClientSetUriPath(CoapMessage *message, const char_t *path);

error_t coapClientGetUriPath(const CoapMessage *message, char_t *path,
   size_t maxLen);

error_t coapClientSetUriQuery(CoapMessage *message, const char_t *queryString);

error_t coapClientGetUriQuery(const CoapMessage *message, char_t *queryString,
   size_t maxLen);

error_t coapClientGetLocationPath(const CoapMessage *message, char_t *path,
   size_t maxLen);

error_t coapClientGetLocationQuery(const CoapMessage *message,
   char_t *queryString, size_t maxLen);

error_t coapClientSetOpaqueOption(CoapMessage *message, uint16_t optionNum,
   uint_t optionIndex, const uint8_t *optionValue, size_t optionLen);

error_t coapClientSetStringOption(CoapMessage *message, uint16_t optionNum,
   uint_t optionIndex, const char_t *optionValue);

error_t coapClientSetUintOption(CoapMessage *message, uint16_t optionNum,
   uint_t optionIndex, uint32_t optionValue);

error_t coapClientGetOpaqueOption(const CoapMessage *message, uint16_t optionNum,
   uint_t optionIndex, const uint8_t **optionValue, size_t *optionLen);

error_t coapClientGetStringOption(const CoapMessage *message, uint16_t optionNum,
   uint_t optionIndex, const char_t **optionValue, size_t *optionLen);

error_t coapClientGetUintOption(const CoapMessage *message, uint16_t optionNum,
   uint_t optionIndex, uint32_t *optionValue);

error_t coapClientDeleteOption(CoapMessage *message, uint16_t optionNum,
   uint_t optionIndex);

error_t coapClientSetPayload(CoapMessage *message, const void *payload,
   size_t payloadLen);

error_t coapClientGetPayload(const CoapMessage *message, const uint8_t **payload,
   size_t *payloadLen);

error_t coapClientWritePayload(CoapMessage *message, const void *data,
   size_t length);

error_t coapClientReadPayload(CoapMessage *message, void *data, size_t size,
   size_t *length);

//C++ guard
#ifdef __cplusplus
   }
#endif

#endif
