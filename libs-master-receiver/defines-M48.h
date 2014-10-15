/* AVR Defines for USART library

This file assigns registers, particular to an AVR type, to common constants.
It is valid for bootloader and application firmware.

I/O pin values for controlling the bootloader operation are given at the end.

Software: AVR-GCC 4.8.2
Target:   Any AVR with sufficient output ports and a timer
Tested:   ATMega48 series at 8MHz internal clock.
*/

/****************************************************************************
 *   Copyright (C) 2013 by Ken Sarkies ksarkies@internode.on.net            *
 *                                                                          *
 *   This file is part of Remote-Power-Switch                                  *
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

#ifndef _DEFINES_M48_H_
#define _DEFINES_M48_H_

#include	<avr/io.h>

/* Choose whether to use hardware flow control for serial comms. */
//#define USE_HARDWARE_FLOW

/* These are the defines for the selected device */
#define BAUD                115200

// Simple serial I/O (must define cpu frequency and baudrate before this include) */
#include <util/setbaud.h>

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

/* UART Error Definitions. */
#define NO_DATA                 0x01
#define BUFFER_OVERFLOW         0x02
#define OVERRUN_ERROR           0x04
#define FRAME_ERROR             0x08
#define STATE_MACHINE           0x10
#define CHECKSUM                0x11

/* definitions for UART control */
#define	BAUD_RATE_HIGH_REG	    UBRR0H
#define	BAUD_RATE_LOW_REG	    UBRR0L
#define	UART_CONTROL_REG	    UCSR0B
#define	UART_FORMAT_REG	        UCSR0C
#define FRAME_SIZE              UCSZ00
#define	ENABLE_TRANSMITTER_BIT	TXEN0
#define	ENABLE_RECEIVER_BIT	    RXEN0
#define	UART_STATUS_REG	        UCSR0A
#define	TRANSMIT_COMPLETE_BIT	TXC0
#define	RECEIVE_COMPLETE_BIT	RXC0
#define	UART_DATA_REG	        UDR0
#define DOUBLE_RATE             U2X0

/* UART Flow control ports */
#define UART_CTS_PORT           PIND
#define UART_CTS_PORT_DIR       DDRD
#define UART_CTS_PIN            2
#define UART_RTS_PORT           PORTD
#define UART_RTS_PORT_DIR       DDRD
#define UART_RTS_PIN            3

#endif

