Remote Power Switch
-------------------

This small project makes use of Hope RF modules sold by Futurlec and others
to enable remote switching of power.

The background is a 12V power distribution system in a building which is
isolated from the grid and uses solar batteries for all power. The low voltage
used in the system requires care to be taken in regard to losses, so it was
decided to avoid using wall switches which would require additional cable length
with its attendant loss.

The first prototype uses a transmitter module in a hand unit managed by an Atmel
ATTiny2312, and a receiver module in the master station mounted at the power
distribution box. While the RF modules supposedly used SPI, the transmitter
modules actually used a modification of this, meaning that an inbuilt SPI
peripheral on the microcontroller couldn't be used. The transmission system is
FM in the 433MHz band.

The system operates by transmitting a single ASCII number continuously when a
button is pressed on the remote unit. The receiver will collect all received
symbols and uses a set of "bins", one for each possible symbol, to hold a
count of the number of each symbol received. A decision is made after 10 symbols
have arrived by selecting the symbol with the most counts. No more decisions are
made until a preset delay has elapsed following the last reception.

A second prototype is planned to use transceiver modules, the hardware doubling
as a serial communications system between two base stations.

More information will be posted on www.jiggerjuice.info

K. Sarkies
19 March 2014

