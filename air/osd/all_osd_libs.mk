#builds all the osd libs

STATIC_LIBRARY_PATH = ../../../lib/osd/

flash_lib = $(STATIC_LIBRARY_PATH)flash_variables.a
osd_telem_none_lib  = $(STATIC_LIBRARY_PATH)quantracker_air_osd.a
osd_telem_tx_lib = $(STATIC_LIBRARY_PATH)quantracker_air_osd_tx.a
osd_telem_rx_lib = $(STATIC_LIBRARY_PATH)quantracker_air_osd_rx.a
graphics_lib = (STATIC_LIBRARY_PATH)quantracker_air_graphics_api.a

all_libs = $(flash_lib) $(graphics_lib) $(osd_telem_none_lib) \
$(osd_telem_tx_lib) $(osd_telem_rx_lib) 

all: $(all_libs)

$(flash_lib):
	$(MAKE) -C ./ -f flash_variables_lib.mk

$(graphics_lib):
	$(MAKE) -C ./ -f graphics_api_lib.mk

$(osd_telem_none_lib):
	$(MAKE)  -C ./ -f osd_lib.mk

$(osd_telem_tx_lib):
	$(MAKE) -C ./ TELEMETRY_DIRECTION=QUAN_OSD_TELEM_TRANSMITTER -f osd_lib.mk

$(osd_telem_rx_lib):
	$(MAKE) -C ./ TELEMETRY_DIRECTION=QUAN_OSD_TELEM_RECEIVER -f osd_lib.mk

.PHONY: clean

clean:
	$(MAKE) -C ./ -f flash_variables_lib.mk clean
	$(MAKE) -C ./ -f graphics_api_lib.mk clean
	$(MAKE) -C ./ -f osd_lib.mk clean
	$(MAKE) -C ./ TELEMETRY_DIRECTION=QUAN_OSD_TELEM_TRANSMITTER -f osd_lib.mk clean
	$(MAKE) -C ./ TELEMETRY_DIRECTION=QUAN_OSD_TELEM_RECEIVER -f osd_lib.mk clean

