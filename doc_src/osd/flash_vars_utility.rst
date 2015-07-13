--------------
Flash_vars.exe
--------------

This app can be used to read to :

*  Write the flash variables to the OSD from a file
*  Read  the flash variables to a file from the OSD. 

A file read from a board can then be used to write the Flash on other boards.
   
There should be a sample file called osd_params.txt in this directory.
in the file empty lines and lines starting with '#' should be ignored

Usage:
   Connect the OSD to the PC via usb cable, but dont switch on the OSD
   run flash_vars.exe on the PC
   You should see a message to say
   
::

   -----------------Ready to Upload-------------------------
   Switch on the OSD board and then immediately press any key

On switching on the OSD it will wait approx 3 seconds looking for the sequence to
start the flash menu so if the flash_vars is too late it will not catch it.

If all goes well however, The Flash Variables file will be uploaded or downloaded to/from the OSD.


sample read invocation:

::

   ./flash_vars.exe -read osd_params.txt -port /dev/ttyUSB0

sample write invocation:

:: 

   ./flash_vars.exe -write osd_params.txt -port /dev/ttyUSB0

Changelog

* Can now read from flash variables to a file

* Fixed vector values can now contain whitespace
  e.g [ 1 ,2 , 3 ] is accepted

* Fixed so that accepts return and linefeed as end of line char
  Now automatically exits to start OSD when done. (Hope eventually to do it interactively
  , so its wysiwyg)
