EESchema Schematic File Version 2  date Wed 18 May 2016 11:24:51 BST
LIBS:conn_20
LIBS:conn_6
LIBS:conn_5
LIBS:conn_3
LIBS:conn_2
LIBS:led_ipc-7351_2005
LIBS:power
LIBS:device
LIBS:diodesch_ipc7531_2005
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:lm358
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
LIBS:max4390
LIBS:spdt
LIBS:ts5a3157
LIBS:fms6141
LIBS:mcp6562
LIBS:stm32f405rgt6
LIBS:si501
LIBS:74lvc1g86
LIBS:si8642
LIBS:fiducial
LIBS:bd9g101g
LIBS:ld39015mxxr
EELAYER 25  0
EELAYER END
$Descr A4 11700 8267
encoding utf-8
Sheet 5 8
Title ""
Date "18 may 2016"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Connection ~ 4050 2750
Wire Wire Line
	4350 1900 4050 1900
Wire Wire Line
	4050 1900 4050 2750
Connection ~ 5950 2150
Wire Wire Line
	4850 2150 5950 2150
Wire Wire Line
	6600 2700 6500 2700
Wire Wire Line
	6600 3400 6500 3400
Wire Wire Line
	6600 3200 6500 3200
Wire Wire Line
	6600 3000 6500 3000
Wire Wire Line
	7200 2300 7300 2300
Wire Wire Line
	7300 2300 7300 2350
Wire Wire Line
	6500 4300 6600 4300
Wire Wire Line
	6500 4100 6600 4100
Wire Wire Line
	6500 3900 6600 3900
Wire Wire Line
	6500 3700 6600 3700
Wire Wire Line
	6500 3500 6600 3500
Wire Wire Line
	6500 2900 6600 2900
Wire Wire Line
	3450 4450 3550 4450
Wire Wire Line
	3450 4250 3550 4250
Wire Wire Line
	3450 4050 3550 4050
Wire Wire Line
	3450 3850 3550 3850
Wire Wire Line
	3450 3650 3550 3650
Wire Wire Line
	3450 3250 3550 3250
Wire Wire Line
	3550 3050 3450 3050
Connection ~ 6200 2500
Wire Wire Line
	6200 2350 6200 2500
Wire Wire Line
	3450 2750 4350 2750
Wire Wire Line
	3450 2550 4350 2550
Wire Wire Line
	3450 2950 3550 2950
Wire Wire Line
	6600 2500 5850 2500
Wire Wire Line
	3450 2650 3550 2650
Wire Wire Line
	3450 2850 3550 2850
Wire Wire Line
	5550 2700 5550 2550
Connection ~ 5550 2700
Wire Wire Line
	3450 3150 3550 3150
Wire Wire Line
	3450 3350 3550 3350
Wire Wire Line
	3450 3550 3550 3550
Wire Wire Line
	3450 3750 3550 3750
Wire Wire Line
	3450 3950 3550 3950
Wire Wire Line
	3450 4150 3550 4150
Wire Wire Line
	3450 4350 3550 4350
Wire Wire Line
	6500 2800 6600 2800
Wire Wire Line
	6500 3600 6600 3600
Wire Wire Line
	6500 3800 6600 3800
Wire Wire Line
	6500 4000 6600 4000
Wire Wire Line
	6500 4200 6600 4200
Wire Wire Line
	6500 4400 6600 4400
Wire Wire Line
	3450 3450 3550 3450
Wire Wire Line
	6600 3100 6500 3100
Wire Wire Line
	6600 3300 6500 3300
Wire Wire Line
	6600 2600 5650 2600
Wire Wire Line
	5650 2600 5650 2700
Wire Wire Line
	5650 2700 5400 2700
Wire Wire Line
	4850 1900 5950 1900
Wire Wire Line
	5950 1900 5950 2500
Connection ~ 5950 2500
Wire Wire Line
	4350 2150 4200 2150
Wire Wire Line
	4200 2150 4200 2550
Connection ~ 4200 2550
$Comp
L R R30
U 1 1 5580AC4E
P 4600 2150
F 0 "R30" V 4680 2150 50  0000 C CNN
F 1 "0R" V 4600 2150 50  0000 C CNN
F 2 "0603" H 4600 2150 60  0001 C CNN
F 3 "D" H 4600 2150 60  0001 C CNN
F 4 "M" H 4600 2150 60  0001 C CNN "MFG Name"
F 5 "P" H 4600 2150 60  0001 C CNN "MFG Part Num"
F 6 "Resistor(Link)/0R/0603" H 4600 2150 60  0001 C CNN "Description"
	1    4600 2150
	0    1    1    0   
$EndComp
$Comp
L R R25
U 1 1 5580AC49
P 4600 1900
F 0 "R25" V 4680 1900 50  0000 C CNN
F 1 "0R" V 4600 1900 50  0000 C CNN
F 2 "0603" H 4600 1900 60  0001 C CNN
F 3 "D" H 4600 1900 60  0001 C CNN
F 4 "M" H 4600 1900 60  0001 C CNN "MFG Name"
F 5 "P" H 4600 1900 60  0001 C CNN "MFG Part Num"
F 6 "Resistor(Link)/0R/0603" H 4600 1900 60  0001 C CNN "Description"
	1    4600 1900
	0    1    1    0   
$EndComp
Text HLabel 6500 2900 0    45   BiDi ~ 0
XP_GND
Text HLabel 3550 3050 2    45   BiDi ~ 0
XP_PB1
Text HLabel 6500 2800 0    45   BiDi ~ 0
XP_PB8
Text HLabel 3550 3850 2    45   BiDi ~ 0
XP_GND
Text HLabel 7200 2300 0    45   Output ~ 0
VIN-
$Comp
L CONN_1 P11
U 1 1 54F74E74
P 7300 2500
F 0 "P11" V 7300 2650 40  0000 L CNN
F 1 "SIL1" H 7300 2555 30  0001 C CNN
F 2 "SIL1" H 7300 2500 60  0001 C CNN
F 4 "0.1 in pitch header/1 contact" H 7300 2500 60  0001 C CNN "Description"
	1    7300 2500
	0    1    1    0   
$EndComp
Text HLabel 6500 3600 0    45   BiDi ~ 0
XP_GND
Text HLabel 6500 3200 0    45   Output ~ 0
XP_GND
$Comp
L PWR_FLAG #FLG03
U 1 1 5474845E
P 5550 2550
F 0 "#FLG03" H 5550 2820 30  0001 C CNN
F 1 "PWR_FLAG" H 5550 2780 30  0000 C CNN
	1    5550 2550
	1    0    0    -1  
$EndComp
$Comp
L PWR_FLAG #FLG04
U 1 1 54748449
P 6200 2350
F 0 "#FLG04" H 6200 2620 30  0001 C CNN
F 1 "PWR_FLAG" H 6200 2580 30  0000 C CNN
	1    6200 2350
	1    0    0    -1  
$EndComp
Text HLabel 6500 4300 0    45   BiDi ~ 0
XP_PA13
Text HLabel 3550 2850 2    45   Output ~ 0
XP_GND
Text HLabel 3550 2650 2    45   Output ~ 0
XP_GND
Text HLabel 4350 2750 2    45   Output ~ 0
VIDEO_OUT_+V
Text HLabel 4350 2550 2    45   Output ~ 0
VIDEO_IN_+V
Text HLabel 5400 2700 0    45   Output ~ 0
XP_GND
Text HLabel 3550 2950 2    45   Output ~ 0
XP_+5V
$Comp
L CONN_20 P3
U 1 1 54746DE4
P 3100 3500
F 0 "P3" V 3050 3500 60  0000 C CNN
F 1 "SIL20" V 3150 3500 60  0000 C CNN
F 2 "SIL20" H 3100 3500 60  0001 C CNN
F 4 "0.1 in pitch header/20 contacts" H 3100 3500 60  0001 C CNN "Description"
	1    3100 3500
	-1   0    0    -1  
$EndComp
$Comp
L CONN_20 P2
U 1 1 54746D91
P 6950 3450
F 0 "P2" V 6900 3450 60  0000 C CNN
F 1 "SIL20" V 7000 3450 60  0000 C CNN
F 2 "SIL20" H 6950 3450 60  0001 C CNN
F 4 "0.1 in pitch header/20 contacts" H 6950 3450 60  0001 C CNN "Description"
	1    6950 3450
	1    0    0    -1  
$EndComp
Text HLabel 1900 5550 0    45   BiDi ~ 0
XP_GND
Text HLabel 5850 2500 0    45   Output ~ 0
VIN+
Text HLabel 3550 3250 2    45   Input ~ 0
XP_PC15
Text HLabel 6500 4400 0    45   Output ~ 0
XP_NRST
Text HLabel 1900 5450 0    45   BiDi ~ 0
XP_GND
Text HLabel 6500 4200 0    45   BiDi ~ 0
XP_GND
Text HLabel 6500 4100 0    45   BiDi ~ 0
XP_PA14
Text HLabel 6500 3900 0    45   BiDi ~ 0
XP_PA12
Text HLabel 6500 3800 0    45   BiDi ~ 0
XP_PA11
Text HLabel 6500 3700 0    45   BiDi ~ 0
XP_PC12
Text HLabel 6500 3500 0    45   BiDi ~ 0
XP_PB3
Text HLabel 6500 3400 0    45   BiDi ~ 0
XP_PB4
Text HLabel 6500 3300 0    45   BiDi ~ 0
XP_PB5
Text HLabel 6500 4000 0    45   BiDi ~ 0
XP_+3V
Text HLabel 6500 3100 0    45   BiDi ~ 0
XP_PB6
Text HLabel 6500 3000 0    45   BiDi ~ 0
XP_PB7
Text HLabel 6500 2700 0    45   BiDi ~ 0
XP_PB9
Text HLabel 3550 4350 2    45   BiDi ~ 0
XP_PA8
Text HLabel 3550 4250 2    45   BiDi ~ 0
XP_PC9
Text HLabel 3550 4150 2    45   BiDi ~ 0
XP_PC5
Text HLabel 3550 4050 2    45   BiDi ~ 0
XP_PC4
Text HLabel 3550 3950 2    45   BiDi ~ 0
XP_PC3
Text HLabel 3550 3750 2    45   BiDi ~ 0
XP_PA0
Text HLabel 3550 3650 2    45   BiDi ~ 0
XP_PA1
Text HLabel 3550 3550 2    45   BiDi ~ 0
XP_PC1
Text HLabel 3550 3450 2    45   BiDi ~ 0
XP_PC0
Text HLabel 3550 3350 2    45   BiDi ~ 0
XP_GND
Text HLabel 3550 3150 2    45   Input ~ 0
XP_PC14
Text HLabel 3550 4450 2    45   BiDi ~ 0
XP_GND
$EndSCHEMATC
