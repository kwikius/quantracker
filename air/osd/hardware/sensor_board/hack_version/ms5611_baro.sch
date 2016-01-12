EESchema Schematic File Version 2  date Fri 08 Jan 2016 14:31:29 GMT
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
EELAYER 25  0
EELAYER END
$Descr A4 11700 8267
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
	6500 3200 6150 3200
Wire Wire Line
	6550 3400 6150 3400
Wire Wire Line
	3350 3200 3150 3200
Wire Wire Line
	4150 3700 4150 3850
Connection ~ 4500 3200
Wire Wire Line
	4500 3200 4500 3300
Wire Wire Line
	4500 3300 4750 3300
Wire Wire Line
	4750 3200 3950 3200
Wire Wire Line
	4150 3300 4150 3200
Connection ~ 4150 3200
Wire Wire Line
	4750 3500 4600 3500
Wire Wire Line
	4600 3500 4600 3900
Wire Wire Line
	4600 3900 6350 3900
Wire Wire Line
	6350 3900 6350 3500
Wire Wire Line
	4750 3400 4450 3400
Wire Wire Line
	4450 3400 4450 3750
Wire Wire Line
	4450 3750 4150 3750
Connection ~ 4150 3750
Wire Wire Line
	6550 3500 6150 3500
Connection ~ 6350 3500
Wire Wire Line
	6550 3300 6150 3300
Text HLabel 6550 3500 2    60   Input ~ 0
BARO_NCS
Text HLabel 6550 3400 2    60   Output ~ 0
BARO_SDO
Text HLabel 6550 3300 2    60   Input ~ 0
BARO_SDI
Text HLabel 6500 3200 2    60   Input ~ 0
BARO_SCK_IN
$Comp
L GND #PWR?
U 1 1 568FC096
P 4150 3850
F 0 "#PWR?" H 4150 3850 30  0001 C CNN
F 1 "GND" H 4150 3780 30  0001 C CNN
	1    4150 3850
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 568FC025
P 4150 3500
F 0 "C?" H 4200 3600 50  0000 L CNN
F 1 "C" H 4200 3400 50  0000 L CNN
	1    4150 3500
	1    0    0    -1  
$EndComp
$Comp
L INDUCTOR L?
U 1 1 568FC014
P 3650 3200
F 0 "L?" V 3600 3200 40  0000 C CNN
F 1 "INDUCTOR" V 3750 3200 40  0000 C CNN
	1    3650 3200
	0    1    1    0   
$EndComp
Text HLabel 3150 3200 0    60   BiDi ~ 0
+3.3V
$Comp
L MS5611 U?
U 1 1 568FBFCB
P 5450 3350
F 0 "U?" H 5450 3100 60  0000 C CNN
F 1 "MS5611" H 5450 3600 60  0000 C CNN
	1    5450 3350
	1    0    0    -1  
$EndComp
$EndSCHEMATC
