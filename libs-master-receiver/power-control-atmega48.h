/*        Power Control Project Master/Remote Unit
*       Ken Sarkies ksarkies@internode.on.net
*
* File              : power-control-atmega48.h
* Compiler          : AVR-GCC/avr-libc(>= 1.2.5)
* Revision          : $Revision: 0.0 $
* Updated by        : $ author K. Sarkies 26/06/2011 $
*
* Target platform   : ATMega48
*
* Microcontroller settings for the ATMega48 AVR microcontroller as used in
* the master receiver board.
*/
/***************************************************************************
 *   Copyright (C) 2011 by Ken Sarkies                                     *
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
#ifndef _POWER_CONTROL_ATMEGA48_H_
#define _POWER_CONTROL_ATMEGA48_H_

#include <inttypes.h>

/** Microcontroller Port allocations:
- Port D 0-1 Reserved for USART
- Port D 2   Switch output 7
- Port D 3-7 RFM01 Interface
- Port B 0-2 Switch outputs 4-6
- Port B 3-5 Firmware SPI interface
- Port B 6-7 Reserved
- Port C 0-3 Switch outputs 0-3
- Port C 4-5 Switch outputs 8-9
- Port C 6   Reset
*/

/** @name Microcontroller settings for RFM01 control pins and software SPI port
settings. These need not be the hardware SPI port, which could be reserved for
firmware updates. */
/*@{*/
#define SPI_PORT    PORTD
#define SPI_PIN     PIND
#define SPI_SETUP   DDRD
#define CS_PORT     PORTD
#define CS_SETUP    DDRD
#define nIRQ        3
#define nSEL        4
#define SCK         5
#define SDO         6
#define SDI         7
/*@}*/

#define TCOUNT         TCNT2
#define TIMER_CONT_REG TCCR2B
#define TIMER_MASK_REG TIMSK2
#define TIMER_FLAG_REG TIFR2
#define TOV            TOV2
#define TOIE           TOIE2

#define REC_INTERRUPT   INT1_vect
#define TIMER_INTERRUPT TIMER2_OVF_vect

/* Change some register names to match up with other AVRs */
#define WDTCR                   WDTCSR

#endif

