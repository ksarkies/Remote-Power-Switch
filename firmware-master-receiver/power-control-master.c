/**
@mainpage Power Control Project Master Unit
@version 0.0.0
@author Ken Sarkies (www.jiggerjuice.net)
@date 20 May 2011
@brief Code for a Master Power Switch unit

*** This sets the sleep mode, and requires the 8-bit timer 0 to generate
*** a timing tick.

This is a project to control power remotely using a wireless connection
between remote control units and a master switch unit. The master switch
board will interface to an ET-REL8 relay board using a 10 pin connector.
Each relay is activated by a separate port pin. It also interfaces to a
Hope RFM01 433MHz receiver module.

In principle the Master Power Switch units can be used for a wide range of
purposes as they simply receive command bytes over a serial link interface.
The interface used is the SPI interface to match the radio receiver module
RFM01. Other interfaces could simply be substituted.

The protocol used needs to match the sync pattern of the RF receiver:

AA AA AA 2D D4 ...

@note
Software: AVR-GCC 3.4.5
@note
Target:   Any AVR with sufficient output ports and a timer
@note
Tested:   ATMega48 at 8MHz internal clock.
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
#include <util/delay.h>
#include "power-control-master.h"
#include "../libs-master-receiver/power-control-atmega48.h"
#include "../libs-master-receiver/timer.h"
#include "../libs-master-receiver/rfm01.h"
#include "../libs-master-receiver/rfm01.c"

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

/*****************************************************************************/
/* Global Variables */

/** Set of 12 byte bins to count data received values. These will be used to
determine the median data value after a number of valid receives. */
uint8_t dataCount[12];
/** Real Time Clock Global Variable, ticks are 30.5 per second */
uint32_t timeValue;

/*****************************************************************************/
/* Local Prototypes */

void hardwareInit(void);
void wdtInit(void);
void snooze(void);
void setSwitch(uint8_t bestData);
void clearSwitches(void);
/*****************************************************************************/
/** @brief Main Program */

int main(void)
{
    hardwareInit();                 /* Initialize the processor specific hardware */
    timer0Init(0,5);                /* Configure the timer */
    timeValue = 0;                  /* reset timer */
    initSPI();
    receiverConfigure();            /* Initialize the receiver */
    uint8_t data = 0;
    uint8_t invalidData = 0;
    uint8_t validData = 0;
    uint8_t locked = FALSE;
    for (uint8_t bin=0; bin < 12; bin++) dataCount[bin] = 0;

/** The main loop will check for symbols received. This is tested for
validity and counted in bins to determine the median value as a best
estimate. Then the data is interpreted to toggle one of a number of ports
or perform other actions. The system is then locked to prevent any further
interpretations until a period of 2 seconds has elapsed following the last
valid received symbol. */

    for(;;)
    {
/** The processor sleeps until woken by a received byte or a timer tick */
        snooze();
	    uint16_t receiverStatus = readDataByte();
/** The MSB of the status word is tested as FFIT. */
        if ((receiverStatus & 0x8000) != 0)    /* Check if something received */
        {
/** If nothing has been received for 2 seconds, release and clear the system to
allow other control commands to be activated. */
            if (timeValue > 61)
            {
                locked = FALSE;
                invalidData = 0;
                validData = 0;
                for (uint8_t bin=0; bin < 12; bin++) dataCount[bin] = 0;
            }
            timer0Init(0,5);
            timeValue = 0;                  /* reset timer */
            data = low(receiverStatus);
            resetFIFO();
/** When data is received, check if it is valid and increment its bin */
            if ((data < 12) && !locked)
            {
                dataCount[data]++;
                validData++;
            }
            else invalidData++;
/** Wait for 10 valid received data and then check the bins */
            if (validData == 10)
            {
                locked = TRUE;              /* prevent more recognitions */
                validData = 0;              /* prevent returning here while locked */
/** Look through bins and find the location of the peak (will return first equal peak) */
                uint8_t maxData = 0;
                uint8_t bestData = 0;
                for (uint8_t i=0; i<12; i++)
                {
                    if (dataCount[i] > maxData)
                    {
                        maxData = dataCount[i];
                        bestData = i;
                    }
                }
/** With the estimated data value, set the appropriate port. Toggle the
existing value of the port. */
                if (bestData < 10) setSwitch(bestData);
/** Use the remaining four symbols for general controls */
/* Turn off all ports */
                else if (bestData == 11) clearSwitches();
            }
        }
    }
}

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
/** @brief Interrupt on nIRQ from Receiver.

This ISR does nothing except to allow the processor to wakeup.
*/
ISR(REC_INTERRUPT)
{
}

/****************************************************************************/
/** @brief Interrupt on RTC timer overflow

This ISR simply updates the RTC.
*/
ISR(TIMER_INTERRUPT)
{
  timeValue++;
}
