Remote Power Switch Rx Firmware
-------------------------------

The unit enters sleep mode until woken by a received character or the timer.
It collects all received characters, filters out invalid characters, and counts
instances of each valid character. After 10 valid characters have been received,
the character with the highest count is considered to be the correct
transmission. This is a form of maximum likelihood estimator.

The ATMega48 AVR microcontroller must have 8MHz internal RC oscillator and
no other fuse bits set except for the serial programming enable.

More information has been posted on:

www.jiggerjuice.info/electronics/projects/remotecontrol/remote-control-switch.html

K. Sarkies

