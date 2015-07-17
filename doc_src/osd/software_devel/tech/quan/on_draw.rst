
`Quantracker Air OSD drawing api Index`_

==============================
void quan::uav::osd::on_draw()
==============================

This user supplied function is called by the osd driver,
to update the osd drawing overlay bitmap. 

The on_draw function is called entirely in the context of the `draw_task`_ . The only
consequence of this is that care must be taken when sharing data between tasks.

The display uses 2 overlay buffers.
When the on_draw function has returned the draw_task requests the low level
system to swap the buffers. The low level system takes the newly drawn
buffer to display and gives the draw_task back the other buffer to update.

Use the `drawing api`_ functions to write the OSD overlay on the display buffer.  

There are no constraints on the amount of time the on_draw function takes, except that
until the buffer is updated, the driver will continue to display the previous buffer.

.......
include
.......

   `#include \<quan/uav/osd/api.hpp\>`_

....
link
....

one of 

* quantracker/lib/osd/quantracker_air_osd.a
* quantracker/lib/osd/quantracker_air_osd_tx.a
* quantracker/lib/osd/quantracker_air_osd_rx.a

......
useage
......

::

   // my_on_draw.cpp 

   #include <quan/uav/osd/api.hpp>

   void quan::uav::osd::on_draw()
   {
      // user supplied drawing code
   }

.......
example
.......
      
   `~/quantracker/examples/hello_world/main.cpp`_


`Quantracker Air OSD drawing api Index`_

.. _`draw_task`: draw_task.html
.. _`drawing api`:
.. _`Quantracker Air OSD drawing api Index`: drawing_api.html
.. _`#include \<quan/uav/osd/api.hpp\>`: https://github.com/kwikius/quan-trunk/blob/master/quan/uav/osd/api.hpp
.. _`library supplied draw_task`: https://github.com/kwikius/quantracker/blob/master/air/osd/video/draw_task.cpp
.. _`~/quantracker/examples/hello_world/main.cpp` :
   https://github.com/kwikius/quantracker/blob/master/examples/hello_world/main.cpp