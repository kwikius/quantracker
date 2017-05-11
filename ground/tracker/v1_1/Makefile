# Copyright (c) 2013 -2017 Andy Little 
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

# use make QUANTRACKER_CMDLINE=True for command line mode

OBJDIR = obj/
BINDIR = bin/

LINKER_SCRIPT = system/stm32f4.ld

QUANTRACKER_ROOT_DIR = ../../../../quantracker/

include $(QUANTRACKER_ROOT_DIR)include/quantracker/build/osd.mk

ifeq ($(QUANTRACKER_CMDLINE),True)
CFLAGS += -DQUANTRACKER_GROUND_COMMANDLINE_MODE
endif

static_library_files = \
   quantracker_air_graphics_api.a \
   quantracker_air_osd_rx.a \
   flash_variables.a \
   quantracker_air_system.a

static_libraries = $(patsubst %,$(STATIC_LIBRARY_PATH)%,$(static_library_files))

local_objects = main.o command_line.o tracker_task.o modem_task.o button_pressed_task.o tracking_update.o

obj_local_objects = $(patsubst %,$(OBJDIR)%, $(local_objects))

system_objects = flash.o gcs_serial.o modem_serial.o rtos_hooks.o setup.o spbrk.o user.o
obj_system_objects = $(patsubst %,$(OBJDIR)%, $(system_objects))

osd_objects = on_draw.o on_telemetry_received.o
obj_osd_objects = $(patsubst %,$(OBJDIR)%, $(osd_objects))

azimuth_objects = encoder.o motor.o servo.o
obj_azimuth_objects =  $(patsubst %,$(OBJDIR)azimuth_%, $(azimuth_objects))

elevation_objects = servo.o
obj_elevation_objects =  $(patsubst %,$(OBJDIR)elevation_%, $(elevation_objects))

# font path
USER_SOURCE_PATH = $(QUANTRACKER_ROOT_DIR)/examples/osd_example1/

user_resources = fonts.o

obj_user_resources = $(patsubst %,$(OBJDIR)%, $(user_resources))

objects = $(obj_local_objects) $(obj_system_objects) $(obj_osd_objects) $(obj_user_resources) \
$(obj_azimuth_objects) $(obj_elevation_objects)

all : test

test: $(BINDIR)main.elf
	@ echo "...copying"
	$(CP) $(CPFLAGS) $(BINDIR)main.elf $(BINDIR)main.bin
	$(OD) $(ODFLAGS) $(BINDIR)main.elf > $(BINDIR)main.lst
	$(SIZ) -A $(BINDIR)main.elf

$(BINDIR)main.elf: $(objects) $(static_libraries)
	@ echo "..linking"
	$(LD) $(LDFLAGS) -o $(BINDIR)main.elf $(objects) $(INIT_LIBS) \
   -Wl,--undefined=_sbrk $(static_libraries)

$(obj_local_objects): $(OBJDIR)%.o : %.cpp
	$(CC) $(CFLAGS) $< -o $@

$(obj_system_objects): $(OBJDIR)%.o : system/%.cpp
	$(CC) $(CFLAGS) $< -o $@

$(obj_osd_objects): $(OBJDIR)%.o : osd/%.cpp
	$(CC) $(CFLAGS) $< -o $@

$(obj_azimuth_objects): $(OBJDIR)azimuth_%.o : azimuth/%.cpp
	$(CC) $(CFLAGS) $< -o $@

$(obj_elevation_objects): $(OBJDIR)elevation_%.o : elevation/%.cpp
	$(CC) $(CFLAGS) $< -o $@

$(obj_user_resources) : $(OBJDIR)%.o : $(USER_SOURCE_PATH)board/%.cpp
	$(CC) $(CFLAGS) -DQUAN_OSD_TARGET_BOARD $< -o $@

$(static_libraries) :
	$(MAKE) -C ../../../air/osd -f all_osd_libs.mk

#-----------------------------------------
st_flash_upload : test
	st-flash write $(BINDIR)main.bin 0x8000000

upload_sp : test
	$(STM32FLASH) -b 115200 -f -v -w $(BINDIR)main.bin /dev/ttyUSB0

clean:
	-rm -rf $(OBJDIR)*.o $(BINDIR)*.elf $(BINDIR)*.bin $(BINDIR)*.lst
