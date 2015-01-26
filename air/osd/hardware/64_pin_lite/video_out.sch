EESchema Schematic File Version 2  date Mon 26 Jan 2015 12:08:39 GMT
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
Sheet 7 8
Title ""
Date "21 jan 2015"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Wire Wire Line
	5150 2350 4950 2350
Connection ~ 3150 4900
Wire Wire Line
	4150 5200 4150 4900
Wire Wire Line
	4150 4900 3050 4900
Wire Wire Line
	3150 5850 3150 5950
Wire Wire Line
	8550 2650 7600 2650
Wire Wire Line
	7200 2650 7000 2650
Wire Wire Line
	4950 2350 4950 2400
Connection ~ 4800 3600
Wire Wire Line
	4800 3600 4800 4050
Wire Wire Line
	4800 4050 3600 4050
Wire Wire Line
	3600 4050 3600 3700
Wire Wire Line
	3600 3700 3700 3700
Connection ~ 4800 2400
Wire Wire Line
	4800 2400 4800 2900
Wire Wire Line
	4800 2900 3550 2900
Wire Wire Line
	3550 2900 3550 2500
Wire Wire Line
	3550 2500 3700 2500
Wire Wire Line
	3400 2300 3700 2300
Wire Wire Line
	4950 2400 4700 2400
Wire Wire Line
	3700 3500 3300 3500
Wire Wire Line
	4700 3600 5200 3600
Wire Wire Line
	5600 3600 6000 3600
Wire Wire Line
	6000 3600 6000 2450
Wire Wire Line
	6000 2450 8550 2450
Wire Wire Line
	8550 2550 8250 2550
Wire Wire Line
	8550 2750 8350 2750
Wire Wire Line
	3150 4900 3150 4950
Wire Wire Line
	3100 5950 4150 5950
Wire Wire Line
	4150 5950 4150 5600
Connection ~ 3150 5950
Wire Wire Line
	8550 2350 5550 2350
$Comp
L C C25
U 1 1 5474951D
P 5400 3600
F 0 "C25" H 5450 3700 50  0000 L CNN
F 1 "33uF" V 5500 3450 50  0000 L CNN
	1    5400 3600
	0    1    1    0   
$EndComp
$Comp
L C C24
U 1 1 54749516
P 5350 2350
F 0 "C24" H 5400 2450 50  0000 L CNN
F 1 "33uF" V 5450 2200 50  0000 L CNN
	1    5350 2350
	0    1    1    0   
$EndComp
$Comp
L C C23
U 1 1 5470A75F
P 4150 5400
F 0 "C23" H 4200 5500 50  0000 L CNN
F 1 "0u1" H 4200 5300 50  0000 L CNN
	1    4150 5400
	1    0    0    -1  
$EndComp
Text HLabel 3100 5950 0    45   BiDi ~ 0
VIDEO_OUT_GND
Text HLabel 3050 4900 0    45   Input ~ 0
VIDEO_OUT_+5V
$Comp
L LM358 U7
U 3 1 5470A57E
P 3250 5400
F 0 "U7" H 3200 5600 60  0000 L CNN
F 1 "LM358" H 3200 5150 60  0000 L CNN
	3    3250 5400
	1    0    0    -1  
$EndComp
$Comp
L DIODESCH D5
U 1 1 546F5AD9
P 7400 2650
F 0 "D5" H 7400 2750 40  0000 C CNN
F 1 "DIODESCH" H 7400 2550 40  0000 C CNN
	1    7400 2650
	1    0    0    -1  
$EndComp
Text HLabel 3400 2300 0    60   Input ~ 0
RAW_DAC_OUT1
Text HLabel 7000 2650 0    45   Input ~ 0
VIDEO_OUT_+V
Text HLabel 8350 2750 0    45   BiDi ~ 0
VIDEO_OUT_GND
$Comp
L CONN_5 P5
U 1 1 546DBC86
P 8900 2550
F 0 "P5" V 8850 2550 50  0000 C CNN
F 1 "CONN_5" V 8950 2550 50  0000 C CNN
	1    8900 2550
	1    0    0    1   
$EndComp
Text HLabel 8250 2550 0    45   Input ~ 0
VIDEO_OUT
$Comp
L LM358 U?
U 1 1 54675F35
P 4200 3600
AR Path="/54675A51/54675AC5" Ref="U?"  Part="1" 
AR Path="/54675A51/54675F35" Ref="U7"  Part="1" 
F 0 "U7" H 4200 3800 60  0000 L CNN
F 1 "LM358" H 4150 3350 60  0000 L CNN
	1    4200 3600
	1    0    0    -1  
$EndComp
Text HLabel 3300 3500 0    60   Input ~ 0
RAW_DAC_OUT2
$Comp
L LM358 U7
U 2 1 54675AC5
P 4200 2400
F 0 "U7" H 4200 2600 60  0000 L CNN
F 1 "LM358" H 4150 2150 60  0000 L CNN
	2    4200 2400
	1    0    0    -1  
$EndComp
$EndSCHEMATC
