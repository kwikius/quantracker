`OSD Index`_

------------------------
The Dependencies.mk file
------------------------

Dependencies.mk is a makefile in the quantracker root directory
whose sole purpose is to let the quantracker makefiles 
know where to find the quantracker `dependent libraries`_ ( in other words the software 
libraries that quantracker depends on to build) see `toolchain.html`_ for details
about where to get these libraries. [#installer]_ 
You can also save the template root directory file "Sample-Dependencies.mk" 
as Dependencies.mk and edit the contents.

The following variables need to be defined. [#trailing]_ 

TOOLCHAIN_PREFIX 
   The top-level directory to the arm-gcc compiler toolchain. e.g if your toolchain is in
   /home/kat/armgccv1 it would  be defined as TOOLCHAIN_PREFIX= /home/kat/armgccv1/

   
TOOLCHAIN_GCC_VERSION 
  The version of arm-gcc. The gcc version must be equal to or greater than 4.7, as the quantracker library
  makes much use of C++11 features. You can find your arm-gcc version by
  navigating at the command line to the top-level directory of the arm-gcc toolchain. 
  From there go to the bin subdirectory and invoke  './arm-none-eabi-gcc --version'
  The output should be something like :
   
  :: 

     arm-none-eabi-gcc (GNU Tools for ARM Embedded Processors) 4.9.3 20141119 (release) [ARM/embedded-4_9-branch revision 218278]
     Copyright (C) 2014 Free Software Foundation, Inc.
     This is free software; see the source for copying conditions.  There is NO
     warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

  In the above case the version is 4.9.3

STM32_STD_PERIPH_LIB_DIR 
   The path to the STM32 std peripherals library

FREE_RTOS_DIR
   The path to the FreeRTOS library

QUAN_INCLUDE_PATH
   The path to the quan library (github/quan-trunk)

MAVLINK_INCLUDE_PATH 
   The path to the mavlink library

STM32FLASH
   The path to invoke the stm32flash executable

.. _`OSD Index`: ../index.html
.. _`dependent libraries`: toolchain.html
.. _`toolchain.html`: toolchain.html

.. [#trailing] N.B. Dont forget the trailing '/'

.. [#installer] If you use the installer script it will create and install a Dependencies.mk file automatically
   
`OSD Index`_