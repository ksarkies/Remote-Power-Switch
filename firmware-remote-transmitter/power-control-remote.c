/**
@mainpage Power Control Project Remote Unit on ATTiny2313
@version 0.0.0
@author Ken Sarkies (www.jiggerjuice.info)
@date 27 June 2011
@brief Test code for a Remote Power Switch unit on AVR

This is a project to control power remotely using a wireless connection
between remote control units and a master switch unit.

In principle the remote control units can be used for a wide range of
purposes as they simply send keyboard command bytes from keypresses
over a serial link interface. The interface used is the SPI interface to
match the radio transmitter module Hope RFM02. Other interfaces could simply
be substituted.

The protocol used needs to match the sync pattern of the RF receiver.
The sync pattern is:

AA AA AA 2D D4 ...

@note
Software: AVR-GCC 4.8.2
@note
Target:   Any AVR
@note
Tested:   ATMega48, ATTiny2313 at 8MHz internal clock.
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

#define F_CPU               8000000

#include <inttypes.h>
#include <avr/sfr_defs.h>
#include <avr/wdt.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#if defined __AVR_ATmega48__
    #include "../libs-master-receiver/power-control-atmega48.h"
#elif defined __AVR_ATtiny2313__
    #include "../libs-master-receiver/power-control-attiny2313.h"
#else
    #error "Unsupported MPU"
#endif
#include "power-control-remote.h"
#include "../libs-master-receiver/rfm02.h"
#include "../libs-master-receiver/rfm02.c"

/* Convenience macros (we don't use them all) */
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

/*****************************************************************************/
/* Global Variables */

/*****************************************************************************/
/* Local Prototypes */

/* Single scan for the pressed key */
uint8_t scan(void);
/* Initialize the keypad scan ports */
void initKbd(void);
/****************************************************************************/
/** @brief Main Program */

int main(void)
{
#if defined __AVR_ATmega48__
/** ATmega48 Power down Watchdog Timer, comparator, USI, USART, Timers 0,1 */
    outb(PRR,0xFF);
#elif defined __AVR_ATtiny2313__
/** ATtiny2313 Power down analogue comparator and ports, and watchdog timer */
    outb(ACSR,0x80);                /* disable analogue comparator */
    outb(DIDR,0x03);                /* disable digital input buffers on comparator */
    wdt_reset();
    outb(MCUSR,0x00);               /* Clear the reset flags */
    outb(WDTCSR,inb(WDTCSR)|(1<<WDCE) | (1<<WDE));
    outb(WDTCSR,0x00);              /* turn off wdt as per datasheet */
/** Use Interrupt 1 with low level triggering in power down mode */
    sbi(GIMSK,INT1);                /* Enable Interrupt 1 */
    cbi(MCUCR,3);                   /* Low level trigger on interrupt 1 */
    cbi(MCUCR,2);
#endif

    initSPI();
    initKbd();
    rfm02Configure();

    uint8_t key = 0;

    for(;;)
    {
/** Test the keypad and put RFM02 and microcontroller to sleep if nothing
is happening. The microcontroller will wakeup on an external interrupt 1 and
will move on to the debounce.
Sleep mode to Power Down requires a level interrupt on INT0 and INT1. To get this
to work, we need to clear global interrupts immediately after waking from sleep.*/
        uint8_t testkey = scan();   /* Get first value for the keypress */
        if (testkey == 0xFF)
        {
          	writeCMD(SLEEP, 16);    /* Put RF module to sleep until any command sent */
            sei();                  /* Enable global interrupts */
            set_sleep_mode(SLEEP_MODE_PWR_DOWN);
            sleep_mode();           /* Enter sleep mode */
        }
        cli();                      /* Disable interrupts until ready to sleep again*/
/** Start with a debounce. This loops continually until more than four valid
keypresses occur. */
        for (uint8_t i=0; i<4; i++) /* Drop out when 4 sequential scans agree */
        {
            _delay_ms(1);
            key = scan();           /* Get an update on the keypress */
            if ((key != testkey) || (key == 0xFF))
            {
                i=0;                /* Reset counter if value not stable */
                testkey = key;      /* Update testkey in case it was wrong */
            }
        }
/** End with a debounce back to unpressed state. This loops continually until
more than four invalid keypresses occur, and the key is sent repeatedly with a
short delay. At 9600 bps the time to send the 7 bytes is 6 ms, so we get about
150 per second. */
        for (uint8_t i=0; i<4; i++) /* Drop out when 4 sequential scans invalid */
        {
            if (key < 12)           /* Transmit the keypress. */
            {
                startDataTx(BIT_RATE);
                writeByteTx(key);
                stopDataTx();
            }
            _delay_ms(1);
            if (scan() != 0xFF) i=0;/* Reset counter if not yet invalid */
        }
    }

}

/****************************************************************************/
/** @brief Interrupt on keypress.

This ISR does nothing but does allow the processor to wakeup. We need to use
a low level interrupt since only these are recognized in power down state.
*/
ISR(KBD_INTERRUPT)
{
}

/****************************************************************************/
/** @brief Single scan for the pressed key
Data returned is either a valid keypress 0-11, or invalid FF (-1) */

uint8_t scan()
{
    uint8_t key = 0xFF;                     /* Invalid keypress value */
    uint8_t row = 0;
    uint8_t column = _BV(FIRST_COL);        /* First column to scan */
    while ((column & COL_BITS) > 0)         /* Stop when shifted beyond end of column field */
    {
/* Columns are active low, set current column to low and remainder high */
        outb(COLUMNS,(inb(COLUMNS) | COL_BITS) & ~column);
        _delay_us(100);                     /* Time to settle */
        row = ((~inb(ROWIN) >> FIRST_ROW) & 0x07); /* Invert rows also as they are active low */
        if (row > 0) break;                 /* skip out if a row pin goes low */
        column <<= 1;                       /* Shift to next column */
    }
/** Make up a simple unique keycode and translate to the corresponding key */
/** Any value not matching is returned as 0xFF, including no keypress and multiple keypress. */
    uint8_t keycode = ((column << (4-FIRST_COL)) | row);
    if (keycode > 0)
    {
        switch(keycode)
        {
        case 0x11: key = 1; break;
        case 0x12: key = 2; break;
        case 0x14: key = 3; break;
        case 0x21: key = 4; break;
        case 0x22: key = 5; break;
        case 0x24: key = 6; break;
        case 0x41: key = 7; break;
        case 0x42: key = 8; break;
        case 0x44: key = 9; break;
        case 0x82: key = 0; break;
        case 0x81: key = 10; break;
        case 0x84: key = 11;
        }
    }
/* Set all columns low to trigger an interrupt on keypress */
    outb(COLUMNS,inb(COLUMNS) & ~COL_BITS);
    return (key);
}

/****************************************************************************/
/** @brief Initialize the keypad scan ports. */

void initKbd(void)
{
/** Set four column pins as outputs set to zero */
    outb(COLUMND,inb(COLUMND) | COL_BITS);
    outb(COLUMNS,inb(COLUMNS) & ~COL_BITS);
/** Set INT1 port as input with pullup resistor activated */
    outb(INT_SETUP,inb(INT_SETUP) & ~_BV(INT_BIT));
    outb(INT_PORT,inb(INT_PORT) | _BV(INT_BIT));
/** Set three row pins as inputs with pullup resistors activated */
    outb(ROWD,inb(ROWD) & ~ROW_BITS);
    outb(ROWS,inb(ROWS) | ROW_BITS);
}

/****************************************************************************/

