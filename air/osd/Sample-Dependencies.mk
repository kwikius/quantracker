# Quantracker OSD Firmware
# Example Dependencies.mk
# Rename this to Dependencies.mk and save in this directory
#(~quantracker/air/osd/)
# Modify the following variables to the paths, etc
# for these libraries.

###############################################
### The arm-gcc compiler
# Download from : https://launchpad.net/gcc-arm-embedded

 TOOLCHAIN_PREFIX =/opt/gcc-arm-none-eabi-4_7-2013q2/

# To find the version of arm-gcc run ./gcc --version 
# from within $(TOOLCHAIN_PREFIX)arm-none-eabi/bin directory

 TOOLCHAIN_GCC_VERSION = 4.7.4

######################################################################
###  the quan library                                          
## Download from : https://github.com/kwikius/quan-trunk 

QUAN_INCLUDE_PATH = /home/andy/website/quan-trunk

#######################################################################
### Mavlink
## Download from : https://github.com/mavlink/mavlink

 MAVLINK_INCLUDE_PATH = /home/andy/website/fpv/mavlink

#######################################################################
###  FreeRTOS                                               
## Download from : http://www.freertos.org/a00104.html

 FREE_RTOS_DIR = /home/andy/cpp/lib/FreeRTOSV8.1.2/FreeRTOS/

################################################################
###  The STM32F4 standard peripherals library               
## Download from : http://www.st.com/st-web-ui/static/active/en/st_prod_software_internet/resource/technical/software/firmware/stm32f4_dsp_stdperiph_lib.zip

 STM32_STD_PERIPH_LIB_DIR = /opt/stm32f4/STM32F4xx_DSP_StdPeriph_Lib_V1.0.0/Libraries/

########################################################################
# Optional. if not set defaults to O. You will need optimisation of at least O

#OPTIMISATION_LEVEL = O3

# Optional defaults to transmitter
# NB telem_none not tested yet
# one of QUAN_OSD_TELEM_RECEIVER QUAN_OSD_TELEM_TRANSMITTER QUAN_OSD_TELEM_NONE

#QUAN_TELEMETRY_DIRECTION = QUAN_OSD_TELEM_TRANSMITTER

##########################################################################
