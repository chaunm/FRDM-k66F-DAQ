/**
 * @file tftp_client.c
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
#include "tftp/tftp_client.h"
#include "tftp/tftp_client_misc.h"
#include "debug.h"

//Check TCP/IP stack configuration
#if (TFTP_CLIENT_SUPPORT == ENABLED)


/**
 * @brief TFTP client initialization
 * @param[in] context Pointer to the TFTP client context
 **/

void tftpClientInit(TftpClientContext *context)
{
   //Sanity check
   if(context != NULL)
   {
      //Initialize context
      memset(context, 0, sizeof(TftpClientContext));

      //Initialize state machine
      context->state = TFTP_CLIENT_STATE_CLOSED;
   }
}


/**
 * @brief Bind the TFTP client to a particular network interface
 * @param[in] context Pointer to the TFTP client context
 * @param[in] interface Network interface to be used
 * @return Error code
 **/

error_t tftpClientBindToInterface(TftpClientContext *context,
   NetInterface *interface)
{
   //Make sure the TFTP client context is valid
   if(context == NULL)
      return ERROR_INVALID_PARAMETER;

   //Explicitly associate the TFTP client with the specified interface
   context->interface = interface;

   //Successful processing
   return NO_ERROR;
}


/**
 * @brief Establish connection with the TFTP server
 * @param[in] context Pointer to the TFTP client context
 * @param[in] serverIpAddr IP address of the TFTP server to connect to
 * @param[in] serverPort UDP port number
 * @return Error code
 **/

error_t tftpClientConnect(TftpClientContext *context,
   const IpAddr *serverIpAddr, uint16_t serverPort)
{
   //Check parameters
   if(context == NULL || serverIpAddr == NULL)
      return ERROR_INVALID_PARAMETER;

   //Check current state
   if(context->state != TFTP_CLIENT_STATE_CLOSED)
      return ERROR_WRONG_STATE;

   //Save the IP address of the remote TFTP server
   context->serverIpAddr = *serverIpAddr;
   //Save the UDP port number to be used
   context->serverPort = serverPort;

   //Successful processing
   return NO_ERROR;
}


/**
 * @brief Open a file for reading or writing
 * @param[in] context Pointer to the TFTP client context
 * @param[in] filename NULL-terminated string specifying the filename
 * @param[in] mode File access mode
 * @return Error code
 **/

error_t tftpClientOpenFile(TftpClientContext *context,
   const char_t *filename, uint_t mode)
{
   error_t error;

   //Check parameters
   if(context == NULL || filename == NULL)
      return ERROR_INVALID_PARAMETER;

   //Initialize status code
   error = NO_ERROR;

   //Read or write access?
   if(mode & TFTP_FILE_MODE_WRITE)
   {
      //Wait for the WRQ request to be accepted
      while(!error)
      {
         //Check current state
         if(context->state == TFTP_CLIENT_STATE_CLOSED)
         {
            //Open connection with the remote TFTP server
            error = tftpClientOpenConnection(context);

            //Check status code
            if(!error)
            {
               //Send WRQ packet
               if(mode & TFTP_FILE_MODE_NETASCII)
                  tftpClientSendWrqPacket(context, filename, "netascii");
               else
                  tftpClientSendWrqPacket(context, filename, "octet");

               //Initialize block number
               context->block = 0;
               //The write request has been sent out
               context->state = TFTP_CLIENT_STATE_WRQ;
            }
         }
         else if(context->state == TFTP_CLIENT_STATE_WRQ)
         {
            //Wait for an ACK packet to be received
            error = tftpClientProcessEvents(context);
         }
         else if(context->state == TFTP_CLIENT_STATE_ACK)
         {
            //The write request has been accepted
            error = NO_ERROR;
            //We are done
            break;
         }
         else
         {
            //Close connection
            tftpClientCloseConnection(context);
            //Back to default state
            context->state = TFTP_CLIENT_STATE_CLOSED;
            //The write request has been rejected
            error = ERROR_OPEN_FAILED;
         }
      }
   }
   else
   {
      //Wait for the RRQ request to be accepted
      while(!error)
      {
         //Check current state
         if(context->state == TFTP_CLIENT_STATE_CLOSED)
         {
            //Open connection with the remote TFTP server
            error = tftpClientOpenConnection(context);

            //Check status code
            if(!error)
            {
               //Send RRQ packet
               if(mode & TFTP_FILE_MODE_NETASCII)
                  tftpClientSendRrqPacket(context, filename, "netascii");
               else
                  tftpClientSendRrqPacket(context, filename, "octet");

               //Initialize block number
               context->block = 1;
               //The read request has been sent out
               context->state = TFTP_CLIENT_STATE_RRQ;
            }
         }
         else if(context->state == TFTP_CLIENT_STATE_RRQ)
         {
            //Wait for a DATA packet to be received
            error = tftpClientProcessEvents(context);
         }
         else if(context->state == TFTP_CLIENT_STATE_DATA)
         {
            //The read request has been accepted
            error = NO_ERROR;
            //We are done
            break;
         }
         else
         {
            //Close connection
            tftpClientCloseConnection(context);
            //Back to default state
            context->state = TFTP_CLIENT_STATE_CLOSED;
            //The read request has been rejected
            error = ERROR_OPEN_FAILED;
         }
      }
   }

   //Return status code
   return error;
}


/**
 * @brief Write data to the file
 * @param[in] context Pointer to the TFTP client context
 * @param[in] data Pointer to a buffer containing the data to be written
 * @param[in] length Number of data bytes to write
 * @param[in] written Number of bytes that have been written
 * @param[in] flags Reserved parameter
 * @return Error code
 **/

error_t tftpClientWriteFile(TftpClientContext *context,
   const void *data, size_t length, size_t *written, uint_t flags)
{
   error_t error;
   size_t n;
   size_t totalLength;

   //Initialize status code
   error = NO_ERROR;

   //Total number of bytes that have been written
   totalLength = 0;

   //Write as much data as possible
   while(totalLength < length)
   {
      //Check current state
      if(context->state == TFTP_CLIENT_STATE_DATA)
      {
         //Handle retransmissions
         error = tftpClientProcessEvents(context);
      }
      else if(context->state == TFTP_CLIENT_STATE_ACK)
      {
         //Send buffer available for writing?
         if(context->outDataLen < TFTP_CLIENT_BLOCK_SIZE)
         {
            //Compute the number of bytes available
            n = TFTP_CLIENT_BLOCK_SIZE - context->outDataLen;
            //Limit the number of bytes to copy at a time
            n = MIN(n, length - totalLength);

            //Copy data to the send buffer
            memcpy(context->outPacket + sizeof(TftpDataPacket) +
               context->outDataLen, data, n);

            //Advance data pointer
            data = (uint8_t *) data + n;
            context->outDataLen += n;

            //Total number of bytes that have been written
            totalLength += n;
         }

         //Check whether the send buffer is full
         if(context->outDataLen >= TFTP_CLIENT_BLOCK_SIZE)
         {
            //The block number increases by one for each new block of data
            context->block++;

            //Send DATA packet
            tftpClientSendDataPacket(context);

            //Wait for the DATA packet to be acknowledged
            context->state = TFTP_CLIENT_STATE_DATA;
         }
      }
      else
      {
         //Report an error
         error = ERROR_WRITE_FAILED;
      }

      //Any error to report?
      if(error)
         break;
   }

   //Total number of bytes successfully written
   if(written != NULL)
      *written = totalLength;

   //Return status code
   return error;
}


/**
 * @brief Read data from the file
 * @param[in] context Pointer to the TFTP client context
 * @param[in] data Pointer to the buffer where to copy the data
 * @param[in] size Size of the buffer, in bytes
 * @param[out] received Number of data bytes that have been read
 * @param[in] flags Reserved parameter
 * @return Error code
 **/

error_t tftpClientReadFile(TftpClientContext *context,
   void *data, size_t size, size_t *received, uint_t flags)
{
   error_t error;
   size_t n;

   //Initialize status code
   error = NO_ERROR;

   //Total number of bytes that have been received
   *received = 0;

   //Read as much data as possible
   while(*received < size)
   {
      //Check current state
      if(context->state == TFTP_CLIENT_STATE_DATA)
      {
         //Any data pending in the receive buffer?
         if(context->inDataPos < context->inDataLen)
         {
            //Compute the number of bytes available for reading
            n = context->inDataLen - context->inDataPos;
            //Limit the number of bytes to copy at a time
            n = MIN(n, size - *received);

            //Copy data from the receive buffer
            memcpy(data, context->inPacket + sizeof(TftpDataPacket) +
               context->inDataPos, n);

            //Advance data pointer
            data = (uint8_t *) data + n;
            context->inDataPos += n;

            //Total number of bytes that have been received
            *received += n;
         }

         //Check whether the receive buffer is empty
         if(context->inDataPos >= context->inDataLen)
         {
            //Acknowledge the DATA packet
            tftpClientSendAckPacket(context);

            //Increment block number
            context->block++;

            //Check the length of the DATA packet
            if(context->inDataLen < TFTP_CLIENT_BLOCK_SIZE)
            {
               //A data packet of less than 512 bytes signals termination
               //of the transfer
               context->state = TFTP_CLIENT_STATE_COMPLETE;
            }
            else
            {
               //Wait for the next DATA packet to be received
               context->state = TFTP_CLIENT_STATE_ACK;
            }
         }
      }
      else if(context->state == TFTP_CLIENT_STATE_ACK)
      {
         //Handle retransmissions
         error = tftpClientProcessEvents(context);
      }
      else if(context->state == TFTP_CLIENT_STATE_COMPLETE)
      {
         //The user must be satisfied with data already on hand
         if(*received > 0)
         {
            //Some data are pending in the receive buffer
            error = NO_ERROR;
            break;
         }
         else
         {
            //Normal termination of the transfer
            error = ERROR_END_OF_STREAM;
         }
      }
      else
      {
         //Report an error
         error = ERROR_READ_FAILED;
      }

      //Any error to report?
      if(error)
         break;
   }

   //Return status code
   return error;
}


/**
 * @brief Flush pending write operations
 * @param[in] context Pointer to the TFTP client context
 * @return Error code
 **/

error_t tftpClientFlushFile(TftpClientContext *context)
{
   error_t error;

   //Initialize status code
   error = NO_ERROR;

   //Wait for the last DATA packet to be acknowledged
   while(!error)
   {
      //Check current state
      if(context->state == TFTP_CLIENT_STATE_DATA)
      {
         //Handle retransmissions
         error = tftpClientProcessEvents(context);
      }
      else if(context->state == TFTP_CLIENT_STATE_ACK)
      {
         //The block number increases by one for each new block of data
         context->block++;

         //Send DATA packet
         tftpClientSendDataPacket(context);

         //A data packet of less than 512 bytes signals termination
         //of the transfer
         context->state = TFTP_CLIENT_STATE_LAST_DATA;
      }
      else if(context->state == TFTP_CLIENT_STATE_LAST_DATA)
      {
         //Handle retransmissions
         error = tftpClientProcessEvents(context);
      }
      else if(context->state == TFTP_CLIENT_STATE_COMPLETE)
      {
         //Normal termination of the transfer
         error = NO_ERROR;
         break;
      }
      else
      {
         //Report an error
         error = ERROR_WRITE_FAILED;
      }
   }

   //Return status code
   return error;
}


/**
 * @brief Close the file
 * @param[in] context Pointer to the TFTP client context
 **/

void tftpClientCloseFile(TftpClientContext *context)
{
   //Close connection with the TFTP server
   tftpClientCloseConnection(context);

   //Back to default state
   context->state = TFTP_CLIENT_STATE_CLOSED;
}

#endif
