#
# Copyright (c) 2013 -2015 Andy Little 

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
#
# Asumes /air/osd/Dependencies.mk has been customised
include ../../Dependencies.mk

#$(QUAN_INCLUDE_PATH) should be previously defined in ../../.Dependencies.mk
INCLUDES = $(QUAN_INCLUDE_PATH)
CC = g++
LD = g++

DEFINES = QUAN_FLASH_DEBUG QUAN_PC_SIM

INCLUDE_ARGS = $(patsubst %,-I%,$(INCLUDES))

DEFINE_ARGS = $(patsubst %,-D%,$(DEFINES))

#CFLAGS = -pedantic -Wall -std=c++11 -Os -g $(INCLUDE_ARGS) $(DEFINE_ARGS)
CFLAGS = -std=c++11  $(INCLUDE_ARGS) $(DEFINE_ARGS)

objects = main.o osd_sim.o flash.o quan_flash.o quan_flash_menu.o

all: flash_menu_debug.exe

flash_menu_debug.exe : $(objects)
	$(LD) $(objects) -o $@

main.o : main.cpp
	$(CC) $(CFLAGS) -c $< -o $@

osd_sim.o : osd_sim.cpp
	$(CC) $(CFLAGS) -c $< -o $@

flash.o : ../../flash.cpp
	$(CC) $(CFLAGS) -c $< -o $@

quan_flash.o : $(QUAN_INCLUDE_PATH)/quan_matters/src/stm32/flash/flash.cpp
	$(CC) $(CFLAGS) $(INCLUDE_ARGS) -c $< -o $@

quan_flash_menu.o : $(QUAN_INCLUDE_PATH)/quan_matters/src/stm32/flash/flash_menu.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	-rm -rf *.o *.exe

