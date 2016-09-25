`OSD Index`_

======================================
Uploading firmware via the Serial Port
======================================

--------
Hardware
--------

   To flash with stm32Flash you need a PC serial port and a USB TTL serial connector.
   A 6 pin Arduino Serial connector works well. It can be either a 3 volt
   or 5 volt one [1]_, but must have power out, since the programming serial port
   is electrically isolated from the rest of the board. When connected
   a red LED should light right by the connector.
   Make sure that you connect to the OSD board with the Black wire
   connected to the V-Blk pin on the 6 way connector.

   .. image:: V2_1_mavlink_sp_pinout.png

   Alternatively you can use any usb to TTL serial connector and connect the pins appropriately.
   The OSD pins have some protection so within reason it should be possible to swap the Tx and Rx
   connections if it isnt working. Usually (but not always) TX at one end connects to RX at the other
   and vice versa

----------
Stm32flash
----------

   Whichever operating system you use, you need stm32flash. For windows,
   download and unzip `stm32flash_win.zip`_ .

   For Linux, Download Stm32flash_ and  make [sudo] make install. You may also need to add yourself to the 'dialout'
   group to allow you access to the usb serial port: ``sudo usermod -a -G dialout $USER`` ( You will need to reboot for the change to take effect)

   For a quick and dirty upload with stm3flash, get `your desired firmware`_ and
   move the stm32flash executable into the same directory as the firmware.

   Test the execution by starting a command prompt,
   navigating to the directory where the firmware is and issuing 'stm32flash' (for Linux)
   or 'stm32flash.exe'(for Windows),
   to see if it's installed. It should show a page or so of help info.

----------------------------
Find the correct serial port
----------------------------

.....
Linux
.....

   The easiest way on Linux is to remove any usb peripherals except the usb serial connector
   and type

       ``$ setserial -g /dev/ttyUSB[01234]``

   which responds with

       ``/dev/ttyUSB0, UART: unknown, Port: 0x0000, IRQ: 0, Flags: low_latency``

   when I connect my Serial USB adaptor. To be honest if
   you have no other USB serial connectors plugged connected it will usually be /dev/ttyUSB0

.......
Windows
.......

   In Windows remove any extraneous usb devices except the usb serial connector
   and open Device Manager (type Device Manager in
   the box at the bottom of the start menu and it should come up in the list).
   Look for The Ports(COM & LPT) section and there should be an item saying 'USB Serial Port' (COMx)
   where x is a number e.g (say) COM6

----------------------
Powering The OSD board
----------------------

   You will need a power supply of between 8 and 12 Volts [2]_
   to the board ( A servo connector with power and ground connected to (say) a 12 V (3S) lipo is fine)
   and ideally a switch to be able to switch it on and off, though you can just use the connector
   but a switch makes it much easier.


------------------------------------
Checking the OSD board is recognised
------------------------------------

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
      The connectors are not polarised, so check and check again.

   *  At a command prompt navigate to the directory with the 'xxxx.bin' binary in.
   *
      -   Linux

         +   Assuming you are on /dev/ttyUSB0  ...
         +   *Without Pressing return after yet*, type the command 'stm32flash /dev/ttyUSB0'.

      -   Windows

         +   Assuming you are on COM6  ...
         +   *Without Pressing return after yet*, type the command 'stm32flash.exe COM6'.

   *  Switch on power to the board *without joggling the wires or touching the board* .
   *  Now Press [return] !
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

      If not:

         + Check you are on the correct serial port using the diagnostics for your system (as stated above).
         + Check you have put on the Programming jumper.
         + Check your connections and check you didnt joggle the board too much when turning on power.

   *
      If everything is ok, then, assuming you are in the directory where your firmware
      '.bin' file is, disconnect power [4]_.


-------------------------
Programming the OSD board
-------------------------

   *  Do the steps as above, but this time change the command( assuming your firmware is called 'my_osd_firmware.bin):

  		-   Linux

  			+  'stm32flash -f -v -w my_osd_firmware.bin /dev/ttyUSB0'

  		-   Windows

  			+  'stm32flash.exe -f -v -w my_osd_firmware.bin COM6'

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

   *  Congratulations! .. Your firmware  should now be loaded on the board.

   *
      Final important step before testing its working.
      *Unplug the Programming Jumper* (Its easy to forget that step!).


`OSD Index`_

.. _`your desired firmware` :
   osd/firmwares.html
.. _`stm32flash_win.zip`:
   https://github.com/kwikius/quantracker/blob/master/bin/stm32flash_win.zip?raw=true
.. _Stm32Flash: http://stm32flash.googlecode.com
.. _`OSD Index`: osd/index.html
.. _`click here for the pinout`: v1_board.html

.. [1] You only need to connect the pins in bold in the diagram.
.. [2] It should be ok to power the board from up to 25 volts. It just hasnt been verified yet.
       Use the 12V power supply inputs rather than the 3V or 5V breakouts to power the board
       , since the startup time of the psu gives a smooth wakeup from reset.
.. [3] (This is another issue with V1 board. Ideally this can be done better on V2).
.. [4] Disconnecting power executes a reset.

