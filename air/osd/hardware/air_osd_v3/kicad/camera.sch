EESchema Schematic File Version 2  date Thu 18 Feb 2016 18:58:17 GMT
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
LIBS:osd-cache
EELAYER 25  0
EELAYER END
$Descr A4 11700 8267
encoding utf-8
Sheet 4 8
Title ""
Date "18 feb 2016"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L DIODESCH_IPC7531_2005 D4
U 1 1 558BB778
P 3250 3450
F 0 "D4" H 3250 3550 40  0000 C CNN
F 1 "MBR0530T1G" H 3250 3350 40  0000 C CNN
F 2 "SOD-123" H 3250 3450 60  0001 C CNN
F 3 "D" H 3250 3450 60  0001 C CNN
F 4 "On Semiconductor" H 3250 3450 60  0001 C CNN "MFG Name"
F 5 "MBR0530T1G" H 3250 3450 60  0001 C CNN "MFG Part Num"
F 6 "Diode/Schottky/30V/500mA/SOD-123" H 3250 3450 60  0001 C CNN "Description"
	1    3250 3450
	-1   0    0    1   
$EndComp
$Comp
L INDUCTOR L6
U 1 1 5582ABE4
P 5100 3450
F 0 "L6" V 5050 3450 40  0000 C CNN
F 1 "BLM18EG601SN1" V 5200 3450 40  0000 C CNN
F 2 "0603" H 5100 3450 60  0001 C CNN
F 3 "D" H 5100 3450 60  0001 C CNN
F 4 "Murata" H 5100 3450 60  0001 C CNN "MFG Name"
F 5 "BLM18EG601SN1" H 5100 3450 60  0001 C CNN "MFG Part Num"
F 6 "Inductor/Bead/0603" H 5100 3450 60  0001 C CNN "Description"
	1    5100 3450
	0    1    1    0   
$EndComp
Connection ~ 6750 3550
Wire Wire Line
	6750 3550 6750 4350
Wire Wire Line
	6750 4350 4050 4350
Wire Wire Line
	4050 4350 4050 4100
Wire Wire Line
	4800 3450 3400 3450
Wire Wire Line
	5400 3450 6950 3450
Wire Wire Line
	6950 3350 6750 3350
Wire Wire Line
	6600 3550 6950 3550
Wire Wire Line
	3100 3450 2700 3450
Wire Wire Line
	4050 3700 4050 3450
Connection ~ 4050 3450
$Comp
L C C39
U 1 1 5580ACEA
P 4050 3900
F 0 "C39" H 4100 4000 50  0000 L CNN
F 1 "0.1uF" H 4100 3800 50  0000 L CNN
F 2 "0603" H 4050 3900 60  0001 C CNN
F 3 "D" H 4050 3900 60  0001 C CNN
F 4 "M" H 4050 3900 60  0001 C CNN "MFG Name"
F 5 "P" H 4050 3900 60  0001 C CNN "MFG Part Num"
F 6 "Capacitor/0.1uF/10V/+-10%/X7R/0603" H 4050 3900 60  0001 C CNN "Description"
	1    4050 3900
	1    0    0    -1  
$EndComp
Text HLabel 2700 3450 0    45   Input ~ 0
VIDEO_IN_+V
Text HLabel 6750 3350 0    45   Output ~ 0
VIDEO_IN
Text HLabel 6600 3550 0    45   Input ~ 0
VIDEO_IN_GND
$Comp
L CONN_3 K1
U 1 1 546F3160
P 7300 3450
F 0 "K1" V 7250 3450 50  0000 C CNN
F 1 "SIL3" V 7350 3450 40  0000 C CNN
F 2 "SIL3" H 7300 3450 60  0001 C CNN
F 4 "0.1 in pitch header connector/3 contacts" H 7300 3450 60  0001 C CNN "Description"
	1    7300 3450
	1    0    0    1   
$EndComp
$EndSCHEMATC
