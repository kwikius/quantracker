EESchema Schematic File Version 2  date Wed 13 Apr 2016 11:03:40 BST
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
LIBS:mcp6561
LIBS:dennis_frie-cache
EELAYER 25  0
EELAYER END
$Descr A4 11700 8267
encoding utf-8
Sheet 1 1
Title ""
Date "13 apr 2016"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text Notes 1200 1600 0    200  ~ 0
Dennis Frie OSD circuit\nfrom www.rcgroups.com/forums/showthread.php?t=1473207
Text Notes 7350 6000 0    60   ~ 0
Make high for white
Text Notes 7400 5700 0    60   ~ 0
Make low output for black or input for passthru
Text Label 5100 5950 0    60   ~ 0
White
Text Label 5050 5700 0    60   ~ 0
Dimming
Text Label 2050 4200 0    60   ~ 0
Video_In_Out
Wire Wire Line
	7650 3700 7650 3450
Connection ~ 5950 5150
Wire Wire Line
	9450 4200 8350 4200
Connection ~ 5500 5150
Wire Wire Line
	5500 5150 5500 4800
Connection ~ 5000 4050
Wire Wire Line
	7150 4050 5000 4050
Connection ~ 5950 4250
Wire Wire Line
	5950 4250 5500 4250
Wire Wire Line
	5500 4250 5500 4400
Wire Wire Line
	5000 3850 5000 4050
Wire Wire Line
	5000 4050 5000 4200
Wire Wire Line
	5000 4200 5000 4400
Connection ~ 5950 3350
Wire Wire Line
	5950 3350 5000 3350
Wire Wire Line
	5000 3350 5000 3450
Wire Wire Line
	4000 3600 4000 3500
Connection ~ 3950 4200
Wire Wire Line
	3950 4200 3950 4400
Wire Wire Line
	4300 4200 4000 4200
Wire Wire Line
	4000 4200 3950 4200
Wire Wire Line
	3950 4200 1900 4200
Wire Wire Line
	4000 4000 4000 4200
Connection ~ 4000 4200
Wire Wire Line
	5950 4400 5950 4250
Wire Wire Line
	5950 4250 5950 3350
Wire Wire Line
	5950 3350 5950 3100
Wire Wire Line
	5950 2600 5950 2450
Wire Wire Line
	4700 4200 5000 4200
Connection ~ 5000 4200
Wire Wire Line
	6100 4650 6700 4650
Wire Wire Line
	6700 4650 6700 4350
Wire Wire Line
	6700 4350 7150 4350
Wire Wire Line
	5000 4900 5000 5150
Wire Wire Line
	5950 5150 5950 4900
Wire Wire Line
	5300 5400 5300 5150
Connection ~ 5300 5150
Wire Wire Line
	4000 3000 4000 2850
Wire Wire Line
	4000 2850 3250 2850
Wire Wire Line
	3250 2850 3250 5950
Wire Wire Line
	3250 5950 6850 5950
Wire Wire Line
	3950 4900 3950 5700
Wire Wire Line
	3950 5700 6850 5700
Wire Wire Line
	5000 5150 5300 5150
Wire Wire Line
	5300 5150 5500 5150
Wire Wire Line
	5500 5150 5950 5150
Wire Wire Line
	5950 5150 7650 5150
Wire Wire Line
	7650 5150 7650 4700
$Comp
L +5V #PWR?
U 1 1 570E0993
P 7650 3450
F 0 "#PWR?" H 7650 3540 20  0001 C CNN
F 1 "+5V" H 7650 3540 30  0000 C CNN
	1    7650 3450
	1    0    0    -1  
$EndComp
Text Notes 7950 3650 0    60   ~ 0
Comparator on Arduino
Text Label 8600 4200 0    60   ~ 0
CSYNCH
Text HLabel 6850 5700 2    60   Input ~ 0
Dimming
Text HLabel 6850 5950 2    60   Input ~ 0
White
$Comp
L CONN_1 P?
U 1 1 570E07F6
P 9600 4200
F 0 "P?" H 9680 4200 40  0000 L CNN
F 1 "CONN_1" H 9600 4255 30  0001 C CNN
	1    9600 4200
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 570E07D7
P 5300 5400
F 0 "#PWR?" H 5300 5400 30  0001 C CNN
F 1 "GND" H 5300 5330 30  0001 C CNN
	1    5300 5400
	1    0    0    -1  
$EndComp
$Comp
L MCP6561 U?
U 1 1 570E078A
P 7850 4200
F 0 "U?" H 7900 4400 60  0000 L CNN
F 1 "Comparator" H 7800 4000 60  0000 L CNN
	1    7850 4200
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR?
U 1 1 570E0612
P 5950 2450
F 0 "#PWR?" H 5950 2540 20  0001 C CNN
F 1 "+5V" H 5950 2540 30  0000 C CNN
	1    5950 2450
	1    0    0    -1  
$EndComp
$Comp
L POT R5
U 1 1 570E056E
P 5950 4650
F 0 "R5" H 5950 4550 50  0000 C CNN
F 1 "10K" H 5950 4650 50  0000 C CNN
	1    5950 4650
	0    1    1    0   
$EndComp
$Comp
L DIODE D5
U 1 1 570E0546
P 5500 4600
F 0 "D5" H 5500 4700 40  0000 C CNN
F 1 "1N4148" H 5500 4500 40  0000 C CNN
	1    5500 4600
	0    1    1    0   
$EndComp
$Comp
L R R3
U 1 1 570E04F7
P 5000 4650
F 0 "R3" V 5080 4650 50  0000 C CNN
F 1 "100K" V 5000 4650 50  0000 C CNN
	1    5000 4650
	1    0    0    -1  
$EndComp
$Comp
L DIODE D2
U 1 1 570E04DB
P 5000 3650
F 0 "D2" H 5000 3750 40  0000 C CNN
F 1 "1N4148" H 5000 3550 40  0000 C CNN
	1    5000 3650
	0    1    1    0   
$EndComp
$Comp
L R R4
U 1 1 570E04CF
P 5950 2850
F 0 "R4" V 6030 2850 50  0000 C CNN
F 1 "10K" V 5950 2850 50  0000 C CNN
	1    5950 2850
	1    0    0    -1  
$EndComp
$Comp
L C C1
U 1 1 570E04B8
P 4500 4200
F 0 "C1" H 4550 4300 50  0000 L CNN
F 1 "0.1uF" H 4550 4100 50  0000 L CNN
	1    4500 4200
	0    1    1    0   
$EndComp
$Comp
L R R2
U 1 1 570E046D
P 3950 4650
F 0 "R2" V 4030 4650 50  0000 C CNN
F 1 "100R" V 3950 4650 50  0000 C CNN
	1    3950 4650
	1    0    0    -1  
$EndComp
$Comp
L CONN_1 P?
U 1 1 570E03B3
P 1750 4200
F 0 "P?" H 1830 4200 40  0000 L CNN
F 1 "CONN_1" H 1750 4255 30  0001 C CNN
	1    1750 4200
	-1   0    0    1   
$EndComp
$Comp
L R R1
U 1 1 570E0394
P 4000 3250
F 0 "R1" V 4080 3250 50  0000 C CNN
F 1 "120R" V 4000 3250 50  0000 C CNN
	1    4000 3250
	-1   0    0    1   
$EndComp
$Comp
L DIODE D1
U 1 1 570E0388
P 4000 3800
F 0 "D1" H 4000 3900 40  0000 C CNN
F 1 "1N4148" H 4000 3700 40  0000 C CNN
	1    4000 3800
	0    1    1    0   
$EndComp
$EndSCHEMATC
