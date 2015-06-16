----------------------
OSD firmware downloads
----------------------

Since the board is a Dev board we are currently providing  some example firmwraes that are designed to be customised.
As the OSD develops we hope to  develop some classic themes. Below are the firmwares that we have currently. 

........
Example1 
........

Download from Github `firmware_example1.bin`_

This OSD firmware is designed as a minimal but useful display for use with fixed wing aircraft.

The display widgets consist of
 - a compass with integral home arrow, 
 - an artificial horizon 
 - battery voltage
 
 When the OSD display starts up and while it is waiting for a GPS lock, it displays the number of satellites and the gps status.
 
 Once a valid satellite fix is obtained, the OSD assumes it is at the home position and the Display stops showing satelliete status 
 and shows the altitude and distance from home. 



.. _`firmware_example1.bin`: https://github.com/kwikius/quantracker/blob/master/examples/osd_example1/board/main.bin



