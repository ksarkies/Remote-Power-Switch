/*****************************************************************************
*        Power Control Project Remote Unit
*       Ken Sarkies ksarkies@internode.on.net
*
* File              : power-control-remote.h
* Compiler          : AVR-GCC/avr-libc(>= 1.2.5)
* Revision          : $Revision: 0.0 $
* Updated by        : $ author K. Sarkies 06/06/2011 $
*
* Target platform   : ATTiny44, ATTiny2313, ATMega48
*
* HopeRf RFM02 FSK Wireless Module Library
* The RF02 is a transmitter chip whose command set differs
* from the RF01 and RF12 command sets.
*
**********************************************************/
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
#ifndef _POWER_CONTROL_REMOTE_H_
#define _POWER_CONTROL_REMOTE_H_

/**********************************************************/
/** @name RFM01 SPI commands */
/*@{*/

#define RFM02_CMD_CONFIG		0x8000
#define RFM02_CMD_POWERMGMT		0xC000
#define RFM02_CMD_FREQ			0xA000
#define RFM02_CMD_RATE			0xC800
#define RFM02_CMD_POWER_SET		0xB0
#define RFM02_CMD_SYNC          0xC200
#define RFM02_CMD_SLEEP     	0xC400
#define RFM02_CMD_WAKEUP		0xE000
#define RFM02_CMD_DATA_TX		0xC6
#define RFM02_CMD_STATUS_READ	0xCC00
/*@}*/
/** @name
RF02 Power Management Bits
- bit 0   enable clock out
- bit 1   enable wakeup timer
- bit 2   enable low battery detector
- bit 3   enable power amplifier
- bit 4   enable synthesizer
- bit 5   enable crystal oscillator
- bit 6   automatically power up amplifier (bit 3 must be 0)
- bit 7   automatically power up crystal oscillator and
        synthesizer (bits 4,5 must be 0).*/
/*@{*/
#define RFM02_PMBIT_OSC         _BV(5)
#define RFM02_PMBIT_SYNTH       _BV(4)
#define RFM02_PMBIT_AMP         _BV(3)
#define RFM02_PMBIT_BATT        _BV(2)
#define RFM02_PMBIT_TIMER       _BV(1)
#define RFM02_PMBIT_CKOUT       _BV(0)
/*@}*/
/** @name
RFM02 TX Sync and Low Battery setting Bits */
/*@{*/
#define RFM02_TXSYN_DISTIMCAL   _BV(7)
#define RFM02_TXSYN_TXSYNC      _BV(5)
/*@}*/
/** @name
PLL bandwidth modes. Higher bandwidth allows higher data
rate but more phase noise (POR 2 ??).
- 0 D240h   19.2
- 1 D2C0h   38.4
- 2 D200h   68.9
- 3 D280h  115.2 */
/*@{*/
#define RFM02_PLL_MODE1             0xD240
#define RFM02_PLL_MODE2             0xD2C0
#define RFM02_PLL_MODE3             0xD200
#define RFM02_PLL_MODE4             0xD280
/*@}*/

/**********************************************************/
/* Sets the Configuration of the Transmitter for RFM02 */
void rfm02Configure(void);
/* Write a Command to the RFM02 over SPI */
uint16_t writeCMD(uint16_t command, uint8_t n);
/* Start Transmission of Data to the RFM02 over SD */
void startDataTx(uint8_t dataRate);
/* Write a Data Start Command to the RFM02 over SPI */
void writeDataCMD(void);
/* Transmit Byte Data to the RFM02 over SDI */
void writeByteTx(uint8_t data);
/* End Transmission of Data to the RFM02 over SDI */
void stopDataTx(void);
/* Bitbang the data on the SPI */
void writeSPI(uint8_t n);
/* Initialize the SPI interface */
void initSPI(void);
/* Single scan for the pressed key */
uint8_t scan(void);
/* Initialize the keypad scan ports */
void initKbd(void);

#endif
