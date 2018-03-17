/**
 * @file debug.c
 * @brief Debugging facilities
 *
 * @section License
 *
 * Copyright (C) 2010-2016 Oryx Embedded SARL. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 * @author Oryx Embedded SARL (www.oryx-embedded.com)
 * @version 1.7.4
 **/

//Dependencies
#include "mk66f18.h"
#include "debug.h"


/**
 * @brief Debug UART initialization
 * @param[in] baudrate UART baudrate
 **/

/**
 * @brief Debug UART initialization
 * @param[in] baudrate UART baudrate
 **/

void debugInit(uint32_t baudrate)
{
   uint32_t div;
   uint32_t moduleClockFreq;

   //Clock 1 output divider value (system clock)
//   div = (SIM->CLKDIV1 & SIM_CLKDIV1_OUTDIV1_MASK) >> SIM_CLKDIV1_OUTDIV1_SHIFT;
   //UART0 module clock frequency
   moduleClockFreq = SystemCoreClock / 3;// (div + 1);

   //Compute the UART baudrate prescaler
   div = ((2 * moduleClockFreq) + (baudrate / 2)) / baudrate;

   //Enable PORTB peripheral clock
   SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;
   //Enable UART0 peripheral clock
   SIM->SCGC1 |= SIM_SCGC1_UART4_MASK;

   //Configure PTB16 (UART0_RX) and PTB17 (UART0_TX)
   PORTC->PCR[14] = PORT_PCR_MUX(3);
   PORTC->PCR[15] = PORT_PCR_MUX(3);

   //LSB is transmitted first
   UART4->S2 = 0;

   //8-bit mode, no parity
   UART4->C1 = 0;
   //Transmit data is not inverted
   UART4->C3 = 0;
   //Baud rate fine ajust
   UART4->C4 = UART_C4_BRFA(div & 0x1F);

   //Select baudrate
   UART4->BDH = UART_BDH_SBR((div >> 13) & 0x1F);
   UART4->BDL = UART_BDL_SBR((div >> 5) & 0xFF);

   //Transmit FIFO watermark
   UART4->TWFIFO = UART_TWFIFO_TXWATER(0);
   //Receive FIFO watermark
   UART4->RWFIFO = UART_RWFIFO_RXWATER(1);

   //Enable transmitter and receiver
   UART4->C2 = UART_C2_TE_MASK | UART_C2_RE_MASK;
}


/**
 * @brief Display the contents of an array
 * @param[in] stream Pointer to a FILE object that identifies an output stream
 * @param[in] prepend String to prepend to the left of each line
 * @param[in] data Pointer to the data array
 * @param[in] length Number of bytes to display
 **/

void debugDisplayArray(FILE *stream,
   const char_t *prepend, const void *data, size_t length)
{
   uint_t i;

   for(i = 0; i < length; i++)
   {
      //Beginning of a new line?
      if((i % 16) == 0)
         fprintf(stream, "%s", prepend);
      //Display current data byte
      fprintf(stream, "%02" PRIX8 " ", *((uint8_t *) data + i));
      //End of current line?
      if((i % 16) == 15 || i == (length - 1))
         fprintf(stream, "\r\n");
   }
}


/**
 * @brief Write character to stream
 * @param[in] c The character to be written
 * @param[in] stream Pointer to a FILE object that identifies an output stream
 * @return On success, the character written is returned. If a writing
 *   error occurs, EOF is returned
 **/

int_t fputc(int_t c, FILE *stream)
{
   //Standard output or error output?
   if(stream == stdout || stream == stderr)
   {
      //Wait for the transmitter to be ready
      while(!(UART4->S1 & UART_S1_TDRE_MASK));
      //Send character
      UART4->D = c;
      //Wait for the transfer to complete
      while(!(UART4->S1 & UART_S1_TDRE_MASK));

      //On success, the character written is returned
      return c;
   }
   //Unknown output?
   else
   {
      //If a writing error occurs, EOF is returned
      return EOF;
   }
}
