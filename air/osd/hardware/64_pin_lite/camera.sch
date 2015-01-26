EESchema Schematic File Version 2  date Mon 26 Jan 2015 12:32:55 GMT
LIBS:lm358
LIBS:conn_6
LIBS:conn_5
LIBS:conn_3
LIBS:conn_2
LIBS:conn_20
LIBS:si501
LIBS:stm32f405rgt6
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:special
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:adg704
LIBS:bd9g101g
LIBS:dac084s085
LIBS:fms6141
LIBS:ld39015mxxr
LIBS:mcp6562
LIBS:opa830
LIBS:si8642
LIBS:tlv3501
LIBS:74lvc1g86
EELAYER 25  0
EELAYER END
$Descr A4 11700 8267
encoding utf-8
Sheet 4 8
Title ""
Date "26 jan 2015"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Wire Wire Line
	4100 3450 3750 3450
Wire Wire Line
	5200 3550 5000 3550
Wire Wire Line
	5200 3350 5000 3350
Wire Wire Line
	4500 3450 5200 3450
$Comp
L DIODESCH D4
U 1 1 546F5A7F
P 4300 3450
F 0 "D4" H 4300 3550 40  0000 C CNN
F 1 "DIODESCH" H 4300 3350 40  0000 C CNN
	1    4300 3450
	1    0    0    -1  
$EndComp
Text HLabel 5000 3350 0    45   Output ~ 0
VIDEO_IN
Text HLabel 5000 3550 0    45   Input ~ 0
VIDEO_IN_GND
Text HLabel 3750 3450 0    45   Input ~ 0
VIDEO_IN_+V
$Comp
L CONN_3 K1
U 1 1 546F3160
P 5550 3450
F 0 "K1" V 5500 3450 50  0000 C CNN
F 1 "CONN_3" V 5600 3450 40  0000 C CNN
	1    5550 3450
	1    0    0    1   
$EndComp
$EndSCHEMATC
