EESchema Schematic File Version 2  date Fri 16 Jan 2015 10:20:26 GMT
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
LIBS:ts5a317
LIBS:si8621
LIBS:video_telem-cache
EELAYER 25  0
EELAYER END
$Descr A4 11700 8267
encoding utf-8
Sheet 1 1
Title ""
Date "16 jan 2015"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text Notes 6500 1100 0    60   ~ 0
Or use Votage divider at output of Switch \nand use e.g TSH122 Video driver with SAG\ninstead of MAX4390
Wire Wire Line
	5500 3550 5500 3200
Wire Wire Line
	5500 3200 6700 3200
Wire Wire Line
	6700 3200 6700 4750
Wire Wire Line
	6700 4750 7250 4750
Wire Wire Line
	4800 2350 4800 2200
Connection ~ 4000 2900
Wire Wire Line
	4250 1650 4000 1650
Wire Wire Line
	4000 1650 4000 3000
Connection ~ 3800 3700
Wire Wire Line
	3800 3800 3800 3700
Connection ~ 3700 2900
Wire Wire Line
	3700 2900 4000 2900
Wire Wire Line
	4150 4800 3300 4800
Wire Wire Line
	3300 4800 3300 1300
Wire Wire Line
	6300 1950 6300 2100
Wire Wire Line
	6300 2100 6200 2100
Connection ~ 1350 2850
Wire Wire Line
	7400 1550 8500 1550
Wire Wire Line
	6900 1550 6800 1550
Wire Wire Line
	1350 2400 1350 2850
Wire Wire Line
	5800 1450 5350 1450
Wire Wire Line
	5800 1650 5750 1650
Wire Wire Line
	3300 1300 4250 1300
Wire Wire Line
	6300 1150 6300 950 
Wire Wire Line
	3200 1600 3300 1600
Connection ~ 3300 1600
Wire Wire Line
	1500 1600 1250 1600
Wire Wire Line
	5750 1650 5750 2350
Wire Wire Line
	1350 1900 1350 1600
Connection ~ 1350 1600
Wire Wire Line
	2050 1600 1900 1600
Wire Wire Line
	6550 2350 6850 2350
Wire Wire Line
	6850 2350 6850 1550
Connection ~ 6850 1550
Wire Wire Line
	2400 950  2550 950 
Wire Wire Line
	2550 950  2550 1150
Wire Wire Line
	1300 2850 2550 2850
Wire Wire Line
	2550 2850 2550 2050
Wire Wire Line
	5750 2350 6050 2350
Wire Wire Line
	3700 3000 3700 2800
Wire Wire Line
	3700 3550 3700 3700
Wire Wire Line
	3700 3700 4000 3700
Wire Wire Line
	4000 3700 4000 3400
Wire Wire Line
	4800 900  4800 800 
Wire Wire Line
	4800 800  4400 800 
Wire Wire Line
	6150 4700 6500 4700
Wire Wire Line
	6500 4700 6500 2850
Wire Wire Line
	6500 2850 5500 2850
Wire Wire Line
	5500 2850 5500 1800
Wire Wire Line
	5500 1800 5350 1800
Wire Wire Line
	6150 4200 6900 4200
Wire Wire Line
	6900 4200 6900 4450
Wire Wire Line
	6900 4450 7250 4450
$Comp
L GND #PWR?
U 1 1 54AFD95E
P 4800 2350
F 0 "#PWR?" H 4800 2350 30  0001 C CNN
F 1 "GND" H 4800 2280 30  0001 C CNN
	1    4800 2350
	1    0    0    -1  
$EndComp
$Comp
L SI8621 U?
U 1 1 54AFD652
P 8100 4600
F 0 "U?" H 8650 3900 60  0000 C CNN
F 1 "SI8621" H 8100 3950 60  0000 C CNN
	1    8100 4600
	1    0    0    -1  
$EndComp
$Comp
L CRYSTAL X?
U 1 1 54AFCEAF
P 2100 5300
F 0 "X?" H 2100 5450 60  0000 C CNN
F 1 "CRYSTAL" H 2100 5150 60  0000 C CNN
	1    2100 5300
	1    0    0    -1  
$EndComp
Text HLabel 4400 800  0    45   Input ~ 0
VIDEO_+5V
$Comp
L GND #PWR?
U 1 1 54AFCD29
P 3800 3800
F 0 "#PWR?" H 3800 3800 30  0001 C CNN
F 1 "GND" H 3800 3730 30  0001 C CNN
	1    3800 3800
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 54AFCD0D
P 4000 3200
F 0 "C?" H 4050 3300 50  0000 L CNN
F 1 "C" H 4050 3100 50  0000 L CNN
	1    4000 3200
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 54AFCCFA
P 3700 3250
F 0 "R?" V 3780 3250 50  0000 C CNN
F 1 "R" V 3700 3250 50  0000 C CNN
	1    3700 3250
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 54AFCCF3
P 3700 2550
F 0 "R?" V 3780 2550 50  0000 C CNN
F 1 "R" V 3700 2550 50  0000 C CNN
	1    3700 2550
	1    0    0    -1  
$EndComp
$Comp
L TS5A317 U?
U 1 1 54AFC92A
P 4800 1550
F 0 "U?" H 4700 1450 60  0000 C CNN
F 1 "TS5A3157" H 5000 1000 60  0000 C CNN
	1    4800 1550
	-1   0    0    -1  
$EndComp
$Comp
L STM32F051 U?
U 1 1 54AFAB32
P 5150 4550
F 0 "U?" H 5100 4350 60  0000 C CNN
F 1 "STM32F051" H 5150 4200 60  0000 C CNN
	1    5150 4550
	1    0    0    -1  
$EndComp
Text HLabel 1250 1600 0    60   Input ~ 0
VIDEO_IN
Text HLabel 1300 2850 0    60   Input ~ 0
VIDEO_GND
Text HLabel 6300 950  0    45   Input ~ 0
VIDEO_+5V
Text HLabel 6200 2100 0    45   Input ~ 0
VIDEO_GND
Text HLabel 2400 950  0    45   Input ~ 0
VIDEO_+5V
$Comp
L R R23
U 1 1 544CEF88
P 7150 1550
F 0 "R23" V 7230 1550 50  0000 C CNN
F 1 "75R" V 7150 1550 50  0000 C CNN
	1    7150 1550
	0    1    1    0   
$EndComp
$Comp
L R R22
U 1 1 544A412C
P 6300 2350
F 0 "R22" V 6380 2350 50  0000 C CNN
F 1 "24R" V 6300 2350 50  0000 C CNN
	1    6300 2350
	0    1    1    0   
$EndComp
Text HLabel 8500 1550 2    60   Output ~ 0
VIDEO_OUT
$Comp
L FMS6141 U10
U 1 1 544A2987
P 2450 1600
F 0 "U10" H 2800 1250 60  0000 L CNN
F 1 "FMS6141" H 2650 1850 60  0000 L CNN
F 2 "SC70-5" H 2450 1600 60  0001 C CNN
F 3 "/home/andy/electronics/video/video_amp/FMS6141_video_amp_with_filter.pdf" H 2450 1600 60  0001 C CNN
	1    2450 1600
	1    0    0    -1  
$EndComp
$Comp
L OPA830 U13
U 1 1 541EDCE2
P 6300 1550
F 0 "U13" H 6250 1750 60  0000 L CNN
F 1 "MAX4390" H 6400 1350 60  0000 L CNN
F 2 "SOT23-5" H 6300 1550 60  0001 C CNN
F 3 "/home/andy/electronics/video/video_amp/opa830.pdf" H 6300 1550 60  0001 C CNN
	1    6300 1550
	1    0    0    -1  
$EndComp
$Comp
L C C26
U 1 1 541E8561
P 1700 1600
F 0 "C26" H 1750 1700 50  0000 L CNN
F 1 "0u1" H 1750 1500 50  0000 L CNN
F 2 "0603" H 1700 1600 60  0001 C CNN
	1    1700 1600
	0    1    1    0   
$EndComp
$Comp
L R R11
U 1 1 541E78CF
P 1350 2150
F 0 "R11" V 1430 2150 50  0000 C CNN
F 1 "75R" V 1350 2150 50  0000 C CNN
F 2 "0603" H 1350 2150 60  0001 C CNN
	1    1350 2150
	-1   0    0    1   
$EndComp
$EndSCHEMATC
