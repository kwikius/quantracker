`OSD Technical Index`_

---------------
Flash Variables
---------------
The OSD has a mechanism for persistent storage of variables in Flash memory.
The stm32f4 mpu doesnt have EEPROM memory but it is possible to write to Flash [#flash_wear]_ .

Each flash variable must have a unique name.
and each Flash variable must have a type. Currently available types are:

   * bool
   * int8_t
   * int16_t
   * int32_t
   * uint8_t
   * uint16_t
   * uint32_t
   * float
   * three_d::vect<int32_t>
   * three_d::vect<float>
   * char[10] [#TBA]_
   * char[20] [#TBA]_
   * char[30] [#TBA]_
   * char[40] [#TBA]_

The system is designed so you can write a custom set of flash variables for your own display. 
It is suggested that you use `quantracker/examples/osd_example1/common/flash.cpp`_ as a template.
The flash symbol table named flash_variable_symtab is defined around 2/3 way down the page.
The EE_SYMTAB_ENTRY macro can be used to ease defining entries.
The symtab_entry_t structure is defined in the quan::stm32::flash namespace `\<quan/stm32/flash.hpp\>`_


::

   struct symtab_entry_t {
      const char* const name;  // user name of the symbol
      uint32_t const type_tag;  // id representing the type of the variable
      bool (*pfn_validity_check)(void*); //range check function 
      const char * const info; // some help about the flash variable
      bool readonly;  // if the value is readonly ( not writable from user flash menu)
   };


Fields:
   * name
         The name of each variable should be unique within 
         the flash variable namespace.

   * type_tag 
         To find the type tag, it is suggested to use the EE_SYMTAB_ENTRY macro. 
         In order for this to work a structure has been defined around 1/3 down the page in `flash.cpp`_ 
         called flash_variable_type, whose sole purpose is to provide a typedef of the flash variable 
         in form of its name for the EE_SYMTAB_ENTRY.

   * pfn_validity_check
         The range check function, pfn_validity_check, takes a void \* argument as input. The void \* should
         be converted to a pointer to the type of the variable to be checked. It isnt const so you can
         modify the original. All the usual caveats when using void * apply!
         In `flash.cpp`_ there is a function called nop_check, which does nothing and 
         can be used if you dont need range checking.

   * info
         Holds a small bit of helpful info about the variable which the user can read via the flash-menu.

   * readonly
         Set this true so that the variable can be read but not modified from the flash-menu. You can
         of course modify it in code, using the set_flash_value(name, value) functions 
         defined in `\<quan/stm32/flash/flash_convert.hpp\>`_  [#stw]_

-------------------------------------
viewing and modifying flash variables
-------------------------------------

      The flash variables can be viewed and modified by the flash menu or programmatically in code.
      Not that modifying flash while the OSD is running (ie once the scheduler has started)
      is a "stop the world" operation [#stw]_
      Though the actual write of a location is quite quick around 20 usec,
      it may very occasionally require erasing a page and
      copying all variables to another page which may take considerably longer. 
      I hope to try to implement it at some point but it may make the screen flicker.
      I hope also to provide facilities to access external memories as well.

      See also `accessing flash variables via the flash menu`_
      
.. _`accessing flash variables via the flash menu`: ../flash_menu.html
.. _`OSD Technical Index`: tech/index.html
.. _`\<quan/stm32/flash.hpp\>`: https://github.com/kwikius/quan-trunk/blob/master/quan/stm32/flash.hpp
.. _`\<quan/stm32/flash/flash_convert.hpp\>`: https://github.com/kwikius/quan-trunk/blob/master/quan/stm32/flash/flash_convert.hpp
.. _`quantracker/examples/osd_example1/common/flash.cpp`: https://github.com/kwikius/quantracker/blob/master/examples/osd_example1/common/flash.cpp
.. _`flash.cpp`: https://github.com/kwikius/quantracker/blob/master/examples/osd_example1/common/flash.cpp
.. [#flash_wear] Excessive Wear on the flash is prevented as follows.
                 2 flash pages are used.
                 When the flash variable value is changed, a new version of the variable is
                 appended to flash page (if there is room) until the whole page is used up.
                 To read the algorithm cycles through to find the latests version of the variable written.
                 Once the page is filled the flash variables are all copied to the other page 
                 and the cycle repeats.
.. [#TBA] null terminatd strings To be added.
.. [#stw] If the scheduler is running you need to suspend the OSD . TODO how to suspend the OSD

`OSD Technical Index`_
