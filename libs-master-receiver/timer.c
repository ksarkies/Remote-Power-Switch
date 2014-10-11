/**
@mainpage Atmel AVR Timer Library
@version 0.1
@author Ken Sarkies (www.jiggerjuice.net) adapted from code by Chris Efstathiou
@date 2 September 2007
@brief Library of timer functions for the Atmel AVR microcontrollers.

This library aims to provide a range of timer functions for initializing and
controlling the opertaion of the timers. It appears to be somewhat difficult to
provide a complete set of general functions despite a good compatibility
between timers within the Atmel range of AVR processors, because the ISRs are
often used in a wide variety of different ways specific to the application. The
use of callback functions to the application can result in heavy use of
resources.

Currently only timer 0 is supported here with initialization and reading of its
value.

@note Software: AVR-GCC 3.4.5
@note Target:   All Atmel MCUs with timer functionality
@note Tested:   ATMega88 at 8MHz.
*/
/***************************************************************************
 *   Copyright (C) 2007 by Ken Sarkies                                     *
 *   ksarkies@trinity.asn.au                                               *
 *                                                                         *
 *   This file is part of Acquisition                                      *
 *                                                                         *
 *   Acquisition is free software; you can redistribute it and/or modify   *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   Acquisition is distributed in the hope that it will be useful,        *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with Acquisition if not, write to the                           *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA.             *
 ***************************************************************************/
#define TRUE  1
#define FALSE 0

#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"

/****************************************************************************/
/**
    @brief   Initialise Timer 0

This function will initialise the timer with the mode of operation and the
clock rate to be used. An error will be returned if the timer is busy.

Because ATMega64, ATMega128, ATMega103 offer different scale factors, there
needs to be a conversion provided between the specification here and the scale
setting. The additional clock settings provided for those MCUs are not used
here, nor are the external clock settings of the remaining MCUs.

Timer 0 is typically an 8-bit timer and has very basic functionality. Some MCUs
offer PWM capability while most do not.

    @param  mode
            Ignored for simple timers.
    @param  timerClock
            00  Stopped
            01  F_CLK
            02  F_CLK/8
            03  F_CLK/64
            04  F_CLK/256
            05  F_CLK/1024

The timer continues to run until it is stopped by calling this function with
timerClock=0. At the moment, mode does nothing.
*/

void timer0Init(uint8_t mode,uint16_t timerClock)
{
  if (timerClock > 5) timerClock = 5;
#if defined(__AVR_ATMega64__) || \
    defined(__AVR_ATMega128__) || \
    defined(__AVR_ATMega103)
/* Rescale clock values to match those of the above MCUs */
  if (timerClock > 2) ++timerClock;
  if (timerClock > 4) ++timerClock;
#endif
  outb(TIMER_CONT_REG0,((inb(TIMER_CONT_REG0) & 0xF8)|(timerClock & 0x07)));
#if defined (TCNT0L)
  outw(TCNT0,0);                    /* 16 bit - clear both registers */
#else
  outb(TCNT0,0);                    /* Clear the register */
#endif
#if (TIMER_INTERRUPT_MODE == 1)
  sbi(TIMER_FLAG_REG0, TOV0);       /* Force clear the interrupt flag */
  sbi(TIMER_MASK_REG0, TOIE0);      /* Enable the overflow interrupt */
  sei();
#endif
}

/****************************************************************************/
/**
    @brief   Read Timer 0

This function will return the current timer value as a 16 bit unsigned integer
even if the timer is only 8 bit. This allows for a possibility of a 16 bit
timer being at timer 0 (so far this is not the case in any MCU).

In the event of a 16 bit register, the hardware registers must be accessed
high byte first. The avr-gcc compiler does this automatically.

    @return Timer Value.
*/

uint16_t timer0Read()
{
#if defined (TCNT0L)
  return inw(TCNT0);
#else
  return (int16_t) inb(TCNT0);
#endif
}

/****************************************************************************/
