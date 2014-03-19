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
#include "power-control-master.h"
#if defined __AVR_ATmega48__
    #include "power-control-atmega48.h"
#else
    #error "Unsupported MPU"
#endif

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

/** @name RFM01 Receiver Parameters specific for this application */
/*@{*/
/** Band is 434MHz, required by the hardware (POR 433MHz)
-    1  434
-    2  868
-    3  915 */
#define RF_BAND                      1
/** crystal load capacitance for the selected crystal, (POR 10pF)
    0000 is 8.5pF, increment in 0.5pF steps to maximum 15 (16pF). */
#define CRYSTAL_LOAD_CAPACITANCE     6
/** Baseband Filter Bandwidth 67kHz (POR 5 = 134kHz)
-    1  400kHz
-    2  340kHz
-    3  270kHz
-    4  200kHz
-    5  134kHz
-    6   67kHz */
#define BASEBAND_BW                  6
/** clock output frequency (used for external clock to microcontroller etc)
-    0  1.00MHz
-    1  1.25MHz
-    2  1.66MHz
-    3  2.00MHz
-    4  2.50MHz
-    5  3.33MHz
-    6  5.00MHz
-    7 10.00MHz */
#define CLOCK_FREQ_OUT               7
/** Carrier frequency to exactly 433MHz (POR 0x680, 438.96MHz)
Carrier frequency F, a 12 bit value between 96 and 3903 giving centre frequency
        f0 = 10MHz * C1 * (C2 + F/4000)
where C1/C2 are 1/43, 2/43 or 3/30 for bands 433, 868 and 915 respectively.
That is steps of 2.5kHz for the 433 and 868 RFM01_PM_SETTINGS bands, and 7.5kHz for the 915kHz band. */
#define CARRIER                   1200
/** Valid data indicator source (POR 3 - always on)
-    0  Digital RSSI output
-    1  Data Quality Detector
-    2  Clock recovery lock
-    3  Always on */
#define VDI_SOURCE                   1
/** Low Noise Amplifier gain (POR 1 = -6dB)
-    0   0dB
-    1  -6dB
-    2  -14dB
-    3  -20dB */
#define LNA_GAIN                     0
/** Digital RSSI Threshold (0-7), increments by 6dB from -103dB (POR 0 = -103dB)
(actual threshold must add the (negative) lna gain) */
#define RSSI_THRESHOLD               0
/** AFC frequency offset measurement (POR 3 = keep independent of VDI)
-    0  off
-    1  Run once after power up
-    2  Keep value while receiving
-    3  Keep value independently of VDI */
#define AFC_MODE                     2
/** Frequency offset range limit plus and minus (POR 3 = 4 x fres)
-    0  off
-    1 16 x fres
-    2  8 x fres
-    3  4 x fres
where freq is 2.5kHz for 315/433MHz, 5kHz for 868MHz and 7.5kHz for 916MHz bands.*/
#define AFC_RANGE                    3
/** Data Filter and Clock Recovery Enables (POR 00 = slow mode)
-    bit 0  Fast Mode enable
-    bit 1  Automatic mode (fast then drop to slow mode) */
#define DCR_ENABLE                   2
/** Type of Data Filter  (POR 1 = digital, only choice available)
-    0      OOK to filter
-    1      Digital
-    3      Analogue */
#define DCR_TYPE                     1
/** Data Quality Detector Threshold (0-7). This counts the number of correct
transitions at the I/Q output/ This should be 4 if the bit rate is close
to the deviation. Set above 4 if bitrate not close to deviation (POR 4). */
#define DCR_DQDTHRESH                4
/** Enable the FIFO (POR 0,1)
-    bit 0  Enable 16 bit deep FIFO mode
-    bit 1  Enable FIFO start after syncron detection 0x2DD4. Clear to reset FIFO. */
#define FIFO_ENABLE                  1
/** FIFO fill start source (POR 1 = syncron word)
-    0  VDI
-    1  Syncron 0x2DD4
-    3  Always */
#define FIFO_START                   1
/** FIFO Interrupt Trigger level (0-16 bits) (POR 8) */
#define FIFO_TRIGGER                 8
/** Expected data bitrate being received, 9600 baud. Must match the Tx bitrate */
#define BIT_RATE                  0x23
/** Low battery voltage setting (OR with disable timer calibration if needed)*/
#define LBAT_LOWBATT              0x00
/** Wakeup period (not used) */
#define WAKEUP_EXPONENT           0x00
#define WAKEUP_DELAY              0x00
/*@}*/

/** @name Complete command words */
/*@{*/
#define CONFIGURATION   RFM01_CMD_CONFIG | ((RF_BAND & 0x03) << 11) | ((CRYSTAL_LOAD_CAPACITANCE & 0x0F) << 4) | ((BASEBAND_BW & 0x07) << 1) | RFM01_PMBIT_OSC | RFM01_PMBIT_CKOUT
#define FREQUENCY       RFM01_CMD_FREQ   | (CARRIER & 0xFFF)
#define RECEIVER        RFM01_CMD_RECV   | ((VDI_SOURCE & 0x03) << 6) | ((LNA_GAIN & 0x03) << 4) | ((RSSI_THRESHOLD & 0x07) << 1)
#define BATTERY         RFM01_CMD_BATT   | ((CLOCK_FREQ_OUT & 0x07) << 5) | (LBAT_LOWBATT & 0x1F)
#define AFC             RFM01_CMD_AFC    | ((AFC_MODE & 0x03) << 6) | ((AFC_RANGE & 0x03) << 4) | RFM01_AFC_FINE_ENABLE | RFM01_AFC_OUTPUT_ENABLE | RFM01_AFC_CALC_ENABLE | RFM01_AFC_FORCE_ENABLE
#define BASEFILTER      RFM01_CMD_BFILT  | ((DCR_ENABLE & 0x03) << 6) | ((DCR_TYPE & 0x03) << 3) | (DCR_DQDTHRESH & 0x07)
#define DATARATE        RFM01_CMD_RATE   | (BIT_RATE & 0x0FF)
#define FIFO            RFM01_CMD_FIFO   | ((FIFO_TRIGGER & 0x0F) << 4) | ((FIFO_START & 0x03) << 2) | (FIFO_ENABLE & 0x03)
#define WAKEUP          RFM01_CMD_WAKEUP | ((WAKEUP_EXPONENT & 0x1F) << 8) | (WAKEUP_DELAY & 0xFF)       
/*@}*/

/* Wilhelm Krug settings
#define CONFIGURATION   0x896C
#define FREQUENCY       0xA4B0
#define RECEIVER        0xC080
#define BATTERY         0xC2C0
#define AFC             0xC6BF
#define BASEFILTER      0xC4AC
#define DATARATE        0xC891
#define FIFO            0xCE89
*/

/*****************************************************************************/
/* Global Variables */

uint16_t lastRfm01Status = 0;
/** Set of 12 byte bins to count data received values. These will be used to
determine the median data value after a number of valid receives. */
uint8_t dataCount[12];
/** Real Time Clock Global Variable, ticks are 30.5 per second */
uint32_t timeValue;
/*****************************************************************************/
/** @brief Main Program */

int main(void)
{
    hardwareInit();                 /* Initialize the processor specific hardware */
    resetTimer();                   /* Configure the timer */
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
            resetTimer();
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
