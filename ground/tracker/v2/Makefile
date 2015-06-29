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
	@echo '   #            Hi. Welcome to quantracker / ground / v2/ tracker. #'
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

OBJDIR = obj/
BINDIR = bin/

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

#ifeq ($(MAVLINK_INCLUDE_PATH), )
#$(error "MAVLINK_INCLUDE_PATH must be defined to the path to the MAVlink library - see README.")
#endif

ifeq ($(FREE_RTOS_DIR), )
$(error "FREE_RTOS_DIR must be defined to the path to the FreeRTOS library - see README.")
endif

ifeq ($(STM32_STD_PERIPH_LIB_DIR), )
$(error "STM32_STD_PERIPH_LIB_DIR must be defined to the path to the STM32 Std peripherals library - see README.")
endif
#
#ifeq ($(USER_SOURCE_PATH), )
#$(error "USER_SOURCE_PATH must be defined to the path to the path to your custom sources - see README.")
#endif

#ifeq ($(USER_SOURCE_FILES), )
#$(error "USER_SOURCE_FILES must be defined to the list of your custom source files - see README.")
#endif

USER_SOURCE_PATH = ../../../examples/osd_example1/

STM32_SRC_DIR = $(STM32_STD_PERIPH_LIB_DIR)STM32F4xx_StdPeriph_Driver/src/

STM32_INCLUDES = $(STM32_STD_PERIPH_LIB_DIR)CMSIS/Include \
$(STM32_STD_PERIPH_LIB_DIR)CMSIS/Device/ST/STM32F4xx/Include \
$(STM32_STD_PERIPH_LIB_DIR)STM32F4xx_StdPeriph_Driver/inc

RTOS_INCLUDES = \
$(FREE_RTOS_DIR)Source/include/ \
$(FREE_RTOS_DIR)Source/portable/GCC/ARM_CM4F \
$(APP_SRC_PATH)

STM32F4_SPECIFIC_FLASH_SRC = $(QUAN_INCLUDE_PATH)/quan_matters/src/stm32/f4/specific_flash.cpp

GENERIC_FLASH_SRC_PATH = $(QUAN_INCLUDE_PATH)/quan_matters/src/stm32/flash/

TARGET_PROCESSOR = STM32F4

video_objects = video_buffer.o video_column.o video_row.o video_pixel.o video_spi.o \
video_dma.o video_setup.o graphics_api.o graphics_api_extras.o receive_telemetry_task.o \
draw_task.o sync_sep.o led.o black_level.o dac.o

obj_video_objects = $(patsubst %,$(OBJDIR)%,$(video_objects))

local_objects = main.o spbrk.o setup.o rtos_hooks.o sliprings_sp_irq.o \
tracker_mode_task.o rx_telemetry.o pan_servo.o text_commands.o on_draw.o

obj_local_objects = $(patsubst %,$(OBJDIR)%,$(local_objects))

rtos_objects = tasks.o queue.o list.o timers.o 
obj_rtos_objects = $(patsubst %,$(OBJDIR)%,$(rtos_objects))

stm32_objects = misc.o
obj_stm32_objects = $(patsubst %,$(OBJDIR)%,$(stm32_objects))

GRAPHICS_API_PATH = $(QUAN_INCLUDE_PATH)/quan_matters/src/uav/osd/

graphics_api_objects = draw_arc.o draw_bitmap.o draw_circle.o \
draw_line.o draw_text.o draw_box.o flood_fill.o
obj_graphics_api_objects = $(patsubst %,$(OBJDIR)%,$(graphics_api_objects))

quan_generic_flash_objects = quan_generic_flash.o quan_generic_flash_error.o \
quan_generic_flash_menu.o
obj_quan_generic_flash_objects = $(patsubst %,$(OBJDIR)%,$(quan_generic_flash_objects))

#user_objects =  $(patsubst %.cpp,%.o,$(USER_SOURCE_FILES))

#user_resources = bitmaps.o fonts.o
user_resources = fonts.o
obj_user_resources = $(patsubst %,$(OBJDIR)%,$(user_resources))

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

DEFINES += QUAN_STM32F4 QUAN_FREERTOS QUAN_OSD_TELEM_RECEIVER STM32F40_41xxx \
QUAN_OSD_SOFTWARE_SYNCSEP

# DEFINES += QUAN_FLASH_DEBUG
STARTUP = startup.s
# custom linker script 
LINKER_SCRIPT = stm32f4.ld

SYSTEM_INIT = system_init.cpp
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

board_type4 : DEFINES += QUAN_OSD_BOARD_TYPE=4

DEFINE_ARGS = $(patsubst %,-D%,$(DEFINES))

CFLAGS  = -Wall -Wdouble-promotion -std=c++11 -fno-rtti -fno-exceptions -c -g \
-$(OPTIMISATION_LEVEL) $(DEFINE_ARGS) $(INCLUDE_ARGS) $(PROCESSOR_FLAGS) \
 $(CFLAG_EXTRAS) -fno-math-errno -Wl,-u,vsprintf -lm -fdata-sections -ffunction-sections

LFLAGS  = -T$(LINKER_SCRIPT) -$(OPTIMISATION_LEVEL) -nostartfiles -nodefaultlibs \
 $(PROCESSOR_FLAGS) $(INIT_LIBS) --specs=nano.specs $(CFLAG_EXTRAS) -Wl,--gc-sections

CPFLAGS = -Obinary
ODFLAGS = -d 

all: board_type4

board_type4 : test

objects = $(obj_video_objects) $(obj_local_objects) $(obj_rtos_objects) \
$(obj_stm32_objects) $(obj_graphics_api_objects) $(obj_user_objects) $(obj_user_resources) \
$(obj_quan_generic_flash_objects) $(OBJDIR)quan_stm32_f4_specific_flash.o \
$(OBJDIR)startup.o $(OBJDIR)system_init.o $(OBJDIR)port.o $(OBJDIR)heap_3.o

clean:
	-rm -rf $(OBJDIR)*.o $(BINDIR)*.elf $(BINDIR)*.bin $(BINDIR)*.lst

test: $(BINDIR)main.elf
	@ echo "...copying"
	$(CP) $(CPFLAGS) $(BINDIR)main.elf $(BINDIR)main.bin
	$(OD) $(ODFLAGS) $(BINDIR)main.elf > $(BINDIR)main.lst
	$(SIZ) -A $(BINDIR)main.elf

$(BINDIR)main.elf: $(objects) 
	@ echo "..linking"
	$(LD) $(LFLAGS) -o $(BINDIR)main.elf $(objects)

$(obj_video_objects): $(OBJDIR)%.o : ../../../air/osd/video/%.cpp
	$(CC) $(CFLAGS) $< -o $@

$(obj_local_objects): $(OBJDIR)%.o : %.cpp
	$(CC) $(CFLAGS) $< -o $@

$(obj_quan_generic_flash_objects) : $(OBJDIR)quan_generic_%.o : $(GENERIC_FLASH_SRC_PATH)%.cpp
	$(CC) $(CFLAGS) $< -o $@

$(OBJDIR)quan_stm32_f4_specific_flash.o : $(STM32F4_SPECIFIC_FLASH_SRC)
	$(CC) $(CFLAGS) $< -o $@

$(obj_graphics_api_objects) : $(OBJDIR)%.o : $(GRAPHICS_API_PATH)%.cpp
	$(CC) $(CFLAGS) $< -o $@

$(obj_user_objects) : $(OBJDIR)%.o : $(USER_SOURCE_PATH)common/%.cpp
	$(CC) $(CFLAGS) -DQUAN_OSD_TARGET_BOARD $< -o $@

$(obj_user_resources) : $(OBJDIR)%.o : $(USER_SOURCE_PATH)board/%.cpp
	$(CC) $(CFLAGS) -DQUAN_OSD_TARGET_BOARD $< -o $@

$(OBJDIR)system_init.o : $(SYSTEM_INIT)
	$(CC) $(CFLAGS) $< -o $@

$(OBJDIR)startup.o: $(STARTUP)
	$(CC) $(CFLAGS) $< -o $@ 

C_FLAGS_1  = -Wall -c -g -$(OPTIMISATION_LEVEL) $(DEFINE_ARGS) $(INCLUDE_ARGS) \
 $(PROCESSOR_FLAGS) $(CFLAG_EXTRAS) -fdata-sections -ffunction-sections

$(obj_stm32_objects) : $(OBJDIR)%.o : $(STM32_SRC_DIR)%.c
	$(CC1) $(C_FLAGS_1) -D'assert_param(args)= ' $(patsubst %,-I%,$(STM32_INCLUDES)) $< -o $@

$(obj_rtos_objects) : $(OBJDIR)%.o : $(FREE_RTOS_DIR)Source/%.c
	$(CC1) $(C_FLAGS_1) $(patsubst %,-I%,$(RTOS_INCLUDES)) $< -o $@

$(OBJDIR)port.o : $(FREE_RTOS_DIR)Source/portable/GCC/ARM_CM4F/port.c
	$(CC1) $(C_FLAGS_1) $(patsubst %,-I%,$(RTOS_INCLUDES)) $< -o $@

$(OBJDIR)heap_3.o : $(FREE_RTOS_DIR)Source/portable/MemMang/heap_3.c
	$(CC1) $(C_FLAGS_1) $(patsubst %,-I%,$(RTOS_INCLUDES)) $< -o $@

#-----------------------------------------
upload : test
	st-flash write $(BINDIR)main.bin 0x8000000

upload_sp : test
	$(STM32FLASH) -f -v -w $(BINDIR)main.bin /dev/ttyUSB0

#deps conditional
endif
