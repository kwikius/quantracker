
#required for Ubuntu 12.x placid as system headers have been put in starnge places
# these have beeen defined to thos in my bash .profile
CPLUS_INCLUDE_PATH=
C_INCLUDE_PATH=
LIBRARY_PATH=
#TOOLCHAIN_PREFIX = /home/andy/arm/arm-cortex-m4-hardfloat-toolchain/bin/
TOOLCHAIN_PREFIX =/opt/gcc-arm-none-eabi-4_7-2013q2/
TOOLCHAIN_GCC_VERSION = 4.7.4

CC      = $(TOOLCHAIN_PREFIX)bin/arm-none-eabi-g++
CC1     = $(TOOLCHAIN_PREFIX)bin/arm-none-eabi-gcc
LD      = $(TOOLCHAIN_PREFIX)bin/arm-none-eabi-g++
CP      = $(TOOLCHAIN_PREFIX)bin/arm-none-eabi-objcopy
OD      = $(TOOLCHAIN_PREFIX)bin/arm-none-eabi-objdump
  
INCLUDES = /opt/stm32f0/STM32F0xx_StdPeriph_Lib_V1.1.0/Libraries/CMSIS/Include \
/opt/stm32f0/STM32F0xx_StdPeriph_Lib_V1.1.0/Libraries/CMSIS/Device/ST/STM32F0xx/Include \
/home/andy/website/quan-trunk \
/home/andy/website/fpv/mavlink

#/home/andy/website/fpv/mavlink/include/mavlink/v1.0 \
#/home/andy/website/fpv/mavlink/include/mavlink/v1.0/ardupilotmega

INIT_LIB_PREFIX = $(TOOLCHAIN_PREFIX)/lib/gcc/arm-none-eabi/$(TOOLCHAIN_GCC_VERSION)/armv6-m/

DEFINES = QUAN_STM32F0 QUAN_NO_EXCEPTIONS QUAN_STM32_SUBFAMILY_05X HSE_VALUE=8000000
STARTUP = startupf0.s
LINKER_SCRIPT = stm32f0.ld
SYSTEM_INIT = system_initf0
PROCESSOR_FLAGS = -mcpu=cortex-m0 -mthumb -mfloat-abi=soft

INIT_LIBS = $(INIT_LIB_PREFIX)crti.o $(INIT_LIB_PREFIX)crtn.o

INCLUDE_ARGS = $(patsubst %,-I%,$(INCLUDES))

DEFINE_ARGS = $(patsubst %,-D%,$(DEFINES))

CFLAG_EXTRAS = -fno-math-errno -Os
# for float printf format etc
# CFLAG_EXTRAS += -Wl,-u,vsprintf -lm

CFLAGS  = -std=c++11 -fno-rtti -fno-exceptions -c -g $(DEFINE_ARGS) $(INCLUDE_ARGS) \
  $(PROCESSOR_FLAGS) $(CFLAG_EXTRAS)

LFLAGS  = -T$(LINKER_SCRIPT)  -nostartfiles -nodefaultlibs $(PROCESSOR_FLAGS) $(INIT_LIBS) \
   --specs=nano.specs $(CFLAG_EXTRAS)

#LFLAGS  =  -v
CPFLAGS = -Obinary
#ODFLAGS = -D
ODFLAGS = -d
# to see source in listing. However with optimisation doesnt work too well
#ODFLAGS += -S

all: test

objects = main.o setup.o spbrk.o $(SYSTEM_INIT).o serial_port.o dac.o \
frsky.o frsky_zapp1.o aircraft.o mavlink.o timer.o gps.o on_gps.o settings.o

clean:
	-rm -rf *.o *.elf *.bin *.lst

test: main.elf
	@ echo "...copying"
	$(CP) $(CPFLAGS) main.elf main.bin
	$(OD) $(ODFLAGS) main.elf > main.lst

main.elf: startup.o $(objects) 
	@ echo "..linking"
	$(LD) $(LFLAGS) -o main.elf $(objects) startup.o

$(objects): %.o : %.cpp
	$(CC) $(CFLAGS) $< -o $@

startup.o: $(STARTUP)
	$(CC) $(CFLAGS) -o startup.o $(STARTUP) 

upload : test
	st-flash write main.bin 0x8000000
