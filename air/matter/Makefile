
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

