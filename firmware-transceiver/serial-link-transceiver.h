/*****************************************************************************
*        Power Control Project Remote Unit
*       Ken Sarkies ksarkies@internode.on.net
*
* File              : serial-link-transceiver.h
* Compiler          : AVR-GCC/avr-libc(>= 1.2.5)
* Revision          : $Revision: 0.0 $
* Updated by        : $ author K. Sarkies 09/10/2014 $
*
* Target platform   : ATMega48
*
* HopeRf RFM12B FSK Wireless Module Library
*
**********************************************************/
/***************************************************************************
 *   Copyright (C) 2014 by Ken Sarkies                                     *
 *   ksarkies@internode.on.net                                             *
 *                                                                         *
 *   This file is part of Power Controller                                 *
 *                                                                         *
 *   This is free software; you can redistribute it and/or modify          *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This software is distributed in the hope that it will be useful,      *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with the software. If not, write to the:                        *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA.             *
 ***************************************************************************/
#ifndef _SERIAL_LINK_TRANSCEIVER_H_
#define _SERIAL_LINK_TRANSCEIVER_H_

#define MAX_MESSAGE         64
#define TIMEOUT             10

/* Baud rate of the external serial interface */
#define BAUD                38400
/* Buffer size for serial communications.
Set to maximum (power of 2 necessary depending on the processor SRAM. */
#if defined(__AVR_ATmega48__)
#warning "MCU atmega48"
#define UART_RX0_BUFFER_SIZE    128
#define UART_TX0_BUFFER_SIZE    128
#elif defined(__AVR_ATmega88__)
#warning "MCU atmega88"
#define UART_RX0_BUFFER_SIZE    256
#define UART_TX0_BUFFER_SIZE    256
#elif defined(__AVR_ATmega168__)
#warning "MCU atmega168"
#define UART_RX0_BUFFER_SIZE    256
#define UART_TX0_BUFFER_SIZE    256
#elif defined(__AVR_ATmega328P__)
#warning "MCU atmega328"
#define UART_RX0_BUFFER_SIZE    1024
#define UART_TX0_BUFFER_SIZE    1024
#else
#error "Processor not supported"
#endif

#define REC_INTERRUPT       INT1_vect
#define TIMER_INTERRUPT     TIMER0_OVF_vect

/* Change some register names to match up with other AVRs */
#define WDTCR               WDTCSR

#endif

