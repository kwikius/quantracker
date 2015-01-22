#
# Copyright (c) 2013 -2015 Andy Little 
#
# With Grateful Acknowledgements to the prior work of:
#   Sami Korbonen(Openpilot.org)
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
#
#change to your own quan directory

INCLUDES = /home/andy/website/quan-trunk
CC = g++
LD = g++
DEFINES = QUAN_STM32F0

INCLUDE_ARGS = $(patsubst %,-I%,$(INCLUDES))

DEFINE_ARGS = $(patsubst %,-D%,$(DEFINES))

all: sine_wave_gen.exe

sine_wave_gen.o : air_sine_wave_gen.cpp  ../sin_array.txt
	$(CC) $(INCLUDE_ARGS) $(DEFINE_ARGS) -std=c++11 -c air_sine_wave_gen.cpp -o sine_wave_gen.o

sine_wave_gen.exe : sine_wave_gen.o
	$(LD) -o sine_wave_gen.exe sine_wave_gen.o
	
clean:
	-rm -rf *.o *.exe

