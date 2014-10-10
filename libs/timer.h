/****************************************************************************/
/*
 Title  :   C  include file for the Timer Functions library
 Author:    Ken Sarkies
 File:      $Id: timer.h, v 0.1 25/4/2007 $
 Software:  AVR-GCC 3.8.2
 Target:    All AVR MCUs with timer functionality.
 Tested:    ATMega168
*/
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

#ifndef TIMER_H
#define TIMER_H

#ifndef  _SFR_ASM_COMPAT        /**< Special function register compatibility.*/
#define  _SFR_ASM_COMPAT     1
#endif

/** Define this to allow code size to be reduced by removal of unwanted
functions. Any or all may be used. */
#ifndef TIMER_INTERRUPT_MODE    /* Interrupts are used */
#define TIMER_INTERRUPT_MODE  1
#endif

/****************************************************************************/
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
