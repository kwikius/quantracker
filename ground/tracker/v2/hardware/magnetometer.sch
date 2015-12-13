EESchema Schematic File Version 2  date Thu 10 Dec 2015 12:44:52 GMT
LIBS:quantracker
LIBS:stm32
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
LIBS:tcm3105
LIBS:quantracker_air_osd_v2_1
LIBS:mc34931
LIBS:tracker_using_quantracker_osd-cache
EELAYER 25  0
EELAYER END
$Descr A4 11700 8267
encoding utf-8
Sheet 2 5
Title ""
Date "10 dec 2015"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Wire Wire Line
	4650 2200 4650 2150
Text HLabel 3800 3200 2    60   Input ~ 0
MCU_GND
Text HLabel 6150 4000 2    60   Output ~ 0
MAG_RDY
Text HLabel 3800 3000 2    60   Input ~ 0
5VOut
Text HLabel 5600 3100 2    60   BiDi ~ 0
MAG_SDA
Text HLabel 4750 2750 2    60   Input ~ 0
MAG_SCL
$Comp
L R R?
U 1 1 55996006
P 5750 4000
F 0 "R?" V 5830 4000 50  0000 C CNN
F 1 "100R" V 5750 4000 50  0000 C CNN
	1    5750 4000
	0    1    1    0   
$EndComp
$Comp
L R 100R?
U 1 1 55996005
P 5250 3100
F 0 "100R?" V 5330 3100 50  0000 C CNN
F 1 "R" V 5300 3300 50  0000 C CNN
	1    5250 3100
	0    1    1    0   
$EndComp
$Comp
L R R?
U 1 1 55996004
P 4350 2750
F 0 "R?" V 4450 2850 50  0000 C CNN
F 1 "100R" V 4350 2750 50  0000 C CNN
	1    4350 2750
	0    1    1    0   
$EndComp
$Comp
L R R?
U 1 1 55996003
P 5300 3650
F 0 "R?" V 5380 3650 50  0000 C CNN
F 1 "10K" V 5300 3650 50  0000 C CNN
	1    5300 3650
	1    0    0    -1  
$EndComp
Text HLabel 5350 3350 2    60   Output ~ 0
3.3VO
$Comp
L R R?
U 1 1 55996002
P 5550 2800
F 0 "R?" V 5630 2800 50  0000 C CNN
F 1 "10K" V 5550 2800 50  0000 C CNN
	1    5550 2800
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 55996001
P 4650 2450
F 0 "R?" V 4730 2450 50  0000 C CNN
F 1 "10K" V 4650 2450 50  0000 C CNN
	1    4650 2450
	1    0    0    -1  
$EndComp
Text HLabel 5600 2500 2    60   Output ~ 0
3.3VO
Wire Wire Line
	4650 3100 5000 3100
Wire Wire Line
	4650 2900 4650 3100
Wire Wire Line
	3550 2900 4650 2900
Wire Wire Line
	4650 2750 4750 2750
Wire Wire Line
	4600 2750 4650 2750
Connection ~ 5550 3100
Wire Wire Line
	5550 3050 5550 3100
Wire Wire Line
	5300 3350 5350 3350
Wire Wire Line
	5300 3400 5300 3350
Connection ~ 5300 4000
Wire Wire Line
	5300 3900 5300 4000
Wire Wire Line
	3800 3200 3550 3200
Wire Wire Line
	3800 3000 3550 3000
Wire Wire Line
	4450 3100 3550 3100
Wire Wire Line
	4450 4000 4450 3100
Wire Wire Line
	5300 4000 4450 4000
Wire Wire Line
	5500 4000 5300 4000
Wire Wire Line
	6000 4000 6150 4000
Wire Wire Line
	5550 3100 5500 3100
Wire Wire Line
	5600 3100 5550 3100
Wire Wire Line
	4650 2700 4650 2750
Connection ~ 4650 2750
Wire Wire Line
	5550 2500 5550 2550
Wire Wire Line
	5600 2500 5550 2500
Wire Wire Line
	3900 2800 3550 2800
Wire Wire Line
	3900 2750 3900 2800
Wire Wire Line
	4100 2750 3900 2750
$Comp
L CONN_5 P?
U 1 1 55996000
P 3150 3000
F 0 "P?" V 3100 3000 50  0000 C CNN
F 1 "CONN_5" V 3200 3000 50  0000 C CNN
	1    3150 3000
	-1   0    0    1   
$EndComp
$EndSCHEMATC
