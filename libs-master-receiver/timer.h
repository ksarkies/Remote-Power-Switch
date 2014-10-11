/****************************************************************************/
/*
 Title  :   C  include file for the Timer Functions library
 Author:    Ken Sarkies
 File:      $Id: timer.h, v 0.1 25/4/2007 $
 Software:  AVR-GCC 3.4.5
 Target:    All AVR MCUs with timer functionality.
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

#ifndef TIMER_H
#define TIMER_H

#ifndef  _SFR_ASM_COMPAT        /**< Special function register compatibility.*/
#define  _SFR_ASM_COMPAT     1
#endif

#ifndef F_CPU               /* CPU speed in Hz */
#define F_CPU               8000000
#endif

/** Define this to allow code size to be reduced by removal of unwanted
functions. Any or all may be used. */
#ifndef TIMER_INTERRUPT_MODE    /* Interrupts are used */
#define TIMER_INTERRUPT_MODE  1
#endif

/****************************************************************************/
extern void timer0Init(uint8_t mode,uint16_t timerClockScale);
extern uint16_t timer0Read(void);

/** Deal with ATMega*8 MCUs and others with 16 bit control registers */
#if defined TCCR0
#define TIMER_CONT_REG0 TCCR0
#elif defined TCCR0B
#define TIMER_CONT_REG0 TCCR0B
#endif
/** Deal with MCUs with independent mask registers */
#if defined TIMSK
#define TIMER_MASK_REG0 TIMSK
#define TIMER_MASK_REG1 TIMSK
#define TIMER_MASK_REG2 TIMSK
#define TIMER_MASK_REG3 TIMSK
#else
#if defined TIMSK0
#define TIMER_MASK_REG0 TIMSK0
#endif
#if defined TIMSK1
#define TIMER_MASK_REG1 TIMSK1
#endif
#if defined TIMSK2
#define TIMER_MASK_REG2 TIMSK2
#endif
#if defined TIMSK3
#define TIMER_MASK_REG3 TIMSK3
#endif
#endif
/** Deal with MCUs with independent flag registers */
#if defined TIFR
#define TIMER_FLAG_REG0 TIFR
#define TIMER_FLAG_REG1 TIFR
#define TIMER_FLAG_REG2 TIFR
#define TIMER_FLAG_REG3 TIFR
#else
#if defined TIFR0
#define TIMER_FLAG_REG0 TIFR0
#endif
#if defined TIFR1
#define TIMER_FLAG_REG1 TIFR1
#endif
#if defined TIFR2
#define TIMER_FLAG_REG2 TIFR2
#endif
#if defined TIFR3
#define TIMER_FLAG_REG3 TIFR3
#endif
#endif

/* Convenience macros (we don't use them all) */
#define  _BV(bit) (1 << (bit))
#define  inb(sfr) _SFR_BYTE(sfr)
#define  inw(sfr) _SFR_WORD(sfr)
#define  outb(sfr, val) (_SFR_BYTE(sfr) = (val))
#define  outw(sfr, val) (_SFR_WORD(sfr) = (val))
#define  cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#define  sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))

#endif
