Remote Power Switch Transceiver Firmware
----------------------------------------

The Transceiver unit provides a serial link with input and output at RS232 or
logic levels.

The serial bit rate at both ends of the link needs to be the same as that set in
the firmware. This can be changed by recompiling the firmware. The transceiver
can handle bursts up to the length of the serial buffers. These are made larger
for AVRs with larger RAM.

The throughput of the unit is restricted by buffer size and AVR speed. A large
amount of serial data sent in a burst can result in loss.

The AVR C library rfm12lib by Hansinator, Tixiv and Soeren provides the software
support for the RFM12 modules. The interrupt driven UART library developed by
Peter Fleury is also used.

More information is found on:
www.jiggerjuice.info/electronics/projects/remotecontrol/serial-link-transceiver.html

The ATMega48 series is used. The USART buffer sizes are:
ATMega48    128
ATMega88    256
ATMega168   256
ATMega328   1024

K. Sarkies
20 April 2015

TODO: Add sleep modes for AVR and RFM12 after 10 seconds inactivity. Wake on
      USART and INT0.

TODO: Attempts were made to improve throughput, without success, for use with
the Battery Management System. A request for a long directory listing results
in the latter losing characters. Sending the BMS general data at low priority
and the directory listing at high priority made no difference. The most
likely explanation is the limited buffering available in the transceiver AVR.

Some improvements would be:
- Add USART flow control.
- Change the transceiver SCP interactions to hardware rather than software.

