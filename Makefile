# Top level Makefile
# if no arguments given then do info
ifneq ($(MAKECMDGOALS),)

.PHONY: ground_tracker_v1 osd_libs clean_osd_libs \
osd_example1 clean_osd_example1 osd_ac clean_osd_ac

ground_tracker_v1:
	make -C ground/tracker/v1

osd_libs:
	make -C air/osd/ -f all_osd_libs.mk

clean_osd_libs:
	make -C air/osd/ -f all_osd_libs.mk clean

osd_example1: 
	make -C examples/osd_example1/board

clean_osd_example1:
	make -C examples/osd_example1/board clean
  
osd_ac: 
	make -C examples/osd_ac/board

clean_osd_ac:
	make -C examples/osd_ac/board clean

else
quantracker-make-help:
	@echo ''
	@echo ''
	@echo '********************************************************************'
	@echo ''
	@echo 'Welcome to the Quantracker DIY Antenna Tracker and OSD  project.' 
	@echo ''
	@echo ''
	@echo '********************** Making OSD examples *************************'
	@echo Before making Air OSD projects, first invoke \'make osd_libs\' to build
	@echo the static libraries.
	@echo ''
	@echo invoke \'make osd_ac\' to make the osd_ac example. \(\'make clean_osd_ac\' to clean\)
	@echo invoke \'make osd_example1\' to build the osd_example1 example. \(\'make clean_osd_example1\' to clean\)
	@echo ''
	@echo '********************** Making Antenna Tracker V1 ********************'
	@echo You can build that project using the Makefile there if you wish, or
	@echo , if you want to build the original DIY Antenna Tracker from this Makefile
	@echo , invoke \'make ground_tracker_v1\' from here.
	@echo The resulting binaries will be in the ground/tracker/v1/ subdirectory.
	@echo ''
	@echo '********************************************************************'
	@echo ''
	@echo ''
	
endif
