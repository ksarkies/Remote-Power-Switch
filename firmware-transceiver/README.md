Remote Power Switch Transceiver Firmware
----------------------------------------

The Transceiver unit provides a serial link with output at RS232 or logic levels
for the Battery Management System to communicate with a monitoring PC.

The serial bit rate at both ends of the link needs to be the same as that set in
the firmware. This can be changed by recompiling the firmware. The RF baud rate
does not need to be the same as the serial baud rate but keeping these the same
simplifies the buffer allocations. The transceiver can handle bursts up to the
length of the serial buffers.

The AVR C library rfm12lib by Hansinator, Tixiv and Soeren provides the software
support for the RFM12 modules. The interrupt driven UART library developed by
Peter Fleury is also used.

More information is found on:
www.jiggerjuice.info/electronics/projects/remotecontrol/serial-link-transceiver.html

K. Sarkies
20 October 2014

TODO: Add sleep modes for AVR and RFM12 after 10 seconds inactivity. Wake on
      USART and INT0.
