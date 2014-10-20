/* baudrate setting calculator for ATMega48/88/168/328 series */

/* Computed settings with round up capability */
#define BAUD_SETTING_LOW    ((2UL*F_CPU/(BAUD*16UL))-1UL)/2UL
#define BAUD_SETTING_HIGH   ((2UL*F_CPU/(BAUD*8UL))-1UL)/2UL

/* Value by 256 to give more precision */
#define BAUD_ACTUAL_LOW     (16UL*F_CPU)/(BAUD_SETTING_LOW+1UL)
#define BAUD_ACTUAL_HIGH    (32UL*F_CPU)/(BAUD_SETTING_HIGH+1UL)

/* Compute absolute error in BAUDs */
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

/* Set the BAUD setting to the one with the smallest error */
#if (BAUD_ERROR_HIGH > BAUD_ERROR_LOW)
    #define BAUD_SETTING    BAUD_SETTING_LOW
#else
    #define BAUD_SETTING    BAUD_SETTING_HIGH | 0x8000
#endif

