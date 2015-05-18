EESchema Schematic File Version 2  date Sat 16 May 2015 15:45:47 BST
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
LIBS:tracker_using_quantracker_osd-cache
EELAYER 25  0
EELAYER END
$Descr A4 11700 8267
encoding utf-8
Sheet 2 3
Title ""
Date "16 may 2015"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text HLabel 7100 6300 0    60   Input ~ 0
NOT_PAN_MOTOR_DIR
Text HLabel 4750 6350 0    60   Input ~ 0
PAN_MOTOR_DIR
$Comp
L 74HC04 U?
U 1 1 5553511B
P 5450 6350
F 0 "U?" H 5600 6500 40  0000 C CNN
F 1 "74HC04" H 5650 6250 40  0000 C CNN
	1    5450 6350
	1    0    0    -1  
$EndComp
Text Notes 6300 3650 0    60   ~ 0
2V
Wire Wire Line
	6350 4650 6250 4650
Wire Wire Line
	6350 3700 6350 4650
Wire Wire Line
	7100 3700 6350 3700
Connection ~ 7100 3700
$Comp
L LM358 U?
U 1 1 55534E8A
P 5750 4750
AR Path="/555339C1/55534429" Ref="U?"  Part="1" 
AR Path="/555339C1/55534E8A" Ref="U?"  Part="1" 
F 0 "U?" H 5750 4950 60  0000 L CNN
F 1 "LM358" H 5700 4500 60  0000 L CNN
	1    5750 4750
	-1   0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 55534E88
P 7000 4850
F 0 "R?" V 7080 4850 50  0000 C CNN
F 1 "R" V 7000 4850 50  0000 C CNN
	1    7000 4850
	0    1    1    0   
$EndComp
$Comp
L R R?
U 1 1 55534E87
P 7100 4000
F 0 "R?" V 7180 4000 50  0000 C CNN
F 1 "R" V 7100 4000 50  0000 C CNN
	1    7100 4000
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 55534E86
P 7100 3400
F 0 "R?" V 7180 3400 50  0000 C CNN
F 1 "R" V 7100 3400 50  0000 C CNN
	1    7100 3400
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 55534E85
P 5900 5150
F 0 "R?" V 5980 5150 50  0000 C CNN
F 1 "R" V 5900 5150 50  0000 C CNN
	1    5900 5150
	0    1    1    0   
$EndComp
Text HLabel 7000 3050 0    60   Input ~ 0
3.3VO
$Comp
L GND #PWR?
U 1 1 55534E84
P 7100 4400
F 0 "#PWR?" H 7100 4400 30  0001 C CNN
F 1 "GND" H 7100 4330 30  0001 C CNN
	1    7100 4400
	1    0    0    -1  
$EndComp
Wire Wire Line
	7100 3050 7100 3150
Wire Wire Line
	7000 3050 7100 3050
Wire Wire Line
	7100 4400 7100 4250
Wire Wire Line
	5100 4750 4950 4750
Wire Wire Line
	5250 4750 5100 4750
Wire Wire Line
	6350 4850 6250 4850
Wire Wire Line
	6750 4850 6350 4850
Wire Wire Line
	8900 4850 7250 4850
Wire Wire Line
	6350 5150 6150 5150
Wire Wire Line
	6350 4850 6350 5150
Connection ~ 6350 4850
Wire Wire Line
	5100 5150 5650 5150
Wire Wire Line
	5100 4750 5100 5150
Connection ~ 5100 4750
Wire Wire Line
	7100 3700 7100 3650
Wire Wire Line
	7100 3750 7100 3700
Text HLabel 4950 4750 0    60   Output ~ 0
AZIMUTH_MOTOR_CURRENT
Text Label 8550 4850 2    60   ~ 0
RAW_CURRENT_IN
Text Label 8450 4950 2    60   ~ 0
5VO
Text Label 8450 5050 2    60   ~ 0
GND
Text Notes 8800 4650 0    60   ~ 0
PanMotorCurrent
$Comp
L CONN_3 K?
U 1 1 55534ACE
P 9250 4950
F 0 "K?" V 9200 4950 50  0000 C CNN
F 1 "CONN_3" V 9300 4950 40  0000 C CNN
	1    9250 4950
	1    0    0    -1  
$EndComp
Text HLabel 4950 2750 0    60   Output ~ 0
AZIMUTH_MOTOR_SPEED
Connection ~ 6350 2650
Connection ~ 7100 1700
Wire Wire Line
	7100 1700 6350 1700
Wire Wire Line
	6350 1700 6350 2650
Wire Wire Line
	7100 1750 7100 1700
Wire Wire Line
	7100 1700 7100 1650
Connection ~ 5100 2750
Wire Wire Line
	5100 2750 5100 3150
Wire Wire Line
	5100 3150 5650 3150
Connection ~ 6350 2850
Wire Wire Line
	6350 2850 6350 3150
Wire Wire Line
	6350 3150 6150 3150
Wire Wire Line
	7250 2650 6350 2650
Wire Wire Line
	6350 2650 6250 2650
Wire Wire Line
	8900 2850 7750 2850
Wire Wire Line
	8100 2750 8100 2650
Wire Wire Line
	8100 2750 8900 2750
Wire Wire Line
	8100 2650 7750 2650
Wire Wire Line
	7250 2850 6350 2850
Wire Wire Line
	6350 2850 6250 2850
Wire Wire Line
	5250 2750 5100 2750
Wire Wire Line
	5100 2750 4950 2750
Wire Wire Line
	7100 2400 7100 2250
Wire Wire Line
	7000 1050 7100 1050
Wire Wire Line
	7100 1050 7100 1150
$Comp
L GND #PWR?
U 1 1 55534609
P 7100 2400
F 0 "#PWR?" H 7100 2400 30  0001 C CNN
F 1 "GND" H 7100 2330 30  0001 C CNN
	1    7100 2400
	1    0    0    -1  
$EndComp
Text HLabel 7000 1050 0    60   Input ~ 0
3.3VO
$Comp
L R R?
U 1 1 55534487
P 5900 3150
F 0 "R?" V 5980 3150 50  0000 C CNN
F 1 "R" V 5900 3150 50  0000 C CNN
	1    5900 3150
	0    1    1    0   
$EndComp
$Comp
L R R?
U 1 1 55534465
P 7100 1400
F 0 "R?" V 7180 1400 50  0000 C CNN
F 1 "R" V 7100 1400 50  0000 C CNN
	1    7100 1400
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 55534462
P 7100 2000
F 0 "R?" V 7180 2000 50  0000 C CNN
F 1 "R" V 7100 2000 50  0000 C CNN
	1    7100 2000
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 55534441
P 7500 2850
F 0 "R?" V 7580 2850 50  0000 C CNN
F 1 "R" V 7500 2850 50  0000 C CNN
	1    7500 2850
	0    1    1    0   
$EndComp
$Comp
L R R?
U 1 1 5553443B
P 7500 2650
F 0 "R?" V 7580 2650 50  0000 C CNN
F 1 "R" V 7500 2650 50  0000 C CNN
	1    7500 2650
	0    1    1    0   
$EndComp
$Comp
L LM358 U?
U 1 1 55534429
P 5750 2750
F 0 "U?" H 5750 2950 60  0000 L CNN
F 1 "LM358" H 5700 2500 60  0000 L CNN
	1    5750 2750
	-1   0    0    -1  
$EndComp
Text Label 8650 6600 2    60   ~ 0
AZIMUTH_MOTOR_PWM
Text Label 8650 6500 2    60   ~ 0
NOT_AZIMUTH_MOTOR_DIR
Text Label 8650 6400 2    60   ~ 0
AZIMUTH_MOTOR_DIR
Text Label 8650 6700 2    60   ~ 0
AZIMUTH_MOTOR_GND
$Comp
L CONN_4 P?
U 1 1 55533EBC
P 9250 6550
F 0 "P?" V 9200 6550 50  0000 C CNN
F 1 "CONN_4" V 9300 6550 50  0000 C CNN
	1    9250 6550
	1    0    0    -1  
$EndComp
Text Label 8600 3250 2    60   ~ 0
ENCA
Text Label 8600 3150 2    60   ~ 0
ENCB
Text Label 8650 3050 2    60   ~ 0
ENC5V
Text Label 8650 2950 2    60   ~ 0
ENCGND
Text Label 8650 2850 2    60   ~ 0
MOTORB
Text Label 8650 2750 2    60   ~ 0
MOTORA
Text Notes 9450 2600 2    60   ~ 0
PanMotor Fdbk
$Comp
L CONN_6 P?
U 1 1 55533AEB
P 9250 3000
F 0 "P?" V 9200 3000 60  0000 C CNN
F 1 "CONN_6" V 9300 3000 60  0000 C CNN
	1    9250 3000
	1    0    0    -1  
$EndComp
Text Notes 9400 6200 2    60   ~ 0
PanMotor Ctrl
Text Notes 9400 3800 2    60   ~ 0
PanMotor Pwr
$Comp
L CONN_2 P?
U 1 1 55533AE9
P 9300 4050
F 0 "P?" V 9250 4050 40  0000 C CNN
F 1 "CONN_2" V 9350 4050 40  0000 C CNN
	1    9300 4050
	1    0    0    -1  
$EndComp
$EndSCHEMATC
