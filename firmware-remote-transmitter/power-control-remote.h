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

#endif
