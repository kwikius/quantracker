`Quantracker OSD Index`_

-----------
Hello World 
-----------

..........................................
A minimal Quantracker Air OSD  Application
..........................................

::

   #include <quantracker/osd/osd.hpp>
   #include "fonts.hpp"

   int main()
   {
      osd_setup();           
      create_draw_task();    
      vTaskStartScheduler(); 
   }

   void quan::uav::osd::on_draw()
   {
      draw_text("Hello World",{-60,0});
   }


This is a complete but very simple Quantracker OSD application. It just overlays "Hello World" on the 
display. It can be used to test that your toolchain is working and as a template for
more sophisticated applications.

see the `source code`_ on Github

`Quantracker OSD Index`_


.. _`source code`: https://github.com/kwikius/quantracker/blob/master/examples/hello_world/

.. _`Quantracker OSD Index`: ../index.html
