# Quantracker OSD Firmware
# Example Dependencies.mk
# Rename this to Dependencies.mk and save in this directory
#(~quantracker/air/osd/)
#
# Modify the following variables to the paths, etc
# for these libraries on your system.


#####################################################################
### The arm-gcc compiler
# Download : https://launchpad.net/gcc-arm-embedded/4.9/4.9-2014-q4-major/+download/gcc-arm-none-eabi-4_9-2014q4-20141203-linux.tar.bz2 -O gcc-arm

 TOOLCHAIN_PREFIX =/opt/gcc-arm-none-eabi-4_7-2013q2/

# To find the version of arm-gcc run ./gcc --version 
# from within $(TOOLCHAIN_PREFIX)arm-none-eabi/bin directory

 TOOLCHAIN_GCC_VERSION = 4.7.4

######################################################################
### The quan library                                          
## Download : https://github.com/kwikius/quan-trunk/archive/master.zip 

QUAN_INCLUDE_PATH = /home/andy/website/quan-trunk

######################################################################
### MAVlink
## Download : https://github.com/mavlink/c_library/archive/master.zip

 MAVLINK_INCLUDE_PATH = /home/andy/website/fpv/mavlink

#######################################################################
### FreeRTOS                                               
## Download: http://downloads.sourceforge.net/project/freertos/FreeRTOS/V8.2.0/FreeRTOSV8.2.0.zip

 FREE_RTOS_DIR = /home/andy/cpp/lib/FreeRTOSV8.1.2/FreeRTOS/

#######################################################################
### The STM32F4 standard peripherals library               
## Download : http://www.st.com/st-web-ui/static/active/en/st_prod_software_internet/resource/technical/software/firmware/stm32f4_dsp_stdperiph_lib.zip

 STM32_STD_PERIPH_LIB_DIR = /opt/stm32f4/STM32F4xx_DSP_StdPeriph_Lib_V1.0.0/Libraries/


################################## OTHER OPTIONS ######################

# Optional. if not set defaults to O. You will need optimisation of at least O
#OPTIMISATION_LEVEL = O3

# Optional defaults to transmitter
# NB telem_none not tested yet
# one of QUAN_OSD_TELEM_RECEIVER QUAN_OSD_TELEM_TRANSMITTER QUAN_OSD_TELEM_NONE

#QUAN_TELEMETRY_DIRECTION = QUAN_OSD_TELEM_TRANSMITTER

########################################################################

