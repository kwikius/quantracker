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

QUANTRACKER_ROOT_DIR := ../../

include $(QUANTRACKER_ROOT_DIR)include/quantracker/build/osd.mk

STM32_SRC_DIR = $(STM32_STD_PERIPH_LIB_DIR)STM32F4xx_StdPeriph_Driver/src/

GRAPHICS_API_PATH := $(QUAN_INCLUDE_PATH)/quan_matters/src/uav/osd/

OUTPUT_ARCHIVE_FILE := ../../lib/osd/quantracker_air_graphics_api.a

OBJDIR := obj/graphics_api/

un_obj_objects = draw_arc.o draw_bitmap.o draw_circle.o draw_line.o draw_text.o flood_fill.o \
draw_box.o draw_horizontal_line.o

objects  := $(patsubst %, $(OBJDIR)%,$(un_obj_objects))

.PHONY: all clean

all: $(OUTPUT_ARCHIVE_FILE)

$(OUTPUT_ARCHIVE_FILE) : $(objects)
	$(AR) rcs $@ $(objects)

$(objects) : $(OBJDIR)%.o : $(GRAPHICS_API_PATH)%.cpp
	$(CC) $(CFLAGS) $< -o $@

clean:
	-rm -rf $(OBJDIR)*.o $(OUTPUT_ARCHIVE_FILE)
