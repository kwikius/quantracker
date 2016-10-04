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

OSD_ARCHIVE_FILE := $(QUANTRACKER_ROOT_DIR)lib/osd/$(TARGET_LIB_NAME_PREFIX)_system.a

OBJDIR := obj/system/

SYSTEM_INIT := system_init.cpp
STARTUP := startup.s

C_FLAGS_1  = -Wall -c -g -$(OPTIMISATION_LEVEL) $(DEFINE_ARGS) $(INCLUDE_ARGS) \
 $(PROCESSOR_FLAGS) $(CFLAG_EXTRAS) -fdata-sections -ffunction-sections

unobj_rtos_objects := tasks.o queue.o list.o timers.o
 rtos_objects := $(patsubst %, $(OBJDIR)$(TARGET_LIB_NAME_PREFIX)_%,$(unobj_rtos_objects))

unobj_quan_objects := malloc_free.o

unobj_system_objects := $(unobj_rtos_objects)  $(unobj_quan_objects) \
 startup.o system_init.o port.o heap_3.o rtos_hooks.o spbrk.o system.o

objects := $(patsubst %, $(OBJDIR)$(TARGET_LIB_NAME_PREFIX)_%,$(unobj_system_objects))

.PHONY: all clean

all : $(OSD_ARCHIVE_FILE)
   
$(OSD_ARCHIVE_FILE) : $(objects)
	$(AR) rcs $@ $(objects)

$(OBJDIR)$(TARGET_LIB_NAME_PREFIX)_system_init.o : $(SYSTEM_INIT)
	$(CC) $(CFLAGS) $< -o $@

$(OBJDIR)$(TARGET_LIB_NAME_PREFIX)_system.o : system.cpp
	$(CC) $(CFLAGS) $< -o $@  

$(OBJDIR)$(TARGET_LIB_NAME_PREFIX)_spbrk.o : spbrk.cpp
	$(CC) $(CFLAGS) $< -o $@  

$(OBJDIR)$(TARGET_LIB_NAME_PREFIX)_startup.o: $(STARTUP)
	$(CC) $(CFLAGS) $< -o $@ 

$(rtos_objects) : $(OBJDIR)$(TARGET_LIB_NAME_PREFIX)_%.o : $(FREE_RTOS_DIR)Source/%.c
	$(CC1) $(C_FLAGS_1) $(patsubst %,-I%,$(RTOS_INCLUDES)) $< -o $@

$(OBJDIR)$(TARGET_LIB_NAME_PREFIX)_port.o : $(FREE_RTOS_DIR)Source/portable/GCC/ARM_CM4F/port.c
	$(CC1) $(C_FLAGS_1) $(patsubst %,-I%,$(RTOS_INCLUDES)) $< -o $@

$(OBJDIR)$(TARGET_LIB_NAME_PREFIX)_heap_3.o : $(FREE_RTOS_DIR)Source/portable/MemMang/heap_3.c
	$(CC1) $(C_FLAGS_1) $(patsubst %,-I%,$(RTOS_INCLUDES)) $< -o $@

$(OBJDIR)$(TARGET_LIB_NAME_PREFIX)_rtos_hooks.o : rtos_hooks.cpp 
	$(CC) $(CFLAGS) $< -o $@

$(OBJDIR)$(TARGET_LIB_NAME_PREFIX)_malloc_free.o : $(QUAN_INCLUDE_PATH)/quan_matters/src/stm32/malloc_free.cpp
	$(CC) $(CFLAGS) $< -o $@

clean:
	-rm -rf $(OSD_ARCHIVE_FILE) $(objects) 
