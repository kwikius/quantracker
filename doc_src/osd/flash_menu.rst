
`OSD Index`_

------------------------------
Quantracker Air OSD Flash Menu
------------------------------

The flash menu mode can be entered as follows:
   Create a connector which joins txo, rxi power and ground pins as uusal ,
   but also connects the secondary tx0 and rxi pins with a jumper.
   
   Start a serial port console at 9600 baud with 8 bits, no parity and 1 stop bit.
   When you switch on the OSD it should present you with:

::

   Quantracker Air OSD 2.1
   ~~~~Flash menu mode~~~~


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


To set the values of flash variables at one go you can also use the app in 
      `quantracker/air/osd/utilities/flash_vars_upload/`_ . See the reame there for details.

.. _`quantracker/air/osd/utilities/flash_vars_upload/`: https://github.com/kwikius/quantracker/tree/devel/air/osd/utilities/flash_vars_upload
.. _`OSD Index`: index.html

`OSD Index`_





      
      





   

