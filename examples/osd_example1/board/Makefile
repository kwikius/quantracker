# Copyright (c) 2013 -2015 Andy Little 
#
# With Grateful Acknowledgements to the prior work of:
#   Sami Korhonen(Openpilot.org)
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

HAVE_DEPENDENCIES_FILE := $(shell if test -f "../../../Dependencies.mk"; then echo "True"; fi)

ifeq ($(HAVE_DEPENDENCIES_FILE), )
  quantracker-make-help:
	@echo ' '
	@echo '   ########## HELP - OSD firmware build needs more info ############'
	@echo '   #                                                               #'
	@echo '   #            Hi. Welcome to quantracker / air / OSD.            #'
	@echo '   #                                                               #'
	@echo '   #            To build the OSD firmware, you need to             #'
	@echo '   #            create a Dependencies.mk file.                     #'
	@echo '   #                                                               #'
	@echo '   #            Please read "Sample-Dependencies.mk" .             #'
	@echo '   #            in the main quantracker directory                  #'
	@echo '   #            for further Details.                               #'
	@echo '   #                                                               #'
	@echo '   #################################################################'
	@echo ' '	
else
# need the sourcedir for freertos compile
APP_SRC_PATH := $(patsubst %/,%,$(dir $(abspath $(lastword $(MAKEFILE_LIST)))))

DEFINES = 

# You will need a custom Dependencies.mk
include ../../../Dependencies.mk

###############################################################
ifeq ($(TOOLCHAIN_PREFIX), )
$(error "TOOLCHAIN_PREFIX must be defined to the path to the gcc-arm compiler - see README.")
endif

ifeq ($(TOOLCHAIN_GCC_VERSION), )
$(error "TOOLCHAIN_GCC_VERSION must be defined to the gcc-arm compiler version - see README.")
endif

ifeq ($(QUAN_INCLUDE_PATH), )
$(error "QUAN_INCLUDE_PATH must be defined to the path to the quan library - see README.")
endif

ifeq ($(MAVLINK_INCLUDE_PATH), )
$(error "MAVLINK_INCLUDE_PATH must be defined to the path to the MAVlink library - see README.")
endif

ifeq ($(FREE_RTOS_DIR), )
$(error "FREE_RTOS_DIR must be defined to the path to the FreeRTOS library - see README.")
endif

ifeq ($(STM32_STD_PERIPH_LIB_DIR), )
$(error "STM32_STD_PERIPH_LIB_DIR must be defined to the path to the STM32 Std peripherals library - see README.")
endif

STM32_SRC_DIR = $(STM32_STD_PERIPH_LIB_DIR)STM32F4xx_StdPeriph_Driver/src/

STM32_INCLUDES = $(STM32_STD_PERIPH_LIB_DIR)CMSIS/Include \
$(STM32_STD_PERIPH_LIB_DIR)CMSIS/Device/ST/STM32F4xx/Include \
$(STM32_STD_PERIPH_LIB_DIR)STM32F4xx_StdPeriph_Driver/inc

RTOS_INCLUDES = \
$(FREE_RTOS_DIR)Source/include/ \
$(FREE_RTOS_DIR)Source/portable/GCC/ARM_CM4F \
$(abspath ../../../air/osd)

STM32F4_SPECIFIC_FLASH_SRC = $(QUAN_INCLUDE_PATH)/quan_matters/src/stm32/f4/specific_flash.cpp

GENERIC_FLASH_SRC_PATH = $(QUAN_INCLUDE_PATH)/quan_matters/src/stm32/flash/

TARGET_PROCESSOR = STM32F4

# in the board directory
local_objects = main.o spbrk.o setup.o serial_port.o fsk_output.o \
fsk_task.o frsky_task.o frsky_zapp1.o led_task.o mavlink_task.o  settings.o \
heartbeat_task.o  mode_check.o board_graphics_api.o led.o

# in the common directory
USER_SOURCE_FILES = on_draw.cpp draw_altitude.cpp draw_artificial_horizon.cpp \
draw_compass.cpp draw_home.cpp draw_gps_state.cpp draw_batteries.cpp symbology.cpp \
flash.cpp aircraft.cpp

#......... Put in separate graphics lib...............
GRAPHICS_API_PATH = $(QUAN_INCLUDE_PATH)/quan_matters/src/uav/osd/
graphics_api_objects = draw_arc.o draw_bitmap.o draw_circle.o \
draw_line.o draw_text.o flood_fill.o

#...............Put in separate flash lib ...................................
quan_generic_flash_objects = quan_generic_flash.o quan_generic_flash_error.o \
quan_generic_flash_menu.o

user_objects =  $(patsubst %.cpp,%.o,$(USER_SOURCE_FILES))
user_resources = bitmaps.o fonts.o

ifeq ($(OPTIMISATION_LEVEL), )
OPTIMISATION_LEVEL = O
endif

ifeq ( $(CFLAG_EXTRAS), )
CFLAG_EXTRAS = -fno-math-errno
endif

ifeq ( $(TELEMETRY_DIRECTION), )
TELEMETRY_DIRECTION = QUAN_OSD_TELEM_TRANSMITTER
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
  
ifeq ($(TARGET_PROCESSOR), STM32F4)
# specific flags for stm32f4
#DEFINES += QUAN_STM32F4 QUAN_FREERTOS $(TELEMETRY_DIRECTION) STM32F40_41xxx
DEFINES += QUAN_STM32F4 QUAN_FREERTOS $(TELEMETRY_DIRECTION) STM32F40_41xxx
# Define if using software sync sep rather than LM1881
DEFINES += QUAN_OSD_SOFTWARE_SYNCSEP
#DEFINES += QUAN_STM32F4 QUAN_OSD_TELEM_RECEIVER
#DEFINES += QUAN_FLASH_DEBUG

# custom linker script 
LINKER_SCRIPT = ../../../air/osd/stm32f4.ld

PROCESSOR_FLAGS = -march=armv7e-m -mtune=cortex-m4 -mhard-float -mthumb \
-mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mthumb -mfloat-abi=hard

INCLUDES = $(STM32_INCLUDES)

INIT_LIB_PREFIX = $(TOOLCHAIN_PREFIX)/lib/gcc/arm-none-eabi/$(TOOLCHAIN_GCC_VERSION)/armv7e-m/fpu/
else
$(error no target processor defined)
endif
#endif

INIT_LIBS = $(INIT_LIB_PREFIX)crti.o $(INIT_LIB_PREFIX)crtn.o

INCLUDES += $(QUAN_INCLUDE_PATH) $(RTOS_INCLUDES) $(MAVLINK_INCLUDE_PATH)

INCLUDE_ARGS = $(patsubst %,-I%,$(INCLUDES))

# QUAN_DISPLAY_INTERLACED 
DEFINES += HSE_VALUE=8000000  $(QUAN_TELEMETRY_DIRECTION)

#board_type1 : DEFINES += QUAN_OSD_BOARD_TYPE=1 QUAN_DISCOVERY
#board_type2 : DEFINES += QUAN_OSD_BOARD_TYPE=2 QUAN_DISCOVERY
board_type3 : DEFINES += QUAN_OSD_BOARD_TYPE=3 QUAN_DISCOVERY
#V1 of the finished board
board_type4 : DEFINES += QUAN_OSD_BOARD_TYPE=4
board_type4_disco : DEFINES += QUAN_OSD_BOARD_TYPE=4 QUAN_DISCOVERY

DEFINE_ARGS = $(patsubst %,-D%,$(DEFINES))

CFLAGS  = -Wall -Wdouble-promotion -std=c++11 -fno-rtti -fno-exceptions -c -g \
-$(OPTIMISATION_LEVEL) $(DEFINE_ARGS) $(INCLUDE_ARGS) $(PROCESSOR_FLAGS) \
 $(CFLAG_EXTRAS) -fno-math-errno -Wl,-u,vsprintf -lm -fdata-sections -ffunction-sections

LFLAGS  = -T$(LINKER_SCRIPT) -$(OPTIMISATION_LEVEL) -nostartfiles -nodefaultlibs \
 $(PROCESSOR_FLAGS) $(INIT_LIBS) --specs=nano.specs $(CFLAG_EXTRAS) -Wl,--gc-sections

CPFLAGS = -Obinary
ODFLAGS = -d 

all: board_type4
#board_type1 : test
#board_type2 : test
board_type3 : test
board_type4 : test
board_type4_disco : test

objects = $(local_objects)  $(graphics_api_objects) $(user_objects) $(user_resources) \
$(quan_generic_flash_objects) quan_stm32_f4_specific_flash.o 

clean:
	-rm -rf *.o *.elf *.bin *.lst

test: main.elf
	@ echo "...copying"
	$(CP) $(CPFLAGS) main.elf main.bin
	$(OD) $(ODFLAGS) main.elf > main.lst
	$(SIZ) -A main.elf

main.elf: $(objects) 
	@ echo "..linking"
	$(LD) $(LFLAGS) -o main.elf $(objects) ../../../air/osd/quantracker_air_osd.a

$(local_objects): %.o : %.cpp
	$(CC) $(CFLAGS) $< -o $@

$(quan_generic_flash_objects) : quan_generic_%.o : $(GENERIC_FLASH_SRC_PATH)%.cpp
	$(CC) $(CFLAGS) $< -o $@

quan_stm32_f4_specific_flash.o : $(STM32F4_SPECIFIC_FLASH_SRC)
	$(CC) $(CFLAGS) -o quan_stm32_f4_specific_flash.o $(STM32F4_SPECIFIC_FLASH_SRC)

$(graphics_api_objects) : %.o : $(GRAPHICS_API_PATH)%.cpp
	$(CC) $(CFLAGS) $< -o $@

$(user_objects) : %.o : ../common/%.cpp
	$(CC) $(CFLAGS) -DQUAN_OSD_TARGET_BOARD $< -o $@

$(user_resources) : %.o : %.cpp
	$(CC) $(CFLAGS) -DQUAN_OSD_TARGET_BOARD $< -o $@

#-----------------------------------------
upload : test
	st-flash write main.bin 0x8000000

upload_sp : test
	stm32flash -f -v -w main.bin /dev/ttyUSB0

#deps conditional
endif