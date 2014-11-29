EESchema Schematic File Version 2  date Sat 29 Nov 2014 10:43:44 GMT
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
Date "29 nov 2014"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Wire Wire Line
	2700 2800 2800 2800
Wire Wire Line
	2800 2800 2800 2750
Wire Wire Line
	7850 2600 7850 2750
Wire Wire Line
	7850 2750 7950 2750
Wire Wire Line
	4550 4050 4150 4050
Wire Wire Line
	5100 4700 5100 4800
Wire Wire Line
	5100 4800 4300 4800
Connection ~ 6800 4900
Wire Wire Line
	6800 4750 6800 4900
Wire Wire Line
	4550 3750 4250 3750
Wire Wire Line
	4550 3150 4200 3150
Wire Wire Line
	7050 3300 7050 4050
Wire Wire Line
	7050 3300 7800 3300
Wire Wire Line
	7050 4050 6250 4050
Wire Wire Line
	6250 3150 6750 3150
Wire Wire Line
	5100 2550 5100 2400
Wire Wire Line
	5100 2400 4250 2400
Wire Wire Line
	7800 3700 6950 3700
Wire Wire Line
	6950 3700 6950 4900
Wire Wire Line
	6950 4900 5700 4900
Wire Wire Line
	5700 4900 5700 4700
Wire Wire Line
	5700 2550 5700 2450
Wire Wire Line
	5700 2450 6900 2450
Wire Wire Line
	6900 2450 6900 3600
Wire Wire Line
	6900 3600 7800 3600
Wire Wire Line
	6250 3000 6350 3000
Wire Wire Line
	6350 3000 6350 2450
Connection ~ 6350 2450
Wire Wire Line
	4550 4200 4300 4200
Wire Wire Line
	7800 3500 6650 3500
Wire Wire Line
	6650 3500 6650 3750
Wire Wire Line
	6650 3750 6250 3750
Wire Wire Line
	7800 3400 6400 3400
Wire Wire Line
	6400 3400 6400 3450
Wire Wire Line
	6400 3450 6250 3450
Wire Wire Line
	7800 3800 6750 3800
Wire Wire Line
	6750 3800 6750 3150
Wire Wire Line
	4550 3450 4250 3450
Wire Wire Line
	6700 2300 6700 2450
Connection ~ 6700 2450
Wire Wire Line
	7850 2200 7850 2100
Wire Wire Line
	7850 2100 7900 2100
Wire Wire Line
	2700 2250 2800 2250
Wire Wire Line
	2800 2250 2800 2350
Text HLabel 2700 2800 0    45   BiDi ~ 0
MAVLINK_GND
Text HLabel 2700 2250 0    45   Input ~ 0
MAVLINK_+3.3V
Text Label 7950 2750 0    45   ~ 0
FC_GND
Text Label 7900 2100 0    45   ~ 0
FC_+5V
$Comp
L C C22
U 1 1 54734555
P 7850 2400
F 0 "C22" H 7900 2500 50  0000 L CNN
F 1 "0u1" H 7900 2300 50  0000 L CNN
	1    7850 2400
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
Text Label 7100 3800 0    45   ~ 0
FC_FRSKY_TXO
Text Label 7100 3700 0    45   ~ 0
FC_GND
Text Label 7100 3600 0    45   ~ 0
FC_+5V
Text Label 7100 3500 0    45   ~ 0
FC_MAVLINK_RXI
Text Label 7100 3400 0    45   ~ 0
FC_MAVLINK_TXO
Text Label 7100 3300 0    45   ~ 0
FC_FRSKY_RXI
Text Label 6100 4900 0    45   ~ 0
FC_GND
$Comp
L PWR_FLAG #FLG05
U 1 1 547339AD
P 6800 4750
F 0 "#FLG05" H 6800 5020 30  0001 C CNN
F 1 "PWR_FLAG" H 6800 4980 30  0000 C CNN
	1    6800 4750
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
P 8150 3550
F 0 "P4" V 8100 3550 60  0000 C CNN
F 1 "CONN_6" V 8200 3550 60  0000 C CNN
	1    8150 3550
	1    0    0    1   
$EndComp
Text HLabel 4250 3450 0    45   Input ~ 0
MAVLINK_TXO
Text HLabel 4250 3750 0    45   Output ~ 0
MAVLINK_RXI
$EndSCHEMATC
