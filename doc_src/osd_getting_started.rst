Index_

============================================
Getting started with the Quantracker Air OSD
============================================

----------------
The Board layout
----------------

  .. image:: osd_board.png

The above diagram highlights the basic connections of the board [1]_. 

The Board is designed to be used with Ardupilot.
The board is designed to be hardware compatible with MinimOSD [2]_ ,
and so the layout should be familiar if you have used MinimOSD. 
The 6 pin header at the right hand end of the diagram is 
used for the MAVLink serial connection. 
The Header at the left end is used for connecting the Camera
and the Video transmitter [3]_ .

As well as its use as a MAVLink port, the serial port is also used to flash
firmware onto the board using a stock FTDI module. 

Index_

.. _Index: index.html
.. _`see the mcu schematic`: htpps://github.com/kwikius/blob/master/quantracker/air/osd/hardware/64_pin_lite/schematic_pdf/osd-MCY.pdf?raw=true


.. [1] There are many other available pins, which are broken out from
       the STM32F4 processor. For a more complete view of the functions 
       on the board `see the mcu schematic`_. 
       The board has the capability to be a full Flight controller, as well as an OSD)
       With an abundance of open source flight control software , it is hoped to realise
       that shortly.

.. [2] There are minor differences.

.. [3] The output to the Video transmitter also provides 2 buffered Audio channels. 
       Options include sending FSK telemetry data and uses a a Variometer, voice alerts etc.


