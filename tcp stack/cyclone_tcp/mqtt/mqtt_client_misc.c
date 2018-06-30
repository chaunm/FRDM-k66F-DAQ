/**
 * @file mqtt_client_misc.c
 * @brief Helper functions for MQTT client
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
#define TRACE_LEVEL MQTT_TRACE_LEVEL

//Dependencies
#include "core/net.h"
#include "mqtt/mqtt_client.h"
#include "mqtt/mqtt_client_packet.h"
#include "mqtt/mqtt_client_transport.h"
#include "mqtt/mqtt_client_misc.h"
#include "debug.h"

//Check TCP/IP stack configuration
#if (MQTT_CLIENT_SUPPORT == ENABLED)


/**
 * @brief Update MQTT client state
 * @param[in] context Pointer to the MQTT client context
 * @param[in] newState New state to switch to
 **/

void mqttClientChangeState(MqttClientContext *context, MqttClientState newState)
{
   //Switch to the new state
   context->state = newState;
}


/**
 * @brief Check keep-alive time interval
 * @param[in] context Pointer to the MQTT client context
 * @return Error code
 **/

error_t mqttClientCheckKeepAlive(MqttClientContext *context)
{
   error_t error;
   systime_t time;
   systime_t keepAlive;

   //Initialize status code
   error = NO_ERROR;

   //In the absence of sending any other control packets, the client must
   //send a PINGREQ packet
   if(context->state == MQTT_CLIENT_STATE_IDLE ||
      context->state == MQTT_CLIENT_STATE_PACKET_SENT)
   {
      //A keep-alive value of zero has the effect of turning off the keep
      //alive mechanism
      if(context->settings.keepAlive != 0)
      {
         //Get current time
         time = osGetSystemTime();

         //Convert the keep-alive value to milliseconds
         keepAlive = context->settings.keepAlive * 1000;

         //It is the responsibility of the client to ensure that the interval
         //between control packets being sent does not exceed the keep-alive value
         if(timeCompare(time, context->keepAliveTimestamp + keepAlive) >= 0)
         {
            //Format PINGREQ packet
            error = mqttClientFormatPingReq(context);

            //Check status code
            if(!error)
            {
               //Debug message
               TRACE_INFO("MQTT: Sending PINGREQ packet (%" PRIuSIZE " bytes)...\r\n", context->packetLen);
               TRACE_DEBUG_ARRAY("  ", context->packet, context->packetLen);

               //Point to the beginning of the packet
               context->packetPos = 0;

               //Send PINGREQ packet
               mqttClientChangeState(context, MQTT_CLIENT_STATE_SENDING_PACKET);
            }
         }
      }
   }

   //Return status code
   return error;
}


/**
 * @brief Serialize fixed header
 * @param[in] buffer Pointer to the output buffer
 * @param[in,out] pos Current position
 * @param[in] type MQTT control packet type
 * @param[in] dup DUP flag
 * @param[in] qos QoS field
 * @param[in] retain RETAIN flag
 * @param[in] remainingLen Length of the variable header and the payload
 * @return Error code
 **/

error_t mqttSerializeHeader(uint8_t *buffer, size_t *pos, MqttPacketType type,
   bool_t dup, MqttQosLevel qos, bool_t retain, size_t remainingLen)
{
   uint_t i;
   uint_t k;
   size_t n;
   MqttPacketHeader *header;

   //Point to the current position
   n = *pos;

   //The Remaining Length is encoded using a variable length encoding scheme
   if(remainingLen < 128)
      k = 1;
   else if(remainingLen < 16384)
      k = 2;
   else if(remainingLen < 2097152)
      k = 3;
   else if(remainingLen < 268435456)
      k = 4;
   else
      return ERROR_INVALID_LENGTH;

   //Sanity check
   if(n < (sizeof(MqttPacketHeader) + k))
      return ERROR_BUFFER_OVERFLOW;

   //Position where to format the header
   n -= sizeof(MqttPacketHeader) + k;

   //Point to the MQTT packet header
   header = (MqttPacketHeader *) (buffer + n);

   //Encode the first byte of the header
   header->type = type;
   header->dup = dup;
   header->qos = qos;
   header->retain = retain;

   //Encode the Remaining Length field
   for(i = 0; i < k; i++)
   {
      //The least significant seven bits of each byte encode the data
      header->length[i] = remainingLen & 0xFF;
      remainingLen >>= 7;

      //The most significant bit is used to indicate that there are
      //following bytes in the representation
      if(remainingLen > 0)
         header->length[i] |= 0x80;
   }

   //Update current position
   *pos = n;

   //Successful processing
   return NO_ERROR;
}


/**
 * @brief Write a 8-bit integer to the output buffer
 * @param[in] buffer Pointer to the output buffer
 * @param[in] bufferLen Maximum number of bytes the output buffer can hold
 * @param[in,out] pos Current position
 * @param[in] value 8-bit integer to be serialized
 * @return Error code
 **/

error_t mqttSerializeByte(uint8_t *buffer, size_t bufferLen,
   size_t *pos, uint8_t value)
{
   size_t n;

   //Point to the current position
   n = *pos;

   //Make sure the output buffer is large enough
   if((n + sizeof(uint8_t)) > bufferLen)
      return ERROR_BUFFER_OVERFLOW;

   //Write the byte to the output buffer
   buffer[n++] = value;

   //Advance current position
   *pos = n;

   //Successful processing
   return NO_ERROR;
}


/**
 * @brief Write a 16-bit integer to the output buffer
 * @param[in] buffer Pointer to the output buffer
 * @param[in] bufferLen Maximum number of bytes the output buffer can hold
 * @param[in,out] pos Current position
 * @param[in] value 16-bit integer to be serialized
 * @return Error code
 **/

error_t mqttSerializeShort(uint8_t *buffer, size_t bufferLen,
   size_t *pos, uint16_t value)
{
   size_t n;

   //Point to the current position
   n = *pos;

   //Make sure the output buffer is large enough
   if((n + sizeof(uint16_t)) > bufferLen)
      return ERROR_BUFFER_OVERFLOW;

   //Write the short integer to the output buffer
   buffer[n++] = MSB(value);
   buffer[n++] = LSB(value);

   //Advance current position
   *pos = n;

   //Successful processing
   return NO_ERROR;
}


/**
 * @brief Serialize string
 * @param[in] buffer Pointer to the output buffer
 * @param[in] bufferLen Maximum number of bytes the output buffer can hold
 * @param[in,out] pos Current position
 * @param[in] string Pointer to the string to be serialized
 * @param[in] stringLen Length of the string, in bytes
 * @return Error code
 **/

error_t mqttSerializeString(uint8_t *buffer, size_t bufferLen,
   size_t *pos, const void *string, size_t stringLen)
{
   size_t n;

   //Point to the current position
   n = *pos;

   //Make sure the output buffer is large enough to hold the string
   if((n + sizeof(uint16_t) + stringLen) > bufferLen)
      return ERROR_BUFFER_OVERFLOW;

   //Encode the length field
   buffer[n++] = MSB(stringLen);
   buffer[n++] = LSB(stringLen);

   //Write the string to the output buffer
   memcpy(buffer + n, string, stringLen);

   //Advance current position
   *pos = n + stringLen;

   //Successful processing
   return NO_ERROR;
}


/**
 * @brief Serialize raw data
 * @param[in] buffer Pointer to the output buffer
 * @param[in] bufferLen Maximum number of bytes the output buffer can hold
 * @param[in,out] pos Current position
 * @param[in] data Pointer to the raw data to be serialized
 * @param[in] dataLen Length of the raw data, in bytes
 * @return Error code
 **/

error_t mqttSerializeData(uint8_t *buffer, size_t bufferLen,
   size_t *pos, const void *data, size_t dataLen)
{
   size_t n;

   //Point to the current position
   n = *pos;

   //Make sure the output buffer is large enough to hold the data
   if((n + dataLen) > bufferLen)
      return ERROR_BUFFER_OVERFLOW;

   //Write the data to the output buffer
   memcpy(buffer + n, data, dataLen);

   //Advance current position
   *pos = n + dataLen;

   //Successful processing
   return NO_ERROR;
}


/**
 * @brief Deserialize fixed header
 * @param[in] buffer Pointer to the input buffer
 * @param[in] bufferLen Length of the input buffer
 * @param[in,out] pos Current position
 * @param[out] type MQTT control packet type
 * @param[out] dup DUP flag from the fixed header
 * @param[out] qos QoS field from the fixed header
 * @param[out] retain RETAIN flag from the fixed header
 * @param[out] remainingLen Length of the variable header and the payload
 * @return Error code
 **/

error_t mqttDeserializeHeader(uint8_t *buffer, size_t bufferLen, size_t *pos,
   MqttPacketType *type, bool_t *dup, MqttQosLevel *qos, bool_t *retain, size_t *remainingLen)
{
   uint_t i;
   size_t n;
   MqttPacketHeader *header;

   //Point to the current position
   n = *pos;

   //Make sure the input buffer is large enough
   if((n + sizeof(MqttPacketHeader)) > bufferLen)
      return ERROR_INVALID_LENGTH;

   //Point to the MQTT packet header
   header = (MqttPacketHeader *) (buffer + n);

   //Save MQTT control packet type
   *type = (MqttPacketType) header->type;

   //Save flags
   *dup = header->dup;
   *qos = (MqttQosLevel) header->qos;
   *retain = header->retain;

   //Advance current position
   n += sizeof(MqttPacketHeader);

   //Prepare to decode the Remaining Length field
   *remainingLen = 0;

   //The Remaining Length is encoded using a variable length encoding scheme
   for(i = 0; i < 4; i++)
   {
      //Sanity check
      if((n + sizeof(uint8_t)) > bufferLen)
         return ERROR_INVALID_LENGTH;

      //Advance current position
      n += sizeof(uint8_t);

      //The most significant bit is used to indicate that there are
      //following bytes in the representation
      if(header->length[i] & 0x80)
      {
         //Applications can send control packets of size up to 256 MB
         if(i == 3)
            return ERROR_INVALID_SYNTAX;

         //The least significant seven bits of each byte encode the data
         *remainingLen |= (header->length[i] & 0x7F) << (7 * i);
      }
      else
      {
         //The least significant seven bits of each byte encode the data
         *remainingLen |= header->length[i] << (7 * i);
         //This is the last byte
         break;
      }
   }

   //Return the current position
   *pos = n;

   //Successful processing
   return NO_ERROR;
}


/**
 * @brief Read a 8-bit integer from the input buffer
 * @param[in] buffer Pointer to the input buffer
 * @param[in] bufferLen Length of the input buffer
 * @param[in,out] pos Current position
 * @param[out] value Value of the 8-bit integer
 * @return Error code
 **/

error_t mqttDeserializeByte(uint8_t *buffer, size_t bufferLen,
   size_t *pos, uint8_t *value)
{
   size_t n;

   //Point to the current position
   n = *pos;

   //Make sure the input buffer is large enough
   if((n + sizeof(uint8_t)) > bufferLen)
      return ERROR_BUFFER_OVERFLOW;

   //Read the short integer from the input buffer
   *value = buffer[n];

   //Advance current position
   *pos = n + sizeof(uint8_t);

   //Successful processing
   return NO_ERROR;
}


/**
 * @brief Read a 16-bit integer from the input buffer
 * @param[in] buffer Pointer to the input buffer
 * @param[in] bufferLen Length of the input buffer
 * @param[in,out] pos Current position
 * @param[out] value Value of the 16-bit integer
 * @return Error code
 **/

error_t mqttDeserializeShort(uint8_t *buffer, size_t bufferLen,
   size_t *pos, uint16_t *value)
{
   size_t n;

   //Point to the current position
   n = *pos;

   //Make sure the input buffer is large enough
   if((n + sizeof(uint16_t)) > bufferLen)
      return ERROR_BUFFER_OVERFLOW;

   //Read the short integer from the input buffer
   *value = (buffer[n] << 8) | buffer[n + 1];

   //Advance current position
   *pos = n + sizeof(uint16_t);

   //Successful processing
   return NO_ERROR;
}


/**
 * @brief Deserialize string
 * @param[in] buffer Pointer to the input buffer
 * @param[in] bufferLen Length of the input buffer
 * @param[in,out] pos Current position
 * @param[out] string Pointer to the string
 * @param[out] stringLen Length of the string, in bytes
 * @return Error code
 **/

error_t mqttDeserializeString(uint8_t *buffer, size_t bufferLen,
   size_t *pos, char_t **string, size_t *stringLen)
{
   size_t n;

   //Point to the current position
   n = *pos;

   //Make sure the input buffer is large enough
   if((n + sizeof(uint16_t)) > bufferLen)
      return ERROR_BUFFER_OVERFLOW;

   //Decode the length field
   *stringLen = (buffer[n] << 8) | buffer[n + 1];

   //Make sure the input buffer is large enough
   if((n + sizeof(uint16_t) + *stringLen) > bufferLen)
      return ERROR_BUFFER_OVERFLOW;

   //Read the string from the input buffer
   *string = (char_t *) buffer + n + 2;

   //Advance current position
   *pos = n + 2 + *stringLen;

   //Successful processing
   return NO_ERROR;
}

#endif
