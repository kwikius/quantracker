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

QUANTRACKER_ROOT_DIR = ../../

TELEMETRY_DIRECTION = QUAN_OSD_TELEM_NONE

include $(QUANTRACKER_ROOT_DIR)include/quantracker/build/osd.mk

OUTPUT_ARCHIVE_FILE := ../../lib/osd/flash_variables.a

OBJDIR := obj/flash_variables/
AR = $(TOOLCHAIN_PREFIX)bin/arm-none-eabi-ar

STM32F4_SPECIFIC_FLASH_SRC := $(QUAN_INCLUDE_PATH)/quan_matters/src/stm32/f4/specific_flash.cpp

GENERIC_FLASH_SRC_PATH := $(QUAN_INCLUDE_PATH)/quan_matters/src/stm32/flash/

all: $(OUTPUT_ARCHIVE_FILE)

un_obj_quan_generic_flash_objects = quan_generic_flash.o quan_generic_flash_error.o \
quan_generic_flash_menu.o

quan_generic_flash_objects =  $(patsubst %, $(OBJDIR)%,$(un_obj_quan_generic_flash_objects))

objects = $(quan_generic_flash_objects) $(OBJDIR)quan_stm32_f4_specific_flash.o

$(quan_generic_flash_objects) : $(OBJDIR)quan_generic_%.o : $(GENERIC_FLASH_SRC_PATH)%.cpp
	$(CC) $(CFLAGS) $< -o $@

$(OBJDIR)quan_stm32_f4_specific_flash.o : $(STM32F4_SPECIFIC_FLASH_SRC)
	$(CC) $(CFLAGS) $< -o $@

clean:
	-rm -rf $(OBJDIR)*.o $(OUTPUT_ARCHIVE_FILE)

$(OUTPUT_ARCHIVE_FILE) : $(objects)
	$(AR) rcs $@ $(objects)
