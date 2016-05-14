----------
OSD Timing
----------

The OSD uses 4 timers to control the OSD [1]_. See the diagram

  .. image:: osd_symbolic_timing_schematic.png

   * Line counter
        In External synch mode Listens to the csynch pulse and counts lines
        Has 4 compare channels
         - Ch1 starts the telemetry lines
         - Ch2 ends the telemtry lines
         - Ch3 starts the OSD rows
         - Ch4 ends the OSD rows
        In internal synch mode, generates the video timing
   
   * Line timer
        Starts at the beginning of telem and OSD rows.
        Has 2 channels
          - Ch1 Opens the gate for the pixel clock
          - Ch2 closes the gate for the pixe clock
            
   * pixel-clock
         Runs as a slave of the Line timer when the gate is opened

   * Idletime sync sep measurer
         This timer


. [1] It should be possible to reduce this, TODO.

