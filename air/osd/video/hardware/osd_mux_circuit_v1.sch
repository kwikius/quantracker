EESchema Schematic File Version 2  date Sun 31 Aug 2014 14:35:51 BST
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
LIBS:osd_mux_circuit-cache
EELAYER 25  0
EELAYER END
$Descr A4 11700 8267
encoding utf-8
Sheet 1 1
Title ""
Date "26 aug 2014"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Wire Wire Line
	3700 2700 3150 2700
Connection ~ 8450 2900
Wire Wire Line
	8450 2900 8450 3300
Wire Wire Line
	8450 3300 7200 3300
Wire Wire Line
	7200 3300 7200 3000
Wire Wire Line
	7200 3000 7300 3000
Wire Wire Line
	3300 3900 3300 3650
Wire Wire Line
	3300 3650 3200 3650
Wire Wire Line
	6550 3900 6550 4100
Wire Wire Line
	5850 3500 5850 3750
Wire Wire Line
	5850 3750 5400 3750
Wire Wire Line
	5400 1700 5300 1700
Wire Wire Line
	6200 2000 6200 1700
Wire Wire Line
	6200 1700 5900 1700
Wire Wire Line
	4700 2800 4900 2800
Connection ~ 6550 2800
Wire Wire Line
	6550 2950 6550 2800
Wire Wire Line
	7300 2800 5400 2800
Wire Wire Line
	6200 2800 6200 2400
Connection ~ 6200 2800
Wire Wire Line
	5850 2800 5850 3100
Connection ~ 5850 2800
Wire Wire Line
	6550 3350 6550 3500
Wire Wire Line
	8700 3400 8600 3400
Wire Wire Line
	8600 3400 8600 3650
Wire Wire Line
	8700 2900 8300 2900
Wire Wire Line
	3700 2900 3650 2900
Wire Wire Line
	3650 2900 3650 3200
Wire Wire Line
	3650 3200 4800 3200
Wire Wire Line
	4800 3200 4800 2800
Connection ~ 4800 2800
$Comp
L GND #PWR?
U 1 1 53FC643E
P 8600 3650
F 0 "#PWR?" H 8600 3650 30  0001 C CNN
F 1 "GND" H 8600 3580 30  0001 C CNN
	1    8600 3650
	1    0    0    -1  
$EndComp
$Comp
L CONN_1 P?
U 1 1 53FC6432
P 8850 3400
F 0 "P?" H 8930 3400 40  0000 L CNN
F 1 "CONN_1" H 8850 3455 30  0001 C CNN
	1    8850 3400
	1    0    0    -1  
$EndComp
$Comp
L CONN_1 P?
U 1 1 53FC6428
P 3050 3650
F 0 "P?" H 3130 3650 40  0000 L CNN
F 1 "CONN_1" H 3050 3705 30  0001 C CNN
	1    3050 3650
	-1   0    0    1   
$EndComp
$Comp
L GND #PWR?
U 1 1 53FC641F
P 3300 3900
F 0 "#PWR?" H 3300 3900 30  0001 C CNN
F 1 "GND" H 3300 3830 30  0001 C CNN
	1    3300 3900
	1    0    0    -1  
$EndComp
Text Notes 1750 3200 0    60   ~ 0
assume input is DC coupled to gnd
$Comp
L DIODESCH D?
U 1 1 53FC6371
P 5850 3300
F 0 "D?" H 5850 3400 40  0000 C CNN
F 1 "DIODESCH" H 5850 3200 40  0000 C CNN
	1    5850 3300
	0    1    1    0   
$EndComp
$Comp
L DIODE D?
U 1 1 53FC6358
P 6550 3700
F 0 "D?" H 6550 3800 40  0000 C CNN
F 1 "DIODE" H 6550 3600 40  0000 C CNN
	1    6550 3700
	0    1    1    0   
$EndComp
Text Notes 2550 1350 0    60   ~ 0
For transparent whitein = 0 and notblackin = 1;\nfro white whitein = 1 and blackin = 0;\nfor black, black_in = 0 and whitein = 1;\n
$Comp
L CONN_1 P?
U 1 1 53FC5808
P 3000 2700
F 0 "P?" H 3080 2700 40  0000 L CNN
F 1 "CONN_1" H 3000 2755 30  0001 C CNN
	1    3000 2700
	-1   0    0    1   
$EndComp
$Comp
L DIODE D?
U 1 1 53FC5777
P 6200 2200
F 0 "D?" H 6200 2300 40  0000 C CNN
F 1 "DIODE" H 6200 2100 40  0000 C CNN
	1    6200 2200
	0    1    1    0   
$EndComp
$Comp
L R R?
U 1 1 53FC5749
P 5650 1700
F 0 "R?" V 5730 1700 50  0000 C CNN
F 1 "100R" V 5650 1700 50  0000 C CNN
	1    5650 1700
	0    1    1    0   
$EndComp
$Comp
L CONN_1 WhiteIn
U 1 1 53FC5729
P 5150 1700
F 0 "WhiteIn" H 5230 1700 40  0000 L CNN
F 1 "CONN_1" H 5150 1755 30  0001 C CNN
	1    5150 1700
	-1   0    0    1   
$EndComp
$Comp
L CONN_1 NotBlack_in
U 1 1 53FC56E8
P 5250 3750
F 0 "NotBlack_in" H 5330 3750 40  0000 L CNN
F 1 "CONN_1" H 5250 3805 30  0001 C CNN
	1    5250 3750
	-1   0    0    1   
$EndComp
$Comp
L GND #PWR?
U 1 1 53FC56C5
P 6550 4100
F 0 "#PWR?" H 6550 4100 30  0001 C CNN
F 1 "GND" H 6550 4030 30  0001 C CNN
	1    6550 4100
	1    0    0    -1  
$EndComp
$Comp
L CONN_1 P?
U 1 1 53FC5691
P 8850 2900
F 0 "P?" H 8930 2900 40  0000 L CNN
F 1 "CONN_1" H 8850 2955 30  0001 C CNN
	1    8850 2900
	1    0    0    -1  
$EndComp
$Comp
L DIODESCH D?
U 1 1 53FC5649
P 6550 3150
F 0 "D?" H 6550 3250 40  0000 C CNN
F 1 "DIODESCH" H 6550 3050 40  0000 C CNN
	1    6550 3150
	0    1    1    0   
$EndComp
$Comp
L R R?
U 1 1 53FC5558
P 5150 2800
F 0 "R?" V 5230 2800 50  0000 C CNN
F 1 "1K" V 5150 2800 50  0000 C CNN
	1    5150 2800
	0    1    1    0   
$EndComp
$Comp
L LM358N U?
U 1 1 53FC554A
P 7800 2900
AR Path="/53FC5542" Ref="U?"  Part="1" 
AR Path="/53FC554A" Ref="U?"  Part="1" 
F 0 "U?" H 7750 3100 60  0000 L CNN
F 1 "OPA832" H 7750 2650 60  0000 L CNN
	1    7800 2900
	1    0    0    -1  
$EndComp
$Comp
L LM358N U?
U 1 1 53FC5542
P 4200 2800
F 0 "U?" H 4150 3000 60  0000 L CNN
F 1 "OPA832" H 4150 2550 60  0000 L CNN
	1    4200 2800
	1    0    0    -1  
$EndComp
$EndSCHEMATC
