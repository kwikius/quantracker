EESchema Schematic File Version 2
LIBS:flight_controller-rescue
LIBS:spst
LIBS:power
LIBS:device
LIBS:quantracker_air_osd_v2_1
LIBS:mpu6000
LIBS:mpu9250
LIBS:conn_5
LIBS:conn_6
LIBS:conn_3
LIBS:andy_conn
LIBS:conn_7
LIBS:conn_4
LIBS:74ls125
LIBS:sn74lvc1g3157
LIBS:74lvc1g86
LIBS:ms5611
LIBS:fm25v01
LIBS:flight_controller-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 5
Title ""
Date "8 jan 2016"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Wire Wire Line
	6000 4250 6000 4400
Wire Wire Line
	3650 3950 3450 3950
Wire Wire Line
	3650 3850 3100 3850
Wire Wire Line
	5350 3950 5650 3950
Wire Wire Line
	5650 3950 5650 4950
Wire Wire Line
	5650 4950 2950 4950
Wire Wire Line
	3650 3750 3100 3750
Wire Wire Line
	3650 4050 3550 4050
Wire Wire Line
	3550 4050 3550 4200
Wire Wire Line
	5350 3750 6100 3750
Wire Wire Line
	3450 4700 5500 4700
Wire Wire Line
	5500 4700 5500 4050
Wire Wire Line
	5500 4050 5350 4050
Wire Wire Line
	5350 3850 5500 3850
Wire Wire Line
	5500 3850 5500 3750
Connection ~ 5500 3750
Wire Wire Line
	6000 3850 6000 3750
Connection ~ 6000 3750
$Comp
L GND-RESCUE-flight_controller #PWR?
U 1 1 568FC7C9
P 6000 4400
F 0 "#PWR?" H 6000 4400 30  0001 C CNN
F 1 "GND" H 6000 4330 30  0001 C CNN
F 2 "" H 6000 4400 60  0001 C CNN
F 3 "" H 6000 4400 60  0001 C CNN
	1    6000 4400
	1    0    0    -1  
$EndComp
$Comp
L C-RESCUE-flight_controller C?
U 1 1 568FC7C1
P 6000 4050
F 0 "C?" H 6050 4150 50  0000 L CNN
F 1 "C" H 6050 3950 50  0000 L CNN
F 2 "" H 6000 4050 60  0001 C CNN
F 3 "" H 6000 4050 60  0001 C CNN
	1    6000 4050
	1    0    0    -1  
$EndComp
Text HLabel 3450 3950 0    60   Input ~ 0
+3.3V
$Comp
L GND-RESCUE-flight_controller #PWR?
U 1 1 568FC6D3
P 3550 4200
F 0 "#PWR?" H 3550 4200 30  0001 C CNN
F 1 "GND" H 3550 4130 30  0001 C CNN
F 2 "" H 3550 4200 60  0001 C CNN
F 3 "" H 3550 4200 60  0001 C CNN
	1    3550 4200
	1    0    0    -1  
$EndComp
Text HLabel 3450 4700 0    60   Input ~ 0
FRAM_SDI
Text HLabel 2950 4950 0    60   Input ~ 0
FRAM_SCK
Text HLabel 3100 3850 0    60   Input ~ 0
FRAM_SDO
Text HLabel 6100 3750 2    60   Input ~ 0
+3.3V
Text HLabel 3100 3750 0    60   Input ~ 0
FRAM_NCS
$Comp
L FM25V01 U?
U 1 1 568FC49C
P 4500 3900
F 0 "U?" H 4500 3700 60  0000 C CNN
F 1 "FM25V01" H 4500 4000 60  0000 C CNN
F 2 "" H 4500 3900 60  0001 C CNN
F 3 "" H 4500 3900 60  0001 C CNN
	1    4500 3900
	1    0    0    -1  
$EndComp
$EndSCHEMATC
