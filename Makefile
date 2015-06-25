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
	make -C examples/osd_ac/board
   
else
quantracker-make-help:
	@echo '------------------------------------------------------'
	@echo 'Welcome to the quantracker DIY Antenna Tracker project. '
	@echo 'The project is growing and I opted to start to merge all the related projects' 
	@echo 'as subdirectories in the github/kwikius/quantracker project.'
	@echo 'As a first step I moved the original quantracker project into ground/tracker/v1/'
	@echo ''
	@echo 'You can build that project using the Makefile there if you wish, or'
	@echo ', if you want to build the original DIY Antenna Tracker from this Makefile' 
	@echo ', you can use the command '\''make ground_tracker_v1'\'' from here. '
	@echo 'The resulting binaries will be in the ground/tracker/v1/ subdirectory'
	@echo '-----------------------------------------------'
	
endif
