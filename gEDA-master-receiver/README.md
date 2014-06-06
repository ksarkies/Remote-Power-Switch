Remote Power Switch Rx PCB
--------------------------

This board has a dual purpose; serving as a receiver for the power switch and as
an RF to serial converter. The board is designed to allow cutting to separate
the microcontroller and the switch parts. Provision for a ribbon cable
connection between the two is given so that boards may be interchanged with
different types. It uses an Atmel microcontroller and a RFM01 Hope RF 433MHz FM
receiver. The alternate configuration takes an RFM12 transceiver with different
jumper settings. The Receiver configuration uses jumpers labelled R, the
Transceiver configuration jumpers are labelled X.

The switches are P-type MOSFETs with low speed BJT-resistor drivers for
simplicity. The microcontroller is a low cost ATMega48 or pin compatible
ATMEL device with provision for a crystal oscillator if needed. The serial
interface is provided by a MAX232 RS-232 driver, although the serial interface
could be simple logic level if desired by omitting the MAX232 and jumpering
between its input and output pins. The serial connector is a SIL header
compatible with the ET series of boards. An SPI programming connector is also
provided in a form compatible with other projects. The vertical version of this
must be used.

More information has been posted on:

www.jiggerjuice.info/electronics/projects/remotecontrol/remote-control-switch.html

K. Sarkies

