
`OSD Index`_

============================================
Getting started with the Quantracker Air OSD
============================================

----------------
The Board layout
----------------

The board is entirely Open Source hardware and software.
`The V2.2 pcb is currently available at OSHPark`_ .
`The full schematic is available here as a set of pdfs`_ .
`The source code is available on GitHub`_ .

  .. image:: V2_1_osd_board.png

The above diagram highlights the connections of the board [1]_. 
 
The initial `board firmwares`_ are designed to be used with Ardupilot and
the layout of the board connectors is designed to be compatible with MinimOSD [2]_ .
The 6 pin header at the right hand end of the diagram is 
used for the MAVLink serial connection, for communicating setup parameters and to flash
firmware onto the board. 
The Header at the left end is used for connecting the Camera
and the Video transmitter [3]_ .  Power to the video transmitter and camera is provided at the breakout pins
labelled VIDI+IN and VIDO+In respectively. (There is a schottky diode on each from the input to the output, so
expect around 0.5 V voltage drop ).

The 2 pin header labelled prog is used to put the board in programming mode.

The board firmware automatically detects PAL and NTSC cameras, 
or the board will output white symbology on a black background if no camera signal is present.

For more info , please look at the `OSD index`_.

.. _`OSD Index`: osd/index.html
.. _Index: index.html
.. _`see the mcu schematic`: https://github.com/kwikius/quantracker/blob/master/air/osd/hardware/64_pin_lite/air_osd_v1/schematic_pdf/osd-MCU.pdf?raw=true
.. _`The full schematic is available here as a set of pdfs`: https://github.com/kwikius/quantracker/blob/master/air/osd/hardware/64_pin_lite/air_osd_v2_1/schematic_pdf
.. _`The V2.2 pcb is currently available at OSHPark`: https://oshpark.com/shared_projects/PtiEsgAu
.. _`The source code is available on GitHub`: https://github.com/kwikius/quantracker
.. _`board firmwares`: osd/firmwares.html
.. _`The board has the capability to be a full Flight controller with onboard OSD`: https://www.youtube.com/watch?v=KQLArFbRjWg
                                               
.. [1] There are many other available pins, which are broken out from
       the STM32F4 processor. For a more complete view of the functions 
       on the board `see the mcu schematic`_. 
       (`The board has the capability to be a full Flight controller with onboard OSD`_ ).

.. [2] There are minor differences.

.. [3] The output to the Video transmitter also provides 2 buffered Audio channels. 
       Options include sending FSK telemetry data, audio variometer, voice alerts etc.

`OSD Index`_



