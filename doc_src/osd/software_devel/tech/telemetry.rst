`OSD Technical Index`_

---------
Telemetry
---------

The OSD board can transmit or receive data via unused parts of the video signal.

---------
Reception 
---------

.......
Linking
.......

To use the telemetry receiver version of the OSD link the static library ::

   quantracker/lib/osd/quantracker_air_osd_rx.a 

into your OSD application

.....
Setup
.....

In main() 
   Call the predefined library function create_telemetry_receiver_task() 
   to create the  library telemetry task. 
   **create_telemetry_receiver_task() must be called before starting the scheduler.**

...
Use
...

In source code::

   #include  <quantracker/include/osd/receive_telemetry.hpp>

Create your custom definition of the on_telemetry_received() callback to deal with received data.
This function will be called from within the telemetry_receive_task,
therefore read and write access to any global data should be done via mutexes

Within in the on_telemetry_receive() function, use the predefined library functions

::
 
   //  get the size of the receive buffer. 
   size_t get_telemetry_num_bytes(); 

   // Read the minimum of len or the total size of the receive buffer
   // to my_buffer and returns the number of bytes actually read.
   size_t read_telemetry_data(char * my_buffer, size_t len);

.......................
Receive buffer contents
.......................

Before a new reception begins the low level receive buffer is cleared to all zeros,
so any unused bytes will be returned  as zero.
The format of the data is otherwise entirely open.

`OSD Technical Index`_

.. _`OSD Technical Index`: index.html



