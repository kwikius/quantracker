`OSD index`_

------------------------------------------------------
Getting the Quantracker OSD toolchain and Dependencies
------------------------------------------------------

TODO: Windows version

You need The `quantracker dependencies`_ below. 

------------
The easy way
------------

The easy way is to use the `Quantracker_dependencies installer`_. Its currently Linux only,
but I hope to have it working on Windows soon. To use:

   * Download the script. 
   * Create an empty download directory
   * Call the script with Python. 

You can add an optional directory to put the dependencies on the command line,
else you will be prompted for a download directory.
 
------------
The hard Way
------------

If you already have a dependency installed, no problem
since we can direct the toolchain to use it ( see the last part of this section). 
If you dont, then it is up to you where you wish to install the dependencies.
For myself, I use an external 'lib' directory in my home directory 
and install everying in there for simplicity. This avoids cluttering the system directories, but
you may use multiple directories or  if you wish.

Once the libraries have been downloaded, follow the per-dependency installation instructions  
below to install them. You may wish to combine the next step here..

Next, in order to get the compiler to find everything , Enter the quantracker directory
,open the `Sample-Dependencies.mk`_ in a text editor and save it as a file 
named 'Dependencies.mk' in the same root quantracker directory. Edit the defines therein 
to point to the downloaded dependency libraries. N.B.  follow carefully the format for each in the
Sample-Dependencies.mk template. There are some inconsistencies for which I apologise.


That completes the basic installation. 

-------------------------------
Testing everything is installed
-------------------------------

Note: In the following I assume that you have renamed the quantracker download root directory to
'quantracker'. if not just substitute your own version of the name.

To test the installation you will build some libraries inside the quantracker distro.

At the command line enter the root quantracker directory and type 'make osd_libs'. If all goes well
building the libraries will take a few minutes.
If you see error messages they will probably be because some header can be found. 
In that case check that you have entered your paths to the dependencies correctly in your Dependencies.mk. 
(in most cases The relevant dependency not found  may be clear from the  error messages.)

If the osd libraries have been built correctly they should now appear in the quantracker/lib/osd/ subdirectory
as follows:

- flash_variables.a  
- quantracker_air_graphics_api.a  
- quantracker_air_osd.a  
- quantracker_air_osd_rx.a  
- quantracker_air_osd_tx.a  
- README

--------------------------
Building a sample firmware
--------------------------

.. _`quantracker dependencies`:

----------------------------
The Quantracker Dependencies
----------------------------

The versions here will probably have been superceded now.
The latest stable versions are to be preferred, but to get started the ones linked here should work fine.

`arm-gcc`_
   arm-gcc compiler.
      To install, simply unzip to its target directory.

`mavlink`_
   mavlink library. 
      To install, unzip to its target directory and then
      Rename the root directory that appears('c_library-master') to 'mavlink'
   
`quan`_
   The quan library.
      To install , simply unzip to its target directory

`freertos`_
   freeRTOS real-time operating system.
      To install, simply unzip to its target directory 

`quantracker`_
   The quantracker source code and utilities.
      To install simply unzip to its target directory

`stm32`_
   stm32f4 std peripherals library.

`stm32flash`_
   utility for programming the OSD via the serial port. Download and make for linux. For Windows a prebuilt version
   is included in the quantracker/bin/ directory
   
.. _`Quantracker_dependencies installer`: https://github.com/kwikius/quantracker/blob/master/bin/quantracker_deps_installer.py
.. _`arm-gcc`:  https://launchpad.net/gcc-arm-embedded/4.9/4.9-2014-q4-major/+download/gcc-arm-none-eabi-4_9-2014q4-20141203-linux.tar.bz2
.. _`mavlink`: https://github.com/mavlink/c_library/archive/master.zip
.. _`quan`: https://github.com/kwikius/quan-trunk/archive/master.zip
.. _`freertos`: http://downloads.sourceforge.net/project/freertos/FreeRTOS/V8.2.0/FreeRTOSV8.2.0.zip
.. _`quantracker`: https://github.com/kwikius/quantracker/archive/master.zip
.. _`stm32`: http://www.st.com/st-web-ui/static/active/en/st_prod_software_internet/resource/technical/software/firmware/stm32f4_dsp_stdperiph_lib.zip
.. _`OSD index`: ../index.html
.. _`Sample-Dependencies.mk`: https://github.com/kwikius/quantracker/blob/master/Sample-Dependencies.mk
.. _`stm32flash`:  http://sourceforge.net/projects/stm32flash/files/stm32flash-0.4.tar.gz

----------------------------

`OSD index`_