EESchema Schematic File Version 2  date Thu 27 Nov 2014 15:59:43 GMT
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
Sheet 5 8
Title ""
Date "27 nov 2014"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Connection ~ 3650 2700
Wire Wire Line
	3650 2550 3650 2700
Wire Wire Line
	6700 2800 6800 2800
Wire Wire Line
	6700 2600 6800 2600
Wire Wire Line
	4200 2500 4850 2500
Wire Wire Line
	4850 2700 3500 2700
Wire Wire Line
	6800 4300 6700 4300
Wire Wire Line
	6700 4400 6800 4400
Wire Wire Line
	6700 4100 6800 4100
Wire Wire Line
	6700 3900 6800 3900
Wire Wire Line
	6700 3700 6800 3700
Wire Wire Line
	6700 3500 6800 3500
Wire Wire Line
	6700 3300 6800 3300
Wire Wire Line
	6700 3100 6800 3100
Wire Wire Line
	4850 4400 4750 4400
Wire Wire Line
	4850 4200 4750 4200
Wire Wire Line
	4850 4000 4750 4000
Wire Wire Line
	4850 3800 4750 3800
Wire Wire Line
	4850 3600 4750 3600
Wire Wire Line
	4850 3400 4750 3400
Wire Wire Line
	4850 3200 4750 3200
Wire Wire Line
	4850 3000 4750 3000
Wire Wire Line
	4850 2900 4750 2900
Wire Wire Line
	4850 3300 4750 3300
Wire Wire Line
	4850 3500 4750 3500
Wire Wire Line
	4850 3700 4750 3700
Wire Wire Line
	4850 3900 4750 3900
Wire Wire Line
	4850 4100 4750 4100
Wire Wire Line
	4850 4300 4750 4300
Wire Wire Line
	6700 2900 6800 2900
Wire Wire Line
	6700 3000 6800 3000
Wire Wire Line
	6700 3200 6800 3200
Wire Wire Line
	6700 3400 6800 3400
Wire Wire Line
	6700 3600 6800 3600
Wire Wire Line
	6700 3800 6800 3800
Wire Wire Line
	6700 4000 6800 4000
Wire Wire Line
	6700 4200 6800 4200
Wire Wire Line
	4850 3100 4750 3100
Wire Wire Line
	4850 2800 4750 2800
Wire Wire Line
	4850 2600 4750 2600
Wire Wire Line
	6700 2500 6800 2500
Wire Wire Line
	6700 2700 6800 2700
Wire Wire Line
	4450 2500 4450 2350
Connection ~ 4450 2500
$Comp
L PWR_FLAG #FLG03
U 1 1 5474845E
P 3650 2550
F 0 "#FLG03" H 3650 2820 30  0001 C CNN
F 1 "PWR_FLAG" H 3650 2780 30  0000 C CNN
	1    3650 2550
	1    0    0    -1  
$EndComp
Text HLabel 4750 2600 0    45   Output ~ 0
XP_+V
$Comp
L PWR_FLAG #FLG04
U 1 1 54748449
P 4450 2350
F 0 "#FLG04" H 4450 2620 30  0001 C CNN
F 1 "PWR_FLAG" H 4450 2580 30  0000 C CNN
	1    4450 2350
	1    0    0    -1  
$EndComp
Text HLabel 6800 4300 2    45   BiDi ~ 0
XP_GND
Text HLabel 6800 2800 2    45   Output ~ 0
VIDEO_OUT_+V
Text HLabel 6800 2600 2    45   Output ~ 0
VIDEO_IN_+V
Text HLabel 6800 2700 2    45   Output ~ 0
VIDEO_OUT_+V
Text HLabel 6800 2500 2    45   Output ~ 0
VIDEO_IN_+V
Text HLabel 3500 2700 0    45   Output ~ 0
XP_GND
Text HLabel 4750 2800 0    45   Output ~ 0
XP_GND
$Comp
L CONN_20 P3
U 1 1 54746DE4
P 6350 3450
F 0 "P3" V 6300 3450 60  0000 C CNN
F 1 "CONN_20" V 6400 3450 60  0000 C CNN
	1    6350 3450
	-1   0    0    -1  
$EndComp
$Comp
L CONN_20 P2
U 1 1 54746D91
P 5200 3450
F 0 "P2" V 5150 3450 60  0000 C CNN
F 1 "CONN_20" V 5250 3450 60  0000 C CNN
	1    5200 3450
	1    0    0    -1  
$EndComp
Text HLabel 4750 4400 0    45   BiDi ~ 0
XP_GND
Text HLabel 4200 2500 0    45   Output ~ 0
XP_+V
Text HLabel 4750 3100 0    45   Input ~ 0
XP_+3V
Text HLabel 6800 4400 2    45   BiDi ~ 0
XP_GND
Text HLabel 4750 4300 0    45   BiDi ~ 0
XP_GND
Text HLabel 6800 4200 2    45   BiDi ~ 0
XP_PC15
Text HLabel 6800 4100 2    45   BiDi ~ 0
XP_PC14
Text HLabel 6800 3900 2    45   BiDi ~ 0
XP_PC9
Text HLabel 6800 3800 2    45   BiDi ~ 0
XP_PC5
Text HLabel 6800 3700 2    45   BiDi ~ 0
XP_PC4
Text HLabel 6800 3600 2    45   BiDi ~ 0
XP_PC3
Text HLabel 6800 3500 2    45   BiDi ~ 0
XP_PC1
Text HLabel 6800 3400 2    45   BiDi ~ 0
XP_PC0
Text HLabel 6800 4000 2    45   BiDi ~ 0
XP_PC12
Text HLabel 6800 3300 2    45   BiDi ~ 0
XP_PB9
Text HLabel 6800 3200 2    45   BiDi ~ 0
XP_PB8
Text HLabel 6800 3100 2    45   BiDi ~ 0
XP_PB7
Text HLabel 6800 3000 2    45   BiDi ~ 0
XP_PB6
Text HLabel 4750 4200 0    45   BiDi ~ 0
XP_PB5
Text HLabel 4750 4100 0    45   BiDi ~ 0
XP_PB4
Text HLabel 4750 4000 0    45   BiDi ~ 0
XP_PB3
Text HLabel 4750 3900 0    45   BiDi ~ 0
XP_PB1
Text HLabel 4750 3800 0    45   BiDi ~ 0
XP_PA14
Text HLabel 4750 3700 0    45   BiDi ~ 0
XP_PA13
Text HLabel 4750 3600 0    45   BiDi ~ 0
XP_PA12
Text HLabel 4750 3500 0    45   BiDi ~ 0
XP_PA11
Text HLabel 4750 3400 0    45   BiDi ~ 0
XP_PA8
Text HLabel 4750 3300 0    45   BiDi ~ 0
XP_PA1
Text HLabel 4750 3200 0    45   BiDi ~ 0
XP_PA0
Text HLabel 4750 3000 0    45   Input ~ 0
XP_+5V
Text HLabel 6800 2900 2    45   BiDi ~ 0
XP_GND
Text HLabel 4750 2900 0    45   BiDi ~ 0
XP_GND
$EndSCHEMATC
