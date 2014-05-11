v 20121123 2
C 40000 40000 0 0 0 title-B.sym
C 46800 47500 1 270 0 vcc-1.sym
C 42900 41800 1 270 0 capacitor-2.sym
{
T 43600 41600 5 10 0 0 270 0 1
device=POLARIZED_CAPACITOR
T 42700 41500 5 10 1 1 0 0 1
refdes=C1
T 43800 41600 5 10 0 0 270 0 1
symversion=0.1
T 42600 41000 5 10 1 1 0 0 1
value=47uF
T 42900 41800 5 10 0 0 270 0 1
footprint=RCY100P
}
N 46700 40900 42200 40900 4
N 44500 41800 42200 41800 4
N 42200 41800 42200 41100 4
N 45000 46500 45000 48100 4
N 45000 48100 49200 48100 4
N 46000 46900 45500 46900 4
N 45500 46900 45500 47900 4
N 45500 47900 49200 47900 4
N 46800 47100 48700 47100 4
N 48700 47100 48700 47700 4
N 48700 47700 49200 47700 4
N 46800 46900 48900 46900 4
N 48900 46900 48900 47500 4
N 48900 47500 49200 47500 4
N 46000 47100 45800 47100 4
N 45800 47100 45800 47700 4
N 45800 47700 48400 47700 4
N 48400 47700 48400 47300 4
N 48400 47300 49200 47300 4
C 48500 44100 1 0 0 crystal-1.sym
{
T 48700 44600 5 10 0 0 0 0 1
device=CRYSTAL
T 48700 44800 5 10 0 0 0 0 1
symversion=0.1
T 48500 44100 5 10 0 0 0 0 1
footprint=CRYSTAL 300
T 48800 44400 5 10 1 1 0 0 1
refdes=X1
T 49300 44000 5 10 1 1 0 0 1
value=8MHz
}
C 48700 43300 1 90 0 capacitor-1.sym
{
T 48000 43500 5 10 0 0 90 0 1
device=CAPACITOR
T 47800 43500 5 10 0 0 90 0 1
symversion=0.1
T 48700 43300 5 10 0 0 0 0 1
footprint=CK05_type_Capacitor
T 48900 43600 5 10 1 1 180 0 1
refdes=C4
T 48700 43300 5 10 1 1 0 0 1
value=22pF
}
C 49400 43300 1 90 0 capacitor-1.sym
{
T 48700 43500 5 10 0 0 90 0 1
device=CAPACITOR
T 48500 43500 5 10 0 0 90 0 1
symversion=0.1
T 49400 43300 5 10 0 0 0 0 1
footprint=CK05_type_Capacitor
T 49600 43600 5 10 1 1 180 0 1
refdes=C5
T 49400 43300 5 10 1 1 0 0 1
value=22pF
}
C 48400 43000 1 0 0 gnd-1.sym
N 48500 44200 48500 44800 4
N 48500 44800 49200 44800 4
C 49100 43000 1 0 0 gnd-1.sym
N 49200 44600 49200 44200 4
C 42200 45800 1 90 0 gnd-1.sym
C 41900 44200 1 270 1 vcc-1.sym
C 41900 44200 1 0 1 connector6-3.sym
{
T 41600 46050 5 10 0 0 0 6 1
footprint=SIP6N
T 41100 46200 5 10 1 1 0 0 1
refdes=CONN14
}
N 49200 45400 43100 45400 4
N 43100 45400 43100 45600 4
N 43100 45600 41900 45600 4
N 49200 45200 42800 45200 4
N 42800 45200 42800 45300 4
N 42800 45300 41900 45300 4
N 41900 45000 49200 45000 4
N 49200 48300 48400 48300 4
N 48400 48300 48400 49300 4
N 48400 49300 53900 49300 4
N 49200 46000 48200 46000 4
N 48200 46000 48200 49600 4
N 48200 49600 53300 49600 4
N 49200 45800 47900 45800 4
N 47900 45800 47900 49900 4
N 47900 49900 53500 49900 4
N 49200 45600 47600 45600 4
N 47600 45600 47600 50200 4
N 47600 50200 53700 50200 4
N 51800 47700 54900 47700 4
N 53300 46900 53300 49600 4
N 53700 50200 53700 46500 4
C 52100 45000 1 90 0 gnd-1.sym
C 51800 45600 1 270 0 vcc-1.sym
C 51800 45900 1 270 0 vcc-1.sym
N 51800 47500 54900 47500 4
N 51800 47300 54900 47300 4
N 51800 47100 54900 47100 4
N 51800 46900 53000 46900 4
N 51800 46700 52800 46700 4
N 54900 46900 53300 46900 4
N 53700 46500 54900 46500 4
N 51800 46500 52400 46500 4
N 41900 44700 42800 44700 4
N 42800 44700 42800 42700 4
N 42800 42700 52400 42700 4
N 52400 42700 52400 46500 4
C 41300 41200 1 180 1 pwrjack-1.sym
{
T 41300 40700 5 10 1 1 180 6 1
refdes=CONN13
T 41300 41200 5 10 0 0 0 0 1
footprint=Female_Barrel_DC0202
}
C 44500 41200 1 0 0 lm7805.sym
{
T 46100 42500 5 10 0 0 0 0 1
device=78L05
T 45900 42200 5 10 1 1 0 6 1
refdes=U2
T 44500 41200 5 10 0 0 0 0 1
footprint=TO220SW
}
C 46600 40600 1 0 0 gnd-1.sym
C 46500 41800 1 0 0 vcc-1.sym
C 46900 40900 1 90 0 capacitor-1.sym
{
T 46200 41100 5 10 0 0 90 0 1
device=CAPACITOR
T 46000 41100 5 10 0 0 90 0 1
symversion=0.1
T 46900 40900 5 10 0 0 0 0 1
footprint=CK05_type_Capacitor
T 46600 41700 5 10 1 1 180 0 1
refdes=C3
T 46200 41000 5 10 1 1 0 0 1
value=0.1uF
}
N 45300 41200 45300 40900 4
N 46100 41800 46700 41800 4
C 44300 40900 1 90 0 capacitor-1.sym
{
T 43600 41100 5 10 0 0 90 0 1
device=CAPACITOR
T 43400 41100 5 10 0 0 90 0 1
symversion=0.1
T 44300 40900 5 10 0 0 0 0 1
footprint=CK05_type_Capacitor
T 44000 41700 5 10 1 1 180 0 1
refdes=C2
T 43600 41000 5 10 1 1 0 0 1
value=0.33uF
}
N 46800 46500 47300 46500 4
C 47200 50400 1 270 0 resistor-1.sym
{
T 47600 50100 5 10 0 0 270 0 1
device=RESISTOR
T 47200 50400 5 10 0 0 90 0 1
footprint=R025
T 47200 50200 5 10 1 1 180 0 1
value=10K
T 47200 50400 5 10 1 1 180 0 1
refdes=R22
}
C 47100 50400 1 0 0 vcc-1.sym
N 47300 49500 47300 46500 4
T 48700 48100 9 10 1 0 0 0 1
nIRQ
T 48700 47700 9 10 1 0 0 0 1
SCK
T 48700 47500 9 10 1 0 0 0 1
SDO
T 48700 47300 9 10 1 0 0 0 1
SDI
T 47000 46300 9 10 1 0 0 0 1
nFFS
C 49200 44400 1 0 0 ATmega48-2.sym
{
T 49300 49200 5 10 0 0 0 0 1
footprint=DIP28N
T 51500 49000 5 10 1 1 0 6 1
refdes=U1
}
T 54400 40100 9 10 1 0 0 0 1
K. Sarkies 19/04/2014
T 51700 40900 9 10 1 0 0 0 2
Remote Power Switch Master Receiver
	Microcontroller
T 50400 40200 9 10 1 0 0 0 1
1
T 52200 40200 9 10 1 0 0 0 1
2
C 54900 47600 1 0 0 output-2.sym
{
T 55800 47600 5 10 1 1 0 0 1
net=SW0:1
T 55100 48300 5 10 0 0 0 0 1
device=none
T 55800 47700 5 10 0 1 0 1 1
value=UVout
}
C 54900 47400 1 0 0 output-2.sym
{
T 55800 47400 5 10 1 1 0 0 1
net=SW1:1
T 55100 48100 5 10 0 0 0 0 1
device=none
T 55800 47500 5 10 0 1 0 1 1
value=UVout
}
C 54900 47200 1 0 0 output-2.sym
{
T 55800 47200 5 10 1 1 0 0 1
net=SW2:1
T 55100 47900 5 10 0 0 0 0 1
device=none
T 55800 47300 5 10 0 1 0 1 1
value=UVout
}
C 54900 47000 1 0 0 output-2.sym
{
T 55800 47000 5 10 1 1 0 0 1
net=SW3:1
T 55100 47700 5 10 0 0 0 0 1
device=none
T 55800 47100 5 10 0 1 0 1 1
value=UVout
}
C 47900 41700 1 90 0 switch-pushbutton-no-1.sym
{
T 47300 42100 5 10 0 0 90 0 1
device=SWITCH_PUSHBUTTON_NO
T 47900 41700 5 10 0 0 0 0 1
footprint=PB_Tactile
T 48000 41300 5 10 1 1 180 0 1
refdes=S1
}
C 48500 41800 1 90 0 resistor-1.sym
{
T 48100 42100 5 10 0 0 90 0 1
device=RESISTOR
T 48500 41800 5 10 0 0 270 0 1
footprint=R025
T 48300 41000 5 10 1 1 0 0 1
value=10K
T 48300 41200 5 10 1 1 0 0 1
refdes=R21
}
C 48600 41800 1 180 0 vcc-1.sym
C 47800 41400 1 0 0 gnd-1.sym
C 49200 41800 1 90 0 capacitor-1.sym
{
T 49300 42000 5 10 0 0 90 0 1
device=CAPACITOR
T 49100 42000 5 10 0 0 90 0 1
symversion=0.1
T 49200 41800 5 10 0 0 0 0 1
footprint=CK05_type_Capacitor
T 49100 41400 5 10 1 1 180 0 1
refdes=C6
T 48800 41000 5 10 1 1 0 0 1
value=0.1uF
}
C 49100 41500 1 0 1 gnd-1.sym
C 54900 46000 1 0 0 output-2.sym
{
T 55800 46000 5 10 1 1 0 0 1
net=SW8:1
T 55100 46700 5 10 0 0 0 0 1
device=none
T 55800 46100 5 10 0 1 0 1 1
value=UVout
}
C 54900 45800 1 0 0 output-2.sym
{
T 55800 45800 5 10 1 1 0 0 1
net=SW9:1
T 55100 46500 5 10 0 0 0 0 1
device=none
T 55800 45900 5 10 0 1 0 1 1
value=UVout
}
N 53500 49900 53500 46700 4
N 53500 46700 54900 46700 4
N 53900 49300 53900 46300 4
N 53900 46300 54900 46300 4
C 54900 46800 1 0 0 output-2.sym
{
T 55800 46800 5 10 1 1 0 0 1
net=SW4:1
T 55100 47500 5 10 0 0 0 0 1
device=none
T 55800 46900 5 10 0 1 0 1 1
value=UVout
}
C 54900 46600 1 0 0 output-2.sym
{
T 55800 46600 5 10 1 1 0 0 1
net=SW5:1
T 55100 47300 5 10 0 0 0 0 1
device=none
T 55800 46700 5 10 0 1 0 1 1
value=UVout
}
C 54900 46400 1 0 0 output-2.sym
{
T 55800 46400 5 10 1 1 0 0 1
net=SW6:1
T 55100 47100 5 10 0 0 0 0 1
device=none
T 55800 46500 5 10 0 1 0 1 1
value=UVout
}
C 54900 46200 1 0 0 output-2.sym
{
T 55800 46200 5 10 1 1 0 0 1
net=SW7:1
T 55100 46900 5 10 0 0 0 0 1
device=none
T 55800 46300 5 10 0 1 0 1 1
value=UVout
}
N 54900 46100 53000 46100 4
N 53000 46100 53000 46900 4
N 54900 45900 52800 45900 4
N 52800 45900 52800 46700 4
C 55400 44100 1 90 1 gnd-1.sym
C 54300 43800 1 90 0 vcc-1.sym
C 52900 44900 1 0 0 input-2.sym
{
T 53500 45600 5 10 0 0 0 0 1
device=none
T 53400 45000 5 10 0 1 0 7 1
value=UVsignal
T 52800 44900 5 10 1 1 0 0 1
net=SW8:1
}
C 52900 44700 1 0 0 input-2.sym
{
T 53500 45400 5 10 0 0 0 0 1
device=none
T 53400 44800 5 10 0 1 0 7 1
value=UVsignal
T 52800 44700 5 10 1 1 0 0 1
net=SW0:1
}
C 52900 44500 1 0 0 input-2.sym
{
T 53500 45200 5 10 0 0 0 0 1
device=none
T 53400 44600 5 10 0 1 0 7 1
value=UVsignal
T 52800 44500 5 10 1 1 0 0 1
net=SW2:1
}
C 52900 44300 1 0 0 input-2.sym
{
T 53500 45000 5 10 0 0 0 0 1
device=none
T 53400 44400 5 10 0 1 0 7 1
value=UVsignal
T 52800 44300 5 10 1 1 0 0 1
net=SW4:1
}
C 52900 44100 1 0 0 input-2.sym
{
T 53500 44800 5 10 0 0 0 0 1
device=none
T 53400 44200 5 10 0 1 0 7 1
value=UVsignal
T 52800 44100 5 10 1 1 0 0 1
net=SW6:1
}
C 56500 44900 1 0 1 input-2.sym
{
T 55900 45600 5 10 0 0 0 6 1
device=none
T 56000 45000 5 10 0 1 0 1 1
value=UVsignal
T 56500 44900 5 10 1 1 0 6 1
net=SW9:1
}
C 56500 44700 1 0 1 input-2.sym
{
T 55900 45400 5 10 0 0 0 6 1
device=none
T 56000 44800 5 10 0 1 0 1 1
value=UVsignal
T 56500 44700 5 10 1 1 0 6 1
net=SW1:1
}
C 56500 44500 1 0 1 input-2.sym
{
T 55900 45200 5 10 0 0 0 6 1
device=none
T 56000 44600 5 10 0 1 0 1 1
value=UVsignal
T 56500 44500 5 10 1 1 0 6 1
net=SW3:1
}
C 56500 44300 1 0 1 input-2.sym
{
T 55900 45000 5 10 0 0 0 6 1
device=none
T 56000 44400 5 10 0 1 0 1 1
value=UVsignal
T 56500 44300 5 10 1 1 0 6 1
net=SW5:1
}
C 56500 44100 1 0 1 input-2.sym
{
T 55900 44800 5 10 0 0 0 6 1
device=none
T 56000 44200 5 10 0 1 0 1 1
value=UVsignal
T 56500 44100 5 10 1 1 0 6 1
net=SW7:1
}
C 55100 45100 1 180 0 header-12.sym
{
T 55000 45400 5 10 1 1 180 0 1
refdes=CONN12
T 55100 45100 5 10 0 0 180 0 1
footprint=CONNECTOR-6-2
}
C 41300 49500 1 0 0 capacitor-2.sym
{
T 41500 50200 5 10 0 0 0 0 1
device=POLARIZED_CAPACITOR
T 41500 50400 5 10 0 0 0 0 1
symversion=0.1
T 41300 49500 5 10 0 0 0 0 1
footprint=RCY100P
T 41400 49500 5 10 1 1 0 0 1
refdes=C7
T 41900 49500 5 10 1 1 0 0 1
value=1uF
}
C 41300 48700 1 0 0 capacitor-2.sym
{
T 41500 49400 5 10 0 0 0 0 1
device=POLARIZED_CAPACITOR
T 41500 49600 5 10 0 0 0 0 1
symversion=0.1
T 41300 48700 5 10 0 0 0 0 1
footprint=RCY100P
T 41400 48700 5 10 1 1 0 0 1
refdes=C9
T 41900 48700 5 10 1 1 0 0 1
value=1uF
}
N 42200 49300 41300 49300 4
N 41300 49300 41300 48900 4
N 42200 50100 41300 50100 4
N 41300 50100 41300 49700 4
C 45100 49500 1 180 0 capacitor-2.sym
{
T 44900 48800 5 10 0 0 180 0 1
device=POLARIZED_CAPACITOR
T 44900 48600 5 10 0 0 180 0 1
symversion=0.1
T 45100 49500 5 10 0 0 0 0 1
footprint=RCY100P
T 44800 49100 5 10 1 1 180 0 1
refdes=C10
T 44500 49500 5 10 1 1 180 0 1
value=1uF
}
C 45000 49000 1 0 0 gnd-1.sym
N 45100 50100 45100 49300 4
N 41500 47300 42200 47300 4
C 41300 47300 1 90 0 gnd-1.sym
N 41500 47300 41500 48000 4
N 41500 48000 41000 48000 4
N 41000 47700 42200 47700 4
C 41000 47200 1 0 1 connector4-3.sym
{
T 40500 48600 5 10 1 1 0 0 1
refdes=CONN15
}
C 41000 48500 1 270 0 vcc-1.sym
C 44200 49900 1 0 0 capacitor-2.sym
{
T 44400 50600 5 10 0 0 0 0 1
device=POLARIZED_CAPACITOR
T 44100 49900 5 10 1 1 0 0 1
refdes=C8
T 44400 50800 5 10 0 0 0 0 1
symversion=0.1
T 44700 49700 5 10 1 1 0 0 1
value=1uF
T 44200 49900 5 10 0 0 0 0 1
footprint=RCY100P
}
N 44200 47300 44700 47300 4
N 44700 47300 44700 48700 4
N 44700 48700 49200 48700 4
N 44200 47700 44500 47700 4
N 44500 47700 44500 48500 4
N 44500 48500 49200 48500 4
C 42200 47000 1 0 0 max232-2.sym
{
T 42500 50650 5 10 0 0 0 0 1
device=MAX232
T 43900 50500 5 10 1 1 0 6 1
refdes=U3
T 42500 50850 5 10 0 0 0 0 1
footprint=DIP16
}
C 46000 46600 1 90 0 jumper-1.sym
{
T 45500 46900 5 8 0 0 90 0 1
device=JUMPER
T 45400 47000 5 10 1 1 180 0 1
refdes=J2
T 46000 46600 5 10 0 0 0 0 1
footprint=SIP2N
}
C 46000 46400 1 90 0 jumper-1.sym
{
T 45500 46700 5 8 0 0 90 0 1
device=JUMPER
T 45400 46300 5 10 1 1 180 0 1
refdes=J1
T 46000 46400 5 10 0 0 0 0 1
footprint=SIP2N
}
C 47000 44700 1 180 0 jumper-1.sym
{
T 46700 44200 5 8 0 0 180 0 1
device=JUMPER
T 46500 43900 5 10 1 1 0 0 1
refdes=J4
T 47000 44700 5 10 0 0 0 0 1
footprint=SIP2N
}
C 47200 44700 1 180 0 jumper-1.sym
{
T 46900 44200 5 8 0 0 180 0 1
device=JUMPER
T 47300 43900 5 10 1 1 0 0 1
refdes=J3
T 47200 44700 5 10 0 0 0 0 1
footprint=SIP2N
}
N 46800 46300 46900 46300 4
N 46900 44700 46900 46300 4
N 46800 46700 47100 46700 4
N 47100 46700 47100 44700 4
N 47100 43700 46900 43700 4
C 46900 43400 1 0 0 gnd-1.sym
T 55100 40400 9 10 1 0 0 0 1
0.0
C 46800 47400 1 180 0 header-12.sym
{
T 46500 46100 5 10 1 1 180 0 1
refdes=RF1
T 46700 47700 5 10 1 1 180 0 1
value=RFM01
T 46800 47400 5 10 0 0 180 0 1
footprint=RFM01
}
T 48700 47900 9 10 1 0 0 0 1
nSEL
T 47000 44100 9 10 1 0 0 0 1
R
T 45400 46400 9 10 1 0 0 0 1
R
