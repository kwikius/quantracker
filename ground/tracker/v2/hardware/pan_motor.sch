EESchema Schematic File Version 2  date Wed 20 May 2015 18:56:24 BST
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
Date "20 may 2015"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text Notes 5300 3950 0    60   ~ 0
using ACS712 hall effect current sensor
Wire Wire Line
	7350 5050 7350 5250
Wire Wire Line
	7350 5250 7200 5250
Connection ~ 1800 4700
Wire Wire Line
	1800 5450 1800 4700
Connection ~ 3000 6500
Wire Wire Line
	3000 6500 3000 6350
Connection ~ 3000 5750
Wire Wire Line
	3000 5850 3000 5750
Wire Wire Line
	3450 5550 3450 5400
Connection ~ 5350 6600
Wire Wire Line
	5350 6700 5350 6600
Connection ~ 1800 900 
Wire Wire Line
	2450 1400 2450 900 
Wire Wire Line
	2450 900  1800 900 
Wire Wire Line
	1800 900  1700 900 
Wire Wire Line
	1800 1200 1800 900 
Connection ~ 7350 4450
Wire Wire Line
	7100 4450 7350 4450
Wire Wire Line
	7350 4350 7350 4450
Wire Wire Line
	7350 4450 7350 4550
Wire Wire Line
	8900 3950 8450 3950
Wire Wire Line
	8900 5350 8700 5350
Connection ~ 3800 3150
Wire Wire Line
	3800 3150 3800 3350
Wire Wire Line
	3800 3350 3900 3350
Wire Wire Line
	6450 3250 7400 3250
Wire Wire Line
	7900 3250 8900 3250
Wire Wire Line
	8650 3050 8900 3050
Wire Wire Line
	6750 6700 8900 6700
Wire Wire Line
	7950 6500 8900 6500
Wire Wire Line
	6750 6500 7450 6500
Wire Wire Line
	3700 5500 3450 5500
Wire Wire Line
	6100 4550 5950 4550
Wire Wire Line
	5950 4550 5800 4550
Wire Wire Line
	7100 4650 7150 4650
Wire Wire Line
	8900 3750 7350 3750
Wire Wire Line
	7150 4650 7150 4950
Wire Wire Line
	7150 4950 5950 4950
Wire Wire Line
	5950 4950 5950 4550
Connection ~ 5950 4550
Connection ~ 4600 2650
Connection ~ 5350 1700
Wire Wire Line
	5350 1700 4600 1700
Wire Wire Line
	4600 1700 4600 2650
Wire Wire Line
	5350 1650 5350 1700
Wire Wire Line
	5350 1700 5350 1750
Connection ~ 3350 2750
Wire Wire Line
	3350 2750 3350 3150
Wire Wire Line
	3350 3150 3800 3150
Wire Wire Line
	3800 3150 3900 3150
Connection ~ 4600 2850
Wire Wire Line
	4600 2850 4600 3150
Wire Wire Line
	4600 3150 4500 3150
Wire Wire Line
	4500 3150 4400 3150
Wire Wire Line
	4500 2650 4600 2650
Wire Wire Line
	4600 2650 4800 2650
Wire Wire Line
	8900 2850 5300 2850
Wire Wire Line
	5950 2750 5950 2650
Wire Wire Line
	5950 2750 8900 2750
Wire Wire Line
	5950 2650 5300 2650
Wire Wire Line
	4500 2850 4600 2850
Wire Wire Line
	4600 2850 4800 2850
Wire Wire Line
	3200 2750 3350 2750
Wire Wire Line
	3350 2750 3500 2750
Wire Wire Line
	5250 1050 5350 1050
Wire Wire Line
	5350 1050 5350 1150
Wire Wire Line
	3150 5750 3000 5750
Wire Wire Line
	3000 5750 2850 5750
Wire Wire Line
	8900 6400 7600 6400
Wire Wire Line
	7100 6400 6750 6400
Wire Wire Line
	8900 6600 8350 6600
Wire Wire Line
	5200 6600 5350 6600
Wire Wire Line
	5350 6600 7850 6600
Wire Wire Line
	8900 2950 8650 2950
Wire Wire Line
	8900 3150 7450 3150
Wire Wire Line
	6950 3150 6450 3150
Wire Wire Line
	4400 3350 4500 3350
Wire Wire Line
	4500 3350 4500 3150
Connection ~ 4500 3150
Wire Wire Line
	8900 5150 8700 5150
Wire Wire Line
	8900 3850 8450 3850
Wire Wire Line
	7350 3750 7350 3850
Wire Wire Line
	5350 2250 5350 2500
Wire Wire Line
	5350 2500 5400 2500
Wire Wire Line
	2600 2250 2450 2250
Wire Wire Line
	2450 2250 1800 2250
Wire Wire Line
	1800 2250 1800 2100
Wire Wire Line
	2450 1800 2450 2250
Connection ~ 2450 2250
Wire Wire Line
	5250 7400 5350 7400
Wire Wire Line
	5350 7400 5350 7200
Wire Wire Line
	1650 6500 1800 6500
Wire Wire Line
	1800 6500 3000 6500
Wire Wire Line
	3000 6500 3450 6500
Wire Wire Line
	3450 6500 3450 5950
Wire Wire Line
	1600 4700 1800 4700
Wire Wire Line
	1800 4700 3450 4700
Wire Wire Line
	3450 4700 3450 4900
Wire Wire Line
	1800 5850 1800 6500
Connection ~ 1800 6500
$Comp
L C C?
U 1 1 555CC8B8
P 1800 5650
F 0 "C?" H 1850 5750 50  0000 L CNN
F 1 "C" H 1850 5550 50  0000 L CNN
	1    1800 5650
	1    0    0    -1  
$EndComp
Text HLabel 1600 4700 0    60   Input ~ 0
3.3VO
Text HLabel 1650 6500 0    60   BiDi ~ 0
PAN_MOTOR_GND
$Comp
L R R?
U 1 1 555CC822
P 3000 6100
F 0 "R?" V 3080 6100 50  0000 C CNN
F 1 "10K" V 3000 6100 50  0000 C CNN
	1    3000 6100
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 555CC7B6
P 3450 5150
F 0 "R?" V 3530 5150 50  0000 C CNN
F 1 "4K7" V 3450 5150 50  0000 C CNN
	1    3450 5150
	1    0    0    -1  
$EndComp
$Comp
L MOS_N Q?
U 1 1 555CC783
P 3350 5750
F 0 "Q?" H 3360 5920 60  0000 R CNN
F 1 "MOS_N" H 3360 5600 60  0000 R CNN
	1    3350 5750
	1    0    0    -1  
$EndComp
Text HLabel 5250 7400 0    60   BiDi ~ 0
PAN_MOTOR_GND
$Comp
L R R?
U 1 1 555CC720
P 5350 6950
F 0 "R?" V 5430 6950 50  0000 C CNN
F 1 "10K" V 5350 6950 50  0000 C CNN
	1    5350 6950
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 555CC600
P 2450 1600
F 0 "C?" H 2500 1700 50  0000 L CNN
F 1 "C" H 2500 1500 50  0000 L CNN
	1    2450 1600
	1    0    0    -1  
$EndComp
Text HLabel 1700 900  0    60   Input ~ 0
3.3VO
Text HLabel 2600 2250 2    60   BiDi ~ 0
PAN_MOTOR_GND
Text HLabel 5400 2500 2    60   BiDi ~ 0
PAN_MOTOR_GND
Text HLabel 7200 5250 0    60   BiDi ~ 0
PAN_MOTOR_GND
$Comp
L R R?
U 1 1 555CC20A
P 7350 4800
F 0 "R?" V 7430 4800 50  0000 C CNN
F 1 "3.3K" V 7350 4800 50  0000 C CNN
	1    7350 4800
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 555CC206
P 7350 4100
F 0 "R?" V 7430 4100 50  0000 C CNN
F 1 "1.7K" V 7350 4100 50  0000 C CNN
	1    7350 4100
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 555C95D7
P 8100 6600
F 0 "R?" V 8180 6600 50  0000 C CNN
F 1 "100R" V 8100 6600 50  0000 C CNN
	1    8100 6600
	0    1    1    0   
$EndComp
$Comp
L R R?
U 1 1 555C95CE
P 7700 6500
F 0 "R?" V 7780 6500 50  0000 C CNN
F 1 "100R" V 7700 6500 50  0000 C CNN
	1    7700 6500
	0    1    1    0   
$EndComp
$Comp
L R R?
U 1 1 555C95C9
P 7350 6400
F 0 "R?" V 7430 6400 50  0000 C CNN
F 1 "100R" V 7350 6400 50  0000 C CNN
	1    7350 6400
	0    1    1    0   
$EndComp
$Comp
L R R?
U 1 1 555C9581
P 7650 3250
F 0 "R?" V 7730 3250 50  0000 C CNN
F 1 "100R" V 7650 3250 50  0000 C CNN
	1    7650 3250
	0    1    1    0   
$EndComp
$Comp
L R R?
U 1 1 555C957A
P 7200 3150
F 0 "R?" V 7280 3150 50  0000 C CNN
F 1 "100R" V 7200 3150 50  0000 C CNN
	1    7200 3150
	0    1    1    0   
$EndComp
Text HLabel 8700 5150 0    60   BiDi ~ 0
+12V
Text HLabel 8700 5350 0    60   BiDi ~ 0
PAN_MOTOR_GND
$Comp
L R R?
U 1 1 555C9395
P 4150 3350
F 0 "R?" V 4230 3350 50  0000 C CNN
F 1 "27.4K" V 4150 3350 50  0000 C CNN
	1    4150 3350
	0    1    1    0   
$EndComp
$Comp
L LM358 U?
U 3 1 555C8F6A
P 1700 1650
AR Path="/555339C1/55534429" Ref="U?"  Part="1" 
AR Path="/555339C1/555C8F6A" Ref="U?"  Part="3" 
F 0 "U?" H 1700 1850 60  0000 L CNN
F 1 "LM6132" H 1650 1400 60  0000 L CNN
	3    1700 1650
	-1   0    0    -1  
$EndComp
Text HLabel 6450 3250 0    60   Output ~ 0
ENCODERB
Text HLabel 6450 3150 0    60   Output ~ 0
ENCODER_A
Text HLabel 8650 3050 0    60   BiDi ~ 0
PAN_MOTOR_5V
Text HLabel 8650 2950 0    60   BiDi ~ 0
PAN_MOTOR_GND
Text HLabel 8450 3850 0    60   BiDi ~ 0
PAN_MOTOR_5V
Text HLabel 8450 3950 0    60   BiDi ~ 0
PAN_MOTOR_GND
Text HLabel 6750 6700 0    60   BiDi ~ 0
PAN_MOTOR_GND
Text HLabel 5200 6600 0    60   Input ~ 0
PAN_MOTOR_PWM
Text HLabel 6750 6500 0    60   Input ~ 0
NOT_PAN_MOTOR_DIR
Text HLabel 6750 6400 0    60   Input ~ 0
PAN_MOTOR_DIR
Text HLabel 3700 5500 2    60   Output ~ 0
NOT_PAN_MOTOR_DIR
Text HLabel 2850 5750 0    60   Input ~ 0
PAN_MOTOR_DIR
$Comp
L LM358 U?
U 2 1 55534E8A
P 6600 4550
AR Path="/555339C1/55534429" Ref="U?"  Part="1" 
AR Path="/555339C1/55534E8A" Ref="U?"  Part="2" 
F 0 "U?" H 6600 4750 60  0000 L CNN
F 1 "LM6132" H 6550 4300 60  0000 L CNN
	2    6600 4550
	-1   0    0    -1  
$EndComp
Text HLabel 5800 4550 0    60   Output ~ 0
PAN_MOTOR_CURRENT
Text Label 8200 3750 2    60   ~ 0
RAW_CURRENT_IN
Text Notes 8800 3550 0    60   ~ 0
PanMotorCurrent
$Comp
L CONN_3 K?
U 1 1 55534ACE
P 9250 3850
F 0 "K?" V 9200 3850 50  0000 C CNN
F 1 "CONN_3" V 9300 3850 40  0000 C CNN
	1    9250 3850
	1    0    0    -1  
$EndComp
Text HLabel 3200 2750 0    60   Output ~ 0
PAN_MOTOR_EMF
Text HLabel 5250 1050 0    60   Input ~ 0
3.3VO
$Comp
L R R?
U 1 1 55534487
P 4150 3150
F 0 "R?" V 4230 3150 50  0000 C CNN
F 1 "27.4K" V 4150 3150 50  0000 C CNN
	1    4150 3150
	0    1    1    0   
$EndComp
$Comp
L R R?
U 1 1 55534465
P 5350 1400
F 0 "R?" V 5430 1400 50  0000 C CNN
F 1 "27.4K" V 5350 1400 50  0000 C CNN
	1    5350 1400
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 55534462
P 5350 2000
F 0 "R?" V 5430 2000 50  0000 C CNN
F 1 "27.4K" V 5350 2000 50  0000 C CNN
	1    5350 2000
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 55534441
P 5050 2850
F 0 "R?" V 5130 2850 50  0000 C CNN
F 1 "100K" V 5050 2850 50  0000 C CNN
	1    5050 2850
	0    1    1    0   
$EndComp
$Comp
L R R?
U 1 1 5553443B
P 5050 2650
F 0 "R?" V 5130 2650 50  0000 C CNN
F 1 "100K" V 5050 2650 50  0000 C CNN
	1    5050 2650
	0    1    1    0   
$EndComp
$Comp
L LM358 U?
U 1 1 55534429
P 4000 2750
F 0 "U?" H 4000 2950 60  0000 L CNN
F 1 "LM6132" H 3950 2500 60  0000 L CNN
	1    4000 2750
	-1   0    0    -1  
$EndComp
$Comp
L CONN_4 P?
U 1 1 55533EBC
P 9250 6550
F 0 "P?" V 9200 6550 50  0000 C CNN
F 1 "CONN_4" V 9300 6550 50  0000 C CNN
	1    9250 6550
	1    0    0    -1  
$EndComp
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
Text Notes 9350 5000 2    60   ~ 0
PanMotor Pwr
$Comp
L CONN_2 P?
U 1 1 55533AE9
P 9250 5250
F 0 "P?" V 9200 5250 40  0000 C CNN
F 1 "CONN_2" V 9300 5250 40  0000 C CNN
	1    9250 5250
	1    0    0    -1  
$EndComp
$EndSCHEMATC
