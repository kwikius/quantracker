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

QUANTRACKER_ROOT_DIR := ../../

include $(QUANTRACKER_ROOT_DIR)include/quantracker/build/osd.mk

C_FLAGS_1  = -Wall -c -g -$(OPTIMISATION_LEVEL) $(DEFINE_ARGS) $(INCLUDE_ARGS) \
 $(PROCESSOR_FLAGS) $(CFLAG_EXTRAS) -fdata-sections -ffunction-sections

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

OBJDIR := obj/osd_lib/
STM32_SRC_DIR := $(STM32_STD_PERIPH_LIB_DIR)STM32F4xx_StdPeriph_Driver/src/

# -------video objects --------------------------
unprefixed_video_objects = video_buffer.o video_column.o video_row.o \
video_pixel.o video_spi.o video_dma.o video_setup.o graphics_api.o \
draw_task.o sync_sep.o black_level.o dac.o led.o osd_state.o \
internal_video_mode.o

# add the telemetry tasks to the lib if required
ifeq ($(HAS_TELEMETRY),True)
unprefixed_video_objects += telemetry_task.o
endif

unobj_stm32_objects := misc.o
stm32_objects := $(patsubst %, $(OBJDIR)%,$(unobj_stm32_objects))

video_objects = $(patsubst %, $(OBJDIR)$(TELEMETRY_PREFIX)%,$(unprefixed_video_objects))

objects = $(video_objects) $(stm32_objects)
# ------------------------------------------------

.PHONY: all clean

all : $(OSD_ARCHIVE_FILE)
   
$(OSD_ARCHIVE_FILE) : $(objects)
	$(AR) rcs $@ $(objects)

$(video_objects): $(OBJDIR)$(TELEMETRY_PREFIX)%.o : video/%.cpp
	$(CC) $(CFLAGS) $< -o $@

$(stm32_objects) : $(OBJDIR)%.o : $(STM32_SRC_DIR)%.c
	$(CC1) $(C_FLAGS_1) -D'assert_param(args)= ' $(patsubst %,-I%,$(STM32_INCLUDES)) $< -o $@

clean:
	-rm -rf $(OSD_ARCHIVE_FILE) $(OBJDIR)*.o 
