/**
@mainpage Power Control Project Master Unit
@version 0.0.0
@author Ken Sarkies (www.jiggerjuice.net)
@date 11 October 2014
@brief Library for the RFM02 Transmitter

@note
Software: AVR-GCC 4.8.2
@note
Target:   Any AVR with sufficient output ports and a timer
@note
Tested:   ATTiny2313 at 8MHz internal clock.

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

