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

# to build with telem receive
# invoke with make TELEMETRY_DIRECTION=QUAN_OSD_TELEM_RECEIVER -f osd_lib.mk
# to build with telem transmit
# invoke with make TELEMETRY_DIRECTION=QUAN_OSD_TELEM_TRANSMITTER -f osd_lib.mk
#

HAVE_DEPENDENCIES_FILE := $(shell if test -f "../../Dependencies.mk"; then echo "True"; fi)

ifneq ($(HAVE_DEPENDENCIES_FILE),True)
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

# source directory for the STM32 std peripheral library
STM32_SRC_DIR := $(STM32_STD_PERIPH_LIB_DIR)STM32F4xx_StdPeriph_Driver/src/

STM32_INCLUDES := $(STM32_STD_PERIPH_LIB_DIR)CMSIS/Include \
$(STM32_STD_PERIPH_LIB_DIR)CMSIS/Device/ST/STM32F4xx/Include \
$(STM32_STD_PERIPH_LIB_DIR)STM32F4xx_StdPeriph_Driver/inc

RTOS_INCLUDES := \
$(FREE_RTOS_DIR)Source/include/ \
$(FREE_RTOS_DIR)Source/portable/GCC/ARM_CM4F \
$(APP_SRC_PATH)

TARGET_PROCESSOR = STM32F4

ifeq ($(OPTIMISATION_LEVEL), )
OPTIMISATION_LEVEL := O3
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
AR      = $(TOOLCHAIN_PREFIX)bin/arm-none-eabi-ar

ifeq ($(TARGET_PROCESSOR), STM32F4)
# specific flags for stm32f4
DEFINES += QUAN_STM32F4 QUAN_FREERTOS STM32F40_41xxx \
 QUAN_OSD_SOFTWARE_SYNCSEP  HSE_VALUE=8000000 QUAN_OSD_BOARD_TYPE=4

ifeq ($(TELEMETRY_DIRECTION),QUAN_OSD_TELEM_TRANSMITTER)
OSD_ARCHIVE_FILE := ../../lib/osd/quantracker_air_osd_tx.a
HAS_TELEMETRY := True
TELEMETRY_PREFIX := lib_tx_
DEFINES += QUAN_OSD_TELEM_TRANSMITTER
else
ifeq ($(TELEMETRY_DIRECTION),QUAN_OSD_TELEM_RECEIVER)
OSD_ARCHIVE_FILE := ../../lib/osd/quantracker_air_osd_rx.a
HAS_TELEMETRY := True
TELEMETRY_PREFIX := lib_rx_
DEFINES += QUAN_OSD_TELEM_RECEIVER 
else
OSD_ARCHIVE_FILE := ../../lib/osd/quantracker_air_osd.a
HAS_TELEMETRY := False
TELEMETRY_PREFIX := lib_
endif
endif

# -------video objects --------------------------
unprefixed_video_objects = video_buffer.o video_column.o video_row.o \
video_pixel.o video_spi.o video_dma.o video_setup.o graphics_api.o \
draw_task.o sync_sep.o black_level.o dac.o led.o osd_state.o \
internal_video_mode.o

OBJDIR := obj/osd_lib/

SYSTEM_INIT := system_init.cpp
STARTUP := startup.s

PROCESSOR_FLAGS := -march=armv7e-m -mtune=cortex-m4 -mhard-float -mthumb \
-mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mthumb -mfloat-abi=hard

QUANTRACKER_INCLUDE_PATH := ../../include

INCLUDES := $(QUAN_INCLUDE_PATH) $(QUANTRACKER_INCLUDE_PATH) $(STM32_INCLUDES) $(RTOS_INCLUDES)

INIT_LIB_PREFIX := $(TOOLCHAIN_PREFIX)/lib/gcc/arm-none-eabi/$(TOOLCHAIN_GCC_VERSION)/armv7e-m/fpu/
else
$(error no target processor defined)
endif

INCLUDE_ARGS := $(patsubst %,-I%,$(INCLUDES))

DEFINE_ARGS := $(patsubst %,-D%,$(DEFINES))

CFLAGS  = -Wall -Wdouble-promotion -std=c++11 -fno-rtti -fno-exceptions -c -g \
-$(OPTIMISATION_LEVEL) $(DEFINE_ARGS) $(INCLUDE_ARGS) $(PROCESSOR_FLAGS) \
 $(CFLAG_EXTRAS) -fno-math-errno -Wl,-u,vsprintf -lm -fdata-sections -ffunction-sections

C_FLAGS_1  = -Wall -c -g -$(OPTIMISATION_LEVEL) $(DEFINE_ARGS) $(INCLUDE_ARGS) \
 $(PROCESSOR_FLAGS) $(CFLAG_EXTRAS) -fdata-sections -ffunction-sections

# add the telemetry tasks to the lib if required
ifeq ($(HAS_TELEMETRY),True)
unprefixed_video_objects += telemetry_task.o
endif

unobj_stm32_objects := misc.o
stm32_objects := $(patsubst %, $(OBJDIR)%,$(unobj_stm32_objects))

video_objects = $(patsubst %, $(OBJDIR)$(TELEMETRY_PREFIX)%,$(unprefixed_video_objects))

objects = $(video_objects) $(stm32_objects)
# ------------------------------------------------

all : $(OSD_ARCHIVE_FILE)
   
.PHONY: clean
clean:
	-rm -rf $(OSD_ARCHIVE_FILE) $(OBJDIR)*.o  

$(OSD_ARCHIVE_FILE) : $(objects)
	$(AR) rcs $@ $(objects)

$(video_objects): $(OBJDIR)$(TELEMETRY_PREFIX)%.o : video/%.cpp
	$(CC) $(CFLAGS) $< -o $@

$(stm32_objects) : $(OBJDIR)%.o : $(STM32_SRC_DIR)%.c
	$(CC1) $(C_FLAGS_1) -D'assert_param(args)= ' $(patsubst %,-I%,$(STM32_INCLUDES)) $< -o $@

#deps conditional
endif