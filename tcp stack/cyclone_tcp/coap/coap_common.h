/**
 * @file coap_common.h
 * @brief Definitions common to CoAP client and server
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

#ifndef _COAP_COMMON_H
#define _COAP_COMMON_H

//Dependencies
#include "core/net.h"

//Maximum size of CoAP messages
#ifndef COAP_MAX_MSG_SIZE
   #define COAP_MAX_MSG_SIZE 1024
#elif (COAP_MAX_MSG_SIZE < 16)
   #error COAP_MAX_MSG_SIZE parameter is not valid
#endif

//CoAP port number
#define COAP_PORT 5683
//DTLS-secured CoAP port number
#define COAPS_PORT 5684

//Maximum acceptable length for tokens
#define COAP_MAX_TOKEN_LEN 8

//CoAP payload marker value
#define COAP_PAYLOAD_MARKER   0xFF

//CoAP code definition
#define COAP_CODE(c, d) ((((c) & 0x07U) << 5U) | ((d) & 0x1FU))

//Get code class
#define COAP_GET_CODE_CLASS(code) (((code) >> 5U) & 0x07U)
//Get code subclass
#define COAP_GET_CODE_SUBCLASS(code) ((code) & 0x1FU)

//C++ guard
#ifdef __cplusplus
   extern "C" {
#endif


/**
 * @brief CoAP version numbers
 **/

typedef enum {
   COAP_VERSION_1 = 1 ///<CoAP version 1
} CoapProtocolLevel;


/**
 * @brief CoAP transport protocols
 **/

typedef enum {
   COAP_TRANSPORT_PROTOCOL_UDP  = 1, ///<UDP protocol
   COAP_TRANSPORT_PROTOCOL_DTLS = 2, ///<DTLS protocol
} CoapTransportProtocol;


/**
 * @brief CoAP message types
 **/

typedef enum
{
   COAP_TYPE_CON = 0, ///<Confirmable message
   COAP_TYPE_NON = 1, ///<Non-confirmable message
   COAP_TYPE_ACK = 2, ///<Acknowledgment message
   COAP_TYPE_RST = 3  ///<Reset message
} CoapMessageType;


/**
 * @brief CoAP code classes
 **/

typedef enum
{
   COAP_CODE_CLASS_SUCCESS      = 2,
   COAP_CODE_CLASS_CLIENT_ERROR = 4,
   COAP_CODE_CLASS_SERVER_ERROR = 5
} CoapCodeClass;


/**
 * @brief CoAP method and response codes
 **/

typedef enum
{
   COAP_CODE_EMPTY                        = COAP_CODE(0, 0),
   COAP_CODE_GET                          = COAP_CODE(0, 1),
   COAP_CODE_POST                         = COAP_CODE(0, 2),
   COAP_CODE_PUT                          = COAP_CODE(0, 3),
   COAP_CODE_DELETE                       = COAP_CODE(0, 4),
   COAP_CODE_FETCH                        = COAP_CODE(0, 5),
   COAP_CODE_PATCH                        = COAP_CODE(0, 6),
   COAP_CODE_IPATCH                       = COAP_CODE(0, 7),
   COAP_CODE_CREATED                      = COAP_CODE(2, 1),
   COAP_CODE_DELETED                      = COAP_CODE(2, 2),
   COAP_CODE_VALID                        = COAP_CODE(2, 3),
   COAP_CODE_CHANGED                      = COAP_CODE(2, 4),
   COAP_CODE_CONTENT                      = COAP_CODE(2, 5),
   COAP_CODE_CONTINUE                     = COAP_CODE(2, 31),
   COAP_CODE_BAD_REQUEST                  = COAP_CODE(4, 0),
   COAP_CODE_UNAUTHOZED                   = COAP_CODE(4, 1),
   COAP_CODE_BAD_OPTION                   = COAP_CODE(4, 2),
   COAP_CODE_FORBIDDEN                    = COAP_CODE(4, 3),
   COAP_CODE_NOT_FOUND                    = COAP_CODE(4, 4),
   COAP_CODE_METHOD_NOT_ALLOWED           = COAP_CODE(4, 5),
   COAP_CODE_NOT_ACCEPTABLE               = COAP_CODE(4, 6),
   COAP_CODE_REQUEST_ENTITY_INCOMPLETE    = COAP_CODE(4, 8),
   COAP_CODE_CONFLICT                     = COAP_CODE(4, 9),
   COAP_CODE_PRECONDITION_FAILED          = COAP_CODE(4, 12),
   COAP_CODE_REQUEST_ENTITY_TO_LARGE      = COAP_CODE(4, 13),
   COAP_CODE_UNSUPPORTED_CONTENT_FORMAT   = COAP_CODE(4, 15),
   COAP_CODE_UNPROCESSABLE_ENTITY         = COAP_CODE(4, 22),
   COAP_CODE_INTERNAL_SERVER              = COAP_CODE(5, 0),
   COAP_CODE_NOT_IMPLEMENTED              = COAP_CODE(5, 1),
   COAP_CODE_BAD_GATEWAY                  = COAP_CODE(5, 2),
   COAP_CODE_SERVICE_UNAVAILABLE          = COAP_CODE(5, 3),
   COAP_CODE_GATEWAY_TIMEOUT              = COAP_CODE(5, 4),
   COAP_CODE_PROXYING_NOT_SUPPORTED       = COAP_CODE(5, 5)
} CoapCode;


//CodeWarrior or Win32 compiler?
#if defined(__CWCC__) || defined(_WIN32)
   #pragma pack(push, 1)
#endif


/**
 * @brief CoAP message format
 **/

typedef __start_packed struct
{
#ifdef _CPU_BIG_ENDIAN
   uint8_t version : 2;  //0
   uint8_t type : 2;
   uint8_t tokenLen : 4;
#else
   uint8_t tokenLen : 4; //0
   uint8_t type : 2;
   uint8_t version : 2;
#endif
   uint8_t code;         //1
   uint16_t mid;         //2-3
   uint8_t token[];      //4
} __end_packed CoapMessageHeader;


/**
 * @brief CoAP option format
 **/

typedef __start_packed struct
{
#ifdef _CPU_BIG_ENDIAN
   uint8_t delta : 4;  //0
   uint8_t length : 4;
#else
   uint8_t length : 4; //0
   uint8_t delta : 4;
#endif
} __end_packed CoapOptionHeader;


//CodeWarrior or Win32 compiler?
#if defined(__CWCC__) || defined(_WIN32)
   #pragma pack(pop)
#endif


//C++ guard
#ifdef __cplusplus
   }
#endif

#endif
