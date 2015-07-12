
`drawing api`_

==============================
void quan::uav::osd::on_draw()
==============================

This user supplied function is called by the `library supplied draw_task`_ to refresh the
low level osd drawing overlay bitmap buffer.

When the on_draw function has exited,
the draw_task sends a message to the osd driver to swap buffers and then blocks
until the osd driver returns a message confirming that the buffers have been swapped.
There are no constraints on the amount of time the function takes, except that
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


`drawing api`_

.. _`drawing api`: drawing_api.html
.. _`#include \<quan/uav/osd/api.hpp\>`: https://github.com/kwikius/quan-trunk/blob/master/quan/uav/osd/api.hpp
.. _`library supplied draw_task`: https://github.com/kwikius/quantracker/blob/master/air/osd/video/draw_task.cpp