`OSD Index`_

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
   * char[20] 
   * char[30] 
   * char[40] 

   

.. _`OSD Index`: ../index.html
.. [#flash_wear] Excessive Wear on the flash is prevented as follows.
                 2 flash pages are used.
                 When the flash variable value is changed, a new version of the variable is
                 appended to flash page (if there is room) until the whole page is used up.
                 Once the page is filled the flash variables are all copied to the other page 
                 and the cycle repeats.
.. [#TBA] null terminatd strings To be added.

`OSD Index`_
