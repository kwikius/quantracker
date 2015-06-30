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

osd_example1: 
	$(MAKE) -C examples/osd_example1/board

clean_osd_example1:
	$(MAKE) -C examples/osd_example1/board clean

upload_osd_example1:
	$(MAKE) -C examples/osd_example1/board upload_sp
  
osd_ac: 
	$(MAKE) -C examples/osd_ac/board

clean_osd_ac:
	$(MAKE) -C examples/osd_ac/board clean

upload_osd_ac:
	$(MAKE) -C examples/osd_ac/board upload_sp

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
	@echo invoke \'make osd_ac\' to make the osd_ac example. \(\'make clean_osd_ac\' to clean\)
	@echo invoke \'make upload_osd_ac\' to upload via the serial port 
	@echo ''
	@echo invoke \'make osd_example1\' to build the osd_example1 example. \(\'make clean_osd_example1\' to clean\)
	@echo invoke \'make upload_osd_example1\' to upload via the serial port
	@echo ''
	@echo '************ Making Antenna Tracker V1 ********************'
	@echo ''
	@echo invoke \'make ground_tracker_v1\'. 
	@echo The resulting binaries will be in the ground/tracker/v1/ subdirectory.
	@echo ''
	@echo '********************************************************************'
	@echo ''
	@echo ''
	
endif
