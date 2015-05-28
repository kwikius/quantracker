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
	@echo '   #            in the main quantracker directory                  #'
	@echo '   #            for further Details.                               #'
	@echo '   #                                                               #'
	@echo '   #################################################################'
	@echo ' '	
else

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


ifeq ($(STM32_STD_PERIPH_LIB_DIR), )
$(error "STM32_STD_PERIPH_LIB_DIR must be defined to the path to the STM32 Std peripherals library - see README.")
endif

CC      = $(TOOLCHAIN_PREFIX)bin/arm-none-eabi-g++
AR      = $(TOOLCHAIN_PREFIX)bin/arm-none-eabi-ar

STM32_SRC_DIR = $(STM32_STD_PERIPH_LIB_DIR)STM32F4xx_StdPeriph_Driver/src/

STM32_INCLUDES = $(STM32_STD_PERIPH_LIB_DIR)CMSIS/Include \
$(STM32_STD_PERIPH_LIB_DIR)CMSIS/Device/ST/STM32F4xx/Include \
$(STM32_STD_PERIPH_LIB_DIR)STM32F4xx_StdPeriph_Driver/inc

INCLUDES = $(STM32_INCLUDES) $(QUAN_INCLUDE_PATH)

INCLUDE_ARGS = $(patsubst %,-I%,$(INCLUDES))

TARGET_PROCESSOR = STM32F4

GRAPHICS_API_PATH = $(QUAN_INCLUDE_PATH)/quan_matters/src/uav/osd/

OUTPUT_ARCHIVE_FILE = ../../lib/osd/quantracker_air_graphics_api.a

ifeq ( $(CFLAG_EXTRAS), )
CFLAG_EXTRAS = -fno-math-errno
endif

DEFINES += QUAN_STM32F4 QUAN_FREERTOS  STM32F40_41xxx
# Define if using software sync sep rather than LM1881
DEFINES += QUAN_OSD_SOFTWARE_SYNCSEP

DEFINE_ARGS = $(patsubst %,-D%,$(DEFINES))

CFLAGS  = -Wall -Wdouble-promotion -std=c++11 -fno-rtti -fno-exceptions -c -g \
-$(OPTIMISATION_LEVEL) $(DEFINE_ARGS) $(INCLUDE_ARGS) $(PROCESSOR_FLAGS) \
 $(CFLAG_EXTRAS) -fno-math-errno -Wl,-u,vsprintf -lm -fdata-sections -ffunction-sections

PROCESSOR_FLAGS = -march=armv7e-m -mtune=cortex-m4 -mhard-float -mthumb \
-mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mthumb -mfloat-abi=hard

objects = draw_arc.o draw_bitmap.o draw_circle.o draw_line.o draw_text.o flood_fill.o \
draw_box.o

all: $(OUTPUT_ARCHIVE_FILE)

$(OUTPUT_ARCHIVE_FILE) : $(objects)
	$(AR) rcs $@ $(objects)

$(objects) : %.o : $(GRAPHICS_API_PATH)%.cpp
	$(CC) $(CFLAGS) $< -o $@

clean:
	-rm -rf *.o $(OUTPUT_ARCHIVE_FILE)

endif
