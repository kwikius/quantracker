
`Quantracker Air OSD drawing api Index`_

==============================
void quan::uav::osd::on_draw()
==============================

This user supplied function is called by the `library supplied draw_task`_ to refresh the
low level osd drawing overlay bitmap buffer.

Use the drawing api functions to write the OSD overlay on the display buffer.

The on_draw function is called in the context of the `draw_task`_

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


`Quantracker Air OSD drawing api Index`_

.. _`draw_task`: draw_task.html
.. _`Quantracker Air OSD drawing api Index`: drawing_api.html
.. _`#include \<quan/uav/osd/api.hpp\>`: https://github.com/kwikius/quan-trunk/blob/master/quan/uav/osd/api.hpp
.. _`library supplied draw_task`: https://github.com/kwikius/quantracker/blob/master/air/osd/video/draw_task.cpp