/**
@mainpage Power Control Project Serial Link Transceiver
@version 0.0.0
@author Ken Sarkies (www.jiggerjuice.net)
@date 09 October 2014
@brief Code for a Serial Link Transceiver based on RFM12/RFM12B

This is a project to provide an RF link for serial communications between
a PC and a remote unit. The hardware used is part of the Remote Power Control
project that has been adapted to use a HopeRF RFM12 433MHz transceiver module
rather than the RFM01 receiver module.

The firmware draws on the rfm12lib AVR library provided by Hansinator, Tixiv
and Soeren at:
https://www.das-labor.org/wiki/RFM12_library/en

@note
Software: AVR-GCC 4.8.2
@note
Target:   Any AVR with sufficient output ports and a timer
@note
Tested:   ATMega48 at 8MHz internal clock.
 */
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

#define F_CPU               8000000

#include <inttypes.h>
#include <avr/sfr_defs.h>
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <string.h>
#include "../libs-master-receiver/defines-M48.h"
#include "../libs-master-receiver/serial.h"
#include "../libs-master-receiver/timer.h"
#include "rfm12_config.h"
#include "../auxiliary/rfm12-1.1/src/rfm12.h"
#include "../auxiliary/rfm12-1.1/src/rfm12.c"
#include "serial-link-transceiver.h"

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

/** SPI Clock Timing */
#define SPI_DELAY 1

/*****************************************************************************/
/* Global Variables */

/** @name UART variables */
/*@{*/
volatile uint16_t uartInput;    /**< Character and errorcode read from uart */
volatile uint8_t lastError;     /**< Error code for transmission back */
volatile uint8_t checkSum;      /**< Checksum on message contents */
/*@}*/

/** Real Time Clock Global Variable, ticks are 30.5 per second */
uint32_t timeValue;
/*****************************************************************************/
/* Local Prototypes */

void hardwareInit(void);
void timerInit(void);
void wdtInit(void);
/*****************************************************************************/
/** @brief Main Program */

int main(void)
{
    uint8_t inBuf[MAX_MESSAGE];

    hardwareInit();
    uartInit();
    timer0Init(0,5);

    rfm12_init();
    wdtInit();
    sei();

    for(;;)
    {
        wdt_reset();
		if (rfm12_rx_status() == STATUS_COMPLETE)
		{
			uint8_t *bufcontents = rfm12_rx_buffer();

/* dump buffer contents to uart */
        	uint8_t i;
			for (i=0;i<rfm12_rx_len();i++)
			{
				sendch(bufcontents[i]);
			}
			
/* Tell the implementation that the buffer can be reused for the next data. */
			rfm12_rx_clear();
		}
/* Send a transmission every time a character appears on the serial input */
        if (checkch())
        {
            inBuf[0] = getch();
            rfm12_tx(1, 0, inBuf);
        }
		rfm12_tick();
    }
}

/****************************************************************************/
/** @brief Initialize the hardware

*/

void hardwareInit(void)
{
}

/****************************************************************************/
/** @brief Initialize the watchdog timer to interrupt on maximum delay

*/
void wdtInit(void)
{
/* Initialize the Watchdog timer to interrupt. */
/* IMPORTANT: Disable the "WDT Always On" fuse so that WDT can be turned off. */
    wdt_disable();     /* watchdog timer turn off ready for setup. */
    outb(WDTCR,0);
/* Set the WDT with WDE clear, interrupts enabled, interrupt mode set, and
maximum timeout 8 seconds to give continuous interrupt mode. */
    sei();
//    outb(WDTCR,_BV(WDIE)|_BV(WDP3)|_BV(WDP0));
    outb(WDTCR,_BV(WDIE));  /* For test only: 32 ms timeout */

}

/****************************************************************************/
/** @brief Interrupt on RTC timer overflow

This ISR simply updates the RTC.
*/
ISR(TIMER_INTERRUPT)
{
  timeValue++;
}

