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

INCLUDE_ARGS = $(patsubst %,-I%,$(INCLUDES))
CFLAGS = -std=c++11 -Wall

all: flash_vars_uploader.exe

flash_vars_uploader.exe : main.o serial_port.o
	$(LD) main.o serial_port.o -o flash_vars.exe

main.o : main.cpp
	$(CC) $(CFLAGS) $(INCLUDE_ARGS) -c $< -o $@

serial_port.o : $(QUAN_INCLUDE_PATH)/quan_matters/src/serial_port.cpp
	$(CC) $(CFLAGS) $(INCLUDE_ARGS) -c $< -o $@ 
	
clean:
	-rm -rf *.o *.exe

