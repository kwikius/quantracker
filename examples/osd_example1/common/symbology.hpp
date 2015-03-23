#ifndef QUANTRACKER_AIR_OSD_EXAMPLES_SYMBOLOGY_HPP_INCLUDED
#define QUANTRACKER_AIR_OSD_EXAMPLES_SYMBOLOGY_HPP_INCLUDED

#include <quan/uav/osd/api.hpp>

bool osd_show_home();
quan::uav::osd::pxp_type get_osd_home_position();

bool osd_show_compass();
quan::uav::osd::pxp_type get_osd_compass_position();

bool osd_show_gps_no_fix();
quan::uav::osd::pxp_type get_osd_gps_no_fix_position();

bool osd_show_altitude();
quan::uav::osd::pxp_type get_osd_altitude_position();

bool osd_show_afcl_horizon();
quan::uav::osd::angle_type osd_get_afcl_horizon_pitch_adj();

#endif // QUANTRACKER_AIR_OSD_EXAMPLES_SYMBOLOGY_HPP_INCLUDED
