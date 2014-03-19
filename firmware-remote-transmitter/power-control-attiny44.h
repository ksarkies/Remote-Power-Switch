/*****************************************************************************
*        Power Control Project Master Unit
*       Ken Sarkies ksarkies@trinity.asn.au
*
* File              : power-control-attiny44.h
* Compiler          : AVR-GCC/avr-libc(>= 1.2.5)
* Revision          : $Revision: 0.0 $
* Updated by        : $ author K. Sarkies 20/05/2011 $
*
* Target platform   : ATTiny44
*
* Microcontroller settings for specific device
*
**********************************************************/
#ifndef _POWER_CONTROL_ATTINY44_H_
#define _POWER_CONTROL_ATTINY_H_

/* Microcontroller SPI port settings */
#define SPI_PORT    PORTA
#define SPI_PIN     PINA
#define SPI_SETUP   DDRA
#define CS_PORT     PORTA
#define CS_SETUP    DDRA
#define SDI         6
#define nIRQ        5
#define SCK         4
#define nSEL        7

/* Ports for keypad columns and rows. Columns use lower 4 bits,
rows use lower 2 bits (not enough pins for all three). */
#define  COLUMNS    PORTA
#define  COLUMND    DDRA
#define  COL_BITS   0x0F
#define  ROWS       PORTB
#define  ROWD       DDRB
#define  ROWIN      PINB
#define  ROW_BITS   0x03
#define  INT_SETUP  DDRB
#define  INT_PORT   PORTB
#define  INT_BIT    _BV(2)

#endif

