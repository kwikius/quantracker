Index_

============================================
Getting started with the Quantracker Air OSD
============================================

----------------
The Board layout
----------------

  .. image:: osd_board.png

The above diagram highlights the basic connections of the board [1]_. 
 
The initial board firmware is designed to be used with Ardupilot and
the layout of the board connectors is designed to be compatible with MinimOSD [2]_ .
The 6 pin header at the right hand end of the diagram is 
used for the MAVLink serial connection. 
The Header at the left end is used for connecting the Camera
and the Video transmitter [3]_ .
The board firmware supports PAL and NTSC cameras

As well as its use as a MAVLink port, the serial port can also be used to flash
firmware onto the board using a stock FTDI module. 

The board is entirely Open Source hardware and software.

`The pcb is currently available at OSHPark`_ . 

We hope soon to try to get some finished boards available too.

`The full schematic is available here as a set of pdfs`_  .

For more info , please look at the `OSD index`_.

.. _`OSD index`: osd/index.html
.. _Index: index.html
.. _`see the mcu schematic`: https://github.com/kwikius/quantracker/blob/master/air/osd/hardware/64_pin_lite/air_osd_v1/schematic_pdf/osd-MCU.pdf?raw=true
.. _`The full schematic is available here as a set of pdfs`: https://github.com/kwikius/quantracker/blob/master/air/osd/hardware/64_pin_lite/air_osd_v1/schematic_pdf
.. _`The pcb is currently available at OSHPark`: https://oshpark.com/shared_projects/KlRxcZaZ
                                               
.. [1] There are many other available pins, which are broken out from
       the STM32F4 processor. For a more complete view of the functions 
       on the board `see the mcu schematic`_. 
       The board has the capability to be a full Flight controller, as well as an OSD)
       With an abundance of open source flight control software , it is hoped to realise
       that shortly.

.. [2] There are minor differences.

.. [3] The output to the Video transmitter also provides 2 buffered Audio channels. 
       Options include sending FSK telemetry data and uses a a Variometer, voice alerts etc.

Index_


