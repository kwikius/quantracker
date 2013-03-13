

# Copyright (c) 2013 Andy Little 
#
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

############################################################
# **** you will need to modify these to your own paths ***
TOOLCHAIN_PREFIX = /home/andy/arm/arm-cortex-m4-hardfloat-toolchain/
STM32F4_INCLUDE_PATH = /opt/stm32f4/STM32F4xx_DSP_StdPeriph_Lib_V1.0.0/Libraries/
QUAN_INCLUDE_PATH = /home/andy/website/quan-trunk/
INIT_LIB_PREFIX = $(TOOLCHAIN_PREFIX)lib/gcc/arm-none-eabi/4.7.1/thumb/cortex-m4/float-abi-hard/fpuv4-sp-d16/
############################################################

CC      = $(TOOLCHAIN_PREFIX)bin/arm-none-eabi-g++
CC1     = $(TOOLCHAIN_PREFIX)bin/arm-none-eabi-gcc
LD      = $(TOOLCHAIN_PREFIX)bin/arm-none-eabi-g++
CP      = $(TOOLCHAIN_PREFIX)bin/arm-none-eabi-objcopy
OD      = $(TOOLCHAIN_PREFIX)bin/arm-none-eabi-objdump

STM32F4_LINKER_SCRIPT = stm32f4.ld

INCLUDES = -I$(STM32F4_INCLUDE_PATH)CMSIS/Include/ \
-I$(STM32F4_INCLUDE_PATH)CMSIS/Device/ST/STM32F4xx/Include/ \
-I$(STM32F4_INCLUDE_PATH)STM32F4xx_StdPeriph_Driver/inc/ \
-I$(QUAN_INCLUDE_PATH)

CFLAG_EXTRAS = -fno-math-errno -DQUAN_STM32F4 -DQUAN_NO_EXCEPTIONS 
CFLAG_EXTRAS += -Wl,-u,vsprintf -lm
CFLAG_EXTRAS += -DDEBUG

PROCESSOR_FLAGS = -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mthumb -mfloat-abi=hard

CFLAGS = -std=c++11 -fno-rtti -fno-exceptions -c -O2 -g $(INCLUDES) $(PROCESSOR_FLAGS) $(CFLAG_EXTRAS)

INIT_LIBS = $(INIT_LIB_PREFIX)crti.o $(INIT_LIB_PREFIX)crtn.o

LFLAGS = -T$(STM32F4_LINKER_SCRIPT) -O2 $(PROCESSOR_FLAGS) $(CFLAG_EXTRAS) $(INIT_LIBS) -nodefaultlibs -nostartfiles

CPFLAGS = -Obinary
ODFLAGS = -d

STARTUP = startup.s

all: test

Debug : test

objects = main.o events.o tracker_states.o azimuth_encoder.o azimuth_motor.o azimuth_motor_calc.o loop_timer.o \
 elevation_servo.o telemetry.o frsky_serial_port.o led.o switch_input.o serial_port.o setup.o spbrk.o system_init.o  

clean:
	-rm -rf *.o *.elf *.bin *.lst

test: main.elf
	@ echo "...copying"
	$(CP) $(CPFLAGS) main.elf main.bin
	$(OD) $(ODFLAGS) main.elf > main.lst

main.elf: startup.o $(objects) 
	@ echo "..linking"
	$(LD)   $(LFLAGS)  -o main.elf $(objects) startup.o 

$(objects): %.o : %.cpp
	$(CC) $(CFLAGS) $< -o $@

startup.o: $(STARTUP)
	$(CC) $(CFLAGS) -o startup.o $(STARTUP) 
