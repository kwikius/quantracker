

# Copyright (c) 2012-2013 Andy Little 

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
 
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>

# modify to change your app name
APPLICATION_NAME = quantracker_sim.exe

######################## #######################################
# mod these to your compiler must be a C++11 gcc compiler
CC = /opt/gcc-4.7.2/bin/g++
LD = /opt/gcc-4.7.2/bin/g++
# Modify this to your quan include path
QUAN_INCLUDE_PATH = /home/andy/website/quan-trunk/

# try comment this ...
C_INCLUDE_PATH = /usr/include/i386-linux-gnu

# and uncomment this if you get compile errors
# On my Ubuntu system the C++ headers and default compiler are not c++11
# The C_INCLUDE_PATH is required to get it to compile

#INCLUDES = -I$(QUAN_INCLUDE_PATH) 
INCLUDES = -I$(QUAN_INCLUDE_PATH) -I$(C_INCLUDE_PATH)

#########################################################

local_sources = aircraft.cpp aircraft_symbol.cpp app.cpp bytestuff.cpp cobs.cpp document.cpp \
drawing.cpp events.cpp frsky.cpp main_frame.cpp panel.cpp sp_in_thread.cpp splitter.cpp view.cpp \
window_ids.cpp

local_objects = $(patsubst %.cpp,%.o,$(local_sources))

quan_gx_wxwidgets_sources = draw_box.cpp draw_circle.cpp draw_line.cpp draw_poly_line.cpp \
graphics_context.cpp 

quan_gx_wxwidgets_objects = $(patsubst %.cpp,%.o,$(quan_gx_wxwidgets_sources)) 

objects = $(local_objects) $(quan_gx_wxwidgets_objects) serial_port.o static_rgb_colours.o

# could add -Os etc here
CFLAGS = -Wall -std=c++11 -Os

LDFLAGS =

.PHONY : clean all

all : $(APPLICATION_NAME)

$(local_objects) : %.o : %.cpp
	$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@ `wx-config --cppflags`

$(quan_gx_wxwidgets_objects) : %.o : $(QUAN_INCLUDE_PATH)quan_matters/src/wxwidgets/%.cpp
	$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@ `wx-config --cppflags`

serial_port.o : $(QUAN_INCLUDE_PATH)quan_matters/src/serial_port.cpp
	$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@ `wx-config --cppflags`

static_rgb_colours.o : $(QUAN_INCLUDE_PATH)quan_matters/src/gx/static_rgb_colours.cpp
	$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@ `wx-config --cppflags`

$(APPLICATION_NAME) : $(objects)
	$(LD) $(CFLAGS) $(LDFLAGS) -o $(APPLICATION_NAME) $(objects)  `wx-config --libs`

clean:
	-rm -rf *.o *.exe
