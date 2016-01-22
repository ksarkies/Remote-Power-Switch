Remote Power Switch
-------------------

This small project makes use of Hope RF modules sold by Futurlec and others
to enable remote switching of low voltage power.

(NOTE: These modules seem to be unobtainable. However similar modules such as
the ALPHA-TRX433S, a rebadged version of the 433MHz RFM12B, are available.
The project needs to be redesigned for these and has not been further
developed.)

The background is a 12V power distribution system in a building which is
isolated from the grid and uses solar batteries for all power. The low voltage
used in the system requires care to be taken in regard to losses, so it was
decided to avoid using wall switches which would require additional cable length
with its attendant loss.

A small board uses an RFM02 transmitter module in a hand unit with an Atmel
ATTiny2313. A receiver board using an RFM01 receiver module is mounted at the
power distribution box. While the RF modules supposedly used SPI, the RFM02
modules actually uses a modification of this, meaning that an inbuilt SPI
peripheral on the microcontroller cannot be used. The transmission system is
FM in the 433MHz band.

The system operates by transmitting a single ASCII number continuously when a
button is pressed on the remote unit. The receiver will collect all received
symbols and uses a set of "bins", one for each possible symbol, to hold a
count of the number of each symbol received. A decision is made after 10 symbols
have arrived by selecting the symbol with the most counts. No more decisions are
made until a preset delay has elapsed following the last reception.

More information is available on:
www.jiggerjuice.info/electronics/projects/remotecontrol/remote-control-switch.html

The receiver board is designed to take RFM12 transceiver modules so that the
hardware can double as a serial communications system between two base stations.

More information is available on:
www.jiggerjuice.info/electronics/projects/remotecontrol/serial-link-transceiver.html

K. Sarkies
4 January 2015

