Index_

======================================
Uploading firmware using ST-Link
======================================

  If you have a STM32 Discovery board, an STM32 Nucleo or a STLink programmer, then you can use ST-Link. 
  For this method you will also need to make up a programming cable. 
  Using this method you can also runtime debug the board.
  In Linux, you may need to edit your UdevRules_ so you can access the port. 
  The zip contains the relevant udev files which you can add to /etc/udev/rules.d/
 
  * Windows
      ST-Link for windows can be downloaded and installed from [???] (You can also use TexaneSTLink using MinGW). This is a GUI application, though its also useable from command line
  
  * Linux
  		TexaneSTLink_ 
   	
   	
STLink Hardware
---------------

  To flash with ST-Link, you need the ST-Link software,
  a PC usb port and either a STM32xx-Discovery or a STM32xx-Nucleo. 
  You will also need to make a connector.

  * Windows

   -
      Download the ST-LINK software from http://www.st.com/internet/evalboard/product/251168.jsp
      Somewhere down the pag, Find "Associated software"
      There are links to two zip files. Note that they are a Free Download.
      Download the files and unzip them.
      In each resulting directory there is an installer.
      One is for the ST-LINK utility and the other is for the STLINK USB drivers 
      which Windows needs for the USB programming cable. 
      After running both installers you should have a new icon on the desktop, 
      the STM32 ST-LINK Utility.
   -
      Before continuing, you should now download the firmware from 
   -  make the connector [TODO pic of connector]
   -  Start the ST-LINK Utility Application.
   -  
      Connect to the board using Target>Connect from the Top menu.
      (You may get a warning saying the ST-LINK firmware is outdated. You can ignore this.)
   -
      Open the firmware file using File>Open File from the Top menu ..
      In the first tab above the area showing the device memory make 
      sure that "Device Memory @ 0x8000000:" is selected

   - 
      Now program the device using "Target> Program" Or "Target> Program & Verify" 
      Its easiest to deselect the Reset after programming tickbox 
      ( Not essential but we can do a reset by hand for clarity)

   -
      Select Target> Disconnect and quit the application,
      while leaving the cable and pcb connected.


Index_

.. _Index: index.html
.. _Stm32Flash: http://stm32flash.googlecode.com
.. _TexaneSTLink: https://github.com/texane/stlink/
.. _UdevRules: https://wiki.archlinux.org/index.php/udev
