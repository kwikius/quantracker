EESchema Schematic File Version 2  date Fri 31 Jul 2015 16:41:42 BST
LIBS:power
LIBS:device
LIBS:74xx
LIBS:stm32f405rgt6
LIBS:qspi
EELAYER 43  0
EELAYER END
$Descr A4 11700 8267
encoding utf-8
Sheet 1 1
Title ""
Date "31 jul 2015"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L QSPI U?
U 1 1 55BB9507
P 9150 5400
F 0 "U?" H 9150 5400 60  0000 C CNN
F 1 "QSPI" H 9150 5400 60  0000 C CNN
	1    9150 5400
	1    0    0    -1  
$EndComp
$Comp
L QSPI U?
U 1 1 55BB9501
P 9050 2100
F 0 "U?" H 9050 2100 60  0000 C CNN
F 1 "QSPI" H 9050 2100 60  0000 C CNN
	1    9050 2100
	1    0    0    -1  
$EndComp
Text Notes 3900 3700 0    60   ~ 0
QSPI_IO3
Text Notes 4300 3850 2    60   ~ 0
QSP_IO2
Text Notes 4300 4000 2    60   ~ 0
QSPI_IO1
Text Notes 3900 4150 0    60   ~ 0
QSPI_IO0
Text Notes 4250 3400 2    60   ~ 0
QSPI_nCS
Text Notes 4250 3550 2    60   ~ 0
QSPI_CLK
$Comp
L STM32F405RGT6 U?
U 1 1 55BB869C
P 3100 3750
F 0 "U?" H 3100 3500 60  0000 C CNN
F 1 "STM32F405RGT6" H 3100 4000 60  0000 C CNN
	1    3100 3750
	1    0    0    -1  
$EndComp
$EndSCHEMATC
