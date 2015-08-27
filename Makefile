# Top level Makefile
# if no arguments given then do info
ifneq ($(MAKECMDGOALS),)

.PHONY: ground_tracker_v1 ground_tracker_v2 osd_libs clean_osd_libs \
osd_example1 clean_osd_example1 upload_osd_example1 osd_ac \
clean_osd_ac upload_osd_ac

ground_tracker_v1:
	$(MAKE) -C ground/tracker/v1

ground_tracker_v2:
	$(MAKE) -C ground/tracker/v2

osd_libs:
	$(MAKE) -C air/osd/ -f all_osd_libs.mk

clean_osd_libs:
	$(MAKE) -C air/osd/ -f all_osd_libs.mk clean
#-------------------
osd_example1: 
	$(MAKE) -C examples/osd_example1/board

clean_osd_example1:
	$(MAKE) -C examples/osd_example1/board clean

upload_osd_example1:
	$(MAKE) -C examples/osd_example1/board upload_sp
#----------------
osd_ac: 
	$(MAKE) -C examples/osd_ac/board

clean_osd_ac:
	$(MAKE) -C examples/osd_ac/board clean

upload_osd_ac:
	$(MAKE) -C examples/osd_ac/board upload_sp
#----------------
osd_telem_tx: 
	$(MAKE) -C examples/osd_telem_tx

clean_osd_telem_tx:
	$(MAKE) -C examples/osd_telem_tx clean

upload_osd_telem_tx:
	$(MAKE) -C examples/osd_telem_tx upload_sp
#---------------
osd_telem_rx: 
	$(MAKE) -C examples/osd_telem_rx

clean_osd_telem_rx:
	$(MAKE) -C examples/osd_telem_rx clean

upload_osd_telem_rx:
	$(MAKE) -C examples/osd_telem_rx upload_sp
#-------------------
hello_world:
	$(MAKE) -C examples/hello_world

clean_hello_world:
	$(MAKE) -C examples/hello_world clean 

upload_hello_world:
	$(MAKE) -C examples/hello_world upload_sp

else
quantracker-make-help:
	@echo ''
	@echo ''
	@echo '**********************************************************'
	@echo '*                                                        *'
	@echo '*            Welcome to  Quantracker                     *' 
	@echo '*                                                        *'
	@echo '**********************************************************'
	@echo ''                                                       
	@echo '************ Making OSD examples *************************'
	@echo ''
	@echo invoke \'make osd_example1\' to build the osd_example1 example. \(\'make clean_osd_example1\' to clean\)
	@echo invoke \'make upload_osd_example1\' to upload via the serial port
	@echo ''
	@echo invoke \'make osd_ac\' to make the osd_ac example. \(\'make clean_osd_ac\' to clean\)
	@echo invoke \'make upload_osd_ac\' to upload via the serial port 
	@echo ''
	@echo invoke \'make osd_telem_tx\' to make the osd__telem_tx example. \(\'make clean_osd_telem_tx\' to clean\)
	@echo invoke \'make upload_osd_telem_tx\' to upload via the serial port
	@echo ''
	@echo invoke \'make osd_telem_rx\' to make the osd__telem_rx example. \(\'make clean_osd_telem_rx\' to clean\)
	@echo invoke \'make upload_osd_telem_rx\' to upload via the serial port
	@echo ''
	@echo invoke \'make hello_world\' to make the hello_world example. \(\'make clean_hello_world\' to clean\)
	@echo invoke \'make upload_hello_world\' to upload via the serial port
	@echo ''
	@echo The examples should build the required static libraries, but they can be built and cleaned separately
	@echo invoke \'make osd_libs\' to make the osd static libraries. \(\'make clean_osd_libs\' to clean\)
	@echo ''
	@echo '************ Making Antenna Tracker V2 ********************'
	@echo ''
	@echo invoke \'make ground_tracker_v2\'. 
	@echo The resulting binaries will be in the ground/tracker/v2/bin/ subdirectory.
	@echo ''
	@echo '************ Making Antenna Tracker V1 ********************'
	@echo ''
	@echo invoke \'make ground_tracker_v1\'. 
	@echo The resulting binaries will be in the ground/tracker/v1/ subdirectory.
	@echo ''
	@echo '***********************************************************'

endif
