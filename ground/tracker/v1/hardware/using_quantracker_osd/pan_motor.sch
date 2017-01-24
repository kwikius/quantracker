EESchema Schematic File Version 2
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
LIBS:quantracker_air_osd_v2_1
LIBS:conn_1
LIBS:conn_2
LIBS:conn_3
LIBS:conn_4
LIBS:conn_5
LIBS:conn_6
LIBS:conn_7
LIBS:conn_8
LIBS:cap3t
LIBS:si8621
LIBS:si8622
LIBS:74aup2g240
LIBS:si8620
LIBS:si8610
LIBS:smpsu_module
LIBS:tlcfilt
LIBS:rc3t
LIBS:mic5205
LIBS:tracker-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 3 3
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L SI8620 U4
U 1 1 587F7983
P 3525 3325
F 0 "U4" H 4075 2625 60  0000 C CNN
F 1 "SI8620" H 3525 2675 60  0000 C CNN
F 2 "" H 3525 3325 60  0001 C CNN
F 3 "" H 3525 3325 60  0001 C CNN
	1    3525 3325
	1    0    0    -1  
$EndComp
$Comp
L CONN_5 P3
U 1 1 587F798A
P 9775 3100
F 0 "P3" V 9725 3100 50  0000 C CNN
F 1 "CONN_5" V 9825 3100 50  0000 C CNN
F 2 "" H 9775 3100 60  0001 C CNN
F 3 "" H 9775 3100 60  0001 C CNN
	1    9775 3100
	1    0    0    1   
$EndComp
$Comp
L CONN_4 P2
U 1 1 587F7991
P 6600 6175
F 0 "P2" V 6550 6175 50  0000 C CNN
F 1 "CONN_4" V 6650 6175 50  0000 C CNN
F 2 "" H 6600 6175 60  0001 C CNN
F 3 "" H 6600 6175 60  0001 C CNN
	1    6600 6175
	1    0    0    1   
$EndComp
Text Notes 9800 2700 0    60   ~ 0
Pan motor control
Text Notes 6300 5875 0    60   ~ 0
Panmotor encoder
Text HLabel 2075 2525 0    60   UnSpc ~ 0
3.3V
Text HLabel 1950 7400 0    60   UnSpc ~ 0
GND
Text HLabel 9450 4225 2    60   UnSpc ~ 0
PAN_GND
Text HLabel 2050 3475 0    60   UnSpc ~ 0
PAN_PWM
Text HLabel 2050 3175 0    60   UnSpc ~ 0
PAN_DIR
$Comp
L 74AHC1G04 U5
U 1 1 587F9F1D
P 5475 2800
F 0 "U5" H 5670 2915 50  0000 C CNN
F 1 "74AHC1G04" H 5775 2650 50  0000 C CNN
F 2 "" H 5475 2800 50  0000 C CNN
F 3 "" H 5475 2800 50  0000 C CNN
	1    5475 2800
	1    0    0    -1  
$EndComp
Text Label 5800 2125 0    60   ~ 0
PAN_5V
Text HLabel 2075 4775 0    60   Output ~ 0
MOTOR_ENCA
Text HLabel 2075 5350 0    60   Output ~ 0
MOTOR_ENCB
Text HLabel 4350 6225 0    60   UnSpc ~ 0
+5V
$Comp
L C C18
U 1 1 587FB33F
P 2325 7125
F 0 "C18" H 2350 7225 50  0000 L CNN
F 1 "0.1uF" H 2350 7025 50  0000 L CNN
F 2 "SMD_0603:SMD_0603" H 2363 6975 50  0001 C CNN
F 3 "" H 2325 7125 50  0000 C CNN
	1    2325 7125
	1    0    0    -1  
$EndComp
$Comp
L C C19
U 1 1 587FB43C
P 4525 3800
F 0 "C19" H 4550 3900 50  0000 L CNN
F 1 "C" H 4550 3700 50  0000 L CNN
F 2 "" H 4563 3650 50  0000 C CNN
F 3 "" H 4525 3800 50  0000 C CNN
	1    4525 3800
	1    0    0    -1  
$EndComp
$Comp
L C C20
U 1 1 587FB7FF
P 4800 3800
F 0 "C20" H 4825 3900 50  0000 L CNN
F 1 "C" H 4825 3700 50  0000 L CNN
F 2 "" H 4838 3650 50  0000 C CNN
F 3 "" H 4800 3800 50  0000 C CNN
	1    4800 3800
	1    0    0    -1  
$EndComp
$Comp
L CAP3T C22
U 1 1 587FBE06
P 6625 2125
F 0 "C22" H 6775 1875 60  0000 C CNN
F 1 "NFM18PS105R0J3" H 6825 2375 60  0000 C CNN
F 2 "" H 6625 2125 60  0001 C CNN
F 3 "" H 6625 2125 60  0001 C CNN
	1    6625 2125
	1    0    0    -1  
$EndComp
$Comp
L CONN_3 K4
U 1 1 587FD7E2
P 10475 5875
F 0 "K4" V 10425 5875 50  0000 C CNN
F 1 "CONN_3" V 10525 5875 40  0000 C CNN
F 2 "" H 10475 5875 60  0001 C CNN
F 3 "" H 10475 5875 60  0001 C CNN
	1    10475 5875
	1    0    0    1   
$EndComp
Text HLabel 7500 5975 0    60   UnSpc ~ 0
PAN_GND
Wire Wire Line
	9375 3300 9100 3300
Wire Wire Line
	3875 4225 9450 4225
Wire Wire Line
	3875 4225 3875 4075
Wire Wire Line
	1950 7400 6000 7400
Wire Wire Line
	3175 7400 3175 4075
Wire Wire Line
	9375 3200 9075 3200
Wire Wire Line
	3175 2525 3175 2625
Wire Wire Line
	2075 2525 3175 2525
Wire Wire Line
	4775 3175 4775 2800
Wire Wire Line
	4775 2800 5025 2800
Wire Wire Line
	5925 2800 6225 2800
Wire Wire Line
	6225 2800 6225 2650
Wire Wire Line
	6225 2650 6975 2650
Wire Wire Line
	7275 2650 8600 2650
Wire Wire Line
	8600 2650 8600 2900
Wire Wire Line
	8600 2900 9375 2900
Wire Wire Line
	6275 3175 6275 3000
Wire Wire Line
	6275 3000 7600 3000
Wire Wire Line
	7900 3000 9375 3000
Connection ~ 4775 3175
Wire Wire Line
	4375 3175 6275 3175
Wire Wire Line
	8700 3475 8700 3100
Wire Wire Line
	8700 3100 9375 3100
Wire Wire Line
	2675 3175 2050 3175
Wire Wire Line
	2675 3475 2050 3475
Wire Wire Line
	3875 2625 3875 2125
Wire Wire Line
	3875 2125 6375 2125
Wire Wire Line
	6875 2125 9075 2125
Connection ~ 3175 7400
Wire Wire Line
	2325 6975 2325 2525
Connection ~ 2325 2525
Wire Wire Line
	2325 7275 2325 7400
Connection ~ 2325 7400
Wire Wire Line
	4525 2125 4525 3650
Connection ~ 4525 2125
Wire Wire Line
	4525 3950 4525 4225
Connection ~ 4525 4225
Wire Wire Line
	4525 3575 4800 3575
Wire Wire Line
	4800 3575 4800 3650
Connection ~ 4525 3575
Wire Wire Line
	4800 3950 4800 4225
Connection ~ 4800 4225
Wire Wire Line
	5425 3000 5425 4225
Connection ~ 5425 4225
Wire Wire Line
	5425 2600 5425 2125
Connection ~ 5425 2125
Wire Wire Line
	9100 3300 9100 4225
Connection ~ 9100 4225
Wire Wire Line
	9075 2125 9075 3200
Wire Wire Line
	6625 2325 6625 4225
Connection ~ 6625 4225
Wire Wire Line
	4375 3475 8125 3475
Wire Wire Line
	8425 3475 8700 3475
Wire Wire Line
	7500 5975 10125 5975
Text HLabel 7325 5075 0    60   Input ~ 0
VIN+
$Comp
L D_Schottky D2
U 1 1 587FDF21
P 8150 5075
F 0 "D2" H 8150 5175 50  0000 C CNN
F 1 "D_Schottky" H 8150 4975 50  0000 C CNN
F 2 "" H 8150 5075 50  0000 C CNN
F 3 "" H 8150 5075 50  0000 C CNN
	1    8150 5075
	-1   0    0    1   
$EndComp
Wire Wire Line
	7325 5075 8000 5075
Wire Wire Line
	8300 5075 9025 5075
Wire Wire Line
	9525 5075 9850 5075
Wire Wire Line
	9850 5075 9850 5875
Wire Wire Line
	9850 5875 10125 5875
NoConn ~ 10125 5775
$Comp
L CAP3T C26
U 1 1 587FE706
P 9275 5075
F 0 "C26" H 9425 4825 60  0000 C CNN
F 1 "NFM31PC276B0J3" H 9475 5325 60  0000 C CNN
F 2 "" H 9275 5075 60  0001 C CNN
F 3 "" H 9275 5075 60  0001 C CNN
	1    9275 5075
	1    0    0    -1  
$EndComp
Wire Wire Line
	9275 5275 9275 5975
Connection ~ 9275 5975
Wire Wire Line
	6250 6325 6000 6325
Wire Wire Line
	6000 6325 6000 7400
Wire Wire Line
	4350 6225 5050 6225
Wire Wire Line
	6250 6225 5550 6225
$Comp
L CAP3T C21
U 1 1 587FF1BF
P 5300 6225
F 0 "C21" H 5450 5975 60  0000 C CNN
F 1 "NFM18PS105R0J3" H 5500 6475 60  0000 C CNN
F 2 "" H 5300 6225 60  0001 C CNN
F 3 "" H 5300 6225 60  0001 C CNN
	1    5300 6225
	1    0    0    -1  
$EndComp
Wire Wire Line
	5300 6425 5300 7400
Connection ~ 5300 7400
Wire Wire Line
	4675 4775 2075 4775
Wire Wire Line
	6250 6025 6050 6025
Wire Wire Line
	6050 6025 6050 4775
Wire Wire Line
	6050 4775 4975 4775
Wire Wire Line
	6250 6125 5900 6125
Wire Wire Line
	5900 6125 5900 5350
Wire Wire Line
	5900 5350 4050 5350
Wire Wire Line
	3750 5350 2075 5350
$Comp
L CP1 C24
U 1 1 58801015
P 8450 5500
F 0 "C24" H 8475 5600 50  0000 L CNN
F 1 "CP1" H 8475 5400 50  0000 L CNN
F 2 "" H 8450 5500 50  0000 C CNN
F 3 "" H 8450 5500 50  0000 C CNN
	1    8450 5500
	1    0    0    -1  
$EndComp
$Comp
L CP1 C25
U 1 1 58801095
P 8750 5500
F 0 "C25" H 8775 5600 50  0000 L CNN
F 1 "CP1" H 8775 5400 50  0000 L CNN
F 2 "" H 8750 5500 50  0000 C CNN
F 3 "" H 8750 5500 50  0000 C CNN
	1    8750 5500
	1    0    0    -1  
$EndComp
Wire Wire Line
	8450 5350 8450 5075
Connection ~ 8450 5075
Wire Wire Line
	8750 5350 8750 5075
Connection ~ 8750 5075
Wire Wire Line
	8450 5650 8450 5975
Connection ~ 8450 5975
Wire Wire Line
	8750 5650 8750 5975
Connection ~ 8750 5975
Text Notes 10100 5350 0    60   ~ 0
Pan Motor power
$Comp
L C C23
U 1 1 5880246B
P 7775 5525
F 0 "C23" H 7800 5625 50  0000 L CNN
F 1 "C" H 7800 5425 50  0000 L CNN
F 2 "" H 7813 5375 50  0000 C CNN
F 3 "" H 7775 5525 50  0000 C CNN
	1    7775 5525
	1    0    0    -1  
$EndComp
Wire Wire Line
	7775 5375 7775 5075
Connection ~ 7775 5075
Wire Wire Line
	7775 5675 7775 5975
Connection ~ 7775 5975
$Comp
L R R7
U 1 1 5880D5EC
P 7125 2650
F 0 "R7" V 7205 2650 50  0000 C CNN
F 1 "R" V 7125 2650 50  0000 C CNN
F 2 "" V 7055 2650 50  0000 C CNN
F 3 "" H 7125 2650 50  0000 C CNN
	1    7125 2650
	0    1    1    0   
$EndComp
$Comp
L R R8
U 1 1 5880D660
P 7750 3000
F 0 "R8" V 7830 3000 50  0000 C CNN
F 1 "R" V 7750 3000 50  0000 C CNN
F 2 "" V 7680 3000 50  0000 C CNN
F 3 "" H 7750 3000 50  0000 C CNN
	1    7750 3000
	0    1    1    0   
$EndComp
$Comp
L R R9
U 1 1 5880D6D4
P 8275 3475
F 0 "R9" V 8355 3475 50  0000 C CNN
F 1 "R" V 8275 3475 50  0000 C CNN
F 2 "" V 8205 3475 50  0000 C CNN
F 3 "" H 8275 3475 50  0000 C CNN
	1    8275 3475
	0    1    1    0   
$EndComp
$Comp
L R R5
U 1 1 5880E2B6
P 3900 5350
F 0 "R5" V 3980 5350 50  0000 C CNN
F 1 "R" V 3900 5350 50  0000 C CNN
F 2 "" V 3830 5350 50  0000 C CNN
F 3 "" H 3900 5350 50  0000 C CNN
	1    3900 5350
	0    1    1    0   
$EndComp
$Comp
L R R6
U 1 1 5880E333
P 4825 4775
F 0 "R6" V 4905 4775 50  0000 C CNN
F 1 "R" V 4825 4775 50  0000 C CNN
F 2 "" V 4755 4775 50  0000 C CNN
F 3 "" H 4825 4775 50  0000 C CNN
	1    4825 4775
	0    1    1    0   
$EndComp
$Comp
L PWR_FLAG #FLG010
U 1 1 58820405
P 7825 4025
F 0 "#FLG010" H 7825 4120 50  0001 C CNN
F 1 "PWR_FLAG" H 7825 4205 50  0000 C CNN
F 2 "" H 7825 4025 50  0000 C CNN
F 3 "" H 7825 4025 50  0000 C CNN
	1    7825 4025
	1    0    0    -1  
$EndComp
Wire Wire Line
	7825 4025 7825 4225
Connection ~ 7825 4225
$Comp
L PWR_FLAG #FLG011
U 1 1 58820682
P 8125 1950
F 0 "#FLG011" H 8125 2045 50  0001 C CNN
F 1 "PWR_FLAG" H 8125 2130 50  0000 C CNN
F 2 "" H 8125 1950 50  0000 C CNN
F 3 "" H 8125 1950 50  0000 C CNN
	1    8125 1950
	1    0    0    -1  
$EndComp
Wire Wire Line
	8125 1950 8125 2125
Connection ~ 8125 2125
$Comp
L PWR_FLAG #FLG012
U 1 1 58820928
P 5025 2000
F 0 "#FLG012" H 5025 2095 50  0001 C CNN
F 1 "PWR_FLAG" H 5025 2180 50  0000 C CNN
F 2 "" H 5025 2000 50  0000 C CNN
F 3 "" H 5025 2000 50  0000 C CNN
	1    5025 2000
	1    0    0    -1  
$EndComp
Wire Wire Line
	5025 2000 5025 2125
Connection ~ 5025 2125
$EndSCHEMATC