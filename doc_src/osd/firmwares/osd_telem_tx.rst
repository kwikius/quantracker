`OSD Index`_

---------------------
osd_telem_tx firmware
---------------------

osd_telem_tx:
   This OSD firmware is designed to demonstrate the use of the video signal to transmit data.
   You will need 2 OSD boards. The second OSD board should run the `osd_telem_rx firmware`_ . For testing
   you can connect the transmitter board video out to the receiver board video in.
   The display simply shows a timer. 

   You can customise what is sent. 
   See the ``on_telemetry_transmitted()`` function defined in 
   `quantracker/examples/osd_telem_tx/tx_telemetry.cpp`_ for details.
   `More technical info about the telemetry here`_

   `Download osd_telem_tx firmware`_

.. _`Download osd_telem_tx firmware`: https://github.com/kwikius/quantracker/blob/master/examples/osd_telem_tx/bin/main.bin?raw=true
.. _`OSD Index`: ../index.html
.. _`osd_telem_rx firmware`: osd_telem_rx.html
.. _`More technical info about the telemetry here`: ../software_devel/tech/telemetry.html
.. _`quantracker/examples/osd_telem_tx/tx_telemetry.cpp`: https://github.com/kwikius/quantracker/blob/master/examples/osd_telem_tx/tx_telemetry.cpp

`OSD Index`_

