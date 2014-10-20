/* Baudrate setting calculator for ATMega48/88/168/328 series
for the Peter Fleury AVR UART library:
http://homepage.hispeed.ch/peterfleury/avr-software.html
Based on avr-libc setbaud.h, with acknowledgement.

NOTE: ensure that F_CPU and BAUD are defined before including this header.

Define BAUD_TOL for testing of baudrate error, and
NO_DOUBLE_RATE if the processor doesn't support this setting. */

#ifndef BAUD_TOL
    #define BAUD_TOL  2
#endif

#if !defined(F_CPU)
    #error "F_CPU must be defined"
#elif !defined(BAUD)
    #error "BAUD must be defined"
#elif (F_CPU == 0)
    #error "F_CPU must be nonzero"
#elif (BAUD == 0)
    #error "BAUD must be nonzero"
#else

/* Computed settings with round up capability */
#define BAUD_SETTING_LOW    ((2UL*F_CPU/(BAUD*16UL))-1UL)/2UL
#define BAUD_SETTING_HIGH   ((2UL*F_CPU/(BAUD*8UL))-1UL)/2UL

/* Value by 256 to give more precision */
#define BAUD_ACTUAL_LOW     (16UL*F_CPU)/(BAUD_SETTING_LOW+1UL)
#define BAUD_ACTUAL_HIGH    (32UL*F_CPU)/(BAUD_SETTING_HIGH+1UL)

/* Compute absolute error in baudrates */
#if (BAUD_ACTUAL_LOW < 256UL*BAUD)
    #define BAUD_ERROR_LOW  (256UL*BAUD - BAUD_ACTUAL_LOW)
#else
    #define BAUD_ERROR_LOW  (BAUD_ACTUAL_LOW - 256UL*BAUD)
#endif

#if (BAUD_ACTUAL_HIGH < 256UL*BAUD)
    #define BAUD_ERROR_HIGH (256UL*BAUD - BAUD_ACTUAL_HIGH)
#else
    #define BAUD_ERROR_HIGH (BAUD_ACTUAL_HIGH - 256UL*BAUD)
#endif

/* Set the baudrate setting if no double rate possible */
/* otherwise to the one with the smallest error */
#if (defined(NO_DOUBLE_RATE) || (BAUD_ERROR_HIGH > BAUD_ERROR_LOW))
    #define BAUD_SETTING    BAUD_SETTING_LOW
    #if (BAUD_ERROR_LOW > (BAUD_TOL*BAUD*256)/100)
        #warning "Baudrate out of specification"
    #endif
#else
    #define BAUD_SETTING    BAUD_SETTING_HIGH | 0x8000
    #if (BAUD_ERROR_HIGH > (BAUD_TOL*BAUD*256)/100)
        #warning "Baudrate out of specification"
    #endif
#endif

#endif

