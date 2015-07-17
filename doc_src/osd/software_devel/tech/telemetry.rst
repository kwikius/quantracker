`OSD Technical Index`_

---------
Telemetry
---------

The OSD board can transmit or receive data via unused parts of the video signal.

* Transmission_

* Reception_

.. _Transmission:

++++++++++++
Transmission
++++++++++++

..........
tx linking
..........

   To use the telemetry transmitter version of the OSD, link the static library
   ``quantracker/lib/osd/quantracker_air_osd_tx.a``
   into your OSD application

..........
tx include
..........

   ``#include <quantracker/osd/telemetry_transmitter.hpp>``

........
tx Setup
........

   Before starting the Scheduler, call the predefined library function 
   ``create_telemetry_transmitter_task()``,
   to create the  library telemetry task.

......
tx Use
......

   Create your custom definition of the callback ``on_telemetry_transmitted()`` .
   The function will be called by the OSD system when the video telemetry transmit buffer is empty
   and can be reloaded.

   In the ``on_telemetry_transmitted()`` function
 
      *  Call  ``get_telemetry_transmitter_buffer_length()``, to get the maximum amount of data
         that can be sent.

      *  Call ``write_telemetry_data(buffer, len)`` , to transfer the contents of  buffer
         to the low level telemetry buffer for transmission. 
         The minimum of len and maximum allowable length will be sent.

   The ``on_telemetry_transmitted()`` function is in the context of the telemetry_transmitter_task,
   therefore,  to access global data, you should use a mutex.
      
..........
Tx Example
..........

   See the  `osd_telem_tx`_ example in the Quantracker Github repo.
   
.. _Reception:

+++++++++
Reception 
+++++++++

..........
rx Linking
..........

To use the telemetry receiver version of the OSD, link the static library
``quantracker/lib/osd/quantracker_air_osd_rx.a``
into your OSD application.

..........
rx include
..........

   ``#include <quantracker/osd/telemetry_receiver.hpp>``

........
rx Setup
........
 
   Before starting the Scheduler, call the predefined library function
   ``create_telemetry_receiver_task()``
   to create the library telemetry task. 

......
rx Use
......

   Create your custom definition of the ``on_telemetry_received()`` callback to deal with received data.
   This function will be called from within the telemetry_receive_task,
   therefore read and write access to any global data should be done via mutexes

   Within in the ``on_telemetry_received()`` function, use the predefined library functions
   ``get_telemetry_num_bytes()`` to get the size of the receive buffer and
   ``read_telemetry_data(my_buffer, length)`` to read the data from the low level receive buffer

   The ``on_telemetry_received()`` function is in the context of the telemetry_receiver_task,
   therefore, to access global data, you should use a mutex.

.......................
Receive buffer contents
.......................

   Before a new reception begins the low level receive buffer is cleared to all zeros,
   so any unused bytes will be returned  as zero.
   The format of the data is otherwise entirely open.

..........
rx Example
..........

   See the  `osd_telem_rx`_ example in the Quantracker Github repo.

`OSD Technical Index`_

.. _`OSD Technical Index`: index.html
.. _`osd_telem_tx`: https://github.com/kwikius/quantracker/tree/master/examples/osd_telem_tx/
.. _`osd_telem_rx`: https://github.com/kwikius/quantracker/tree/master/examples/osd_telem_rx/



