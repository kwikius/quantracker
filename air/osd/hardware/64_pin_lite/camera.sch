EESchema Schematic File Version 2  date Wed 17 Jun 2015 00:34:32 BST
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
LIBS:R0link
LIBS:emi_filter
LIBS:osd-cache
EELAYER 25  0
EELAYER END
$Descr A4 11700 8267
encoding utf-8
Sheet 4 8
Title ""
Date "16 jun 2015"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Connection ~ 5000 4350
Wire Wire Line
	5000 3600 5000 4350
Connection ~ 6750 3550
Wire Wire Line
	6750 3550 6750 4350
Wire Wire Line
	6750 4350 4050 4350
Wire Wire Line
	4050 4350 4050 4100
Wire Wire Line
	4700 3450 3450 3450
Wire Wire Line
	5300 3450 6950 3450
Wire Wire Line
	6950 3350 6750 3350
Wire Wire Line
	6600 3550 6950 3550
Wire Wire Line
	3050 3450 2700 3450
Wire Wire Line
	4050 3700 4050 3450
Connection ~ 4050 3450
$Comp
L EMI_FILTER3 FI2
U 1 1 5580AD25
P 5000 3450
F 0 "FI2" H 5150 3600 60  0000 C CNN
F 1 "NFL18ST207X1C3" H 5400 3302 60  0000 C CNN
	1    5000 3450
	1    0    0    -1  
$EndComp
$Comp
L C C39
U 1 1 5580ACEA
P 4050 3900
F 0 "C39" H 4100 4000 50  0000 L CNN
F 1 "0u1" H 4100 3800 50  0000 L CNN
	1    4050 3900
	1    0    0    -1  
$EndComp
Text HLabel 2700 3450 0    45   Input ~ 0
VIDEO_IN_+V
$Comp
L DIODESCH D4
U 1 1 546F5A7F
P 3250 3450
F 0 "D4" H 3250 3550 40  0000 C CNN
F 1 "DIODESCH" H 3250 3350 40  0000 C CNN
	1    3250 3450
	1    0    0    -1  
$EndComp
Text HLabel 6750 3350 0    45   Output ~ 0
VIDEO_IN
Text HLabel 6600 3550 0    45   Input ~ 0
VIDEO_IN_GND
$Comp
L CONN_3 K1
U 1 1 546F3160
P 7300 3450
F 0 "K1" V 7250 3450 50  0000 C CNN
F 1 "CONN_3" V 7350 3450 40  0000 C CNN
	1    7300 3450
	1    0    0    1   
$EndComp
$EndSCHEMATC
