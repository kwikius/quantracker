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
Sheet 5 8
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
	6500 4400 6600 4400
Wire Wire Line
	6500 4200 6600 4200
Wire Wire Line
	6500 4000 6600 4000
Wire Wire Line
	6500 3800 6600 3800
Wire Wire Line
	6500 3600 6600 3600
Wire Wire Line
	6500 3400 6600 3400
Wire Wire Line
	6500 3200 6600 3200
Wire Wire Line
	6500 3000 6600 3000
Wire Wire Line
	6500 2800 6600 2800
Wire Wire Line
	4100 4300 4200 4300
Wire Wire Line
	4100 4100 4200 4100
Wire Wire Line
	4100 3900 4200 3900
Wire Wire Line
	4100 3700 4200 3700
Wire Wire Line
	4100 3500 4200 3500
Wire Wire Line
	4100 3300 4200 3300
Wire Wire Line
	4100 3100 4200 3100
Connection ~ 5550 2700
Wire Wire Line
	5550 2550 5550 2700
Wire Wire Line
	4100 2800 4200 2800
Wire Wire Line
	4100 2600 4200 2600
Wire Wire Line
	5950 2500 6200 2500
Wire Wire Line
	6200 2500 6600 2500
Wire Wire Line
	6600 2700 5550 2700
Wire Wire Line
	5550 2700 5400 2700
Wire Wire Line
	4100 2900 4200 2900
Wire Wire Line
	6600 2600 6500 2600
Wire Wire Line
	4100 2500 4200 2500
Wire Wire Line
	4100 2700 4200 2700
Wire Wire Line
	6200 2350 6200 2500
Connection ~ 6200 2500
Wire Wire Line
	4100 3000 4200 3000
Wire Wire Line
	4100 3200 4200 3200
Wire Wire Line
	4100 3400 4200 3400
Wire Wire Line
	4100 3600 4200 3600
Wire Wire Line
	4100 3800 4200 3800
Wire Wire Line
	4100 4000 4200 4000
Wire Wire Line
	4100 4200 4200 4200
Wire Wire Line
	4100 4400 4200 4400
Wire Wire Line
	6500 2900 6600 2900
Wire Wire Line
	6500 3100 6600 3100
Wire Wire Line
	6500 3300 6600 3300
Wire Wire Line
	6500 3500 6600 3500
Wire Wire Line
	6500 3700 6600 3700
Wire Wire Line
	6500 3900 6600 3900
Wire Wire Line
	6500 4100 6600 4100
Wire Wire Line
	6500 4300 6600 4300
Text HLabel 6500 4400 0    45   BiDi ~ 0
XP_GND
Text HLabel 6500 2800 0    45   Output ~ 0
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
Text HLabel 6500 2600 0    45   Output ~ 0
XP_+V
$Comp
L PWR_FLAG #FLG04
U 1 1 54748449
P 6200 2350
F 0 "#FLG04" H 6200 2620 30  0001 C CNN
F 1 "PWR_FLAG" H 6200 2580 30  0000 C CNN
	1    6200 2350
	1    0    0    -1  
$EndComp
Text HLabel 6500 4200 0    45   BiDi ~ 0
XP_PB9
Text HLabel 4200 2800 2    45   Output ~ 0
VIDEO_OUT_+V
Text HLabel 4200 2600 2    45   Output ~ 0
VIDEO_IN_+V
Text HLabel 4200 2700 2    45   Output ~ 0
VIDEO_OUT_+V
Text HLabel 4200 2500 2    45   Output ~ 0
VIDEO_IN_+V
Text HLabel 5400 2700 0    45   Output ~ 0
XP_GND
Text HLabel 4200 2900 2    45   Output ~ 0
XP_GND
$Comp
L CONN_20 P3
U 1 1 54746DE4
P 3750 3450
F 0 "P3" V 3700 3450 60  0000 C CNN
F 1 "CONN_20" V 3800 3450 60  0000 C CNN
	1    3750 3450
	-1   0    0    -1  
$EndComp
$Comp
L CONN_20 P2
U 1 1 54746D91
P 6950 3450
F 0 "P2" V 6900 3450 60  0000 C CNN
F 1 "CONN_20" V 7000 3450 60  0000 C CNN
	1    6950 3450
	1    0    0    -1  
$EndComp
Text HLabel 1900 5550 0    45   BiDi ~ 0
XP_GND
Text HLabel 5950 2500 0    45   Output ~ 0
XP_+V
Text HLabel 4200 3200 2    45   Input ~ 0
XP_+3V
Text HLabel 6500 4300 0    45   BiDi ~ 0
XP_GND
Text HLabel 1900 5450 0    45   BiDi ~ 0
XP_GND
Text HLabel 6500 4100 0    45   BiDi ~ 0
XP_PB8
Text HLabel 6500 4000 0    45   BiDi ~ 0
XP_PB7
Text HLabel 6500 3800 0    45   BiDi ~ 0
XP_PB5
Text HLabel 6500 3700 0    45   BiDi ~ 0
XP_PB4
Text HLabel 6500 3600 0    45   BiDi ~ 0
XP_PB3
Text HLabel 6500 3500 0    45   BiDi ~ 0
XP_PC12
Text HLabel 6500 3400 0    45   BiDi ~ 0
XP_PA14
Text HLabel 6500 3300 0    45   BiDi ~ 0
XP_PA13
Text HLabel 6500 3900 0    45   BiDi ~ 0
XP_PB6
Text HLabel 6500 3200 0    45   BiDi ~ 0
XP_PA12
Text HLabel 6500 3100 0    45   BiDi ~ 0
XP_PA11
Text HLabel 6500 3000 0    45   BiDi ~ 0
XP_PA8
Text HLabel 6500 2900 0    45   BiDi ~ 0
XP_PC9
Text HLabel 4200 4300 2    45   BiDi ~ 0
XP_PC14
Text HLabel 4200 4200 2    45   BiDi ~ 0
XP_PC15
Text HLabel 4200 4100 2    45   BiDi ~ 0
XP_PC0
Text HLabel 4200 4000 2    45   BiDi ~ 0
XP_PC1
Text HLabel 4200 3900 2    45   BiDi ~ 0
XP_PC3
Text HLabel 4200 3800 2    45   BiDi ~ 0
XP_PA0
Text HLabel 4200 3700 2    45   BiDi ~ 0
XP_PA1
Text HLabel 4200 3600 2    45   BiDi ~ 0
XP_PC4
Text HLabel 4200 3500 2    45   BiDi ~ 0
XP_PC5
Text HLabel 4200 3400 2    45   BiDi ~ 0
XP_PB1
Text HLabel 4200 3300 2    45   BiDi ~ 0
XP_GND
Text HLabel 4200 3100 2    45   Input ~ 0
XP_+5V
Text HLabel 4200 4400 2    45   BiDi ~ 0
XP_GND
Text HLabel 4200 3000 2    45   BiDi ~ 0
XP_GND
$EndSCHEMATC
