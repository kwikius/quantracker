`Quan OSD Technical Index`_

----------------------------------
quan::uav::osd:: Drawing API Index
----------------------------------

The drawing api is coded in C++11.
and makes use of the `quan C++ physical quantities`_ library.

* `quan::uav::osd::on_draw`_
   All drawing takes place inside this user defined callback function.
* `draw_task`_
   The on_draw is called in the context of the OSD's library defined FreeRTOS task


`Quan OSD Technical Index`_

.. _`draw_task`: draw_task.html
.. _`quan::uav::osd::on_draw`: on_draw.html
.. _`Quan OSD Technical Index`: index.html
.. _`quan C++ physical quantities`: http://www.zoomworks.org/quan-trunk/quan_matters/doc/quan/fixed_quantity/intro.html