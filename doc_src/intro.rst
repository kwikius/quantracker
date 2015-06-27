`Quantracker Index`_

=============================================== 
Introducing Quantracker Antenna Tracker and OSD
===============================================


   * `The Antenna Tracker`_
         + `More on the Antenna Tracker...`_

   * `The OSD`_
         + `More on the OSD...`_



.. * _`The Antenna Tracker`


.............................
The Antenna Tracker
.............................

   Cost effective
         The price of the Tracker should be under £100 ($150). The price of the original
         was around £120 ($180). The next iteration should save over that.

   Compact and Lightweight.
      The tracker should be small, capable of fitting in a backpack neatly
      and should be quick and easy to set up at the flying site

   Simple and reliable
      The tracker should be simple to build and use and reliable in use.

   Performance
      From the outset, the tracker has been designed with performance in mind.
      The tracker can rotate continuously in Pan, unlike some servo based trackers.

   Open source hardware and software
      `The source code is available from Github`_

   Optional ground side OSD
      As time goes on and with the high bandwidth data from the video telemetry
      It should be possible to identify a point in space through the OSD so allowing
      , for example, the position of other aircraft to be displayed.

   Continuous rotation in Pan
       The tracker uses sliprings allowing continuos rotation in pan

   Various ways to send telemetry including but not limited to
         
         + Via the video feed itself ( like teletext).
         + Via one or both of the video audio channels.
         + Via an FrSky telemetry receiver.
         + A combination of any or all of the above.

.. raw:: html

   <table><tr>
   <td>
   <iframe width="420" height="315" src="https://www.youtube.com/embed/uj_cmHMo7XM" 
   frameborder="0" allowfullscreen></iframe>
   </td>
   <td>
   <iframe width="420" height="315" src="https://www.youtube.com/embed/85SXBYkTdV0" 
   frameborder="0" allowfullscreen></iframe>
   </td>
   </tr></table>

.. `The OSD`_

.......
The OSD
.......

The On Screen Display (OSD) grew out of the inital project. 
It turns out that the video signal is capable of transmitting
and receiving at a good data rate. As the design of the airborne
transmitter continued, it was decided to make the
airborne unit a full OSD, for which the ecosystem is now well underway.


   Features
      * Open source software and hardware 
         +  `The OSD V2.1 PCB is available from OSHPark`_
         +  `The OSD source code is available from Github`_

      * Compact.
      * Freestanding - can be used with or without the tracker.
      * Works with Ardupilot, Compatible with MinimOSD. 
      * supports PAL and NTSC cameras (can switch between protocols dynamically).
      * Vector graphics.
      * Hugely customisable display, using "osd_maker" App for helping to create the display.
      * Digitally isolated Serial port to help eliminate ground loops.
      * Very versatile Expansion port with I2C, SPI etc, means the OSD can be upgraded to a full FC

   Here are some videos of the OSD in action:

.. raw:: html

   <table><tr>
   <td>
   <iframe width="420" height="315" src="https://www.youtube.com/embed/cclQAswRa8s" 
   frameborder="0" allowfullscreen></iframe>
   </td>
   <td>
   <iframe width="420" height="315" src="https://www.youtube.com/embed/Czrd15weBXM" 
   frameborder="0" allowfullscreen></iframe>
   </td>
   </tr></table>

`Quantracker Index`_

.. _`The OSD V2.1 PCB is available from OSHPark`: https://oshpark.com/shared_projects/yiU4RsLa
.. _`The source code is available from Github`: https://github.com/kwikius/quantracker/tree/master
.. _`The OSD source code is available from Github`: https://github.com/kwikius/quantracker/tree/master/air/osd
.. _`Quantracker Index`: index.html
.. _`More on the Antenna Tracker...`: ../../antenna_tracker/index.html
.. _`More on the OSD...` : osd_getting_started.html
