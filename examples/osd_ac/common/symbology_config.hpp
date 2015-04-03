#ifndef QUANTRACKER_SYMBOLOGY_CONFIG_HPP_INCLUDED
#define QUANTRACKER_SYMBOLOGY_CONFIG_HPP_INCLUDED

#define CHAR_SPACING_X 12       // default size of one character including inter-character space
#define CHAR_SPACING_Y 15       // default size of one character including inter-character space

#define OSD_ITEM_BATTERY_VOLTAGE 0			// Index into OSD data item position and enable arrays
#define OSD_ITEM_BATTERY_CURRENT 1			// Index into OSD data item position and enable arrays
#define OSD_ITEM_BATTERY_REMAIN 2			// Index into OSD data item position and enable arrays

#define OSD_ITEM_ARRAY_SIZE 3


#define OSD_MODE_CHANNEL 7		// RC channel used to select OSD mode (e.g. page). Valid values 0-7.
#define OSD_MODE_VALUE_OFF  1000      // This mode if input value less than this value
#define OSD_MODE_VALUE_PAGE_1  1200      // This mode if input value less than this value (and not less than the value above
#define OSD_MODE_VALUE_PAGE_2  1400      // This mode if input value less than this value
#define OSD_MODE_VALUE_PAGE_3  1600      // This mode if input value less than this value

#define OSD_MODE_OFF  0         // Code to indicate OSD mode is off
#define OSD_MODE_PAGE_1  1      // Code to indicate OSD mode is page 1  (as a bit mask)
#define OSD_MODE_PAGE_2  2      // Code to indicate OSD mode is page 2  (as a bit mask)
#define OSD_MODE_PAGE_3  4      // Code to indicate OSD mode is page 3  (as a bit mask)


#define ENABLE_OFF        0x00
#define ENABLE_ON_PAGE_0  0x01
#define ENABLE_ON_PAGE_1  0x02
#define ENABLE_ON_PAGE_2  0x04
#define ENABLE_ON_PAGE_ALL (ENABLE_ON_PAGE_0 | ENABLE_ON_PAGE_1 | ENABLE_ON_PAGE_2)


#define BATTERY_VOLTAGE_ENABLE ENABLE_ON_PAGE_ALL

#define BATTERY_CURRENT_ENABLE ENABLE_ON_PAGE_ALL

#define BATTERY_REMAIN_ENABLE ENABLE_ON_PAGE_ALL

#define GPS_FIX_TYPE_ENABLE ENABLE_ON_PAGE_ALL

#define GPS_HDOP_ENABLE ENABLE_ON_PAGE_ALL

#define GPS_NUM_SATS_ENABLE ENABLE_ON_PAGE_ALL

#define ARMED_MODE_ENABLE ENABLE_ON_PAGE_ALL

#define FLIGHT_MODE_ENABLE ENABLE_ON_PAGE_ALL

#define GROUNDSPEED_ENABLE ENABLE_OFF

#define AIRSPEED_ENABLE ENABLE_ON_PAGE_ALL

#define ALTITUDE_ENABLE ENABLE_OFF

#define BARO_ALTITUDE_ENABLE ENABLE_ON_PAGE_ALL

#endif // QUANTRACKER_SYMBOLOGY_CONFIG_HPP_INCLUDED

