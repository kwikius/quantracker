#include <cstdio>
#include <quan/constrain.hpp>
#include <quan/uav/osd/features_api.hpp>
#include "osd.hpp"
#include "symbology_config.hpp"

using namespace quan::uav::osd;

static uint8_t osd_items_enable[OSD_ITEM_ARRAY_SIZE];
static uint16_t osd_items_pos_x[OSD_ITEM_ARRAY_SIZE];
static uint16_t osd_items_pos_y[OSD_ITEM_ARRAY_SIZE];


uint8_t get_osd_mode()
{
	uint16_t control_value;
	uint8_t osd_mode;

	control_value = get_rc_raw_chan(OSD_MODE_CHANNEL);

	if(control_value < OSD_MODE_VALUE_OFF)
		osd_mode = OSD_MODE_OFF;
	else if (control_value < OSD_MODE_VALUE_PAGE_1)
		osd_mode = OSD_MODE_PAGE_1;
	else if (control_value < OSD_MODE_VALUE_PAGE_2)
		osd_mode = OSD_MODE_PAGE_2;
	else if (control_value < OSD_MODE_VALUE_PAGE_3)
		osd_mode = OSD_MODE_PAGE_3;
	else
		osd_mode = OSD_MODE_OFF;

	return osd_mode;
}

uint8_t get_osd_item_enabled(uint8_t item_index, uint8_t item_mask)
{
	return ((osd_items_enable[item_index] & item_mask) !=0);
}
