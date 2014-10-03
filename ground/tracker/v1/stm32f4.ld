
/*
 Copyright (c) 2013 Andy Little 

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

/* Heap size will be set to this if allocing memory*/
_HeapSize = 0x1000;

/* linker checks that stack is bigger than this */
_MinStackSize = 0x2000; 

MEMORY
{
   start_vector (rx) : org = 0x08000000, len = 16K
   flash(a)          : org = 0x08004000, len = 32K 
   rom (rx)          : org = 0x0800C000, len = 976K
   ram (xrw)         : org = 0x10000000, len = 64K
   /* ram1 (xrw)     : org = 0x20000000, len = 128K */
}

SECTIONS
{

   ENTRY(Reset_Handler)

   .isr_vector :
   {
      . = ALIGN(4);
      KEEP(*(.isr_vector)) 
      . = ALIGN(4);
   }>start_vector

    /* initialised data */
   .preinit_array :
   {
      PROVIDE_HIDDEN (__preinit_array_start = .);
      KEEP (*(.preinit_array*))
      PROVIDE_HIDDEN (__preinit_array_end = .);
   }>start_vector

   .init_array :
   {
      PROVIDE_HIDDEN (__init_array_start = .);
      KEEP (*(SORT(.init_array.*)))
      KEEP (*(.init_array*))
      PROVIDE_HIDDEN (__init_array_end = .);
   }>start_vector

   .fini_array :
   {
      PROVIDE_HIDDEN (__fini_array_start = .);
      KEEP (*(.fini_array*))
      KEEP (*(SORT(.fini_array.*)))
      PROVIDE_HIDDEN (__fini_array_end = .);
   }>start_vector

    .flash_memory (NOLOAD) :
   {
     
      . = ALIGN(16);
      KEEP(*(.flash_memory))
       FILL(0xFFFF);
      _FLASH_PAGE1 = .;
      . = . + 16K;
       . = ALIGN(16);
      _FLASH_PAGE2 = .;
      . = . + 16K;
   }>flash =0xffff

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

   .check_heap (NOLOAD) :
   { 
      . = ALIGN(4);
      _HEAP_START  = .;
      . = . + _HeapSize;
      . = ALIGN(4);
      _HEAP_MAX = .;
   }>ram

   .check_stack (NOLOAD) :
   {
      . = ALIGN(4);
      . = . + _MinStackSize;
      . = ALIGN(4);

      _estack = ORIGIN(ram) + LENGTH(ram); 
   }>ram
  
   /DISCARD/ :
   {
      libc.a(*)
      libm.a(*)
      libgcc.a(*)
   }

}
