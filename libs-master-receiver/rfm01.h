/*        Power Control Project Master Unit
*       Ken Sarkies ksarkies@trinity.asn.au
*
* File              : rfm01.h
* Compiler          : AVR-GCC/avr-libc(>= 1.2.5)
* Revision          : $Revision: 0.0 $
* Updated by        : $ author K. Sarkies 11 October 2014 $
*
* Target platform   : ATMega48
*
* HopeRf RFM01 FSK Wireless Receiver Module Library
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

#ifndef _RFM01_H_
#define _RFM01_H_

#include <inttypes.h>

/**********************************************************/
/** @name RFM01 SPI commands */
/*@{*/
#define RFM01_CMD_CONFIG        0x8000
#define RFM01_CMD_FREQ          0xA000
#define RFM01_CMD_RECV	        0xC000
#define RFM01_CMD_BATT          0xC200
#define RFM01_CMD_AFC           0xC600
#define RFM01_CMD_BFILT         0xC420
#define RFM01_CMD_RATE			0xC800
#define RFM01_CMD_FIFO          0xCE00
#define RFM01_CMD_WAKEUP		0xE000
#define RFM01_CMD_STATUS_READ	0x0000
/*@}*/
/** @name
RFM01 Configuration Bits
- bit 0   disable clock out
- bit 8   enable crystal oscillator
- bit 9   enable wakeup timer
- bit 10  enable low battery detector */
/*@{*/
#define RFM01_PMBIT_CKOUT       _BV(0)
#define RFM01_PMBIT_OSC         _BV(8)
#define RFM01_PMBIT_TIMER       _BV(9)
#define RFM01_PMBIT_BATT        _BV(10)
/*@}*/

/** @name
RFM01 AFC Enable Settings
- bit 0  enable addition of frequency offset register to the PLL
- bit 1  enable frequency offset register
- bit 2  enable high accuracy mode
- bit 3  force addition of frequency offset register to the PLL (must be reset to zero afterwards) */
/*@{*/
#define RFM01_AFC_CALC_ENABLE   _BV(0)
#define RFM01_AFC_OUTPUT_ENABLE _BV(1)
#define RFM01_AFC_FINE_ENABLE   _BV(2)
#define RFM01_AFC_FORCE_ENABLE  _BV(3)
/*@}*/

/** @name RFM01 Output and FIFO Mode Enables */
/*@{*/
#define RFM01_FIFO_ENABLE       _BV(0)
#define RFM01_FIFO_SYNCHRON_EN  _BV(1)
/*@}*/

void receiverConfigure(void);
void resetFIFO(void);
uint16_t writeCMD(uint16_t command, uint8_t n);
uint16_t readDataByte(void);
void writeSPI(uint8_t n);
void initSPI(void);

#endif
