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

LINKER_SCRIPT = stm32f4.ld

QUANTRACKER_ROOT_DIR = ../../

TELEMETRY_DIRECTION = QUAN_OSD_TELEM_RECEIVER

include $(QUANTRACKER_ROOT_DIR)include/quantracker/build/osd.mk

# just for fonts
RESOURCE_PATH = ../../examples/osd_example1/

local_objects = main.o application.o setup.o on_draw.o rx_telemetry.o

OBJDIR = ./obj/

BINDIR = ./bin/

obj_local_objects = $(patsubst %,$(OBJDIR)%,$(local_objects))

resource_objects = fonts.o

obj_resource_objects = $(patsubst %,$(OBJDIR)%,$(resource_objects))

static_library_files = \
   quantracker_air_graphics_api.a \
   flash_variables.a \
   quantracker_air_osd_rx.a \
   quantracker_air_system.a

objects =  $(obj_local_objects) $(obj_resource_objects) 

all:  test

test: $(BINDIR)main.elf
	@ echo "...copying"
	$(CP) $(CPFLAGS) $(BINDIR)main.elf $(BINDIR)main.bin
	$(OD) $(ODFLAGS) $(BINDIR)main.elf > $(BINDIR)main.lst
	$(SIZ) -A $(BINDIR)main.elf

$(BINDIR)main.elf: $(objects) 
	@ echo "..linking"
	$(LD) $(LDFLAGS) -o $(BINDIR)main.elf $(INIT_LIBS) $(objects) \
-Wl,--undefined=_sbrk $(static_libraries)

$(obj_local_objects): $(OBJDIR)%.o : %.cpp
	$(CC) $(CFLAGS) $< -o $@

$(obj_resource_objects) : $(OBJDIR)%.o : $(RESOURCE_PATH)board/%.cpp
	$(CC) $(CFLAGS) -DQUAN_OSD_TARGET_BOARD $< -o $@
#-----------------------------------------
upload_sp : test
	$(STM32FLASH) -b 115200 -f -v -w $(BINDIR)main.bin /dev/ttyUSB0
#---------------------------------------------
clean:
	-rm -rf $(OBJDIR)*.o $(BINDIR)*.elf $(BINDIR)*.bin $(BINDIR)*.lst
