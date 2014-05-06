Remote Power Switch Tx PCB
--------------------------

This board is designed to fit into a 65mm by 130mm hand case, purchased from
Jaycar, with a 12 digit keypad. It has a microcontroller in sleep mode that
is woken up when a keypress is activated.

The keypad is of unknown manufacturer, supplied by Futurlec. It has a 7 pin
connector underneath, below the top row of buttons. The board will 
need to be redesigned to suit other keypads, taking care about physical
mounting. This keypad is to be mounted underneath the board and facing so that
the board partly covers it. The right-angle SPI connector then mounts on top of
the board. The keypad can also be mounted on top of the board facing the
opposite direction, with the SPI connector mounted underneath also facing
in the opposite direction.

The row and column ports are set with rows with high-level pullup and columns
set to zero so that a connection between any row and column will result in the
interrupt line being pulled low via the diode network.

The unit has survived for over 12 months without a change of batteries.

More information will be posted on www.jiggerjuice.info

K. Sarkies

