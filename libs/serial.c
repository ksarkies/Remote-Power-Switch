/*  Serial routines for the AVR UART */

/****************************************************************************
 *   Copyright (C) 2013 by Ken Sarkies ksarkies@internode.on.net            *
 *                                                                          *
 *   This file is part of XBee-Acquisition                                  *
 *                                                                          *
 * Licensed under the Apache License, Version 2.0 (the "License");          *
 * you may not use this file except in compliance with the License.         *
 * You may obtain a copy of the License at                                  *
 *                                                                          *
 *     http://www.apache.org/licenses/LICENSE-2.0                           *
 *                                                                          *
 * Unless required by applicable law or agreed to in writing, software      *
 * distributed under the License is distributed on an "AS IS" BASIS,        *
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. *
 * See the License for the specific language governing permissions and      *
 * limitations under the License.                                           *
 ***************************************************************************/

#include <avr/io.h>
#include "defines-M48.h"
#include "serial.h"

/* Convenience macros (we don't use them all) */
#define  _BV(bit) (1 << (bit))
#define  inb(sfr) _SFR_BYTE(sfr)
#define  inw(sfr) _SFR_WORD(sfr)
#define  outb(sfr, val) (_SFR_BYTE(sfr) = (val))
#define  outw(sfr, val) (_SFR_WORD(sfr) = (val))
#define  cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#define  sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#define  high(x) ((uint8_t) (x >> 8) & 0xFF)
#define  low(x) ((uint8_t) (x & 0xFF))

/*-----------------------------------------------------------------------------*/
/* Initialise the UART, setting baudrate, Rx/Tx enables, and flow controls

Baud rate is derived from the header call to setbaud.h.
UBRRL_VALUE and UBRRH_VALUE and USE_2X are returned, the latter requires the
U2X bit to be set in UCSRA to force a double baud rate clock.
*/

void uartInit(void)
{
    BAUD_RATE_LOW_REG = UBRRL_VALUE;
    BAUD_RATE_HIGH_REG = UBRRH_VALUE;
#if USE_2X
    sbi(UART_STATUS_REG,DOUBLE_RATE);
#else
    cbi(UART_STATUS_REG,DOUBLE_RATE);
#endif
    UART_FORMAT_REG = (3 << FRAME_SIZE);                // Set 8 bit frames
    UART_CONTROL_REG |= _BV(ENABLE_RECEIVER_BIT) |
                        _BV(ENABLE_TRANSMITTER_BIT);    // enable receive and transmit 
#ifdef USE_HARDWARE_FLOW
    cbi(UART_CTS_PORT_DIR,UART_CTS_PIN);                // Set flow control pins CTS input
    sbi(UART_RTS_PORT_DIR,UART_RTS_PIN);                // RTS output
    cbi(UART_RTS_PORT,UART_RTS_PIN);                    // RTS cleared to enable
#endif
}

/*-----------------------------------------------------------------------------*/
/* Send a character when the Tx is ready

The function waits until CTS is asserted low then waits until the UART indicates
that the character has been sent.
*/

void sendch(unsigned char c)
{
#ifdef USE_HARDWARE_FLOW
        while (inb(UART_CTS_PORT) & _BV(UART_CTS_PIN));     // wait for clear to send
#endif
        UART_DATA_REG = c;                                  // send
        while (!(UART_STATUS_REG & _BV(TRANSMIT_COMPLETE_BIT)));    // wait till gone
        UART_STATUS_REG |= _BV(TRANSMIT_COMPLETE_BIT);      // reset TXCflag
}

/*-----------------------------------------------------------------------------*/
/* Get a character when the Rx is ready (blocking)

The function asserts RTS low then waits for the receive complete bit is set.
RTS is then cleared high. The character is then retrieved.
*/

unsigned char getch(void)
{
#ifdef USE_HARDWARE_FLOW
    cbi(UART_RTS_PORT,UART_RTS_PIN);                        // Enable RTS
#endif
    while (!(UART_STATUS_REG & _BV(RECEIVE_COMPLETE_BIT)));
#ifdef USE_HARDWARE_FLOW
    sbi(UART_RTS_PORT,UART_RTS_PIN);                        // Disable RTS
#endif
    return UART_DATA_REG;
}

