Remote Power Switch Tx PCB
--------------------------

This board is designed to fit into a 65mm by 130mm hand case, purchased from
Jaycar, with a 12 digit keypad. It has an Atmel microcontroller in sleep mode
that is woken up when a keypress is activated. An RFM02 Hope RF 433MHz
transmitter unit is also in sleep mode until needed.

The keypad is of unknown manufacturer, supplied by Futurlec. It has a 7 pin
connector underneath, below the top row of buttons. The board will 
need to be redesigned to suit other keypads, taking care about physical
mounting. This keypad can be mounted underneath the board and facing so that
the board partly covers it, or ir could be mounted on top of the board facing
the opposite direction if it is mounted high enough for other components to be
mounted underneath it. The right-angle SPI connector can be mounted on top of
the board under the keypad, or underneath facing in the opposite direction.
Space constraints require the RF board to sit partly underneath the keypad.

The aerial for the RF board needs to be 173mm which is longer than the case. To
provide an efficient aerial it is made up of a 120mm length of wire with 6
turns wound over a 0.25W resistor (acting purely as a former) to make a bottom
loaded quarter wave aerial.

The row and column ports are set with the rows having high-level pullup and the
columns set to zero. A connection between any row and column will then result
in the interrupt line being pulled low via the diode network.

The prototype units have survived for over 12 months without a change of
batteries.

More information has been posted on:

www.jiggerjuice.info/electronics/projects/remotecontrol/remote-control-switch.html

K. Sarkies
30 May 2014
