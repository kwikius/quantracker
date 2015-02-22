Index_

======================================
Uploading firmware via the Serial Port
======================================

This method uses Stm32flash_. 
  
  * stm32flash Windows install
    	- Download and run the [TODO: stm32flash_installer.zip]
  * stm32flash Linux install
      - Download Stm32flash_ and  make [sudo] make install.
  	 
  Once installed, test the execution by entering a command prompt and issuing 'stm32flash' 
  to see if it's installed. It should show a page or so of help info.
  If its not working, verify that stm32flash's parent directory is in your path.
  
  
Stm32Flash Hardware
--------------------
      
  To flash with stm32Flash you need A PC serial port. 
  A USB serial connector is fine. You can use a 6 pin Arduino Serial connector, 
  *but unfortunately you will need to swap two of the connector pins* [1]_. 
  Swapping pins is not difficult. Use a pin or knifeblade to
  lift the little tab and remove the Black wire (pin1) and the Brown wire 
  (pin2) and put the Black wire back in where the Brown wire was
  and the Brown wire where the black wire was. 


  .. image:: mavlink_sp_pinout.png


  Alternatively you can use any usb serial connector.

  You will need to make a power connector to supply between 8 and 12 Volts [2]_ 
  to the board and a switch to be able to switch it on and off.
  (I made up a servo connector to a 3S Lipo Balance lead connector and my switch 
  consisted of plugging the balance lead end together, as its polarised. 
  If you do this, solder header pins into pin 2 (V+) and 3 (V-), 
  but leave pin1 with no pin at all. This is simply because it is easy to connect
  the two +ve pins together resulting in shorting the battery (tell me how I know!)). 
  Its another minor issue on this board and for V2 will have
  to change this but for now just leave that header pin off.
   
  When using this method of uploading, it is important to follow the sequence 
  exactly as described, including the connection sequence. 
   
  *	
      Make sure that *Power is Not Connected*
      ,but that it is easy to connect power without disturbing the board. 
      Ideally put a switch in the power lead. 
      The reason is that touching the board or jiggling the wires
      when connecting can send spurious signals as wires connect 
      and disconnect and result in the bootloader on the board not
      recognising the startup sequence correctly [3]_.
  
  *	Connect the programming jumper on the osd board.
  *   Connect the serial connector to the board and the PC.
  *   
      Verify all connectors are the correct way round. 
      Unfortunately none of the connectors are polarised, so check and check again.

  *   At a command prompt navigate to the directory with the 'xxxx.bin' binary in.
  *   
      -   Linux   
      
         +   Assuming you are on ttyUSB0. - ( Use 'lsusb' to enumerate likely ports)
         +   *Without Pressing return after yet*, type the command 'stm32flash /dev/ttyUSB0'.
         
      -   Windows 
      
         +   Assuming you are on COM6 - ( To find your com port look in Device manager)
         +   *Without Pressing return after yet*, type the command 'stm32flash COM6'.
         
  *  Without joggling the wires or touching the board, switch on power to the board.
  *  Now Press return without jiggling the serial connection to the board!
  *  
     At this point you should see a diagnostic showing that the connection is good.
     It should look something like this::

         stm32flash - http://stm32flash.googlecode.com/

         Serial Config: 57600 8E1
         Version      : 0x31
         Option 1     : 0x00
         Option 2     : 0x00
         Device ID    : 0x0413 (STM32F4xx)
         RAM          : 128KiB  (8192b reserved by bootloader)
         Flash        : 1024KiB (sector size: 4x16384)
         Option RAM   : 15b
         System RAM   : 29KiB

         Resetting device... done.
     
     - If not:
   
         + Check you are on the correct serial port using the diagnostics for your system (as stated above). 
         + Check you have put on the Programming jumper.
         + Check your connections and check you didnt joggle the board too much when turning on power.

  *  
      If everything is ok, then, assuming you are in the directory where your firmware 
      '.bin' file is, disconnect power [4]_.

  *  Do the steps as above, but this time change the command( assuming your firmware is called 'my_osd_firmware.bin):
  
  		-   Linux  
  		
  			+  'stm32flash -f -v -w my_osd_firmware.bin /dev/ttyUSB0'
  		
  		-   Windows
  		
  			+  'stm32flash -f -v -w my_osd_firmware.bin COM6'
  		
  *  
     At this point you should see the same diagnostic as before. 
     Then things will appear to hang for maybe 15 seconds with no more output, so be patient:: 
     
            stm32flash -f -v -w main.bin /dev/ttyUSB0
            stm32flash - http://stm32flash.googlecode.com/

            Using Parser : Raw BINARY
            Serial Config: 57600 8E1
            Version      : 0x31
            Option 1     : 0x00
            Option 2     : 0x00
            Device ID    : 0x0413 (STM32F4xx)
            RAM          : 128KiB  (8192b reserved by bootloader)
            Flash        : 1024KiB (sector size: 4x16384)
            Option RAM   : 15b
            System RAM   : 29KiB

  *  
     Eventually you should see a progress message that the firmware is being loaded. 
     This process can take a while so remember not to jiggle the board meanwhile. 
     

  *
     Finally you should see something like the following::

            Wrote and verified address 0x080271c4 (100.00%) Done.

            Resetting device... done.

  *  Your firmware  should now be loaded on the board.

  *  
     Final important step before testing its working. 
     *Unplug the Programming Jumper* (Its easy to forget that step!).


Index_

.. _Stm32Flash: http://stm32flash.googlecode.com
.. _Index: index.html

.. [1] I got my wires crossed when designing V1 of the board
.. [2] It should be ok to power the board from up to 25 volts. It just hasnt been verified yet.
       Use the 12V power supply inputs rather than the 3V or 5V breakouts to power the board
       , since the startup time of the psu gives a smooth wakeup from reset.
.. [3] (This is another issue with V1 board. Ideally this can be done better on V2).
.. [4] Disconnecting power executes a reset.
      
