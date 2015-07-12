`OSD Index`_

---------------------
osd_telem_rx firmware
---------------------

osd_telem_rx:
   This OSD firmware is designed to demonstrate the use of the video signal to transmit data.
   You will need 2 boards. The second board should run the `osd_telem_tx firmware`_. For testing
   you can connect the transmitter OSD board video out to the receiver OSD board video in.
   The display simply shows a timer. You can customise what is shown on the osd
   See the on_telemetry_received() function defined in 
   `quantracker/examples/osd_telem_rx/rx_telemetry.cpp`_ for details.
   There is `more technical info about the telemetry here`_

   `Download osd_telem_rx firmware`_

.. _`Download osd_telem_rx firmware`: https://github.com/kwikius/quantracker/blob/master/examples/osd_telem_rx/bin/main.bin?raw=true
.. _`OSD Index`: ../index.html
.. _`osd_telem_tx firmware`: osd_telem_tx.html
.. _`more technical info about the telemetry here`: ../software_devel/tech/telemtry.html
.. _`quantracker/examples/osd_telem_rx/rx_telemetry.cpp`:
   https://github.com/kwikius/quantracker/blob/master/examples/osd_telem_rx/rx_telemetry.cpp

`OSD Index`_

