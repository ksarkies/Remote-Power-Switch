/**
@mainpage Power Control Project Master Unit
@version 0.0.0
@author Ken Sarkies (www.jiggerjuice.net)
@date 11 October 2014
@brief Library for the RFM01 Receiver

@note
Software: AVR-GCC 4.8.2
@note
Target:   Any AVR with sufficient output ports and a timer
@note
Tested:   ATMega48 at 8MHz internal clock.

This library implements the software SPI interface between the AVR and the
HopeRF RFM01 receiver module.

This file needs to be included in the main source following the parameter
definitions (normally placed in the header file) and the AVR pin definitions
for the software SPI and the RFM01 control pins.
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

uint16_t lastRfm01Status = 0;

/****************************************************************************/
/** @brief Set the Configuration of the Transmitter for RFM01.

Not all parameters are configurable. The wakeup timer, low battery and the
clock out are disabled. The transmitter chain is powered off.
*/
void receiverConfigure()
{
    _delay_ms(150);
	writeCMD(CONFIGURATION, 16);    /* Configure command */
/* Set the low battery threshold and the microprocessor clock output */
	writeCMD(BATTERY, 16);
	writeCMD(FREQUENCY, 16);        /* Set carrier frequency */
/* Receiver Setting Command, disable the receiver chain */
	writeCMD(RECEIVER, 16);
/* Receiver Setting Command, enable the entire receiver chain */
	writeCMD(RECEIVER | _BV(0), 16);
/* Set the FIFO options */
	resetFIFO();
/* Set the Baseband Data Filter and Clock Recovery options */
	writeCMD(BASEFILTER, 16);
/* Set the expected data bitrate */
	writeCMD(DATARATE, 16);
/* Set the AFC enable pattern, range limits and measurement mode */
	writeCMD(AFC, 16);
}

/****************************************************************************/
/** @brief Reset the FIFO

Clearing bit 1 of the FIFO command will clear the FIFO and restart the synchron
word detection. Set it to restart the FIFO counter.
*/
void resetFIFO(void)
{
    writeCMD(FIFO, 16);
	writeCMD(FIFO | _BV(1), 16);
}

/****************************************************************************/
/** @brief Write a Command to the RFM01 over SPI
  
A command is clocked out to the RFM01 one bit at a time.
All RF01 commands are 16 bit but the second parameter is kept for code
compatibility with the other RF chips.
At the same time a result is clocked back in one bit at a time. The result
is returned as a 16 bit word.

Each bit of output status is available before the clock pulse. After
the last clock pulse the first FIFO data is presented.

For the RFM01 receiver module the returned result is meaningless except
for the status read command which is 16 bits.

Parameter:  16 or 8 bit command
Parameter:  length of command (8 or 16)
Return:     Value returned from the SPI interface 
*/
uint16_t writeCMD(uint16_t command, uint8_t n)
{
	if (n < 16)                                 /* For 8 bit commands */
        command <<= (16-n);                     /* Shift command byte to upper byte */
	uint16_t result = 0;						/* Holds the received SDI */
	cbi(CS_PORT,nSEL);	                        /* Set CS LOW */
	while(n--)									/* Send All Bits MSB First */
	{
		result <<= 1;                           /* Shift left for next bit to receive */
		result |= ((inb(SPI_PIN) >> SDO) & 1);  /* add received bit to result */
		if (command & 0x8000)
			writeSPI(1);						/* Write 1 via SDI */
		else
			writeSPI(0);						/* Write 0 via SDI */
		command <<= 1;							/* Shift left for next bit to send */
	}
	sbi(CS_PORT,nSEL);							/* CS HIGH - Finished Sending Command */
    return result;
}

/****************************************************************************/
/** @brief Read a Data Byte from the RFM01 FIFO over SPI
  
The RF01 has two modes of reading the FIFO. If the FIFO level is set to 1 then
FIFO data available is read out from a dedicated pin, and the FIFO bit(s) can be
read from the SDO pin. Alternatively a status read bit 1 will indicate the FIFO
threshold exceeded and data can be read out following the remainder of the status
word. The latter mode is used here to allow bytewise reception.

A status command (all zeros) is sent to the RFM01 to initiate a databyte read.
The first 16 bits read out are status bits, followed by some data bits.
When the FIFO fills to the trigger point FIFO_TRIGGER, the top status bit
(first one transmitted) of the status word is set and can be tested.

The data is available before the SCK pulse, and is read first.

Normally FIFO_TRIGGER should be 8 to retrieve a single byte. It should not be
close to 16 which is the maximum limit of the FIFO buffer. If FIFO_TRIGGER
is less than 8 then only FIFO_TRIGGER bits are retrieved.

Data is read MSB first.

Return:     Full status word if no data ready, or received data byte in lower byte
            along with upper byte of status word.
*/
uint16_t readDataByte()
{
	uint8_t n = 16;
	uint16_t rfm01Status = 0;	     			/* Holds the RF01 status word */
	uint16_t result = 0;						/* Holds the received data and status */
	cbi(CS_PORT,nSEL);	                        /* Set CS LOW */
	while(n--)                                  /* Start reading in the status word */
	{
		rfm01Status <<= 1;                       /* Shift left for next bit received */
		rfm01Status |= ((inb(SPI_PIN) >> SDO) & 1);  /* add received bit to status */
		writeSPI(0);						    /* Write 0 via SPI for status command */
	}
    lastRfm01Status = rfm01Status;
    if ((rfm01Status & 0x8000) != 0)             /* Check if FFIT is activated */
    {
        n = FIFO_TRIGGER;
        if (n > 8) n = 8;                       /* Limit number read to a byte */
	    while(n--)						        /* Continue to send zeros to get data */
	    {
		    result <<= 1;                       /* Shift left for next bit received */
		    result |= ((inb(SPI_PIN) >> SDO) & 1);  /* add received bit to result */
		    writeSPI(0);						/* Write 0 via SPI for status command */
	    }
        result |= (rfm01Status & 0xFF00);        /* return upper byte of status word with result */
    }
    else result = rfm01Status;                   /* may as well send something back */
    sbi(CS_PORT,nSEL);					        /* CS high - finished sending status Command */
    return result;
}

/****************************************************************************/
/** @brief Bitbang the data on the SPI

Data is setup and a delay is inserted to ensure that the rate is less than 25%
of the clock, as required by the chip. The SCK is strobed high then low after
delays. The function exits with SCK low. The datarate is kept to 250kbps to
cope with a 1MHz clock on the chip at startup.

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
/** @brief Initialize the SPI interface on the microcontroller port D upper four pins.
This is not the microcontroller hardware SPI interface.
*/

void initSPI()
{
/* Set pins for SDI, SCK and CS as outputs with nIRQ and SDO as input */
    outb(SPI_SETUP,(inb(SPI_SETUP) | _BV(SDI) | _BV(SCK)) & ~_BV(nIRQ) & ~_BV(SDO));
    outb(SPI_PORT,inb(SPI_PORT) & ~_BV(SDI) & ~_BV(SCK));/* Set outputs to zero */
    outb(CS_SETUP,inb(CS_SETUP) | _BV(nSEL));
    outb(CS_PORT,inb(CS_PORT) | _BV(nSEL));          /* Set high to deselect*/
}

