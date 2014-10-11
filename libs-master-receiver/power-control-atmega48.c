/**
@mainpage Power Control Project Master Unit ATMega48 Specifics
@version 0.1
@author Ken Sarkies (www.jiggerjuice.net)
@date 28 June 2011
@brief Library of code specific to the ATMega48 in the Power Control
Project Master Unit.

This file holds the code specific to the ATMega48, to allow the main
code to be processor type neutral.

The receiver nIRQ interrupt is attached to INT1 line.

We want to set the processor into the deepest sleep mode for power
saving since it is likely to be sitting idle for long periods. Since
we are using a real time clock, we can only use the power save mode.
This requires timer 2 to be active for the ATMega48, and also requires
INT0 and INT1 to use level interrupt mode. For this reason we disable
INT1 specifically after returning from a sleep to prevent further
interruption until processing is complete, and keep interrupts
enabled to allow the RTC to operate.

The receiver could also be set to lower power mode but we haven't tried
this as the datasheets are vague on this point.

@note Software: AVR-GCC 4.8.2
@note Target:   ATMega48
@note Tested:   ATMega48 at 8MHz.
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
#include <inttypes.h>
#include <avr/sfr_defs.h>
#include <avr/wdt.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include "defines-M48.h"
#include "power-control-atmega48.h"

/** Convenience macros (we don't use them all) */
#define TRUE 1
#define FALSE 0

#define inb(sfr) _SFR_BYTE(sfr)
#define inw(sfr) _SFR_WORD(sfr)
#define outb(sfr, val) (_SFR_BYTE(sfr) = (val))
#define outw(sfr, val) (_SFR_WORD(sfr) = (val))
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#define high(x) ((uint8_t) (x >> 8) & 0xFF)
#define low(x) ((uint8_t) (x & 0xFF))


