v 20121123 2
C 40000 40000 0 0 0 title-B.sym
C 41600 47600 1 90 0 vcc-1.sym
C 41300 47500 1 270 0 gnd-1.sym
N 42700 47800 43400 47800 4
N 42700 47400 43200 47400 4
N 43200 48100 43200 47400 4
N 41900 48000 41800 48000 4
C 41700 48900 1 270 0 resistor-1.sym
{
T 42100 48600 5 10 0 0 270 0 1
device=RESISTOR
T 41700 48900 5 10 0 0 90 0 1
footprint=R025
T 41700 48700 5 10 1 1 180 0 1
value=10K
T 41700 48900 5 10 1 1 180 0 1
refdes=R2
}
C 41600 48900 1 0 0 vcc-1.sym
C 48500 48900 1 0 0 gnd-1.sym
C 46900 49200 1 180 1 vcc-1.sym
C 46900 49200 1 270 1 connector6-3.sym
{
T 48550 49500 5 10 0 0 270 6 1
device=CONNECTOR_6
T 48750 49500 5 10 0 0 270 6 1
footprint=SIP6N
T 47400 49800 5 10 1 1 0 0 1
refdes=Conn1
}
N 46900 45600 49400 45600 4
N 46900 45800 49400 45800 4
N 46900 46000 49400 46000 4
C 42700 48100 1 180 0 header-8.sym
{
T 42400 47200 5 10 1 1 180 0 1
refdes=RF1
T 41100 43400 5 10 0 0 180 0 1
device=RFM02
T 43100 46700 5 10 1 1 180 0 1
value=HopeRF RFM02
T 42700 48100 5 10 0 0 0 0 1
footprint=RFM02
}
N 41600 47400 41900 47400 4
N 41600 47800 41900 47800 4
N 41900 47600 41800 47600 4
N 41800 47600 41800 46800 4
N 41800 46800 44500 46800 4
N 42700 47600 43800 47600 4
N 43800 47600 43800 47000 4
N 43800 47000 44500 47000 4
N 43200 48100 47400 48100 4
N 43400 47800 43400 46200 4
N 43400 46200 44500 46200 4
N 46900 46400 47400 46400 4
N 47400 46400 47400 48100 4
N 49400 46200 46900 46200 4
N 49400 46400 49000 46400 4
N 49000 44600 49000 46400 4
N 49000 44600 43600 44600 4
N 43600 44600 43600 45800 4
N 43600 45800 44500 45800 4
N 49400 46600 48800 46600 4
N 48800 46600 48800 44800 4
N 48800 44800 43800 44800 4
N 43800 44800 43800 45600 4
N 43800 45600 44500 45600 4
N 49400 46800 48600 46800 4
N 48600 46800 48600 45400 4
N 48600 45400 46900 45400 4
C 46800 43700 1 0 0 1N4148.sym
{
T 47202 44201 5 10 1 1 0 0 1
refdes=D1
T 47000 45100 5 10 0 0 0 0 1
device=DIODE
T 47000 44900 5 10 0 0 0 0 1
footprint=ALF300
T 47200 43900 5 10 1 1 0 0 1
value=1N4148
}
C 45800 43700 1 0 0 1N4148.sym
{
T 46202 44201 5 10 1 1 0 0 1
refdes=D2
T 46000 45100 5 10 0 0 0 0 1
device=DIODE
T 46000 44900 5 10 0 0 0 0 1
footprint=ALF300
T 46200 43900 5 10 1 1 0 0 1
value=1N4148
}
N 46000 44400 46000 44800 4
N 47000 44400 47000 45400 4
N 43400 43800 47000 43800 4
N 43400 43800 43400 46000 4
N 43400 46000 44500 46000 4
C 44600 43700 1 0 0 1N4148.sym
{
T 45002 44201 5 10 1 1 0 0 1
refdes=D3
T 44800 45100 5 10 0 0 0 0 1
device=DIODE
T 44800 44900 5 10 0 0 0 0 1
footprint=ALF300
T 45000 43900 5 10 1 1 0 0 1
value=1N4148
}
N 44800 44400 44800 44600 4
C 44200 45500 1 270 0 gnd-1.sym
C 46900 47000 1 270 1 vcc-1.sym
N 47400 49200 47400 48400 4
N 47400 48400 44100 48400 4
N 44100 48400 44100 47200 4
N 44100 47200 44500 47200 4
N 47700 49200 47700 47000 4
N 47700 47000 46900 47000 4
N 46900 46800 48000 46800 4
N 48000 46800 48000 49200 4
N 46900 46600 48300 46600 4
N 48300 46600 48300 49200 4
C 51700 45300 1 90 0 keypad.sym
{
T 50200 47500 5 10 1 1 180 6 1
refdes=KEYPAD
T 49250 46600 5 10 0 0 90 0 1
device=Keypad-3x4
T 49050 45800 5 10 0 0 90 0 1
footprint=Keypad-3x4
}
C 42600 49200 1 0 0 connector2-2.sym
{
T 43300 50500 5 10 1 1 0 6 1
refdes=Conn2
T 42900 50450 5 10 0 0 0 0 1
device=CONNECTOR_2
T 42900 50650 5 10 0 0 0 0 1
footprint=SIP2N
}
C 42300 49700 1 270 0 gnd-1.sym
C 42600 49800 1 90 0 vcc-1.sym
C 44200 48400 1 90 0 resistor-1.sym
{
T 43800 48700 5 10 0 0 90 0 1
device=RESISTOR
T 44200 48400 5 10 0 0 270 0 1
footprint=R025
T 44200 48600 5 10 1 1 0 0 1
value=10K
T 44300 48800 5 10 1 1 0 0 1
refdes=R1
}
C 43900 49300 1 0 0 vcc-1.sym
C 45600 48400 1 90 0 capacitor-1.sym
{
T 44900 48600 5 10 0 0 90 0 1
device=CAPACITOR
T 46000 49000 5 10 1 1 180 0 1
refdes=C1
T 44700 48600 5 10 0 0 90 0 1
symversion=0.1
T 45700 48600 5 10 1 1 0 0 1
value=0.1uF
T 45600 48400 5 10 0 0 0 0 1
footprint=ACY200
}
C 45500 49600 1 180 0 gnd-1.sym
T 51500 41000 9 10 1 0 0 0 1
Remote Power Switch Remote Transmitter
T 54200 40100 9 10 1 0 0 0 1
K. Sarkies 19/04/2014
T 50200 40200 9 10 1 0 0 0 1
1
T 52000 40200 9 10 1 0 0 0 1
1
C 44400 44700 1 0 0 ATtiny2313.sym
{
T 46800 47600 5 10 1 1 0 6 1
refdes=U1
T 44700 47950 5 10 0 0 0 0 1
device=ATtiny2313
T 44700 48150 5 10 0 0 0 0 1
footprint=DIP20
}
T 55000 40400 9 10 1 0 0 0 1
0.0
