#ifndef QUANTRACKER_SYMBOLOGY_CONFIG_HPP_INCLUDED
#define QUANTRACKER_SYMBOLOGY_CONFIG_HPP_INCLUDED

#define CHAR_SPACING_X 12       // default size of one character including inter-character space
#define CHAR_SPACING_Y 15       // default size of one character including inter-character space


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
#define BATTERY_VOLTAGE_X -170
#define BATTERY_VOLTAGE_Y_NTSC -50
#define BATTERY_VOLTAGE_Y_PAL -50

#define BATTERY_CURRENT_ENABLE ENABLE_ON_PAGE_ALL
#define BATTERY_CURRENT_X -170
#define BATTERY_CURRENT_Y_NTSC -65
#define BATTERY_CURRENT_Y_PAL -65

#define BATTERY_REMAIN_ENABLE ENABLE_ON_PAGE_ALL
#define BATTERY_REMAIN_X -158
#define BATTERY_REMAIN_Y_NTSC -80
#define BATTERY_REMAIN_Y_PAL -80

#define GPS_FIX_TYPE_ENABLE ENABLE_ON_PAGE_ALL
#define GPS_FIX_TYPE_X -170
#define GPS_FIX_TYPE_Y_NTSC 75
#define GPS_FIX_TYPE_Y_PAL 75

#define GPS_HDOP_ENABLE ENABLE_ON_PAGE_ALL
#define GPS_HDOP_X -60
#define GPS_HDOP_Y_NTSC 75
#define GPS_HDOP_Y_PAL 75

#define GPS_NUM_SATS_ENABLE ENABLE_ON_PAGE_ALL
#define GPS_NUM_SATS_X -85
#define GPS_NUM_SATS_Y_NTSC 75
#define GPS_NUM_SATS_Y_PAL 75

#define ARMED_MODE_ENABLE ENABLE_ON_PAGE_ALL
#define ARMED_MODE_X -170
#define ARMED_MODE_Y_NTSC 90
#define ARMED_MODE_Y_PAL 90

#define FLIGHT_MODE_ENABLE ENABLE_ON_PAGE_ALL
#define FLIGHT_MODE_X 118
#define FLIGHT_MODE_Y_NTSC 90
#define FLIGHT_MODE_Y_PAL 90

#define GROUNDSPEED_ENABLE ENABLE_OFF
#define GROUNDSPEED_X -170
#define GROUNDSPEED_Y_NTSC 0
#define GROUNDSPEED_Y_PAL 0

#define AIRSPEED_ENABLE ENABLE_ON_PAGE_ALL
#define AIRSPEED_X -170
#define AIRSPEED_Y_NTSC 0
#define AIRSPEED_Y_PAL 0

#define ALTITUDE_ENABLE ENABLE_OFF
#define ALTITUDE_X 94
#define ALTITUDE_Y_NTSC 0
#define ALTITUDE_Y_PAL 0

#define BARO_ALTITUDE_ENABLE ENABLE_ON_PAGE_ALL
#define BARO_ALTITUDE_X 94
#define BARO_ALTITUDE_Y_NTSC 0
#define BARO_ALTITUDE_Y_PAL 0


#endif // QUANTRACKER_SYMBOLOGY_CONFIG_HPP_INCLUDED

