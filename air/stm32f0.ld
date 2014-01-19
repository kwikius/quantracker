
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

/* Heap size will be set to this if allocing memmory*/
_HeapSize = 0x0;

/* linker checks that stack is bigger than this */
_MinStackSize = 0x200; 

/*STM32F051K8XXX  */
MEMORY
{
   rom (rx)             : org = 0x08000000, len = 64K
   processor_options(r) : org = 0x1FFFF800, len = 12
   ram (xrw)            : org = 0x20000000, len = 8K
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

   .check_heap :
   { 
      . = ALIGN(4);
      _HEAP_START  = .;
      . = . + _HeapSize;
      . = ALIGN(4);
      _HEAP_MAX = .;
   }>ram

   .check_stack :
   {
      . = ALIGN(4);
      . = . + _MinStackSize;
      . = ALIGN(4);

      _estack = ORIGIN(ram) + LENGTH(ram); 
   }>ram

   processor_options1 0x1FFFF800 :
   {
      user_options1 = .;
   }>processor_options
   processor_options2 0x1FFFF804 :
   {
      user_options2 = .;
   }>processor_options
   processor_options3 0x1FFFF808 :
   {
      user_options3 = .;
   }>processor_options

   /DISCARD/ :
   {
      libc.a(*)
      libm.a(*)
      libgcc.a(*)
   }

}
