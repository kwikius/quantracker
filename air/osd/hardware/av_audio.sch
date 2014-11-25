EESchema Schematic File Version 2  date Tue 18 Nov 2014 15:10:19 GMT
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
LIBS:stm32f4duino-cache
LIBS:passives
LIBS:sync_clamp-cache
LIBS:osd-cache
EELAYER 25  0
EELAYER END
$Descr A4 11700 8267
encoding utf-8
Sheet 2 5
Title ""
Date "18 nov 2014"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
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
	2900 2300 3700 2300
Wire Wire Line
	4950 2400 4700 2400
Wire Wire Line
	3700 3500 2750 3500
Wire Wire Line
	4950 3600 4700 3600
Text HLabel 4950 3600 2    60   Output ~ 0
DAC_OUT2
$Comp
L LM358 U?
U 1 1 54675F35
P 4200 3600
AR Path="/54675A51/54675AC5" Ref="U?"  Part="1" 
AR Path="/54675A51/54675F35" Ref="U4"  Part="1" 
F 0 "U4" H 4200 3800 60  0000 L CNN
F 1 "LM358" H 4150 3350 60  0000 L CNN
	1    4200 3600
	1    0    0    -1  
$EndComp
Text HLabel 2750 3500 0    60   Input ~ 0
RAW_DAC_OUT2
Text HLabel 4950 2400 2    60   Output ~ 0
DAC_OUT1
Text HLabel 2900 2300 0    60   Input ~ 0
RAW_DAC_OUT1
$Comp
L LM358 U3
U 1 1 54675AC5
P 4200 2400
F 0 "U3" H 4200 2600 60  0000 L CNN
F 1 "LM358" H 4150 2150 60  0000 L CNN
	1    4200 2400
	1    0    0    -1  
$EndComp
$EndSCHEMATC
