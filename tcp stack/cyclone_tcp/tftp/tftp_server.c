/**
 * @file tftp_server.c
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
 * @section Description
 *
 * TFTP is a very simple protocol used to transfer files. Refer to the
 * following RFCs for complete details:
 * - RFC 1123:  Requirements for Internet Hosts
 * - RFC 1350: The TFTP Protocol (Revision 2)
 * - RFC 1782: TFTP Option Extension
 * - RFC 1783: TFTP Blocksize Option
 * - RFC 1784: TFTP Timeout Interval and Transfer Size Options
 *
 * @author Oryx Embedded SARL (www.oryx-embedded.com)
 * @version 1.8.2
 **/

//Switch to the appropriate trace level
#define TRACE_LEVEL TFTP_TRACE_LEVEL

//Dependencies
#include "tftp/tftp_server.h"
#include "tftp/tftp_server_misc.h"
#include "debug.h"

//Check TCP/IP stack configuration
#if (TFTP_SERVER_SUPPORT == ENABLED)


/**
 * @brief Initialize settings with default values
 * @param[out] settings Structure that contains TFTP server settings
 **/

void tftpServerGetDefaultSettings(TftpServerSettings *settings)
{
   //The TFTP server is not bound to any interface
   settings->interface = NULL;

   //TFTP port number
   settings->port = TFTP_PORT;

   //Open file callback function
   settings->openFileCallback = NULL;
   //Write file callback function
   settings->writeFileCallback = NULL;
   //Read file callback function
   settings->readFileCallback = NULL;
   //Close file callback function
   settings->closeFileCallback = NULL;
}


/**
 * @brief TFTP server initialization
 * @param[in] context Pointer to the TFTP server context
 * @param[in] settings TFTP server specific settings
 * @return Error code
 **/

error_t tftpServerInit(TftpServerContext *context, const TftpServerSettings *settings)
{
   error_t error;

   //Debug message
   TRACE_INFO("Initializing TFTP server...\r\n");

   //Ensure the parameters are valid
   if(context == NULL || settings == NULL)
      return ERROR_INVALID_PARAMETER;

   //Clear the TFTP server context
   memset(context, 0, sizeof(TftpServerContext));

   //Save user settings
   context->settings = *settings;

   //Create an event object to poll the state of sockets
   if(!osCreateEvent(&context->event))
   {
      //Failed to create event
      return ERROR_OUT_OF_RESOURCES;
   }

   //Start of exception handling block
   do
   {
      //Open a UDP socket
      context->socket = socketOpen(SOCKET_TYPE_DGRAM, SOCKET_IP_PROTO_UDP);

      //Failed to open socket?
      if(context->socket == NULL)
      {
         //Report an error
         error = ERROR_OPEN_FAILED;
         //Exit immediately
         break;
      }

      //Associate the socket with the relevant interface
      error = socketBindToInterface(context->socket, settings->interface);
      //Unable to bind the socket to the desired interface?
      if(error)
         break;

      //The TFTP server listens for connection requests on port 69
      error = socketBind(context->socket, &IP_ADDR_ANY, settings->port);
      //Unable to bind the socket to the desired port?
      if(error)
         break;

      //End of exception handling block
   } while(0);

   //Did we encounter an error?
   if(error)
   {
      //Free previously allocated resources
      osDeleteEvent(&context->event);
      //Close socket
      socketClose(context->socket);
   }

   //Return status code
   return error;
}


/**
 * @brief Start TFTP server
 * @param[in] context Pointer to the TFTP server context
 * @return Error code
 **/

error_t tftpServerStart(TftpServerContext *context)
{
   OsTask *task;

   //Debug message
   TRACE_INFO("Starting TFTP server...\r\n");

   //Make sure the TFTP server context is valid
   if(context == NULL)
      return ERROR_INVALID_PARAMETER;

   //Create the TFTP server task
   task = osCreateTask("TFTP Server", (OsTaskCode) tftpServerTask,
      context, TFTP_SERVER_STACK_SIZE, TFTP_SERVER_PRIORITY);

   //Unable to create the task?
   if(task == OS_INVALID_HANDLE)
      return ERROR_OUT_OF_RESOURCES;

   //Successful processing
   return NO_ERROR;
}


/**
 * @brief TFTP server task
 * @param[in] context Pointer to the TFTP server context
 **/

void tftpServerTask(TftpServerContext *context)
{
   error_t error;
   uint_t i;
   TftpClientConnection *connection;

#if (NET_RTOS_SUPPORT == ENABLED)
   //Process events
   while(1)
   {
#endif
      //Clear event descriptor set
      memset(context->eventDesc, 0, sizeof(context->eventDesc));

      //Specify the events the application is interested in
      for(i = 0; i < TFTP_SERVER_MAX_CONNECTIONS; i++)
      {
         //Point to the structure describing the current connection
         connection = &context->connection[i];

         //Loop through active connections only
         if(connection->state != TFTP_STATE_CLOSED)
         {
            //Wait for a packet to be received
            context->eventDesc[i].socket = connection->socket;
            context->eventDesc[i].eventMask = SOCKET_EVENT_RX_READY;
         }
      }

      //The TFTP server listens for connection requests on port 69
      context->eventDesc[i].socket = context->socket;
      context->eventDesc[i].eventMask = SOCKET_EVENT_RX_READY;

      //Wait for one of the set of sockets to become ready to perform I/O
      error = socketPoll(context->eventDesc, TFTP_SERVER_MAX_CONNECTIONS + 1,
         &context->event, TFTP_SERVER_TICK_INTERVAL);

      //Verify status code
      if(!error)
      {
         //Event-driven processing
         for(i = 0; i < TFTP_SERVER_MAX_CONNECTIONS; i++)
         {
            //Point to the structure describing the current connection
            connection = &context->connection[i];

            //Loop through active connections only
            if(connection->state != TFTP_STATE_CLOSED)
            {
               //Check whether a packet has been received
               if(context->eventDesc[i].eventFlags & SOCKET_EVENT_RX_READY)
               {
                  //Process incoming packet
                  tftpServerProcessPacket(context, connection);
               }
            }
         }

         //Any connection request received on port 69?
         if(context->eventDesc[i].eventFlags & SOCKET_EVENT_RX_READY)
         {
            //Accept connection request
            tftpServerAcceptRequest(context);
         }
      }

      //Handle periodic operations
      tftpServerTick(context);

#if (NET_RTOS_SUPPORT == ENABLED)
   }
#endif
}

#endif
