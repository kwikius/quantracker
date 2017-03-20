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

#--------------------------------------------

# for use as a template edit this 
QUANTRACKER_ROOT_DIR = ../../

TELEMETRY_DIRECTION = QUAN_OSD_TELEM_NONE

LINKER_SCRIPT = $(QUANTRACKER_ROOT_DIR)air/osd/stm32f4.ld

include $(QUANTRACKER_ROOT_DIR)include/quantracker/build/osd.mk

OBJDIR = obj/
BINDIR = bin/

# N.B Changing the library order
# can cause linker errors
static_library_files = \
   $(TARGET_LIB_NAME_PREFIX)_graphics_api.a\
   $(TARGET_LIB_NAME_PREFIX)_system.a \
   $(TARGET_LIB_NAME_PREFIX)_osd.a 
   
board_objects = main.o 

obj_board_objects = $(patsubst %,$(OBJDIR)$(TARGET_LIB_NAME_PREFIX)_%,$(board_objects))

# resource sources are named for the library since they are coded differently
resource_objects = fonts.o bitmaps.o
obj_resource_objects = $(patsubst %,$(OBJDIR)$(TARGET_LIB_NAME_PREFIX)_%,$(resource_objects))

objects = $(obj_board_objects) $(obj_resource_objects)

OUTPUT_FILE_NAME = hello_world
OUTPUT_FILE_PATH = $(BINDIR)$(TARGET_LIB_NAME_PREFIX)_$(OUTPUT_FILE_NAME)
#----------------------------
ELF_FILE = $(OUTPUT_FILE_PATH).elf
BIN_FILE = $(OUTPUT_FILE_PATH).bin
LST_FILE = $(OUTPUT_FILE_PATH).lst
MAP_FILE = $(OUTPUT_FILE_PATH).map

all: test

test: $(ELF_FILE)
	@ echo "...copying"
	$(CP) $(CPFLAGS) $(ELF_FILE) $(BIN_FILE)
	$(OD) $(ODFLAGS) $(ELF_FILE) > $(LST_FILE)
	$(SIZ) -A $(ELF_FILE)

$(ELF_FILE): $(objects) $(static_libraries) 
	@ echo "..linking"
	$(LD) $(LDFLAGS) -Wl,-Map=$(MAP_FILE) -o $(ELF_FILE) $(objects) \
   -Wl,--undefined=_sbrk $(static_libraries) 

$(obj_board_objects): $(OBJDIR)$(TARGET_LIB_NAME_PREFIX)_%.o : %.cpp
	$(CC) $(CFLAGS) $< -o $@

$(obj_resource_objects): $(OBJDIR)%.o : %.cpp
	$(CC) $(CFLAGS) $< -o $@

local_static_library_path = $(QUANTRACKER_ROOT_DIR)lib/osd/
#$(static_libraries) :
#	$(MAKE) -C $(QUANTRACKER_ROOT_DIR)air/osd -f all_osd_libs.mk
$(local_static_library_path)$(TARGET_LIB_NAME_PREFIX)_graphics_api.a :
	$(MAKE) -C $(QUANTRACKER_ROOT_DIR)air/osd -f graphics_api_lib.mk

$(local_static_library_path)$(TARGET_LIB_NAME_PREFIX)_system.a :
	$(MAKE) -C $(QUANTRACKER_ROOT_DIR)air/osd -f system.mk

$(local_static_library_path)$(TARGET_LIB_NAME_PREFIX)_osd.a :
	$(MAKE) -C $(QUANTRACKER_ROOT_DIR)air/osd -f osd_lib.mk

#-----------------------------------------

upload : test
	st-flash write $(BIN_FILE) 0x8000000

upload_sp : test
	$(STM32FLASH) -b 115200 -f -v -w $(BIN_FILE) /dev/ttyUSB0
#----------------------------------------

clean:
	-rm -rf $(objects) $(ELF_FILE) $(BIN_FILE) $(LST_FILE) $(MAP_FILE)
