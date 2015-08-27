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


HAVE_DEPENDENCIES_FILE := $(shell if test -f "../../Dependencies.mk"; then echo "True"; fi)

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
	@echo '   #            in this directory for further Details.             #                                                          #'
	@echo '   #                                                               #'
	@echo '   #################################################################'
	@echo ' '	
else
# need the sourcedir for freertos compile
APP_SRC_PATH := $(patsubst %/,%,$(dir $(abspath $(lastword $(MAKEFILE_LIST)))))

DEFINES = 

# You will need a custom Dependencies.mk
include ../../Dependencies.mk

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

ifeq ($(FREE_RTOS_DIR), )
$(error "FREE_RTOS_DIR must be defined to the path to the FreeRTOS library - see README.")
endif

ifeq ($(STM32_STD_PERIPH_LIB_DIR), )
$(error "STM32_STD_PERIPH_LIB_DIR must be defined to the path to the STM32 Std peripherals library - see README.")
endif

STM32_INCLUDES = $(STM32_STD_PERIPH_LIB_DIR)CMSIS/Include \
$(STM32_STD_PERIPH_LIB_DIR)CMSIS/Device/ST/STM32F4xx/Include \
$(STM32_STD_PERIPH_LIB_DIR)STM32F4xx_StdPeriph_Driver/inc

RTOS_INCLUDES = \
$(FREE_RTOS_DIR)Source/include/ \
$(FREE_RTOS_DIR)Source/portable/GCC/ARM_CM4F \
$(APP_SRC_PATH)

TARGET_PROCESSOR = STM32F4

# what to do about led? Add to system lib or osd_lib?
#unprefixed_video_objects = led.o internal_video_mode.o

video_objects = $(patsubst %, $(TELEMETRY_PREFIX)%,$(unprefixed_video_objects))

INIT_LIB_PREFIX = $(TOOLCHAIN_PREFIX)/lib/gcc/arm-none-eabi/$(TOOLCHAIN_GCC_VERSION)/armv7e-m/fpu/
INIT_LIBS = $(INIT_LIB_PREFIX)crti.o $(INIT_LIB_PREFIX)crtn.o 

STATIC_LIBRARY_PATH = ../../lib/osd/

static_library_files = \
   quantracker_air_graphics_api.a \
   quantracker_air_osd.a \
   flash_variables.a 


local_objects = main.o setup.o on_draw.o 

static_library_files += quantracker_air_system.a 

static_libraries = $(patsubst %,$(STATIC_LIBRARY_PATH)%,$(static_library_files))

user_resources = bitmaps.o fonts.o

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
DEFINES += QUAN_STM32F4 QUAN_FREERTOS $(TELEMETRY_DIRECTION) STM32F40_41xxx
# Define if using software sync sep rather than LM1881
DEFINES += QUAN_OSD_SOFTWARE_SYNCSEP
STARTUP = startup.s
# custom linker script 
LINKER_SCRIPT = stm32f4.ld

# just for fonts
USER_SOURCE_PATH = ../../examples/osd_example1/

SYSTEM_INIT = system_init.cpp
PROCESSOR_FLAGS = -march=armv7e-m -mtune=cortex-m4 -mhard-float -mthumb \
-mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mthumb -mfloat-abi=hard

INCLUDES = $(STM32_INCLUDES)
else
$(error no target processor defined)
endif

QUANTRACKER_INCLUDE_PATH = ../../include

INCLUDES += $(QUAN_INCLUDE_PATH) $(QUANTRACKER_INCLUDE_PATH) \
 $(RTOS_INCLUDES) $(MAVLINK_INCLUDE_PATH)

INCLUDE_ARGS = $(patsubst %,-I%,$(INCLUDES))

DEFINES += HSE_VALUE=8000000 QUAN_OSD_BOARD_TYPE=4

DEFINE_ARGS = $(patsubst %,-D%,$(DEFINES))

CFLAGS  = -Wall -Wdouble-promotion -std=gnu++11 -fno-rtti -fno-exceptions -c -g \
-$(OPTIMISATION_LEVEL) $(DEFINE_ARGS) $(INCLUDE_ARGS) $(PROCESSOR_FLAGS) \
 $(CFLAG_EXTRAS) -fno-math-errno -Wl,-u,vsprintf -lm -fdata-sections -ffunction-sections

LFLAGS  = -T$(LINKER_SCRIPT) -$(OPTIMISATION_LEVEL) -nostartfiles -nodefaultlibs \
 $(PROCESSOR_FLAGS) --specs=nano.specs $(CFLAG_EXTRAS) -Wl,--gc-sections

CPFLAGS = -Obinary
ODFLAGS = -d 

objects =  $(local_objects) $(user_objects) $(user_resources) $(video_objects)

all:  test

clean:
	-rm -rf *.o *.elf *.bin *.lst

test: main.elf
	@ echo "...copying"
	$(CP) $(CPFLAGS) main.elf main.bin
	$(OD) $(ODFLAGS) main.elf > main.lst
	$(SIZ) -A main.elf

main.elf: $(objects) 
	@ echo "..linking"
	$(LD) $(LFLAGS) -o main.elf  $(INIT_LIBS) $(objects) -Wl,--undefined=_sbrk $(static_libraries)

$(video_objects): $(TELEMETRY_PREFIX)%.o : video/%.cpp
	$(CC) $(CFLAGS) $< -o $@

$(local_objects): %.o : %.cpp
	$(CC) $(CFLAGS) $< -o $@

$(user_resources) : %.o : $(USER_SOURCE_PATH)board/%.cpp
	$(CC) $(CFLAGS) -DQUAN_OSD_TARGET_BOARD $< -o $@

#-----------------------------------------
upload : test
	st-flash write main.bin 0x8000000

upload_sp : test
	$(STM32FLASH) -b 115200 -f -v -w main.bin /dev/ttyUSB0

#deps conditional
endif
