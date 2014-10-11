/*        Power Control Project Master Unit
*       Ken Sarkies ksarkies@trinity.asn.au
*
* File              : power-control-master.h
* Compiler          : AVR-GCC/avr-libc(>= 1.2.5)
* Revision          : $Revision: 0.0 $
* Updated by        : $ author K. Sarkies 20/05/2011 $
*
* Target platform   : ATMega48
*
* Parameter settings for the HopeRF RFM01.
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

#ifndef _POWER_CONTROL_MASTER_H_
#define _POWER_CONTROL_MASTER_H_

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

#endif //_POWER_CONTROL_MASTER_H_
