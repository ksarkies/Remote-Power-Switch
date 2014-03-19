/*****************************************************************************
*        Power Control Project Remote Unit
*       Ken Sarkies ksarkies@trinity.asn.au
*
* File              : power-control-attiny2313.h
* Compiler          : AVR-GCC/avr-libc(>= 1.2.5)
* Revision          : $Revision: 0.0 $
* Updated by        : $ author K. Sarkies 06/06/2011 $
*
* Target platform   : ATTiny2313
*
* Microcontroller settings for a specific device
*
**********************************************************

The following settings allow the device to be used with column
and row ports together as a block, and SPI as a block. The
exception is the nSEL line. Choice of ports is also constrained
by the circuit layout.

PD2		INT0
PD4-6	ROW
PB5-7	SPI (programming)
PA2		RESET
PB0-3	COLUMN
PD0		SCK
PD1		SDI
PD3		nIRQ
PB4		nSEL

**********************************************************/

#ifndef _POWER_CONTROL_ATTINY2313_H_
#define _POWER_CONTROL_ATTINY2313_H_

/* Microcontroller SPI port settings */
#define SPI_PORT    PORTD
#define SPI_PIN     PIND
#define SPI_SETUP   DDRD
#define CS_PORT     PORTB
#define CS_SETUP    DDRB
#define SDI         1
#define nIRQ        2
#define SCK         0
#define nSEL        4

/* Ports for keypad columns and rows. Columns use upper 4 bits
(bit 7 not available), rows use lower 3 bits. */
#define  FIRST_COL  0
#define  COLUMNS    PORTB
#define  COLUMND    DDRB
#define  COL_BITS   (0x0F << FIRST_COL)
#define  FIRST_ROW  4
#define  ROWS       PORTD
#define  ROWD       DDRD
#define  ROWIN      PIND
#define  ROW_BITS   (0x07 << FIRST_ROW)
#define  INT_SETUP  DDRD
#define  INT_PORT   PORTD
#define  INT_BIT    3
#define  KBD_INTERRUPT INT1_vect

#endif

