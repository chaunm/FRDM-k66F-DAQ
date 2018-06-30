/**
 * @file coap_debug.h
 * @brief Data logging functions for debugging purpose (CoAP)
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

#ifndef _COAP_DEBUG_H
#define _COAP_DEBUG_H

//Dependencies
#include "core/net.h"
#include "coap/coap_common.h"

//C++ guard
#ifdef __cplusplus
   extern "C" {
#endif


/**
 * @brief CoAP value/string binding
 **/

typedef struct
{
   uint8_t value;
   const char_t *name;
} CoapParamDesc;


//CoAP related functions
error_t coapDumpMessage(const void *message, size_t length);
error_t coapDumpMessageHeader(const uint8_t *p, size_t length, size_t *consumed);
error_t coapDumpOptions(const uint8_t *p, size_t length, size_t *consumed);
error_t coapDumpOption(const CoapOption *option);

const char_t *coapGetParamName(uint_t value,
   const CoapParamDesc *paramList, size_t paramListLen);

//C++ guard
#ifdef __cplusplus
   }
#endif

#endif
