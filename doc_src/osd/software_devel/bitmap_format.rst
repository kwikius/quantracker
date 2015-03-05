`OSD Index`_

--------------------
Bitmap source format
--------------------

.....................
Structure of a bitmap
.....................
To create a custom bitmap, derive your custom class from the basic_bitmap base class 
defined in `\<quan/uav/osd/basic_bitmap.hpp\>`_ . 

It will probably help to also look at the examples 
in `quantracker/examples/osd_example1/board/bitmaps.cpp`_.

The bitmap structure itself is relatively simple. 
The get_data and get_size functions implement the virtual functions
in the basic_bitmap base class.  The array is declared constexpr as a hint to the compiler 
to put it in in rom.

The format of the data array
----------------------------

The array is declared as an array of uint8_t, 
but should be seen as a linear array of 2 bit values, each representing a pixel.
The 2 bit values map to colours as follows (Also see `\<quan/uav/osd/colour.hpp\>`_ )

   * 0b00 = grey
   * 0b01 = white
   * 0b10 = black
   * 0b11 = transparent

The array is split into rows based on what the get_size function you provided returns.

...................
Joining it together
...................

To see how to join it up, look in  `quantracker/examples/osd_example1/board/bitmaps.cpp`_ , 
which has all the bitmaps for the example.

First look at the end of bitmaps.cpp where the get_bitmap(uint32_t id) function is defined.
As you can see, the function looks up the particular bitmap up in the bitmap_array defined above.
Note also that all bitmaps are declared in an anonymous namespace [1]_ .

In the example ,all the available bitmaps are defined in this file, which gets unwieldy.
If you want to put your bitmaps in different source files,
you can of course just #include them inline inside the anonymous namespace in this file, 
which gives the same effect but is easier to work with.

...............
Testing bitmaps
...............

For a quick test of your own bitmaps , have a look at the example in
`devel : quantracker/examples/cmd_line_output/main.cpp`_ , which outputs an ascii-art bitmap on the commandline. 
The default ouput looks like so:

::

   ~~     ~~~~~~~~
   ~  ###  ~~~  ~~
     #   ## ~ ## ~
    ##     #  ## ~
    ##      ##  ~~
    ##      ##  ~~
    ###    #  #  ~
     ###  #    # ~
   ~ ####      #  
   ~~ ####      # 
   ~~~ ####     # 
   ~~~~ ####    # 
   ~~~~ ########  
   ~~~~ ## ####  ~
   ~~~  ##      ~~
   ~~  #### ~~~~~~
      ######   ~~~
    ########## ~~~
               ~~~


.. _`\<quan/uav/osd/basic_bitmap.hpp\>`: https://github.com/kwikius/quan-trunk/blob/master/quan/uav/osd/basic_bitmap.hpp
.. _`quantracker/examples/osd_example1/board/bitmaps.cpp`: https://github.com/kwikius/quantracker/blob/master/examples/osd_example1/board/bitmaps.cpp
.. _`\<quan/uav/osd/colour.hpp\>` : https://github.com/kwikius/quan-trunk/blob/master/quan/uav/osd/colour.hpp
.. _`devel : quantracker/examples/cmd_line_output/main.cpp`: https://github.com/kwikius/quantracker/blob/devel/examples/cmd_line_output/main.cpp
.. _`OSD Index`: ../index.html

.. [1] Putting things in an anonymous namespace in a C++ source file is similar to one useage of static in C. 
         You can use static too, but an anonymous namespace can be neater.

`OSD Index`_
