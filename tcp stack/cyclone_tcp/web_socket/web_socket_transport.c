/**
 * @file web_socket_transport.c
 * @brief WebSocket transport layer
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
#define TRACE_LEVEL WEB_SOCKET_TRACE_LEVEL

//Dependencies
#include "core/net.h"
#include "web_socket/web_socket.h"
#include "web_socket/web_socket_transport.h"
#include "debug.h"

//Check TCP/IP stack configuration
#if (WEB_SOCKET_SUPPORT == ENABLED)


/**
 * @brief Open network connection
 * @param[in] webSocket Handle to a WebSocket
 * @return Error code
 **/

error_t webSocketOpenConnection(WebSocket *webSocket)
{
   error_t error;

   //Invalid socket handle?
   if(webSocket->socket == NULL)
   {
      //Open a TCP socket
      webSocket->socket = socketOpen(SOCKET_TYPE_STREAM, SOCKET_IP_PROTO_TCP);
      //Failed to open socket?
      if(webSocket->socket == NULL)
         return ERROR_OPEN_FAILED;

      //Associate the WebSocket with the relevant interface
      error = socketBindToInterface(webSocket->socket, webSocket->interface);
      //Any error to report?
      if(error)
         return error;
   }

   //Set timeout
   error = socketSetTimeout(webSocket->socket, webSocket->timeout);
   //Any error to report?
   if(error)
      return error;

#if (WEB_SOCKET_TLS_SUPPORT == ENABLED)
      //Use SSL/TLS to secure the connection?
      if(webSocket->tlsInitCallback != NULL)
      {
         TlsConnectionEnd connectionEnd;

         //Allocate SSL/TLS context
         webSocket->tlsContext = tlsInit();
         //Failed to allocate SSL/TLS context?
         if(webSocket->tlsContext == NULL)
            return ERROR_OUT_OF_MEMORY;

         //Client or server operation?
         if(webSocket->endpoint == WS_ENDPOINT_CLIENT)
            connectionEnd = TLS_CONNECTION_END_CLIENT;
         else
            connectionEnd = TLS_CONNECTION_END_SERVER;

         //Select the relevant operation mode
         error = tlsSetConnectionEnd(webSocket->tlsContext, connectionEnd);
         //Any error to report?
         if(error)
            return error;

         //Bind TLS to the relevant socket
         error = tlsSetSocket(webSocket->tlsContext, webSocket->socket);
         //Any error to report?
         if(error)
            return error;

         //Restore SSL/TLS session, if any
         if(webSocket->tlsSession.idLength > 0)
         {
            //Restore SSL/TLS session
            error = tlsRestoreSession(webSocket->tlsContext, &webSocket->tlsSession);
            //Any error to report?
            if(error)
               return error;
         }

         //Invoke user-defined callback, if any
         if(webSocket->tlsInitCallback != NULL)
         {
            //Perform SSL/TLS related initialization
            error = webSocket->tlsInitCallback(webSocket, webSocket->tlsContext);
            //Any error to report?
            if(error)
               return error;
         }
      }
#endif

   //Successful initialization
   return NO_ERROR;
}


/**
 * @brief Establish network connection
 * @param[in] webSocket Handle to a WebSocket
 * @param[in] serverIpAddr IP address of the WebSocket server to connect to
 * @param[in] serverPort TCP port number that will be used to establish the
 *   connection
 * @return Error code
 **/

error_t webSocketEstablishConnection(WebSocket *webSocket,
   const IpAddr *serverIpAddr, uint16_t serverPort)
{
   error_t error;

   //Connect to WebSocket server
   error = socketConnect(webSocket->socket, serverIpAddr, serverPort);

#if (WEB_SOCKET_TLS_SUPPORT == ENABLED)
   //Use SSL/TLS to secure the connection?
   if(webSocket->tlsInitCallback != NULL)
   {
      //Check status code
      if(!error)
      {
         //Establish a SSL/TLS connection
         error = tlsConnect(webSocket->tlsContext);
      }
   }
#endif

   //Return status code
   return error;
}


/**
 * @brief Shutdown network connection
 * @param[in] webSocket Handle to a WebSocket
 * @return Error code
 **/

error_t webSocketShutdownConnection(WebSocket *webSocket)
{
   error_t error;
   size_t n;

   //Initialize status code
   error = NO_ERROR;

#if (WEB_SOCKET_TLS_SUPPORT == ENABLED)
   //Check whether a secure connection is being used
   if(webSocket->tlsContext != NULL)
   {
      //Shutdown SSL/TLS connection
      error = tlsShutdown(webSocket->tlsContext);
   }
#endif

   //Check status code
   if(!error)
   {
      //Further transmissions are disallowed
      error = socketShutdown(webSocket->socket, SOCKET_SD_SEND);
   }

   //Receive data until until the peer has also performed an orderly shutdown
   while(!error)
   {
      //Discard data
      error = socketReceive(webSocket->socket, webSocket->rxContext.buffer,
         WEB_SOCKET_BUFFER_SIZE, &n, 0);
   }

   //Check whether the connection has been properly closed
   if(error == ERROR_END_OF_STREAM)
      error = NO_ERROR;

   //Return status code
   return error;
}


/**
 * @brief Close network connection
 * @param[in] webSocket Handle to a WebSocket
 **/

void webSocketCloseConnection(WebSocket *webSocket)
{
#if (WEB_SOCKET_TLS_SUPPORT == ENABLED)
   //Check whether a secure connection is being used
   if(webSocket->tlsContext != NULL)
   {
      //Save SSL/TLS session
      tlsSaveSession(webSocket->tlsContext, &webSocket->tlsSession);

      //Release SSL/TLS context
      tlsFree(webSocket->tlsContext);
      webSocket->tlsContext = NULL;
   }
#endif

   //Close TCP connection
   if(webSocket->socket != NULL)
   {
      socketClose(webSocket->socket);
      webSocket->socket = NULL;
   }
}


/**
 * @brief Send data using the relevant transport protocol
 * @param[in] webSocket Handle to a WebSocket
 * @param[in] data Pointer to a buffer containing the data to be transmitted
 * @param[in] length Number of bytes to be transmitted
 * @param[out] written Actual number of bytes written (optional parameter)
 * @param[in] flags Set of flags that influences the behavior of this function
 * @return Error code
 **/

error_t webSocketSendData(WebSocket *webSocket, const void *data,
   size_t length, size_t *written, uint_t flags)
{
   error_t error;

#if (WEB_SOCKET_TLS_SUPPORT == ENABLED)
   //Check whether a secure connection is being used
   if(webSocket->tlsContext != NULL)
   {
      //Use SSL/TLS to transmit data to the client
      error = tlsWrite(webSocket->tlsContext, data, length, written, flags);
   }
   else
#endif
   {
      //Transmit data
      error = socketSend(webSocket->socket, data, length, written, flags);
   }

   //Return status code
   return error;
}


/**
 * @brief Receive data using the relevant transport protocol
 * @param[in] webSocket Handle to a WebSocket
 * @param[out] data Buffer into which received data will be placed
 * @param[in] size Maximum number of bytes that can be received
 * @param[out] received Number of bytes that have been received
 * @param[in] flags Set of flags that influences the behavior of this function
 * @return Error code
 **/

error_t webSocketReceiveData(WebSocket *webSocket, void *data,
   size_t size, size_t *received, uint_t flags)
{
   error_t error;

#if (WEB_SOCKET_TLS_SUPPORT == ENABLED)
   //Check whether a secure connection is being used
   if(webSocket->tlsContext != NULL)
   {
      //Use SSL/TLS to receive data from the client
      error = tlsRead(webSocket->tlsContext, data, size, received, flags);
   }
   else
#endif
   {
      //Receive data
      error = socketReceive(webSocket->socket, data, size, received, flags);
   }

   //Return status code
   return error;
}

#endif
