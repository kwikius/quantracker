

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
# **** you will need modify the paths in this section to the paths you saved the libraries in***


# the STM32F4 librraies are available from 
# http://www.st.com/st-web-ui/static/active/en/st_prod_software_internet/resource/technical/software/firmware/stm32f4_dsp_stdperiph_lib.zip
# Change this to the path where you saved the STM32F4 standard Peripheral libraries
STM32F4_INCLUDE_PATH = /opt/stm32f4/STM32F4xx_DSP_StdPeriph_Lib_V1.0.0/Libraries/

# The quan libraries are available from 
# https://github.com/kwikius/quan-trunk
# Change this to the the path twhere you saved the quan libraries
QUAN_INCLUDE_PATH = /home/andy/website/quan-trunk/

# The GCC ARM embedded toolchain (recommended) is available from
#  https://launchpad.net/gcc-arm-embedded
# If using this toolchain, the TOOLCHAIN_ID should be set to GCC_Arm_Embedded (The default)
TOOLCHAIN_ID = GCC_Arm_Embedded
# Otherwise if you are using the toolchain from
# https://github.com/prattmic/arm-cortex-m4-hardfloat-toolchain
# set the TOOLCHAIN_ID as follows
#TOOLCHAIN_ID = Michael_Pratt

# Change this to the path where you installed the  arm gcc compiler toolchain
TOOLCHAIN_PREFIX = /opt/gcc-arm-none-eabi-4_7-2013q2/

# Change this to your version of gcc. 
# (You can find the gcc version by invoking arm-noe-eabi-gcc --version in the $(TOOLCHAIN_PREFIX)/bin/ directory)
TOOLCHAIN_GCC_VERSION = 4.7.4

# generic flash
# stm32f4 flash

STM32F4_SPECIFIC_FLASH_SRC = $(QUAN_INCLUDE_PATH)/quan_matters/src/stm32/f4/specific_flash.cpp

GENERIC_FLASH_SRC_PATH = $(QUAN_INCLUDE_PATH)/quan_matters/src/stm32/flash/

# TEXT_TO_BYTESTREAM_SRC = $(QUAN_INCLUDE_PATH)/quan_matters/src/text_to_bytestream/

# otherwise you are on your own ... !
###################################################################

ifeq ($(strip $(TOOLCHAIN_ID)),GCC_Arm_Embedded)
	INIT_LIB_PREFIX = $(TOOLCHAIN_PREFIX)/lib/gcc/arm-none-eabi/$(TOOLCHAIN_GCC_VERSION)/armv7e-m/fpu/
else
ifeq ($(strip $(TOOLCHAIN_ID)), Michael_Pratt)
	INIT_LIB_PREFIX = $(TOOLCHAIN_PREFIX)/lib/gcc/arm-none-eabi/$(TOOLCHAIN_GCC_VERSION)/thumb/cortex-m4/float-abi-hard/fpuv4-sp-d16/
else
   INIT_LIB_PREFIX = ???
endif
endif

CC      = $(TOOLCHAIN_PREFIX)bin/arm-none-eabi-g++
CC1     = $(TOOLCHAIN_PREFIX)bin/arm-none-eabi-gcc
LD      = $(TOOLCHAIN_PREFIX)bin/arm-none-eabi-g++
CP      = $(TOOLCHAIN_PREFIX)bin/arm-none-eabi-objcopy
OD      = $(TOOLCHAIN_PREFIX)bin/arm-none-eabi-objdump
SIZ     = $(TOOLCHAIN_PREFIX)bin/arm-none-eabi-size

STM32F4_LINKER_SCRIPT = stm32f4.ld

INCLUDES = -I$(STM32F4_INCLUDE_PATH)CMSIS/Include/ \
-I$(STM32F4_INCLUDE_PATH)CMSIS/Device/ST/STM32F4xx/Include/ \
-I$(STM32F4_INCLUDE_PATH)STM32F4xx_StdPeriph_Driver/inc/ \
-I$(QUAN_INCLUDE_PATH)

CFLAG_EXTRAS = -fno-math-errno -DQUAN_STM32F4 -DQUAN_NO_EXCEPTIONS 
CFLAG_EXTRAS += -Wl,-u,vsprintf -lm
CFLAG_EXTRAS += -DDEBUG
CFLAG_EXTRAS += -Wall

PROCESSOR_FLAGS = -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mthumb -mfloat-abi=hard

CFLAGS = -std=c++11 -fno-rtti -fno-exceptions -c -Os -g $(INCLUDES) $(PROCESSOR_FLAGS) $(CFLAG_EXTRAS) \
-fdata-sections -ffunction-sections

INIT_LIBS = $(INIT_LIB_PREFIX)crti.o $(INIT_LIB_PREFIX)crtn.o

LFLAGS = -T$(STM32F4_LINKER_SCRIPT) -Os $(PROCESSOR_FLAGS) $(CFLAG_EXTRAS) $(INIT_LIBS) -nodefaultlibs -nostartfiles \
--specs=nano.specs -Wl,--gc-sections

CPFLAGS = -Obinary
ODFLAGS = -d

STARTUP = startup.s

all: test

Debug : test

quan_generic_flash_objects = quan_generic_flash.o quan_generic_flash_error.o \
quan_generic_flash_menu.o quan_malloc_free.o

local_objects = main.o events.o tracker_states.o azimuth_encoder.o azimuth_motor.o \
azimuth_motor_calc.o loop_timer.o elevation_servo.o telemetry.o frsky_serial_port.o \
led.o switch_input.o serial_port.o setup.o spbrk.o system_init.o text_commands.o \
compass.o update_mag.o systick.o frsky_bytestuff_telemetry.o cobs_fsk_telemetry.o \
fsk_filter.o user.o flash.o

#local_flash_objects = flash.o flash_get_set.o \
 #init_values_from_flash.o

flash_objects  = $(quan_generic_flash_objects) quan_stm32_f4_specific_flash.o 
 
all_objects = startup.o $(local_objects) $(flash_objects)
  
clean:
	-rm -rf *.o *.elf *.bin *.lst

test: main.elf
	@ echo "...copying"
	$(CP) $(CPFLAGS) main.elf main.bin
	$(OD) $(ODFLAGS) main.elf > main.lst
	$(SIZ) -A main.elf

main.elf : $(all_objects) 
	@ echo "..linking"
	$(LD)   $(LFLAGS)  -o main.elf $(all_objects) 

$(quan_generic_flash_objects) : quan_generic_%.o : $(GENERIC_FLASH_SRC_PATH)%.cpp
	$(CC) $(CFLAGS) $< -o $@

$(local_objects): %.o : %.cpp
	$(CC) $(CFLAGS) $< -o $@

$(local_flash_objects) : %.o : flash/%.cpp
	$(CC) $(CFLAGS) $< -o $@

quan_malloc_free.o : $(QUAN_INCLUDE_PATH)/quan_matters/src/stm32/malloc_free.cpp

quan_stm32_f4_specific_flash.o : $(STM32F4_SPECIFIC_FLASH_SRC)
	$(CC) $(CFLAGS) -o quan_stm32_f4_specific_flash.o $(STM32F4_SPECIFIC_FLASH_SRC)

startup.o: $(STARTUP)
	$(CC) $(CFLAGS) -o startup.o $(STARTUP) 

upload : test
	st-flash write main.bin 0x8000000
