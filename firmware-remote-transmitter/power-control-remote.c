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
Software: AVR-GCC 3.4.5
@note
Target:   Any AVR
@note
Tested:   ATMega48, ATTiny2313 at 8MHz internal clock.
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

#define F_CPU               8000000

#include <inttypes.h>
#include <avr/sfr_defs.h>
#include <avr/wdt.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include "power-control-remote.h"
#if defined __AVR_ATmega48__
    #include "power-control-atmega48.h"
#elif defined __AVR_ATtiny2313__
    #include "power-control-attiny2313.h"
#else
    #error "Unsupported MPU"
#endif

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

/** SPI Clock Timing */
#define SPI_DELAY 1

/** @name RFM02 Transmitter parameters c */
/*@{*/

/** Band is 433MHz (POR 0 = 315MHz) */
#define RF_BAND                      1
/** clock output frequency (used for external clock to microcontroller etc) (POR 0 = 1MHz)
-    0  1.00MHz
-    1  1.25MHz
-    2  1.66MHz
-    3  2.00MHz
-    4  2.50MHz
-    5  3.33MHz
-    6  5.00MHz
-    7 10.00MHz */
#define CLOCK_FREQ_OUT               7
/** crystal load capacitance for the selected crystal,
    0000 is 8.5pF, increment in 0.5pF steps to maximum 15 (16pF) (POR 8 = 12.5pF). */
#define CRYSTAL_LOAD_CAPACITANCE     6
/** FSK Deviation is set (POR 0 = 30kHz)*/
#define DEVIATION                    0
/** This bit rate is close to 9600 baud */
#define BIT_RATE                  0x23
/** Relative output power dB (POR 0 = 0dB) */
#define POWER                        0
/** Carrier frequency to exactly 433 MHz (POR 7D0 (2000) = 435MHz
Carrier frequency F, a 12 bit value between 96 and 3903 giving centre frequency
        f0 = 10MHz * C1 * (C2 + F/4000)
where C1/C2 are 1/43, 2/43 or 3/30 for bands 433, 868 and 915 respectively.
That is steps of 2.5kHz for the 433 and 868 RFM01_PM_SETTINGS bands,
and 7.5kHz for the 915kHz band. */
#define CARRIER                   1200
/** Low battery voltage setting (OR with disable timer calibration if needed)*/
#define TXSYN_LOWBATT             0x00
/** Power Management settings for the timer, battery and clock out. */
#define PM_SETTINGS_DEFAULT       RFM02_PMBIT_CKOUT
/** Sleep clock turnoff delay (POR 0x10) */
#define SLEEP_DELAY               0x00
/** Wakeup period (POR 0) (not used here) */
#define WAKEUP_EXPONENT           0x00
#define WAKEUP_DELAY              0x00
/*@}*/

/** @name Complete command words */
/*@{*/
#define CONFIGURATION   RFM02_CMD_CONFIG | ((RF_BAND & 0x03) << 11) | ((CLOCK_FREQ_OUT & 0x07) << 8) | ((CRYSTAL_LOAD_CAPACITANCE & 0x0F) << 4) | (DEVIATION & 0x0F)
#define POWERSET        RFM02_CMD_POWER_SET | (POWER & 0x07)
#define FREQUENCY       RFM02_CMD_FREQ | (CARRIER & 0xFFF)
#define PLL_MODE        RFM02_PLL_MODE1
#define WAKEUP          RFM02_CMD_WAKEUP | ((WAKEUP_EXPONENT & 0x1F) << 8) | (WAKEUP_DELAY & 0xFF)
#define POWERMANAGE     RFM02_CMD_POWERMGMT | (PM_SETTINGS_DEFAULT & 0xFF)
#define DATARATE        RFM02_CMD_RATE | (BIT_RATE & 0xFF)
#define TX_SYNC         RFM02_CMD_SYNC | RFM02_TXSYN_DISTIMCAL | RFM02_TXSYN_TXSYNC | (TXSYN_LOWBATT & 0x1F)
#define SLEEP           RFM02_CMD_SLEEP | SLEEP_DELAY
/*@}*/

/* Wilhelm Krug settings
#define CONFIGURATION   0x8E60
#define POWERSET        0xB0
#define FREQUENCY       0xA4B0
#define PLL_MODE        0xD240
#define POWERMANAGE     0xC020
#define DATARATE        0xC88F
#define TX_SYNC         0xC220
*/
/*****************************************************************************/
/* Global Variables */

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
/** @brief Sets the Configuration of the Transmitter for RFM02.

A delay of 150ms is inserted to allow for power on reset of the module.
*/
void rfm02Configure()
{
    _delay_ms(150);
    writeCMD(CONFIGURATION, 16);
    writeCMD(FREQUENCY, 16);            /* Set carrier frequency */
    writeCMD(POWERSET, 8);              /* Power Setting Command */
    writeCMD(DATARATE, 16);             /* Set data rate */
    writeCMD(TX_SYNC,16);
    writeCMD(PLL_MODE,16);
//	writeCMD(WAKEUP, 16);
/* Power Management Command, set all Tx controls to zero. Manual operation of Tx chain. */
	writeCMD(POWERMANAGE, 16);
}

/****************************************************************************/
/** @brief Write a Command to the RFM02 over SPI
  
A command is clocked out to the RFM02 one bit at a time.  
At the same time a result is clocked back in one bit at a time. The result
is returned as a 16 bit word.

For the RFM02 transmitter module the returned result is meaningless except
for the status read command, in which case it occupies only the lower 8 bits.

Parameter:  16 or 8 bit command
Parameter:  length of command (8 or 16)
Return:     Value returned from the SPI interface 
*/
uint16_t writeCMD(uint16_t command, uint8_t n)
{
	if (n != 16)                                /* For 8 bit commands */
        command <<= (16-n);                     /* Shift command byte to upper byte */
	uint16_t result = 0;						/* Holds the received SDI */
	cbi(CS_PORT,nSEL);	                        /* Set CS LOW */
	while(n--)									/* Send All Bits MSB First */
	{
		if (command & 0x8000)
			writeSPI(1);						/* Write 1 via SDI */
		else
			writeSPI(0);						/* Write 0 via SDI */
		command <<= 1;							/* Shift left for next bit to send */
		result <<= 1;                           /* Shift left for next bit to receive */
		result |= (inb(SPI_PIN) & 1);  		    /* add received bit to result */
	}
	sbi(CS_PORT,nSEL);							/* Set CS high - Finished Sending Command */
    return result;
}

/****************************************************************************/
/** @brief Start Transmission of Data to the RFM02 over SDI
  
Power Management is set for manual startup of PLL, clock and power amplifier.
A Data Rate command sets up the desired data rate.
TX Bit synchronization is set.
This is followed by a Data Transmit command sent over the SDI pin.
Transmission of data must follow immediately. A 5 byte sync pattern is sent
before control is returned to the calling program.
0xC034
parameter:      data rate R (0 - 255), actual rate = 10 MHz / 29 / (R+1)
Return:         RF02 status register byte.
*/
void startDataTx(uint8_t dataRate)
{
	writeCMD(DATARATE, 16);                 /* Set data rate */
/* Enable the Tx Synchronization circuit.*/
	writeCMD(TX_SYNC,16);
/* Power Management Command, Startup oscillator and wait for stability. */
	writeCMD(POWERMANAGE | RFM02_PMBIT_OSC, 16);
    _delay_ms(5);
/* Power Management Command, Startup PLL and wait for stability. */
	writeCMD(POWERMANAGE | RFM02_PMBIT_OSC | RFM02_PMBIT_SYNTH, 16);
    _delay_us(250);
/* Power Management Command, Turn on Power Amplifier. */
	writeCMD(POWERMANAGE | RFM02_PMBIT_OSC | RFM02_PMBIT_SYNTH | RFM02_PMBIT_AMP, 16);
    writeDataCMD();                         /* Send data transmit command */
/* Send sync pattern and synchron as specified for the Receiver RF01 */
    writeByteTx(0xAA);
    writeByteTx(0xAA);
    writeByteTx(0xAA);
    writeByteTx(0xAA);
    writeByteTx(0x2D);
    writeByteTx(0xD4);
}

/****************************************************************************/
/** @brief Write a Data Start Command to the RFM02 over SPI
  
This is similar to the writeCMD function except that the nSEL bit is not
reset at the end, to allow for data transmission. It specifically sends the
8 bit start command.
*/
void writeDataCMD()
{
	cbi(CS_PORT,nSEL);							/* Clear CS to initiate Sending Data Block */
    uint8_t command = RFM02_CMD_DATA_TX;
    for (uint8_t i = 0; i < 8; i++)
	{
		if (command & 0x80)
			writeSPI(1);						/* Write 1 via SDI */
		else
			writeSPI(0);						/* Write 0 via SDI */
		command <<= 1;							/* Shift left for next bit to send */
	}
}

/****************************************************************************/
/** @brief Transmit Byte Data to the RFM02 over SDI
  
One byte of data is sent over the SDI pin, paced by sensing the nIRQ line. Tx 
bit synchronization must be turned on. This mode of operation is not SPI
compatible and no clock pulses are sent. The Power Management should set manual
startup of PLL and power amplifier, and data rate should have been already set.
nSEL is assumed to be low throughout all data transmission.

Data is sent MSB first.

Parameter:      data to send.
Parameter:      number of bytes to send.
Return:         RF02 status register byte.
*/
void writeByteTx(uint8_t data)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        while ((inb(SPI_PIN) & _BV(nIRQ)) != 0);        /* Loop until nIRQ goes low */
        if ((data & 0x80) > 0) sbi(SPI_PORT,SDI);
        else cbi(SPI_PORT,SDI);
        data <<= 1;
        while ((inb(SPI_PIN) & _BV(nIRQ)) == 0);        /* Loop until nIRQ goes high */
    }
}

/****************************************************************************/
/** @brief End Transmission of Data to the RFM02 over SDI
  
A default power manage command will turn off power to all sections. We could
put the device to sleep here if there is to be a significant time to the
next transmission, but in this application we start the next one immediately.

Return:         RF02 status register byte.
*/
void stopDataTx()
{
	sbi(CS_PORT,nSEL);							/* Set CS to finish Sending Data Block */
    _delay_ms(1);
	writeCMD(POWERMANAGE, 16);                  /* Turn off power to all devices */
//    writeCMD(SLEEP,16);                         /* Send sleep command */
}

/****************************************************************************/
/** @brief Bitbang the data on the SPI

Data is setup and a delay is inserted to ensure that the rate is less than 25%
of the clock, as required by the chip. The SCK is strobed high then low after
delays. The function exits with SCK low. The datarate is kept to 250kbps to
cope with a 1MHz clock on the chip.

Parameter:  bit data sent, 0 or 1
*/

void writeSPI(uint8_t n)
{
	cbi(SPI_PORT,SCK);                          /* Clear the SCK */
    _delay_us(SPI_DELAY);                       /* delay to reduce rate to 250kbps */
	if (n) sbi(SPI_PORT,SDI);                   /* Setup data for strobe */
    else cbi(SPI_PORT,SDI);
    _delay_us(SPI_DELAY);                       /* delay to reduce rate to 250kbps */
	sbi(SPI_PORT,SCK);                          /* Set the SCK to latch the data */
    _delay_us(SPI_DELAY);                       /* delay to reduce rate to 250kbps */
	cbi(SPI_PORT,SCK);                          /* Clear the SCK */
    _delay_us(SPI_DELAY);                       /* delay to reduce rate to 250kbps */
}

/****************************************************************************/
/** @brief Initialize the SPI interface on the microcontroller.
This is not the microcontroller hardware SPI interface.
*/

void initSPI()
{
/* Set pins for SPI and CS as outputs with nIRQ as input */
    outb(SPI_SETUP,(inb(SPI_SETUP) | _BV(SDI) | _BV(SCK)) & ~_BV(nIRQ));
    outb(SPI_PORT,inb(SPI_PORT) & ~(_BV(SDI) | _BV(SCK)));/* Set outputs to zero */
    outb(CS_SETUP,inb(CS_SETUP) | _BV(nSEL));
    outb(CS_PORT,inb(CS_PORT) | _BV(nSEL));          /* Set high to deselect*/
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

