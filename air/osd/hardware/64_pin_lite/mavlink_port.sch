EESchema Schematic File Version 2  date Wed 21 Jan 2015 09:35:15 GMT
LIBS:power
LIBS:device
LIBS:transistors
LIBS:andy_conn
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
LIBS:stm32f4duino-cache
LIBS:stm32
LIBS:osd-cache
LIBS:osd-cache
EELAYER 25  0
EELAYER END
$Descr A4 11700 8267
encoding utf-8
Sheet 6 8
Title ""
Date "21 jan 2015"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Connection ~ 7150 4950
Wire Wire Line
	7150 4950 7150 4800
Connection ~ 8050 3500
Wire Wire Line
	8050 4400 8050 3500
Connection ~ 4550 4800
Wire Wire Line
	4550 4800 4550 5150
Wire Wire Line
	4550 5150 5200 5150
Connection ~ 6200 2450
Wire Wire Line
	6200 2450 6200 1800
Wire Wire Line
	6200 1800 5550 1800
Wire Wire Line
	2700 2800 2800 2800
Wire Wire Line
	2800 2800 2800 2750
Wire Wire Line
	8350 1850 8350 2000
Wire Wire Line
	8350 2000 8450 2000
Wire Wire Line
	4550 4050 4150 4050
Wire Wire Line
	5100 4700 5100 4800
Wire Wire Line
	5100 4800 4300 4800
Connection ~ 9000 4950
Wire Wire Line
	9000 4750 9000 4950
Wire Wire Line
	4550 3750 4250 3750
Wire Wire Line
	4550 3150 4200 3150
Wire Wire Line
	9050 3300 9050 4250
Wire Wire Line
	9050 3300 9800 3300
Wire Wire Line
	7250 4250 6950 4250
Wire Wire Line
	6950 4250 6950 4050
Wire Wire Line
	6950 4050 6250 4050
Wire Wire Line
	9050 4250 7750 4250
Wire Wire Line
	7000 3150 8800 3150
Wire Wire Line
	6250 3150 6500 3150
Wire Wire Line
	5100 2550 5100 2400
Wire Wire Line
	5100 2400 4250 2400
Wire Wire Line
	9800 3700 9700 3700
Wire Wire Line
	9700 3700 9700 4950
Wire Wire Line
	9700 4950 5700 4950
Wire Wire Line
	5700 4950 5700 4700
Wire Wire Line
	5700 2550 5700 2450
Wire Wire Line
	5700 2450 8900 2450
Wire Wire Line
	8900 2450 8900 3600
Wire Wire Line
	8900 3600 9800 3600
Wire Wire Line
	6250 3000 6350 3000
Wire Wire Line
	6350 3000 6350 2450
Connection ~ 6350 2450
Wire Wire Line
	4550 4200 4300 4200
Wire Wire Line
	8150 3500 7200 3500
Wire Wire Line
	9800 3500 8650 3500
Wire Wire Line
	7200 3500 7200 3750
Wire Wire Line
	7200 3750 6250 3750
Wire Wire Line
	6500 3400 6400 3400
Wire Wire Line
	9800 3400 7000 3400
Wire Wire Line
	6400 3400 6400 3450
Wire Wire Line
	6400 3450 6250 3450
Wire Wire Line
	9800 3800 8800 3800
Wire Wire Line
	8800 3800 8800 3150
Wire Wire Line
	4550 3450 4250 3450
Wire Wire Line
	6700 2300 6700 2450
Connection ~ 6700 2450
Wire Wire Line
	8350 1450 8350 1350
Wire Wire Line
	8350 1350 8400 1350
Wire Wire Line
	2700 2250 2800 2250
Wire Wire Line
	2800 2250 2800 2350
Wire Wire Line
	5050 1800 4850 1800
Wire Wire Line
	5700 5150 6000 5150
Wire Wire Line
	6000 5150 6000 4950
Connection ~ 6000 4950
Wire Wire Line
	8050 4800 8050 4950
Connection ~ 8050 4950
Wire Wire Line
	7150 4400 7150 4250
Connection ~ 7150 4250
$Comp
L C C37
U 1 1 547D825F
P 7150 4600
F 0 "C37" H 7200 4700 50  0000 L CNN
F 1 "470pF" H 7200 4500 50  0000 L CNN
	1    7150 4600
	1    0    0    -1  
$EndComp
$Comp
L C C38
U 1 1 547D8257
P 8050 4600
F 0 "C38" H 8100 4700 50  0000 L CNN
F 1 "470pF" H 8100 4500 50  0000 L CNN
	1    8050 4600
	1    0    0    -1  
$EndComp
$Comp
L R R29
U 1 1 547D81DE
P 8400 3500
F 0 "R29" V 8480 3500 50  0000 C CNN
F 1 "100R" V 8400 3500 50  0000 C CNN
	1    8400 3500
	0    1    1    0   
$EndComp
$Comp
L R R28
U 1 1 547D81C5
P 7500 4250
F 0 "R28" V 7580 4250 50  0000 C CNN
F 1 "100R" V 7500 4250 50  0000 C CNN
	1    7500 4250
	0    1    1    0   
$EndComp
$Comp
L R R27
U 1 1 547D816E
P 6750 3400
F 0 "R27" V 6830 3400 50  0000 C CNN
F 1 "100R" V 6750 3400 50  0000 C CNN
	1    6750 3400
	0    1    1    0   
$EndComp
$Comp
L R R26
U 1 1 547D813B
P 6750 3150
F 0 "R26" V 6830 3150 50  0000 C CNN
F 1 "100R" V 6750 3150 50  0000 C CNN
	1    6750 3150
	0    1    1    0   
$EndComp
Text Notes 5100 1600 0    45   ~ 0
Optional link
Text Notes 5250 5350 0    45   ~ 0
Optional link
$Comp
L R R25
U 1 1 547C2609
P 5450 5150
F 0 "R25" V 5530 5150 50  0000 C CNN
F 1 "0R" V 5450 5150 50  0000 C CNN
	1    5450 5150
	0    1    1    0   
$EndComp
$Comp
L R R24
U 1 1 547C2601
P 5300 1800
F 0 "R24" V 5380 1800 50  0000 C CNN
F 1 "0R" V 5300 1800 50  0000 C CNN
	1    5300 1800
	0    1    1    0   
$EndComp
Text HLabel 4850 1800 0    45   Input ~ 0
MAVLINK_+5V
Text HLabel 2700 2800 0    45   BiDi ~ 0
MAVLINK_GND
Text HLabel 2700 2250 0    45   Input ~ 0
MAVLINK_+3.3V
Text Label 8450 2000 0    45   ~ 0
FC_GND
Text Label 8400 1350 0    45   ~ 0
FC_+5V
$Comp
L C C22
U 1 1 54734555
P 8350 1650
F 0 "C22" H 8400 1750 50  0000 L CNN
F 1 "0u1" H 8400 1550 50  0000 L CNN
	1    8350 1650
	1    0    0    -1  
$EndComp
$Comp
L C C21
U 1 1 54734552
P 2800 2550
F 0 "C21" H 2850 2650 50  0000 L CNN
F 1 "0u1" H 2850 2450 50  0000 L CNN
	1    2800 2550
	1    0    0    -1  
$EndComp
Text Label 9100 3800 0    45   ~ 0
FC_FRSKY_TXO
Text Label 9100 3600 0    45   ~ 0
FC_+5V
Text Label 9100 3500 0    45   ~ 0
FC_MAVLINK_RXI
Text Label 9100 3400 0    45   ~ 0
FC_MAVLINK_TXO
Text Label 9100 3300 0    45   ~ 0
FC_FRSKY_RXI
Text Label 6100 4950 0    45   ~ 0
FC_GND
$Comp
L PWR_FLAG #FLG05
U 1 1 547339AD
P 9000 4750
F 0 "#FLG05" H 9000 5020 30  0001 C CNN
F 1 "PWR_FLAG" H 9000 4980 30  0000 C CNN
	1    9000 4750
	1    0    0    -1  
$EndComp
$Comp
L PWR_FLAG #FLG06
U 1 1 547339A4
P 6700 2300
F 0 "#FLG06" H 6700 2570 30  0001 C CNN
F 1 "PWR_FLAG" H 6700 2530 30  0000 C CNN
	1    6700 2300
	1    0    0    -1  
$EndComp
Text HLabel 4300 4200 0    45   Input ~ 0
MAVLINK_+3.3V
Text Label 5800 2450 0    45   ~ 0
FC_+5V
Text HLabel 4250 2400 0    45   Input ~ 0
MAVLINK_+3.3V
$Comp
L SI8642 U6
U 1 1 5473378C
P 5400 3600
F 0 "U6" H 5950 2600 60  0000 C CNN
F 1 "SI8642" H 5350 2550 60  0000 C CNN
	1    5400 3600
	1    0    0    -1  
$EndComp
Text HLabel 4150 4050 0    45   Output ~ 0
FRSKY_RXI
Text Notes 6500 1350 0    45   ~ 0
5V Mavlink and\n programming port
Text HLabel 4300 4800 0    45   BiDi ~ 0
MAVLINK_GND
Text HLabel 4200 3150 0    45   Input ~ 0
FRSKY_TXO
$Comp
L CONN_6 P4
U 1 1 546C98A1
P 10150 3550
F 0 "P4" V 10100 3550 60  0000 C CNN
F 1 "CONN_6" V 10200 3550 60  0000 C CNN
	1    10150 3550
	1    0    0    1   
$EndComp
Text HLabel 4250 3450 0    45   Input ~ 0
MAVLINK_TXO
Text HLabel 4250 3750 0    45   Output ~ 0
MAVLINK_RXI
$EndSCHEMATC
