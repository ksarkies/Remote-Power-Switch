Remote Power Switch Tx Firmware
-------------------------------

The unit enters sleep mode until a keyboard interrupt occurs. It then scans
the keys to determine which one was pressed. Debounce is performed, then the
digit is transmitted via the RFM02 module. This process continues as long
as the key is pressed.

The clock of the ATTiny2313 must be set to internal calibrated 8MHz. The clock
divide by 8 must be unset.

More information has been posted on [Jiggerjuice](www.jiggerjuice.info/electronics/projects/remotecontrol/remote-control-switch.html).

K. Sarkies

