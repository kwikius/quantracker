# Top level Makefile
# if no arguments given then do info
ifneq ($(MAKECMDGOALS),)

ground_tracker_v1:
	make -C ground/tracker/v1

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
