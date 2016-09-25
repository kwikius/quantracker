
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

DLL_NAME = quantracker_examples_osd_example1.so

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
 
#access to the air/osd dir is required for symbology header
# though may need refctoring.
INCLUDES = -I$(QUAN_INCLUDE_PATH) -I$(C_INCLUDE_PATH) -I../../../air/osd/

GRAPHICS_API_SRC_PATH = $(QUAN_INCLUDE_PATH)quan_matters/src/uav/osd/

DEFINES = QUANTRACKER_AIR_OSD_PC_SIM_MODE

#########################################################
# 
common_sources =  draw_altitude.cpp draw_artificial_horizon.cpp \
draw_compass.cpp draw_home.cpp on_draw.cpp draw_gps_state.cpp \
draw_batteries.cpp symbology.cpp 

# in $(GRAPHICS_API_SRC_PATH)
# should make a static lib of this 
graphics_api_sources = draw_arc.cpp draw_bitmap.cpp draw_circle.cpp \
draw_line.cpp draw_text.cpp flood_fill.cpp 

common_objects = $(patsubst %.cpp,%.o,$(common_sources)) 

graphics_api_objects = $(patsubst %.cpp,%.o,$(graphics_api_sources))

objects = $(common_objects) sim_graphics_api.o

CFLAGS = -Wall -std=c++11 -fPIC -Os -fvisibility=hidden

LDFLAGS = -shared -Wall -std=c++11 

DEFINE_ARGS = $(patsubst %,-D%,$(DEFINES))

.PHONY : clean clean_graphics_lib all

all : $(DLL_NAME)

.RECIPEPREFIX = ~

$(DLL_NAME) : $(objects) osd_graphics_api.a
~ $(LD) $(LDFLAGS) -o $(DLL_NAME) $(objects) osd_graphics_api.a 

$(common_objects) : %.o : ../common/%.cpp
~ $(CC) -c $(CFLAGS) $(INCLUDES) $(DEFINE_ARGS) $< -o $@

# The graphics api lib pc version
osd_graphics_api.a : $(graphics_api_objects) 
~ ar rcs $@ $(graphics_api_objects) 

$(graphics_api_objects): %.o: $(GRAPHICS_API_SRC_PATH)%.cpp
~ $(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@ 

sim_graphics_api.o : sim_graphics_api.cpp
~ $(CC) -c $(CFLAGS) $(INCLUDES) $(DEFINE_ARGS) $< -o $@

clean_obj:
~ -rm -rf $(objects) *.so

clean_graphics_lib:
~ -rm $(graphics_api_objects) osd_graphics_api.a

clean : 
~ -rm -rf $(graphics_api_objects) $(objects) osd_graphics_api.a *.so
