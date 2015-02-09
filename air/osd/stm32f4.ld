
/*
 Copyright (c) 2013 - 2015 Andy Little 

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program. If not, see <http://www.gnu.org/licenses/>
*/

/* Heap size will be set to this if allocing memmory*/
_HeapSize = 0x8000;

/* linker checks that stack is bigger than this */
_MinStackSize = 0x6000; 

MEMORY
{
   rom (rx)        : org = 0x08000000, len = 1024K
   ram (xrw)       : org = 0x10000000, len = 64K
   video_ram (rw)  : org = 0x20000000, len = 124K 
   telem_ram(rw)   : org = 0x2001F000, len = 4k
}

SECTIONS
{

   ENTRY(Reset_Handler)

   .isr_vector :
   {
      . = ALIGN(4);
      KEEP(*(.isr_vector)) 
      . = ALIGN(4);
   }>rom

   .text :
   {
      . = ALIGN(4);
      *(.text) 
      *(.text*) 
      *(.rodata)
      *(.rodata*) 
      *(.glue_7) 
      *(.glue_7t)

      KEEP (*(.init))
      KEEP (*(.fini))

      . = ALIGN(4);
      _etext = .; 
      _exit = .;
   }>rom

    /* initialised data */
   .preinit_array :
   {
      PROVIDE_HIDDEN (__preinit_array_start = .);
      KEEP (*(.preinit_array*))
      PROVIDE_HIDDEN (__preinit_array_end = .);
   }>rom

   .init_array :
   {
      PROVIDE_HIDDEN (__init_array_start = .);
      KEEP (*(SORT(.init_array.*)))
      KEEP (*(.init_array*))
      PROVIDE_HIDDEN (__init_array_end = .);
   }>rom

   .fini_array :
   {
      PROVIDE_HIDDEN (__fini_array_start = .);
      KEEP (*(.fini_array*))
      KEEP (*(SORT(.fini_array.*)))
      PROVIDE_HIDDEN (__fini_array_end = .);
   }>rom

   _sidata = .;
   .data : AT ( _sidata )
   {
      . = ALIGN(4);
      _sdata = .; 
      *(.data) 
      *(.data*) 

      . = ALIGN(4);
      _edata = .; 
   }>ram

   . = ALIGN(4);
   .bss :
   {
      _sbss = .; 
      __bss_start__ = _sbss;
      *(.bss)
      *(.bss*)
      *(COMMON)
      . = ALIGN(4);
      _ebss = .; 
      __bss_end__ = _ebss;
   }>ram

   .check_heap  :
   { 
      . = ALIGN(4);
      _HEAP_START  = .;
      . = . + _HeapSize;
      . = ALIGN(4);
      _HEAP_MAX = .;
   }>ram

   .check_stack  :
   {
      . = ALIGN(4);
      . = . + _MinStackSize;
      . = ALIGN(4);

      _estack = ORIGIN(ram) + LENGTH(ram); 
   }>ram

   .display_buffer (NOLOAD) :
   { 
     KEEP (*(.display_buffer*))
    . = ALIGN(4);
     video_buffer.o (.display_buffer) 
   }> video_ram 

   .telem_buffer (NOLOAD) :
   { 
     KEEP (*(.telem_buffer*))
    . = ALIGN(4);
     video_buffer.o (.telem_buffer)
   }> telem_ram 
   

   /DISCARD/ :
   {
      libc.a(*)
      libm.a(*)
      libgcc.a(*)
   }

}
