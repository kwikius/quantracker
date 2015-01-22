# Copyright (c) 2013 -2015 Andy Little 
#
# With Grateful Acknowledgements to the prior work of:
#   Sami Korbonen(Openpilot.org)
#   taulabs ( taulabs.com) 
#   brainFPV ( brainfpv.com)
#   Thomas Oldbury (super-osd.com)
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>

#################### ATTENTION ################################
###############################################################
# N.B. You will need to  modify the paths in this section   ###
# to where you have placed these libraries on your system   ###
#                                                           ###
###  The gcc compiler toolchain                             ###
TOOLCHAIN_PREFIX =/opt/gcc-arm-none-eabi-4_7-2013q2/
TOOLCHAIN_GCC_VERSION = 4.7.4
###  the quan library                                       ###        
QUAN_INCLUDE_PATH = $(QUAN_ROOT)
###  FreeRTOS                                               ###
FREE_RTOS_DIR = /home/andy/cpp/lib/FreeRTOSV8.1.2/FreeRTOS/
###  The STM32F4 standard peripherals library               ###
STM32_SRC_DIR = /opt/stm32f4/STM32F4xx_DSP_StdPeriph_Lib_V1.0.0/Libraries/STM32F4xx_StdPeriph_Driver/src/
###############################################################

RTOS_INCLUDES = \
$(FREE_RTOS_DIR)Source/include/ \
$(FREE_RTOS_DIR)Source/portable/GCC/ARM_CM4F \
/home/andy/cpp/projects/quantracker/air/osd/

CMSIS_INCLUDES = /opt/stm32f4/STM32F4xx_DSP_StdPeriph_Lib_V1.0.0/Libraries/CMSIS/Include \
/opt/stm32f4/STM32F4xx_DSP_StdPeriph_Lib_V1.0.0/Libraries/CMSIS/Device/ST/STM32F4xx/Include \
/opt/stm32f4/STM32F4xx_DSP_StdPeriph_Lib_V1.0.0/Libraries/STM32F4xx_StdPeriph_Driver/inc

TARGET_PROCESSOR = STM32F4
OPTIMISATION_LEVEL = O3

video_objects =  video_buffer.o video_column.o video_row.o video_pixel.o video_spi.o \
 video_dma.o font.o video_setup.o on_draw.o graphics_api.o telemetry_task.o \
 draw_task.o sync_sep.o led.o black_level.o dac.o

local_objects = main.o spbrk.o setup.o serial_port.o fsk_output.o \
 fsk_task.o frsky_task.o frsky_zapp1.o led_task.o aircraft.o mavlink_task.o  settings.o \
 heartbeat_task.o rtos_hooks.o

rtos_objects = tasks.o queue.o list.o timers.o 
cmsis_objects = misc.o

#include $(QUAN_INCLUDE_PATH)/quan_matters/src/stm32/make_scripts/stm32.mk

# requires QUAN_INCLUDE_PATH - path to quan lib
# requires TARGET_PROCESSOR either STM32F4 or STM32F0
# OPTIMISATION_LEVEL defaults to Os
################################
## TODO add errors on other libs too 
###################################
ifeq ($(QUAN_INCLUDE_PATH), )
$(error "QUAN_INCLUDE_PATH must be defined to the path to the quan library")
endif

ifeq ($(OPTIMISATION_LEVEL), )
OPTIMISATION_LEVEL = O
endif

ifeq ( $(CFLAG_EXTRAS), )
CFLAG_EXTRAS = -fno-math-errno
endif

#required for Ubuntu 12.x placid as system headers have been put in strange places
# these have beeen defined to thos in my bash .profile
CPLUS_INCLUDE_PATH=
C_INCLUDE_PATH=
LIBRARY_PATH=

CC      = $(TOOLCHAIN_PREFIX)bin/arm-none-eabi-g++
CC1     = $(TOOLCHAIN_PREFIX)bin/arm-none-eabi-gcc
LD      = $(TOOLCHAIN_PREFIX)bin/arm-none-eabi-g++
CP      = $(TOOLCHAIN_PREFIX)bin/arm-none-eabi-objcopy
OD      = $(TOOLCHAIN_PREFIX)bin/arm-none-eabi-objdump
SIZ     = $(TOOLCHAIN_PREFIX)bin/arm-none-eabi-size
  
#MAKE_SCRIPTS_PATH = $(QUAN_INCLUDE_PATH)/quan_matters/src/stm32/make_scripts

ifeq ($(TARGET_PROCESSOR), STM32F4)
# specific flags for stm32f4
DEFINES = QUAN_STM32F4 QUAN_FREERTOS QUAN_OSD_TELEM_TRANSMITTER
# Define if using software sync sep rather than LM1881
DEFINES += QUAN_OSD_SOFTWARE_SYNCSEP
#DEFINES += QUAN_STM32F4 QUAN_OSD_TELEM_RECEIVER

STARTUP = startup.s
# custom linker script 
LINKER_SCRIPT = stm32f4.ld
#LINKER_SCRIPT = $(MAKE_SCRIPTS_PATH)/stm32f4.ld
FLASH_SRC = $(QUAN_INCLUDE_PATH)/quan_matters/src/stm32/f4/flash.cpp
SYSTICK_SRC = $(QUAN_INCLUDE_PATH)/quan_matters/src/stm32/systick.cpp

SYSTEM_INIT = system_init.cpp
PROCESSOR_FLAGS = -march=armv7e-m -mtune=cortex-m4 -mhard-float -mthumb \
-mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mthumb -mfloat-abi=hard

INCLUDES = /opt/stm32f4/STM32F4xx_DSP_StdPeriph_Lib_V1.0.0/Libraries/CMSIS/Include \
/opt/stm32f4/STM32F4xx_DSP_StdPeriph_Lib_V1.0.0/Libraries/CMSIS/Device/ST/STM32F4xx/Include \
/opt/stm32f4/STM32F4xx_DSP_StdPeriph_Lib_V1.0.0/Libraries/STM32F4xx_StdPeriph_Driver/inc \

INIT_LIB_PREFIX = $(TOOLCHAIN_PREFIX)/lib/gcc/arm-none-eabi/$(TOOLCHAIN_GCC_VERSION)/armv7e-m/fpu/
# todo stm32f0 version
#else
#ifeq ($(TARGET_PROCESSOR), STM32F0)
#include $(MAKE_SCRIPTS_PATH)/stm32f0.mk
else
$(error no target processor defined)
endif
#endif

INIT_LIBS = $(INIT_LIB_PREFIX)crti.o $(INIT_LIB_PREFIX)crtn.o

INCLUDES += $(QUAN_INCLUDE_PATH) $(RTOS_INCLUDES) /home/andy/website/fpv/mavlink

INCLUDE_ARGS = $(patsubst %,-I%,$(INCLUDES))

DEFINES += HSE_VALUE=8000000  

#board_type1 : DEFINES += QUAN_OSD_BOARD_TYPE=1 QUAN_DISCOVERY
#board_type2 : DEFINES += QUAN_OSD_BOARD_TYPE=2 QUAN_DISCOVERY
board_type3 : DEFINES += QUAN_OSD_BOARD_TYPE=3 QUAN_DISCOVERY
#V1 of the finished board
board_type4 : DEFINES += QUAN_OSD_BOARD_TYPE=4
board_type4_disco : DEFINES += QUAN_OSD_BOARD_TYPE=4 QUAN_DISCOVERY

DEFINE_ARGS = $(patsubst %,-D%,$(DEFINES))

# for float printf format etc
# CFLAG_EXTRAS += -Wl,-u,vsprintf -lm

CFLAGS  = -Wall -Wdouble-promotion -std=c++11 -fno-rtti -fno-exceptions -c -g -$(OPTIMISATION_LEVEL) $(DEFINE_ARGS) $(INCLUDE_ARGS) \
  $(PROCESSOR_FLAGS) $(CFLAG_EXTRAS) -fno-math-errno -Wl,-u,vsprintf -lm -fdata-sections -ffunction-sections

LFLAGS  = -T$(LINKER_SCRIPT) -$(OPTIMISATION_LEVEL) -nostartfiles -nodefaultlibs \
 $(PROCESSOR_FLAGS) $(INIT_LIBS) --specs=nano.specs $(CFLAG_EXTRAS) -Wl,--gc-sections

CPFLAGS = -Obinary
ODFLAGS = -d 

#all: test
#board_type1 : test
#board_type2 : test
board_type3 : test
board_type4 : test
board_type4_disco : test

objects = $(video_objects) $(local_objects) $(rtos_objects) \
$(cmsis_objects) startup.o system_init.o flash_funs.o port.o heap_3.o

clean:
	-rm -rf *.o *.elf *.bin *.lst

test: main.elf
	@ echo "...copying"
	$(CP) $(CPFLAGS) main.elf main.bin
	$(OD) $(ODFLAGS) main.elf > main.lst
	$(SIZ) -A main.elf

main.elf: $(objects) 
	@ echo "..linking"
	$(LD) $(LFLAGS) -o main.elf $(objects)

$(video_objects): %.o : video/%.cpp
	$(CC) $(CFLAGS) $< -o $@

$(local_objects): %.o : %.cpp
	$(CC) $(CFLAGS) $< -o $@

system_init.o : $(SYSTEM_INIT)
	$(CC) $(CFLAGS) -o system_init.o $(SYSTEM_INIT)

startup.o: $(STARTUP)
	$(CC) $(CFLAGS) -o startup.o $(STARTUP) 

flash_funs.o : $(FLASH_SRC)
	$(CC) $(CFLAGS) -o flash_funs.o $(FLASH_SRC)

C_FLAGS_1  = -Wall -c -g -$(OPTIMISATION_LEVEL) $(DEFINE_ARGS) $(INCLUDE_ARGS) \
 $(PROCESSOR_FLAGS) $(CFLAG_EXTRAS) -fdata-sections -ffunction-sections

$(cmsis_objects) : %.o : $(STM32_SRC_DIR)%.c
	$(CC1) $(C_FLAGS_1) -D'assert_param(args)= ' $(patsubst %,-I%,$(CMSIS_INCLUDES)) $< -o $@

$(rtos_objects) : %.o : $(FREE_RTOS_DIR)Source/%.c
	$(CC1) $(C_FLAGS_1) $(patsubst %,-I%,$(RTOS_INCLUDES)) $< -o $@

port.o : $(FREE_RTOS_DIR)Source/portable/GCC/ARM_CM4F/port.c
	$(CC1) $(C_FLAGS_1) $(patsubst %,-I%,$(RTOS_INCLUDES)) $< -o $@

heap_3.o : $(FREE_RTOS_DIR)Source/portable/MemMang/heap_3.c
	$(CC1) $(C_FLAGS_1) $(patsubst %,-I%,$(RTOS_INCLUDES)) $< -o $@

upload : test
	st-flash write main.bin 0x8000000
