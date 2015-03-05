include ../../air/osd/Dependencies.mk

CC = g++ 
LD = g++

CFLAGS = -std=c++11 -I$(QUAN_INCLUDE_PATH)

all : bitmap_demo.exe

bitmap_demo.exe : main.o output_image.o
	$(LD) -o bitmap_demo.exe main.o output_image.o

output_image.o : $(QUAN_INCLUDE_PATH)/quan_matters/src/uav/osd/detail/output_image.cpp
	$(CC) $(CFLAGS) -c $< -o $@

main.o : main.cpp my_bitmap.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	-rm -rf *.o *.exe


