/*****************************************************************************
*        Power Control Project Master/Remote Unit
*       Ken Sarkies ksarkies@internode.on.net
*
* File              : power-control-atmega48.h
* Compiler          : AVR-GCC/avr-libc(>= 1.2.5)
* Revision          : $Revision: 0.0 $
* Updated by        : $ author K. Sarkies 206/06/2011 $
*
* Target platform   : ATMega48
*
* Microcontroller settings for specific device
*
**********************************************************/
#ifndef _POWER_CONTROL_ATMEGA48_H_
#define _POWER_CONTROL_ATMEGA48_H_

/* Microcontroller SPI port settings */
/* Microcontroller SPI port settings */
#define SPI_PORT    PORTD
#define SPI_PIN     PIND
#define SPI_SETUP   DDRD
#define CS_PORT     PORTD
#define CS_SETUP    DDRD
#define SDI         7
#define nIRQ        6
#define SCK         5
#define nSEL        4
#define SDO         3

#endif

