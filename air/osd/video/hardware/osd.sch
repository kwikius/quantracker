EESchema Schematic File Version 2  date Mon 27 Oct 2014 09:07:34 GMT
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
Sheet 1 4
Title ""
Date "27 oct 2014"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Sheet
S 1350 1100 2150 2600
U 544A4CB8
F0 "Sheet544A4CB7" 60
F1 "video_io_FMS6141_sync_clamp.sch" 60
F2 "AV_DATA_EN" I R 3500 1950 60 
F3 "AV_DATA" O R 3500 1850 60 
F4 "AVDacClk" I R 3500 2550 60 
F5 "AVDacData" I R 3500 2450 60 
F6 "AVDacNSync" I R 3500 2350 60 
F7 "NWhitePx" I R 3500 2150 60 
F8 "NBlackPx" I R 3500 2250 60 
F9 "+3.3V" B R 3500 1600 60 
F10 "+5V" B R 3500 1450 60 
F11 "VIDEO_IN" I R 3500 3000 60 
F12 "GND" B R 3500 3200 60 
F13 "VIDEO_OUT" O R 3500 2800 60 
F14 "CSYNC" O R 3500 2050 60 
F15 "video1" O R 3500 2700 60 
$EndSheet
Wire Wire Line
	5550 2800 5000 2800
Wire Wire Line
	5000 2800 5000 1850
Wire Wire Line
	5000 1850 3500 1850
Wire Wire Line
	5550 3000 4800 3000
Wire Wire Line
	4800 3000 4800 2050
Wire Wire Line
	4800 2050 3500 2050
Wire Wire Line
	5550 3400 4400 3400
Wire Wire Line
	4400 3400 4400 2450
Wire Wire Line
	4400 2450 3500 2450
Wire Wire Line
	5550 3300 4500 3300
Wire Wire Line
	5550 3100 4700 3100
Wire Wire Line
	4700 3100 4700 2150
Wire Wire Line
	4700 2150 3500 2150
Wire Wire Line
	5550 3200 4600 3200
Wire Wire Line
	4600 3200 4600 2250
Wire Wire Line
	4600 2250 3500 2250
Wire Wire Line
	3500 2350 4500 2350
Wire Wire Line
	4500 2350 4500 3300
Wire Wire Line
	3500 2550 4300 2550
Wire Wire Line
	4300 2550 4300 3500
Wire Wire Line
	4300 3500 5550 3500
Wire Wire Line
	3500 1950 4900 1950
Wire Wire Line
	4900 1950 4900 2900
Wire Wire Line
	4900 2900 5550 2900
$Sheet
S 5550 2450 1950 1950
U 5433EE23
F0 "MCU" 60
F1 "mcu.sch" 60
F2 "NBlackPx" O L 5550 3200 60 
F3 "NWhitePx" O L 5550 3100 60 
F4 "AVDacNSync" O L 5550 3300 60 
F5 "AVDacData" O L 5550 3400 60 
F6 "AVDacClk" O L 5550 3500 60 
F7 "AV_DATA" I L 5550 2800 60 
F8 "AV_DATA_EN" O L 5550 2900 60 
F9 "CSYNC" I L 5550 3000 60 
$EndSheet
$Sheet
S 1950 4150 1450 1250
U 5433EDDD
F0 "PSU" 60
F1 "psu.sch" 60
$EndSheet
$EndSCHEMATC
