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

@note Software: AVR-GCC 3.4.5
@note Target:   ATMega48
@note Tested:   ATMega48 at 8MHz.
*/
/***************************************************************************
 *   Copyright (C) 2011 by Ken Sarkies                                     *
 *   ksarkies@trinity.asn.au                                               *
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

/****************************************************************************/
/** @brief Initialize the hardware.
*/
void hardwareInit(void)
{
/** Power down Timers 0,1, TWI, SPI, ADC and USART */
    outb(PRR,0xBF);
/** We use Interrupt 1 with level triggering for power save mode */
    sbi(EIMSK,INT1);                /* Enable Interrupt 1 */
    cbi(EICRA,ISC11);               /* Set level trigger on interrupt 1 */
    cbi(EICRA,ISC10);
/** Set control ports as outputs */
    outb(DDRC,inb(DDRC) | 0x3F);
    outb(DDRB,inb(DDRB) | 0x07);
    outb(DDRD,inb(DDRD) | 0x04);
    sei();                          /* Enable global interrupts */
}
/****************************************************************************/
/** @brief Reset the Timer.

A scale factor 5 is used to select a clock divide of 1024. The timer
used is defined in the header.
*/
void resetTimer(void)
{
/** Set timer to FCPU/1024 which is about 30.5 overflows per second. */
    outb(TIMER_CONT_REG,((inb(TIMER_CONT_REG) & 0xF8) | 5));
    outb(TCOUNT,0);                 /* Clear the register */
    sbi(TIMER_FLAG_REG, TOV);       /* Force clear the overflow interrupt flag */
    sbi(TIMER_MASK_REG, TOIE);      /* Enable the overflow interrupt */
}
/****************************************************************************/
/** @brief Send processor to sleep and manage interrupts.

This function is called at the beginning of every processing loop. The INT1
is enabled before sleeping, and disabled after wakeup.
*/
void snooze(void)
{
    sei();                              /* Enable global interrupts */
    sbi(EIMSK,INT1);                    /* Enable Interrupt 1 */
    set_sleep_mode(SLEEP_MODE_PWR_SAVE);
    sleep_mode();                       /* Enter sleep mode */
    cbi(EIMSK,INT1);                    /* Disable Interrupt 1 */
}
/****************************************************************************/
/** @brief Manage the power switching.

Adapt the switches 0-9 to the actual microcontroller port outputs.

@param[in] uint8_t bestData. The number of the switch to toggle.
*/
void setSwitch(uint8_t bestData)
{
    uint16_t bestBit = (1 << bestData);
/* The lower 4 bits of PORTC are the first four ports */
    if (bestData < 4) outb(PORTC,inb(PORTC) ^ bestBit);
/* The lower 3 bits of PORTB are the next three ports */
    else if (bestData < 7) outb(PORTB,inb(PORTB) ^ (bestBit >> 4));
/* Bit 2 of PORTD is the eighth port */
    else if (bestData == 7) outb(PORTD,inb(PORTD) ^ 4);
/* Bits 4,5 of PORTC are the ninth and tenth ports */
    else if (bestData < 10) outb(PORTC,inb(PORTC) ^ (bestBit >> 4));
}
/****************************************************************************/
/** @brief Turn off all power switching.
*/
void clearSwitches(void)
{
    outb(PORTC,(inb(PORTC) & ~0x3F));
    outb(PORTB,(inb(PORTB) & ~0x07));
    outb(PORTD,(inb(PORTD) & ~0x04));
}
/****************************************************************************/

