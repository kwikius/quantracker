
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

# modify to change the name of the dll

DLL_NAME = osd_draw.so

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
 
INCLUDES = -I$(QUAN_INCLUDE_PATH) -I$(C_INCLUDE_PATH)

GRAPHICS_API_SRC_PATH = $(QUAN_INCLUDE_PATH)quan_matters/src/uav/osd/

DEFINES = QUANTRACKER_AIR_OSD_PC_SIM_MODE

#########################################################

#sources =  draw_altitude.cpp draw_artificial_horizon.cpp \
#draw_compass.cpp draw_home.cpp on_draw.cpp draw_gps_state.cpp \
#draw_batteries.cpp

# in $(GRAPHICS_API_SRC_PATH)
graphics_api_sources = draw_arc.cpp draw_bitmap.cpp draw_circle.cpp \
draw_line.cpp draw_text.cpp flood_fill.cpp sim_graphics_api.cpp

#objects = $(patsubst %.cpp,%.o,$(sources))

common_objects = on_draw.o draw_altitude.o draw_artificial_horizon.o \
draw_compass.o draw_home.o draw_gps_state.o draw_batteries.o \
symbology.o  aircraft.o draw_lat_lon.o draw_mode.o \
draw_speed.o osd_control.o graphics_api_extras.o

graphics_api_objects = $(patsubst %.cpp,%.o,$(graphics_api_sources))

CFLAGS = -Wall -std=c++11 -fPIC -Os -fvisibility=hidden

LFLAGS = -shared -Wall -std=c++11 

DEFINE_ARGS = $(patsubst %,-D%,$(DEFINES))

.PHONY : clean clean_graphics_lib all

all : $(DLL_NAME)

.RECIPEPREFIX = ~

$(DLL_NAME) : $(common_objects) osd_graphics_api.a
~ $(LD) $(LFLAGS) -o $(DLL_NAME) $(common_objects) osd_graphics_api.a 

$(common_objects) : %.o : ../common/%.cpp
~ $(CC) -c $(CFLAGS) $(INCLUDES) $(DEFINE_ARGS) $< -o $@

# The graphics api lib pc version
osd_graphics_api.a : $(graphics_api_objects)
~ ar rcs $@ $(graphics_api_objects)

$(graphics_api_objects): %.o: $(GRAPHICS_API_SRC_PATH)%.cpp
~ $(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@ 

clean_obj:
~ -rm -rf $(common_objects) *.so

clean_graphics_lib:
~ -rm $(graphics_api_objects) osd_graphics_api.a

clean : 
~ -rm -rf $(graphics_api_objects) $(common_objects) osd_graphics_api.a *.so
