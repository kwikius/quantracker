

QUAN_INCLUDE_PATH = $(QUAN_ROOT)

TARGET_PROCESSOR = STM32F4
OPTIMISATION_LEVEL = O3

video_objects = video.o video_buffer.o video_column.o \
  video_row.o video_pixel.o video_spi.o video_dma.o font.o on_draw.o graphics_api.o \
  av_telem_data.o draw_loop.o sync_sep.o led.o black_level.o dac.o

local_objects = main.o spbrk.o setup.o serial_port.o fsk_output.o \
  fsk.o frsky.o frsky_zapp1.o aircraft.o mavlink.o timer.o settings.o \
  events.o heartbeat.o 

#include $(QUAN_INCLUDE_PATH)/quan_matters/src/stm32/make_scripts/stm32.mk

# requires QUAN_INCLUDE_PATH - path to quan lib
# requires TARGET_PROCESSOR either STM32F4 or STM32F0
# OPTIMISATION_LEVEL defaults to Os

ifeq ($(QUAN_INCLUDE_PATH), )
$(error "QUAN_INCLUDE_PATH must be defined to the path to the quan library")
endif

ifeq ($(OPTIMISATION_LEVEL), )
OPTIMISATION_LEVEL = O
endif

ifeq ( $(CFLAG_EXTRAS), )
CFLAG_EXTRAS = -fno-math-errno
endif

#required for Ubuntu 12.x placid as system headers have been put in strange places
# these have beeen defined to thos in my bash .profile
CPLUS_INCLUDE_PATH=
C_INCLUDE_PATH=
LIBRARY_PATH=

TOOLCHAIN_PREFIX =/opt/gcc-arm-none-eabi-4_7-2013q2/
TOOLCHAIN_GCC_VERSION = 4.7.4

CC      = $(TOOLCHAIN_PREFIX)bin/arm-none-eabi-g++
CC1     = $(TOOLCHAIN_PREFIX)bin/arm-none-eabi-gcc
LD      = $(TOOLCHAIN_PREFIX)bin/arm-none-eabi-g++
CP      = $(TOOLCHAIN_PREFIX)bin/arm-none-eabi-objcopy
OD      = $(TOOLCHAIN_PREFIX)bin/arm-none-eabi-objdump
SIZ     = $(TOOLCHAIN_PREFIX)bin/arm-none-eabi-size
  
#MAKE_SCRIPTS_PATH = $(QUAN_INCLUDE_PATH)/quan_matters/src/stm32/make_scripts

ifeq ($(TARGET_PROCESSOR), STM32F4)
# specific flags for stm32f4
DEFINES = QUAN_STM32F4 
# Define if using software sync sep rather than LM1881
DEFINES += QUAN_OSD_SOFTWARE_SYNCSEP

STARTUP = startup.s
# custom linker script 
LINKER_SCRIPT = stm32f4.ld
#LINKER_SCRIPT = $(MAKE_SCRIPTS_PATH)/stm32f4.ld
FLASH_SRC = $(QUAN_INCLUDE_PATH)/quan_matters/src/stm32/f4/flash.cpp
SYSTICK_SRC = $(QUAN_INCLUDE_PATH)/quan_matters/src/stm32/systick.cpp

SYSTEM_INIT = system_init.cpp
PROCESSOR_FLAGS = -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mthumb -mfloat-abi=hard

INCLUDES = /opt/stm32f4/STM32F4xx_DSP_StdPeriph_Lib_V1.0.0/Libraries/CMSIS/Include \
/opt/stm32f4/STM32F4xx_DSP_StdPeriph_Lib_V1.0.0/Libraries/CMSIS/Device/ST/STM32F4xx/Include \
/opt/stm32f4/STM32F4xx_DSP_StdPeriph_Lib_V1.0.0/Libraries/STM32F4xx_StdPeriph_Driver/inc \

INIT_LIB_PREFIX = $(TOOLCHAIN_PREFIX)/lib/gcc/arm-none-eabi/$(TOOLCHAIN_GCC_VERSION)/armv7e-m/fpu/
# todo stm32f0 version
#else
#ifeq ($(TARGET_PROCESSOR), STM32F0)
#include $(MAKE_SCRIPTS_PATH)/stm32f0.mk
else
$(error no target processor defined)
endif
#endif

INIT_LIBS = $(INIT_LIB_PREFIX)crti.o $(INIT_LIB_PREFIX)crtn.o

INCLUDES += $(QUAN_INCLUDE_PATH) /home/andy/website/fpv/mavlink

INCLUDE_ARGS = $(patsubst %,-I%,$(INCLUDES))

DEFINES += HSE_VALUE=8000000

board_type1 : DEFINES += QUAN_OSD_BOARD_TYPE=1
board_type2 : DEFINES += QUAN_OSD_BOARD_TYPE=2
board_type3 : DEFINES += QUAN_OSD_BOARD_TYPE=3

DEFINE_ARGS = $(patsubst %,-D%,$(DEFINES))

# for float printf format etc
# CFLAG_EXTRAS += -Wl,-u,vsprintf -lm

CFLAGS  = -Wall -std=c++11 -fno-rtti -fno-exceptions -c -g -$(OPTIMISATION_LEVEL) $(DEFINE_ARGS) $(INCLUDE_ARGS) \
  $(PROCESSOR_FLAGS) $(CFLAG_EXTRAS) -fno-math-errno -Wl,-u,vsprintf -lm -fdata-sections -ffunction-sections

LFLAGS  = -T$(LINKER_SCRIPT) -$(OPTIMISATION_LEVEL) -nostartfiles -nodefaultlibs \
 $(PROCESSOR_FLAGS) $(INIT_LIBS) --specs=nano.specs $(CFLAG_EXTRAS) -Wl,--gc-sections

CPFLAGS = -Obinary
ODFLAGS = -d 

all: test
board_type1 : test
board_type2 : test
board_type3 : test

objects = $(video_objects) $(local_objects) startup.o system_init.o flash_funs.o 

clean:
	-rm -rf *.o *.elf *.bin *.lst

test: main.elf
	@ echo "...copying"
	$(CP) $(CPFLAGS) main.elf main.bin
	$(OD) $(ODFLAGS) main.elf > main.lst
	$(SIZ) -A main.elf

main.elf: $(objects) 
	@ echo "..linking"
	$(LD) $(LFLAGS) -o main.elf $(objects)

$(video_objects): %.o : video/%.cpp
	$(CC) $(CFLAGS) $< -o $@

$(local_objects): %.o : %.cpp
	$(CC) $(CFLAGS) $< -o $@

system_init.o : $(SYSTEM_INIT)
	$(CC) $(CFLAGS) -o system_init.o $(SYSTEM_INIT)

startup.o: $(STARTUP)
	$(CC) $(CFLAGS) -o startup.o $(STARTUP) 

flash_funs.o : $(FLASH_SRC)
	$(CC) $(CFLAGS) -o flash_funs.o $(FLASH_SRC)

upload : test
	st-flash write main.bin 0x8000000



