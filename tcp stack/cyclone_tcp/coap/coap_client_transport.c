/**
 * @file coap_client_transport.c
 * @brief Transport protocol abstraction layer
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

//Switch to the appropriate trace level
#define TRACE_LEVEL COAP_TRACE_LEVEL

//Dependencies
#include "core/net.h"
#include "coap/coap_client.h"
#include "coap/coap_client_transport.h"
#include "debug.h"

//Check TCP/IP stack configuration
#if (COAP_CLIENT_SUPPORT == ENABLED)


/**
 * @brief Open network connection
 * @param[in] context Pointer to the CoAP client context
 * @return Error code
 **/

error_t coapClientOpenConnection(CoapClientContext *context)
{
   error_t error;

   //Open a UDP socket
   context->socket = socketOpen(SOCKET_TYPE_DGRAM, SOCKET_IP_PROTO_UDP);
   //Failed to open socket?
   if(context->socket == NULL)
      return ERROR_OPEN_FAILED;

   //Associate the socket with the relevant interface
   error = socketBindToInterface(context->socket, context->interface);
   //Any error to report?
   if(error)
      return error;

   //Force the socket to operate in non-blocking mode
   error = socketSetTimeout(context->socket, 0);
   //Any error to report?
   if(error)
      return error;

#if (COAP_CLIENT_DTLS_SUPPORT == ENABLED)
   //DTLS transport protocol?
   if(context->transportProtocol == COAP_TRANSPORT_PROTOCOL_DTLS)
   {
      //Allocate DTLS context
      context->dtlsContext = tlsInit();
      //Failed to allocate DTLS context?
      if(context->dtlsContext == NULL)
         return ERROR_OPEN_FAILED;

      //Select client operation mode
      error = tlsSetConnectionEnd(context->dtlsContext,
         TLS_CONNECTION_END_CLIENT);
      //Any error to report?
      if(error)
         return error;

      //Set the transport protocol to be used (DTLS)
      error = tlsSetTransportProtocol(context->dtlsContext,
         TLS_TRANSPORT_PROTOCOL_DATAGRAM);
      //Any error to report?
      if(error)
         return error;

      //Bind DTLS to the relevant socket
      error = tlsSetSocket(context->dtlsContext, context->socket);
      //Any error to report?
      if(error)
         return error;

      //Force DTLS to operate in non-blocking mode
      error = tlsSetTimeout(context->dtlsContext, 0);
      //Any error to report?
      if(error)
         return error;

      //Restore DTLS session, if any
      if(context->dtlsSession.idLength > 0)
      {
         //Restore DTLS session
         error = tlsRestoreSession(context->dtlsContext, &context->dtlsSession);
         //Any error to report?
         if(error)
            return error;
      }

      //Invoke user-defined callback, if any
      if(context->dtlsInitCallback != NULL)
      {
         //Perform DTLS related initialization
         error = context->dtlsInitCallback(context, context->dtlsContext);
         //Any error to report?
         if(error)
            return error;
      }
   }
#endif

   //Successful processing
   return NO_ERROR;
}


/**
 * @brief Establish network connection
 * @param[in] context Pointer to the CoAP client context
 * @param[in] serverIpAddr IP address of the CoAP server
 * @param[in] serverPort UDP port number
 * @return Error code
 **/

error_t coapClientEstablishConnection(CoapClientContext *context,
   const IpAddr *serverIpAddr, uint16_t serverPort)
{
   error_t error;

   //Only accept datagrams from the specified CoAP server
   error = socketConnect(context->socket, serverIpAddr, serverPort);
   //Any error to report?
   if(error)
      return error;

#if (COAP_CLIENT_DTLS_SUPPORT == ENABLED)
   //DTLS transport protocol?
   if(context->transportProtocol == COAP_TRANSPORT_PROTOCOL_DTLS)
   {
      //Perform DTLS handshake
      error = tlsConnect(context->dtlsContext);
      //Any error to report?
      if(error)
         return error;

      //Ensure the session ID is valid
      if(context->dtlsContext->sessionIdLen > 0)
      {
         //Save DTLS session
         error = tlsSaveSession(context->dtlsContext, &context->dtlsSession);
         //Any error to report?
         if(error)
            return error;
      }
   }
#endif

   //Successful processing
   return NO_ERROR;
}


/**
 * @brief Shutdown network connection
 * @param[in] context Pointer to the CoAP client context
 * @return Error code
 **/

error_t coapClientShutdownConnection(CoapClientContext *context)
{
   error_t error;

   //Initialize status code
   error = NO_ERROR;

#if (COAP_CLIENT_DTLS_SUPPORT == ENABLED)
   //DTLS transport protocol?
   if(context->transportProtocol == COAP_TRANSPORT_PROTOCOL_DTLS)
   {
      //Shutdown DTLS session
      error = tlsShutdown(context->dtlsContext);
   }
#endif

   //Return status code
   return error;
}


/**
 * @brief Close network connection
 * @param[in] context Pointer to the CoAP client context
 **/

void coapClientCloseConnection(CoapClientContext *context)
{
#if (COAP_CLIENT_DTLS_SUPPORT == ENABLED)
   //DTLS transport protocol?
   if(context->transportProtocol == COAP_TRANSPORT_PROTOCOL_DTLS)
   {
      //Valid DTLS context?
      if(context->dtlsContext != NULL)
      {
         //Release DTLS context
         tlsFree(context->dtlsContext);
         context->dtlsContext = NULL;
      }
   }
#endif

   //Valid socket?
   if(context->socket != NULL)
   {
      //Close UDP socket
      socketClose(context->socket);
      context->socket = NULL;
   }
}


/**
 * @brief Send a datagram
 * @param[in] context Pointer to the CoAP client context
 * @param[in] data Pointer to a buffer containing the datagram to be transmitted
 * @param[in] length Length of the datagram, in bytes
 * @return Error code
 **/

error_t coapClientSendDatagram(CoapClientContext *context,
   const void *data, size_t length)
{
   error_t error;

#if (COAP_CLIENT_DTLS_SUPPORT == ENABLED)
   //DTLS transport protocol?
   if(context->transportProtocol == COAP_TRANSPORT_PROTOCOL_DTLS)
   {
      //Transmit datagram
      error = tlsWrite(context->dtlsContext, data, length, NULL, 0);
   }
   else
#endif
   //UDP transport protocol?
   {
      //Transmit datagram
      error = socketSend(context->socket, data, length, NULL, 0);
   }

   //Return status code
   return error;
}


/**
 * @brief Receive a datagram
 * @param[in] context Pointer to the CoAP client context
 * @param[out] data Buffer into which the received datagram will be placed
 * @param[in] size Maximum number of bytes that can be received
 * @param[out] received Number of bytes that have been received
 * @return Error code
 **/

error_t coapClientReceiveDatagram(CoapClientContext *context,
   void *data, size_t size, size_t *received)
{
   error_t error;

   //No data has been read yet
   *received = 0;

#if (COAP_CLIENT_DTLS_SUPPORT == ENABLED)
   //DTLS transport protocol?
   if(context->transportProtocol == COAP_TRANSPORT_PROTOCOL_DTLS)
   {
      //Receive datagram
      error = tlsRead(context->dtlsContext, data, size, received, 0);
   }
   else
#endif
   //UDP transport protocol?
   {
      //Receive datagram
      error = socketReceive(context->socket, data, size, received, 0);
   }

   //Return status code
   return error;
}


/**
 * @brief Wait for incoming datagrams
 * @param[in] context Pointer to the CoAP client context
 * @param[in] timeout Maximum time to wait before returning
 * @return Error code
 **/

error_t coapClientWaitForDatagram(CoapClientContext *context,
   systime_t timeout)
{
   error_t error;
   SocketEventDesc eventDesc[1];

   //Initialize status code
   error = ERROR_BUFFER_EMPTY;

#if (COAP_CLIENT_DTLS_SUPPORT == ENABLED)
   //DTLS transport protocol?
   if(context->transportProtocol == COAP_TRANSPORT_PROTOCOL_DTLS)
   {
      //Check whether a datagram is pending in the receive buffer
      if(tlsIsRxReady(context->dtlsContext))
      {
         //No need to poll the underlying socket for incoming traffic...
         error = NO_ERROR;
      }
   }
#endif

   //Check status code
   if(error == ERROR_BUFFER_EMPTY)
   {
      //Set the events the application is interested in
      eventDesc[0].socket = context->socket;
      eventDesc[0].eventMask = SOCKET_EVENT_RX_READY;

      //Release exclusive access to the CoAP client context
      osReleaseMutex(&context->mutex);

      //Wait for incoming traffic
      error = socketPoll(eventDesc, arraysize(eventDesc), &context->event,
         timeout);

      //Acquire exclusive access to the CoAP client context
      osAcquireMutex(&context->mutex);
   }

   //Return status code
   return error;
}

#endif
