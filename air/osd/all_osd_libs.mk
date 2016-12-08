#builds all the osd libs

STATIC_LIBRARY_PATH = ../../../lib/osd/
ifeq ($(AERFLITE),True)
TARGET_LIB_NAME_PREFIX = aerflite
else
TARGET_LIB_NAME_PREFIX = quantracker_air
endif

flash_lib = $(STATIC_LIBRARY_PATH)flash_variables.a
osd_telem_none_lib  = $(STATIC_LIBRARY_PATH)$(TARGET_LIB_NAME_PREFIX)_osd.a
osd_telem_tx_lib = $(STATIC_LIBRARY_PATH)$(TARGET_LIB_NAME_PREFIX)_osd_tx.a
osd_telem_rx_lib = $(STATIC_LIBRARY_PATH)$(TARGET_LIB_NAME_PREFIX)_osd_rx.a
graphics_lib = (STATIC_LIBRARY_PATH)$(TARGET_LIB_NAME_PREFIX)_graphics_api.a
system_lib = (STATIC_LIBRARY_PATH)$(TARGET_LIB_NAME_PREFIX)_system.a


all_libs = $(flash_lib) $(graphics_lib) $(osd_telem_none_lib) $(system_lib) $(osd_telem_tx_lib)

# currently the tlem is not working for
# aerflite. Eventually it will have tx
ifneq ($(AERFLITE),True)
all_libs +=  $(osd_telem_rx_lib) 
endif

all: $(all_libs)

$(system_lib):
	$(MAKE)  -C ./ -f system.mk

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
	$(MAKE) -C ./ -f system.mk clean
	$(MAKE) -C ./ -f flash_variables_lib.mk clean
	$(MAKE) -C ./ -f graphics_api_lib.mk clean
	$(MAKE) -C ./ -f osd_lib.mk clean
	$(MAKE) -C ./ TELEMETRY_DIRECTION=QUAN_OSD_TELEM_TRANSMITTER -f osd_lib.mk clean
	$(MAKE) -C ./ TELEMETRY_DIRECTION=QUAN_OSD_TELEM_RECEIVER -f osd_lib.mk clean
