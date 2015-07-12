
`OSD Index`_

------------------------------
Quantracker Air OSD Flash Menu
------------------------------

For the `osd_example1`_ and `osd_ac`_ firmwares, you can access the flash menu as follows. [#no_con_change]_

Connect a usb serial connector to the board. 
(A 3V or 5V Arduino serial connector will work fine)

  .. image:: ../V2_1_mavlink_sp_pinout.png


Start a serial port console at 9600 baud with 8 bits, no parity and 1 stop bit.
Note that power must be supplied to the port which is electrically isolated
from the rest of the OSD. You can however use any supply from 3 to 5V . The supply is 
usually present on an FTDI to USB cable. 
When power is supplied a red LED will be lit close by the port.

Next supply power to the OSD. It is easiest if you use a switch inline in the power
supply rather than fumbling with connectors
When you switch on the OSD, The Serial console should present you with something similar to:

::
  
   Press return 3 times for Flash Menu


Then after a few seconds

::

   Time is up! ... Exiting to Flight mode

However if you press return 3 times immediately ( up to 3 seconds) after startup you should be
presented with:

::

   Quantracker Air OSD 2.1
   ~~~~Flash menu mode~~~~

You are now in Flash menu mode.

The various functions are available as commands.

   * help
      Displays a list of the available commands with options

   * info
      Displays some useful info. For info about a particular variable type the name
      of the variable after the command

   * show
      Displays the current values of the variables.
      For value of a particular variable type the name
      of the variable after the command. Varaiables not yet defined are shown as '#undef#'
      In case of undefined variables default values are used in the osd.

   * set
      Set variable to value. Requires the name of the variable and a value in correct format.

   * exit
      Exit the flash menu mode and start showing the OSD.


To set the values of flash variables all at once go you can also use the app in 
      `quantracker/air/osd/utilities/flash_vars_upload/`_ . See the readme there for details.

.. _`quantracker/air/osd/utilities/flash_vars_upload/`: https://github.com/kwikius/quantracker/tree/devel/air/osd/utilities/flash_vars_upload
.. _`OSD Index`: index.html
.. _`osd_example1` : firmwares/osd_example1.html
.. _`osd_ac` : firmwares/osd_example1.html

.. [#no_con_change]  This has been changed from earlier versions so that there is no need to change connectors to enter 
         flash menu mode

`OSD Index`_





      
      





   

