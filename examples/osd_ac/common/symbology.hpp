#ifndef QUANTRACKER_AIR_OSD_EXAMPLES_SYMBOLOGY_HPP_INCLUDED
#define QUANTRACKER_AIR_OSD_EXAMPLES_SYMBOLOGY_HPP_INCLUDED

#include <quan/uav/osd/api.hpp>

bool osd_show_home();
quan::uav::osd::pxp_type get_osd_home_position();

bool osd_show_compass();
quan::uav::osd::pxp_type get_osd_compass_position();

bool osd_show_gps_no_fix();
quan::uav::osd::pxp_type get_osd_gps_no_fix_position();

bool osd_show_gps_fix_type();
quan::uav::osd::pxp_type get_osd_gps_fix_type_position();

bool osd_show_gps_num_sats();
quan::uav::osd::pxp_type get_osd_gps_num_sats_position();

bool osd_show_gps_hdop();
quan::uav::osd::pxp_type get_osd_gps_hdop_position();

bool osd_show_flight_mode();
quan::uav::osd::pxp_type get_osd_flight_mode_position();

bool osd_show_armed_mode();
quan::uav::osd::pxp_type get_osd_armed_mode_position();

bool osd_show_airspeed();
quan::uav::osd::pxp_type get_osd_airspeed_position();

bool osd_show_groundspeed();
quan::uav::osd::pxp_type get_osd_groundspeed_position();

bool osd_show_latitude();
quan::uav::osd::pxp_type get_osd_latitude_position();

bool osd_show_longitude();
quan::uav::osd::pxp_type get_osd_longitude_position();

bool osd_show_altitude();
quan::uav::osd::pxp_type get_osd_altitude_position();

bool osd_show_baro_altitude();
quan::uav::osd::pxp_type get_osd_baro_altitude_position();

bool osd_show_aircraft_battery_voltage();
quan::uav::osd::pxp_type get_osd_aircraft_battery_voltage_position();

bool osd_show_aircraft_battery_current();
quan::uav::osd::pxp_type get_osd_aircraft_battery_current_position();

bool osd_show_aircraft_battery_remaining();
quan::uav::osd::pxp_type get_osd_aircraft_battery_remaining_position();

bool osd_show_afcl_horizon();
quan::uav::osd::angle_type osd_get_afcl_horizon_pitch_adj();

uint32_t osd_get_afcl_horizon_eye_distance();

uint8_t osd_get_page_select_channel_number();

#endif // QUANTRACKER_AIR_OSD_EXAMPLES_SYMBOLOGY_HPP_INCLUDED
