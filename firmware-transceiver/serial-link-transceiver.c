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

The interrupt driven USART I/O library of Peter Fleury and Andy Gock is used.

Characters from the serial port are buffered and sent in a message when a CR,
buffer full or timeout occurs. Incoming messages are sent directly to the serial
port.

The firmware draws on the rfm12lib AVR library provided by Hansinator, Tixiv
and Soeren.
https://www.das-labor.org/wiki/RFM12_library/en

Dependencies:
avr-uart-master https://github.com/andygock/avr-uart
NARTOS          https://github.com/ksarkies/NARTOS
rfm12lib        https://www.das-labor.org/wiki/RFM12_library/en

@note
Software: AVR-GCC 4.8.2
@note
Target:   Any AVR with sufficient output ports and a timer
@note
Tested:   ATMega48 at 8MHz internal clock, ATMega88/168 at 8MHz internal clock.
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

#include <inttypes.h>
#include <avr/sfr_defs.h>
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <string.h>
#include <stdbool.h>
#include "../../NARTOS/timer/timer.h"
#include "rfm12_config.h"
#include "serial-link-transceiver.h"
#include "../auxiliary/rfm12-1.1/src/rfm12.h"
#include "../auxiliary/rfm12-1.1/src/rfm12.c"
#include "../auxiliary/avr-uart-master/uart.h"
#include <util/delay.h>
#include "baud.h"

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

/** Real Time Clock Global Variable, ticks are 30.5 per second */
volatile uint32_t timeCount;
volatile uint8_t indx;
static uint8_t inBuf[MAX_MESSAGE];
volatile uint8_t inputPacketReceiveID;
volatile uint8_t outputPacketSendID;
/*****************************************************************************/
/* Local Prototypes */

void hardwareInit(void);
void timerInit(void);
void wdtInit(void);
void inputPacketReceive(void) __attribute__ ((naked));
void outputPacketSend(void)   __attribute__ ((naked));
/*****************************************************************************/
/** @brief Main Program */

int main(void)
{
    timeCount = 0;

    hardwareInit();
    uart0_init(BAUD_SETTING);
/* Initialise timer to divide by 1025, giving 32ms time tick */
    timer0Init(0,5);

    rfm12_init();
    wdtInit();
    sei();

    indx = 0;

    for(;;)
    {
        wdt_reset();

        uint8_t sendMessage = false;
        uint16_t character = uart0_getc();
/* Wait for a serial incoming message to be built. */
        if (character != UART_NO_DATA)
        {
            inBuf[indx++] = (uint8_t)(character & 0xFF);
/* Signal to transmit if a CR was received, or string too long. */ 
            sendMessage = ((indx > MAX_MESSAGE) || (character == 0x0D));
        }

/* Send a transmission if message is ready to send, or something was
received and time waited is too long. */ 
        if (sendMessage || (timeCount++ > TIMEOUT))
        {
/* Wait for the transmit buffer to be freed and message loaded. */
            if ((indx > 0) && (rfm12_tx(indx, 0, inBuf) != RFM12_TX_OCCUPIED))
                indx = 0;
            timeCount = 0;
        }
        rfm12_tick();

/* If an RF incoming message has been received, take it from the buffer one
character at a time and transmit via the serial port. */
        if (rfm12_rx_status() == STATUS_COMPLETE)
        {
            uint8_t *bufferContents = rfm12_rx_buffer();
            uint8_t messageLength = rfm12_rx_len();
            uint8_t i;
            for (i=0;i<messageLength;i++)
            {
	            uart0_putc(bufferContents[i]);
            }
/* Clear the "in use" status of the receive buffer to be available for
rfm12lib. */
            rfm12_rx_clear();
        }
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
  timeCount++;
}

