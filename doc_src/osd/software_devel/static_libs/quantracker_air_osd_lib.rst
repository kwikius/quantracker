
-------
osd_lib.mk
-------

Makes  the system library for the OSD. As well as the OSD driver functions,
 also provides the code for the FreeRTOS system functions and stm32f4 system startup functions
There are three versions of the library which should not be used together.

They are 
   quantracker_air_osd.a
   quantracker_air_osd_tx.a
   quantracker_air_osd_rx.a

Breakdown of the objects

FreeRTOS
   port.o
   tasks.o 
   queue.o 
   list.o 
   timers.o
   heap3.o
   rtos_hooks.o

stm32 std periph
   misc.o 

stm32f4 system 
   system_init.o
   startup.o
   

--------
building using libs made wth osd_lib.mk
-------

Problem
Making a lib requires headers and defines
These should be unique for all projects built from the lib.
Suggest to put the lib header for the project in one loc
quantracker/include/<lib_name>/FreeRtosConfig.h
then use the same version to build the project

Headers used by the 
quantracker_air_osd.a

FreeRTOSConfig.h  - how FreeRTOS is set up for the library

change this so it separates osd_resources out ( see  th v2 Tracker board)
resources.hpp     - PCB pins and mpu peripherals used
includes processor/board_type4.hpp

// chneg these to tx rx and no telem versions?
video/video_buffer.hpp 
video/video_cfg.hpp

quantracker_air_osd_tx.a

quantracker_air_osd_rx.a
rx_telemetry.hpp