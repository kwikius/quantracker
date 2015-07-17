
`Quantracker Air OSD drawing api Index`_

--------------------------------
The Quantracker Air OS draw_task
--------------------------------

The draw_task manages calling the user supplied 

....
link
....

one of 

* quantracker/lib/osd/quantracker_air_osd.a
* quantracker/lib/osd/quantracker_air_osd_tx.a
* quantracker/lib/osd/quantracker_air_osd_rx.a

.......
include
.......

   `<quantracker/osd/osd.hpp>`_

Use:

   call ``osd_setup()`` in your application setup function

   call ``create_draw_task``  before starting the scheduler

Example:

   `~/quantracker/examples/hello_world/main.cpp`_


`Quantracker Air OSD drawing api Index`_

.. _`<quantracker/osd/osd.hpp>` : 
   https://github.com/kwikius/quantracker/blob/master/include/quantracker/osd/osd.hpp
.. _`Quantracker Air OSD drawing api Index`: drawing_api.html
.. _`~/quantracker/examples/hello_world/main.cpp` :
   https://github.com/kwikius/quantracker/blob/master/examples/hello_world/main.cpp