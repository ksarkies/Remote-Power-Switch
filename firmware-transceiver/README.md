Remote Power Switch Transceiver Firmware
----------------------------------------

The Transceiver unit provides a serial link with output at RS232 or logic levels
for the Battery Management System to communicate with a monitoring PC.

The serial bit rate at both ends of the link needs to be the same as that set in
the firmware. This can be changed by recompiling the firmware. The transceiver
can handle bursts up to the length of the serial buffers. These are made larger
for AVRs with larger RAM.

The throughput of the unit is restricted by buffer size and possibly by AVR
speed. A large amount of serial data sent in a burst can result in loss.

The AVR C library rfm12lib by Hansinator, Tixiv and Soeren provides the software
support for the RFM12 modules. The interrupt driven UART library developed by
Peter Fleury is also used.

More information is found on:
www.jiggerjuice.info/electronics/projects/remotecontrol/serial-link-transceiver.html

The ATMega48 series is used. Buffer sizes are
ATMega48    128
ATMega88    256
ATMega168   256
ATMega328   512

K. Sarkies
7 April 2015

TODO: Add sleep modes for AVR and RFM12 after 10 seconds inactivity. Wake on
      USART and INT0.
