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
#ifeq (1,)
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
# You will need a custom Dependencies.mk
include ../../../Dependencies.mk
# need the sourcedir for freertos compile
APP_SRC_PATH := $(patsubst %/,%,$(dir $(abspath $(lastword $(MAKEFILE_LIST)))))

# the subdir for object files
OBJDIR = obj/
# the sub-directory for 
BINDIR = bin/

DEFINES = 

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

TARGET_PROCESSOR = STM32F4

# static libaries 
STATIC_LIBRARY_PATH = ../../../lib/osd/

static_library_files = \
   quantracker_air_system.a \
   quantracker_air_osd.a \
   quantracker_air_graphics_api.a \
   flash_variables.a

static_libraries = $(patsubst %,$(STATIC_LIBRARY_PATH)%,$(static_library_files))

# in the board directory
board_objects = main.o setup.o serial_port.o fsk_output.o \
fsk_task.o frsky_task.o frsky_zapp1.o mavlink_task.o settings.o \
heartbeat_task.o mode_check.o board_graphics_api.o led.o bitmaps.o fonts.o

obj_board_objects = $(patsubst %,$(OBJDIR)%,$(board_objects))

# in the common directory
common_objects = on_draw.o draw_altitude.o draw_artificial_horizon.o \
draw_compass.o draw_home.o draw_gps_state.o draw_batteries.o \
symbology.o flash.o aircraft.o draw_lat_lon.o draw_mode.o \
draw_speed.o osd_control.o graphics_api_extras.o

obj_common_objects = $(patsubst %,$(OBJDIR)%,$(common_objects))

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
  
ifeq ($(TARGET_PROCESSOR), STM32F4)
# specific flags for stm32f4
DEFINES += QUAN_STM32F4 QUAN_FREERTOS QUAN_OSD_TELEM_TRANSMITTER STM32F40_41xxx \
HSE_VALUE=8000000 QUAN_OSD_BOARD_TYPE=4 QUAN_OSD_SOFTWARE_SYNCSEP

INIT_LIB_PREFIX = $(TOOLCHAIN_PREFIX)/lib/gcc/arm-none-eabi/$(TOOLCHAIN_GCC_VERSION)/armv7e-m/fpu/
INIT_LIBS = $(INIT_LIB_PREFIX)crti.o $(INIT_LIB_PREFIX)crtn.o -Wl,--undefined=_sbrk

PROCESSOR_FLAGS = -march=armv7e-m -mtune=cortex-m4 -mhard-float -mthumb \
-mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mthumb -mfloat-abi=hard

INCLUDES = $(STM32_INCLUDES) $(QUAN_INCLUDE_PATH) $(RTOS_INCLUDES) $(MAVLINK_INCLUDE_PATH)

# custom linker script 
LINKER_SCRIPT = ../../../air/osd/stm32f4.ld

else
$(error no target processor defined)
endif

INCLUDE_ARGS = $(patsubst %,-I%,$(INCLUDES))

DEFINE_ARGS = $(patsubst %,-D%,$(DEFINES))

CFLAGS  = -Wall -Wdouble-promotion -Wno-unused-local-typedefs -std=c++11 -fno-rtti -fno-exceptions -c -g \
-$(OPTIMISATION_LEVEL) $(DEFINE_ARGS) $(INCLUDE_ARGS) $(PROCESSOR_FLAGS) \
 $(CFLAG_EXTRAS) -fno-math-errno -Wl,-u,vsprintf -lm -fdata-sections -ffunction-sections

LFLAGS  = -T$(LINKER_SCRIPT) -$(OPTIMISATION_LEVEL) -nostartfiles -nodefaultlibs \
 $(PROCESSOR_FLAGS) $(INIT_LIBS) --specs=nano.specs $(CFLAG_EXTRAS) -Wl,--gc-sections 

CPFLAGS = -Obinary
ODFLAGS = -d 

all: test

objects = $(obj_board_objects) $(obj_common_objects) 

clean:
	-rm -rf $(OBJDIR)*.o $(BINDIR)*.elf $(BINDIR)*.bin $(BINDIR)*.lst

test: $(BINDIR)main.elf
	@ echo "...copying"
	$(CP) $(CPFLAGS) $(BINDIR)main.elf $(BINDIR)main.bin
	$(OD) $(ODFLAGS) $(BINDIR)main.elf > $(BINDIR)main.lst
	$(SIZ) -A $(BINDIR)main.elf

$(BINDIR)main.elf: $(objects) $(static_libraries) 
	@ echo "..linking"
	$(LD) $(LFLAGS) -o $(BINDIR)main.elf $(objects) $(static_libraries) 

$(obj_board_objects): $(OBJDIR)%.o : %.cpp
	$(CC) $(CFLAGS) $< -o $@

$(obj_common_objects) : $(OBJDIR)%.o : ../common/%.cpp
	$(CC) $(CFLAGS) -DQUAN_OSD_TARGET_BOARD $< -o $@

$(static_libraries) :
	$(MAKE) -C ../../../air/osd -f all_osd_libs.mk

#-----------------------------------------
upload : test
	st-flash write main.bin 0x8000000

upload_sp : test
	$(STM32FLASH) -f -v -w $(BINDIR)main.bin /dev/ttyUSB0

#deps conditional
endif
