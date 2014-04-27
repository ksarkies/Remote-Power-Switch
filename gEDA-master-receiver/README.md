Remote Power Switch Rx PCB
--------------------------

This board has a dual purpose; serving as a receiver for the power switch and as
an RF to serial converter. The board is designed to allow cutting to separate
the microcontroller and the switch parts. Provision for a ribbon cable
connection between the two is given so that different boards may be substituted.

The switches are P-type MOSFETs with low speed BJT-resistor drivers for
simplicity. The microcontroller is a low cost ATtiny2313 or pin compatible
ATMEL device with provision for a crystal oscillator if needed. The serial
interface is provided by a MAX232 RS-232 driver, although the serial interface
could be simple logic level if desired by jumpering between the MAX232
input and output pins. The serial connector is a SIL header compatible with
the ET series of boards.

More information will be posted on www.jiggerjuice.info

K. Sarkies

