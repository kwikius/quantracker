
#include <quan/uav/osd/pal_ntsc_pos.hpp>
#include <quan/stm32/flash.hpp>
#include <quan/stm32/flash/flash_convert.hpp>
#include "symbology.hpp"
#include "osd_control.hpp"

namespace {
   // these are the defaults
   // will be overwritten if customised in flash
   bool want_home = true;
   uint8_t osd_home_page_control = 0x07;
   quan::uav::osd::pal_ntsc_pos osd_home_position = {-160,70,60};
   bool want_compass = true;
   uint8_t osd_compass_page_control = 0x03;
   quan::uav::osd::pal_ntsc_pos osd_compass_position = {0,-115,-80};
   bool want_gps_no_fix = true;
   uint8_t osd_gps_no_fix_page_control = 0x03;
   quan::uav::osd::pal_ntsc_pos osd_gps_no_fix_position = {-160,70,50};
   bool want_altitude = true;
   uint8_t osd_altitude_page_control = 0x03;
   quan::uav::osd::pal_ntsc_pos osd_altitude_position = {95, -15, -15};
   bool want_baro_altitude = true;
   uint8_t osd_baro_altitude_page_control = 0x07;
   quan::uav::osd::pal_ntsc_pos osd_baro_altitude_position = {95, 0, 0};
   bool want_afcl_horizon = true;
   uint8_t osd_afcl_horizon_page_control = 0x03;
   quan::uav::osd::angle_type  osd_afcl_horiz_pitch_adj{0}; 
   uint32_t osd_afcl_horizon_eye_distance = 200;
   bool want_gps_fix_type = true;
   uint8_t osd_gps_fix_type_page_control = 0x03;
   quan::uav::osd::pal_ntsc_pos osd_gps_fix_type_position = {-170,75,75};
   bool want_gps_num_sats = true;
   uint8_t osd_gps_num_sats_page_control = 0x03;
   quan::uav::osd::pal_ntsc_pos osd_gps_num_sats_position = {-80,75,75};
   bool want_gps_hdop = true;
   uint8_t osd_gps_hdop_page_control = 0x03;
   quan::uav::osd::pal_ntsc_pos osd_gps_hdop_position = {-60,75,75};
   bool want_flight_mode = true;
   uint8_t osd_flight_mode_page_control = 0x07;
   quan::uav::osd::pal_ntsc_pos osd_flight_mode_position = {118,90,90};
   bool want_armed_mode = true;
   uint8_t osd_armed_mode_page_control = 0x07;
   quan::uav::osd::pal_ntsc_pos osd_armed_mode_position = {-170,90,90};
   bool want_airspeed = true;
   uint8_t osd_airspeed_page_control = 0x07;
   quan::uav::osd::pal_ntsc_pos osd_airspeed_position = {-170,0,0};
   bool want_groundspeed = true;
   uint8_t osd_groundspeed_page_control = 0x03;
   quan::uav::osd::pal_ntsc_pos osd_groundspeed_position = {-170,-15,-15};
   bool want_latitude = true;
   uint8_t osd_latitude_page_control = 0x03;
   quan::uav::osd::pal_ntsc_pos osd_latitude_position = {-170,-110,-110};
   bool want_longitude = true;
   uint8_t osd_longitude_page_control = 0x03;
   quan::uav::osd::pal_ntsc_pos osd_longitude_position = {33,-110,-110};
   bool want_aircraft_battery_voltage = true;
   uint8_t osd_aircraft_battery_voltage_page_control = 0x07;
   quan::uav::osd::pal_ntsc_pos osd_aircraft_battery_voltage_position = {106, 75, 75};
   bool want_aircraft_battery_current = true;
   uint8_t osd_aircraft_battery_current_page_control = 0x07;
   quan::uav::osd::pal_ntsc_pos osd_aircraft_battery_current_position = {106, 60, 60};
   bool want_aircraft_battery_remaining = true;
   uint8_t osd_aircraft_battery_remaining_page_control = 0x07;
   quan::uav::osd::pal_ntsc_pos osd_aircraft_battery_remaining_position = {118, 45, 45};

   uint8_t osd_page_select_channel_number = 8;
}

uint8_t osd_show_altitude_page() { return osd_altitude_page_control;}
bool osd_show_altitude() { return want_altitude&&((osd_show_altitude_page()&get_osd_mode())!=0);}
uint8_t osd_show_home_page() { return osd_home_page_control;}
bool osd_show_home() { return want_home&&((osd_show_home_page()&get_osd_mode())!=0);}
uint8_t osd_show_compass_page() { return osd_compass_page_control;}
bool osd_show_compass() { return want_compass&&((osd_show_compass_page()&get_osd_mode())!=0);}
uint8_t osd_show_gps_no_fix_page() { return osd_gps_no_fix_page_control;}
bool osd_show_gps_no_fix() { return want_gps_no_fix&&((osd_show_gps_no_fix_page()&get_osd_mode())!=0);}
uint8_t osd_show_afcl_horizon_page() { return osd_afcl_horizon_page_control;}
bool osd_show_afcl_horizon() { return want_afcl_horizon&&((osd_show_afcl_horizon_page()&get_osd_mode())!=0);}
uint8_t osd_show_gps_fix_type_page() { return osd_gps_fix_type_page_control;}
bool osd_show_gps_fix_type() { return want_gps_fix_type&&((osd_show_gps_fix_type_page()&get_osd_mode())!=0);}
uint8_t osd_show_gps_num_sats_page() { return osd_gps_num_sats_page_control;}
bool osd_show_gps_num_sats() { return want_gps_num_sats&&((osd_show_gps_num_sats_page()&get_osd_mode())!=0);}
uint8_t osd_show_gps_hdop_page() { return osd_gps_hdop_page_control;}
bool osd_show_gps_hdop() { return want_gps_hdop&&((osd_show_gps_hdop_page()&get_osd_mode())!=0);}
uint8_t osd_show_flight_mode_page() { return osd_flight_mode_page_control;}
bool osd_show_flight_mode() { return want_flight_mode&&((osd_show_flight_mode_page()&get_osd_mode())!=0);}
uint8_t osd_show_armed_mode_page() { return osd_armed_mode_page_control;}
bool osd_show_armed_mode() { return want_armed_mode&&((osd_show_armed_mode_page()&get_osd_mode())!=0);}
uint8_t osd_show_airspeed_page() { return osd_airspeed_page_control;}
bool osd_show_airspeed() { return want_airspeed&&((osd_show_airspeed_page()&get_osd_mode())!=0);}
uint8_t osd_show_groundspeed_page() { return osd_groundspeed_page_control;}
bool osd_show_groundspeed() { return want_groundspeed&&((osd_show_groundspeed_page()&get_osd_mode())!=0);}
uint8_t osd_show_latitude_page() { return osd_latitude_page_control;}
bool osd_show_latitude() { return want_latitude&&((osd_show_latitude_page()&get_osd_mode())!=0);}
uint8_t osd_show_longitude_page() { return osd_longitude_page_control;}
bool osd_show_longitude() { return want_longitude&&((osd_show_longitude_page()&get_osd_mode())!=0);}
uint8_t osd_show_baro_altitude_page() { return osd_baro_altitude_page_control;}
bool osd_show_baro_altitude() { return want_baro_altitude&&((osd_show_baro_altitude_page()&get_osd_mode())!=0);}
uint8_t osd_show_aircraft_battery_voltage_page() { return osd_aircraft_battery_voltage_page_control;}
bool osd_show_aircraft_battery_voltage() { return want_aircraft_battery_voltage&&((osd_show_aircraft_battery_voltage_page()&get_osd_mode())!=0);}
uint8_t osd_show_aircraft_battery_current_page() { return osd_aircraft_battery_current_page_control;}
bool osd_show_aircraft_battery_current() { return want_aircraft_battery_current&&((osd_show_aircraft_battery_current_page()&get_osd_mode())!=0);}
uint8_t osd_show_aircraft_battery_remaining_page() { return osd_aircraft_battery_remaining_page_control;}
bool osd_show_aircraft_battery_remaining() { return want_aircraft_battery_remaining&&((osd_show_aircraft_battery_remaining_page()&get_osd_mode())!=0);}
uint8_t osd_get_page_select_channel_number() { return osd_page_select_channel_number;}
uint32_t osd_get_afcl_horizon_eye_distance(){ return osd_afcl_horizon_eye_distance;}

quan::uav::osd::pxp_type get_osd_gps_fix_type_position()
{
   return  osd_gps_fix_type_position;
}

quan::uav::osd::pxp_type get_osd_gps_num_sats_position()
{
   return  osd_gps_num_sats_position;
}

quan::uav::osd::pxp_type get_osd_gps_hdop_position()
{
   return  osd_gps_hdop_position;
}

quan::uav::osd::pxp_type get_osd_flight_mode_position()
{
   return  osd_flight_mode_position;
}

quan::uav::osd::pxp_type get_osd_armed_mode_position()
{
   return  osd_armed_mode_position;
}

quan::uav::osd::pxp_type get_osd_airspeed_position()
{
   return  osd_airspeed_position;
}

quan::uav::osd::pxp_type get_osd_groundspeed_position()
{
   return  osd_groundspeed_position;
}

quan::uav::osd::pxp_type get_osd_latitude_position()
{
   return  osd_latitude_position;
}

quan::uav::osd::pxp_type get_osd_longitude_position()
{
   return  osd_longitude_position;
}

quan::uav::osd::pxp_type get_osd_altitude_position()
{
   return  osd_altitude_position;
}

quan::uav::osd::pxp_type get_osd_baro_altitude_position()
{
   return  osd_baro_altitude_position;
}

quan::uav::osd::pxp_type get_osd_aircraft_battery_voltage_position()
{
   return  osd_aircraft_battery_voltage_position;
}

quan::uav::osd::pxp_type get_osd_aircraft_battery_current_position()
{
   return  osd_aircraft_battery_current_position;
}

quan::uav::osd::pxp_type get_osd_aircraft_battery_remaining_position()
{
   return  osd_aircraft_battery_remaining_position;
}

quan::uav::osd::pxp_type get_osd_home_position()
{
   return osd_home_position;
}

quan::uav::osd::pxp_type get_osd_compass_position()
{
   return osd_compass_position;
}

quan::uav::osd::pxp_type get_osd_gps_no_fix_position()
{
   return osd_gps_no_fix_position;
}

quan::uav::osd::angle_type osd_get_afcl_horizon_pitch_adj()
{
   return osd_afcl_horiz_pitch_adj;
}

// if returns false look to the report errors mechanism to find the error
// When called there aint no user io
// so will need to check for and report  errors in mavlink_task at startup
#if !( defined (QUANTRACKER_AIR_OSD_PC_SIM_MODE))
bool init_values_from_flash()
{
   auto & symtab = quan::stm32::flash::get_app_symbol_table();
   // symbol may be valid but not yet defined by user
   
   if ( symtab.is_symbol_name_defined_in_flash("show_home")){
      bool v = false;
      if ( quan::stm32::flash::get_flash_value("show_home",v)){
         want_home = v;
      }
   }
   if ( symtab.is_symbol_name_defined_in_flash("osd_home_pos")){
      quan::three_d::vect<int32_t> v;
      if ( quan::stm32::flash::get_flash_value("osd_home_pos",v)){
         osd_home_position.set_xpos(v.x);
         osd_home_position.set_ypos(v.y,v.z);
      }
   }
   if ( symtab.is_symbol_name_defined_in_flash("osd_home_control")){
      uint8_t v;
      if ( quan::stm32::flash::get_flash_value("osd_home_control",v)){
         osd_home_page_control=v;
      }
  }
   if ( symtab.is_symbol_name_defined_in_flash("show_compass")){
      bool v = false;
      if ( quan::stm32::flash::get_flash_value("show_compass",v)){
         want_compass = v;
      }
   }

   if ( symtab.is_symbol_name_defined_in_flash("osd_compass_pos")){
      quan::three_d::vect<int32_t> v;
      if ( quan::stm32::flash::get_flash_value("osd_compass_pos",v)){
         osd_compass_position.set_xpos(v.x);
         osd_compass_position.set_ypos(v.y,v.z);
      }
   }
   if ( symtab.is_symbol_name_defined_in_flash("osd_compass_control")){
      uint8_t v;
      if ( quan::stm32::flash::get_flash_value("osd_compass_control",v)){
    	  osd_compass_page_control=v;
      }
  }

   if ( symtab.is_symbol_name_defined_in_flash("show_altitude")){
      bool v = false;
      if ( quan::stm32::flash::get_flash_value("show_altitude",v)){
         want_altitude = v;
      }
   }

   if ( symtab.is_symbol_name_defined_in_flash("osd_altitude_pos")){
      quan::three_d::vect<int32_t> v;
      if ( quan::stm32::flash::get_flash_value("osd_altitude_pos",v)){
         osd_altitude_position.set_xpos(v.x);
         osd_altitude_position.set_ypos(v.y,v.z);
      }
   }
   if ( symtab.is_symbol_name_defined_in_flash("osd_altitude_control")){
      uint8_t v;
      if ( quan::stm32::flash::get_flash_value("osd_altitude_control",v)){
    	  osd_altitude_page_control=v;
      }
  }

   if ( symtab.is_symbol_name_defined_in_flash("show_gps_no_fix")){
      bool v = false;
      if ( quan::stm32::flash::get_flash_value("show_gps_no_fix",v)){
         want_gps_no_fix = v;
      }
   }
 
   if ( symtab.is_symbol_name_defined_in_flash("osd_gps_no_fix_pos")){
      quan::three_d::vect<int32_t> v;
      if ( quan::stm32::flash::get_flash_value("osd_gps_no_fix_pos",v)){
         osd_gps_no_fix_position.set_xpos(v.x);
         osd_gps_no_fix_position.set_ypos(v.y,v.z);
      }
   }
   if ( symtab.is_symbol_name_defined_in_flash("osd_gps_no_fix_control")){
      uint8_t v;
      if ( quan::stm32::flash::get_flash_value("osd_gps_no_fix_control",v)){
    	  osd_gps_no_fix_page_control=v;
      }
  }

   if ( symtab.is_symbol_name_defined_in_flash("show_afcl_horizon")){
      bool v = false;
      if ( quan::stm32::flash::get_flash_value("show_afcl_horizon",v)){
         want_afcl_horizon = v;
      }
   }

   if ( symtab.is_symbol_name_defined_in_flash("osd_afcl_horizon_pitch_adj")){
      int16_t v;
      if ( quan::stm32::flash::get_flash_value("osd_afcl_horizon_pitch_adj",v)){
          osd_afcl_horiz_pitch_adj = quan::uav::osd::angle_type{v};
      }
   }
   if ( symtab.is_symbol_name_defined_in_flash("osd_afcl_horizon_control")){
      uint8_t v;
      if ( quan::stm32::flash::get_flash_value("osd_afcl_horizon_control",v)){
    	  osd_afcl_horizon_page_control=v;
      }
  }

    if ( symtab.is_symbol_name_defined_in_flash("show_gps_fix_type")){
       bool v = false;
       if ( quan::stm32::flash::get_flash_value("show_gps_fix_type",v)){
          want_gps_fix_type = v;
       }
    }

    if ( symtab.is_symbol_name_defined_in_flash("osd_gps_fix_type_pos")){
       quan::three_d::vect<int32_t> v;
       if ( quan::stm32::flash::get_flash_value("osd_gps_fix_type_pos",v)){
          osd_gps_fix_type_position.set_xpos(v.x);
          osd_gps_fix_type_position.set_ypos(v.y,v.z);
       }
    }
    if ( symtab.is_symbol_name_defined_in_flash("osd_gps_fix_type_control")){
       uint8_t v;
       if ( quan::stm32::flash::get_flash_value("osd_gps_fix_type_control",v)){
    	   osd_gps_fix_type_page_control=v;
       }
   }

    if ( symtab.is_symbol_name_defined_in_flash("show_gps_num_sats")){
       bool v = false;
       if ( quan::stm32::flash::get_flash_value("show_gps_num_sats",v)){
          want_gps_num_sats = v;
       }
    }

    if ( symtab.is_symbol_name_defined_in_flash("osd_gps_num_sats_pos")){
       quan::three_d::vect<int32_t> v;
       if ( quan::stm32::flash::get_flash_value("osd_gps_num_sats_pos",v)){
          osd_gps_num_sats_position.set_xpos(v.x);
          osd_gps_num_sats_position.set_ypos(v.y,v.z);
       }
    }
    if ( symtab.is_symbol_name_defined_in_flash("osd_gps_num_sats_control")){
       uint8_t v;
       if ( quan::stm32::flash::get_flash_value("osd_gps_num_sats_control",v)){
    	   osd_gps_num_sats_page_control=v;
       }
   }

   if ( symtab.is_symbol_name_defined_in_flash("show_gps_hdop")){
       bool v = false;
       if ( quan::stm32::flash::get_flash_value("show_gps_hdop",v)){
          want_gps_hdop = v;
       }
    }

    if ( symtab.is_symbol_name_defined_in_flash("osd_gps_hdop_pos")){
       quan::three_d::vect<int32_t> v;
       if ( quan::stm32::flash::get_flash_value("osd_gps_hdop_pos",v)){
          osd_gps_hdop_position.set_xpos(v.x);
          osd_gps_hdop_position.set_ypos(v.y,v.z);
       }
    }

    if ( symtab.is_symbol_name_defined_in_flash("osd_gps_hdop_control")){
       uint8_t v;
       if ( quan::stm32::flash::get_flash_value("osd_gps_hdop_control",v)){
    	   osd_gps_hdop_page_control=v;
       }
   }

    if ( symtab.is_symbol_name_defined_in_flash("show_flight_mode")){
       bool v = false;
       if ( quan::stm32::flash::get_flash_value("show_flight_mode",v)){
          want_flight_mode = v;
       }
    }

    if ( symtab.is_symbol_name_defined_in_flash("osd_flight_mode_pos")){
       quan::three_d::vect<int32_t> v;
       if ( quan::stm32::flash::get_flash_value("osd_flight_mode_pos",v)){
          osd_flight_mode_position.set_xpos(v.x);
          osd_flight_mode_position.set_ypos(v.y,v.z);
       }
    }
    if ( symtab.is_symbol_name_defined_in_flash("osd_flight_mode_control")){
        uint8_t v;
        if ( quan::stm32::flash::get_flash_value("osd_flight_mode_control",v)){
        	osd_flight_mode_page_control=v;
        }
    }

   if ( symtab.is_symbol_name_defined_in_flash("show_armed_mode")){
       bool v = false;
       if ( quan::stm32::flash::get_flash_value("show_armed_mode",v)){
          want_armed_mode = v;
       }
    }

    if ( symtab.is_symbol_name_defined_in_flash("osd_armed_mode_pos")){
       quan::three_d::vect<int32_t> v;
       if ( quan::stm32::flash::get_flash_value("osd_armed_mode_pos",v)){
          osd_armed_mode_position.set_xpos(v.x);
          osd_armed_mode_position.set_ypos(v.y,v.z);
       }
    }
    if ( symtab.is_symbol_name_defined_in_flash("osd_armed_mode_control")){
        uint8_t v;
        if ( quan::stm32::flash::get_flash_value("osd_armed_mode_control",v)){
        	osd_armed_mode_page_control=v;
        }
    }

    if ( symtab.is_symbol_name_defined_in_flash("show_airspeed")){
       bool v = false;
       if ( quan::stm32::flash::get_flash_value("show_airspeed",v)){
          want_airspeed = v;
       }
    }

    if ( symtab.is_symbol_name_defined_in_flash("osd_airspeed_pos")){
       quan::three_d::vect<int32_t> v;
       if ( quan::stm32::flash::get_flash_value("osd_airspeed_pos",v)){
          osd_airspeed_position.set_xpos(v.x);
          osd_airspeed_position.set_ypos(v.y,v.z);
       }
    }
    if ( symtab.is_symbol_name_defined_in_flash("osd_airspeed_control")){
        uint8_t v;
        if ( quan::stm32::flash::get_flash_value("osd_airspeed_control",v)){
        	osd_airspeed_page_control=v;
        }
    }

    if ( symtab.is_symbol_name_defined_in_flash("show_groundspeed")){
       bool v = false;
       if ( quan::stm32::flash::get_flash_value("show_groundspeed",v)){
          want_groundspeed = v;
       }
    }

    if ( symtab.is_symbol_name_defined_in_flash("osd_groundspeed_pos")){
       quan::three_d::vect<int32_t> v;
       if ( quan::stm32::flash::get_flash_value("osd_groundspeed_pos",v)){
          osd_groundspeed_position.set_xpos(v.x);
          osd_groundspeed_position.set_ypos(v.y,v.z);
       }
    }
    if ( symtab.is_symbol_name_defined_in_flash("osd_groundspeed_control")){
        uint8_t v;
        if ( quan::stm32::flash::get_flash_value("osd_groundspeed_control",v)){
        	osd_groundspeed_page_control=v;
        }
    }

    if ( symtab.is_symbol_name_defined_in_flash("show_latitude")){
       bool v = false;
       if ( quan::stm32::flash::get_flash_value("show_latitude",v)){
          want_latitude = v;
       }
    }

    if ( symtab.is_symbol_name_defined_in_flash("osd_latitude_pos")){
       quan::three_d::vect<int32_t> v;
       if ( quan::stm32::flash::get_flash_value("osd_latitude_pos",v)){
          osd_latitude_position.set_xpos(v.x);
          osd_latitude_position.set_ypos(v.y,v.z);
       }
    }
    if ( symtab.is_symbol_name_defined_in_flash("osd_latitude_control")){
        uint8_t v;
        if ( quan::stm32::flash::get_flash_value("osd_latitude_control",v)){
        	osd_latitude_page_control=v;
        }
    }

    if ( symtab.is_symbol_name_defined_in_flash("show_longitude")){
       bool v = false;
       if ( quan::stm32::flash::get_flash_value("show_longitude",v)){
          want_longitude = v;
       }
    }

    if ( symtab.is_symbol_name_defined_in_flash("osd_longitude_pos")){
       quan::three_d::vect<int32_t> v;
       if ( quan::stm32::flash::get_flash_value("osd_longitude_pos",v)){
          osd_longitude_position.set_xpos(v.x);
          osd_longitude_position.set_ypos(v.y,v.z);
       }
    }
    if ( symtab.is_symbol_name_defined_in_flash("osd_longitude_control")){
        uint8_t v;
        if ( quan::stm32::flash::get_flash_value("osd_longitude_control",v)){
        	osd_longitude_page_control=v;
        }
    }

    if ( symtab.is_symbol_name_defined_in_flash("show_baro_altitude")){
       bool v = false;
       if ( quan::stm32::flash::get_flash_value("show_baro_altitude",v)){
          want_baro_altitude = v;
       }
    }

    if ( symtab.is_symbol_name_defined_in_flash("osd_baro_altitude_pos")){
       quan::three_d::vect<int32_t> v;
       if ( quan::stm32::flash::get_flash_value("osd_baro_altitude_pos",v)){
          osd_baro_altitude_position.set_xpos(v.x);
          osd_baro_altitude_position.set_ypos(v.y,v.z);
       }
    }
    if ( symtab.is_symbol_name_defined_in_flash("osd_baro_altitude_control")){
        uint8_t v;
        if ( quan::stm32::flash::get_flash_value("osd_baro_altitude_control",v)){
        	osd_baro_altitude_page_control=v;
        }
    }

    if ( symtab.is_symbol_name_defined_in_flash("show_aircraft_battery_voltage")){
       bool v = false;
       if ( quan::stm32::flash::get_flash_value("show_aircraft_battery_voltage",v)){
          want_aircraft_battery_voltage = v;
       }
    }

    if ( symtab.is_symbol_name_defined_in_flash("osd_aircraft_battery_voltage_pos")){
       quan::three_d::vect<int32_t> v;
       if ( quan::stm32::flash::get_flash_value("osd_aircraft_battery_voltage_pos",v)){
          osd_aircraft_battery_voltage_position.set_xpos(v.x);
          osd_aircraft_battery_voltage_position.set_ypos(v.y,v.z);
       }
    }
    if ( symtab.is_symbol_name_defined_in_flash("osd_aircraft_battery_voltage_control")){
        uint8_t v;
        if ( quan::stm32::flash::get_flash_value("osd_aircraft_battery_voltage_control",v)){
        	osd_aircraft_battery_voltage_page_control=v;
        }
    }

    if ( symtab.is_symbol_name_defined_in_flash("show_aircraft_battery_current")){
       bool v = false;
       if ( quan::stm32::flash::get_flash_value("show_aircraft_battery_current",v)){
          want_aircraft_battery_current = v;
       }
    }

    if ( symtab.is_symbol_name_defined_in_flash("osd_aircraft_battery_current_pos")){
       quan::three_d::vect<int32_t> v;
       if ( quan::stm32::flash::get_flash_value("osd_aircraft_battery_current_pos",v)){
          osd_aircraft_battery_current_position.set_xpos(v.x);
          osd_aircraft_battery_current_position.set_ypos(v.y,v.z);
       }
    }
    if ( symtab.is_symbol_name_defined_in_flash("osd_aircraft_battery_current_control")){
        uint8_t v;
        if ( quan::stm32::flash::get_flash_value("osd_aircraft_battery_current_control",v)){
        	osd_aircraft_battery_current_page_control=v;
        }
    }

    if ( symtab.is_symbol_name_defined_in_flash("show_aircraft_battery_remaining")){
       bool v = false;
       if ( quan::stm32::flash::get_flash_value("show_aircraft_battery_remaining",v)){
          want_aircraft_battery_remaining = v;
       }
    }

    if ( symtab.is_symbol_name_defined_in_flash("osd_aircraft_battery_remaining_pos")){
       quan::three_d::vect<int32_t> v;
       if ( quan::stm32::flash::get_flash_value("osd_aircraft_battery_remaining_pos",v)){
          osd_aircraft_battery_remaining_position.set_xpos(v.x);
          osd_aircraft_battery_remaining_position.set_ypos(v.y,v.z);
       }
    }
    if ( symtab.is_symbol_name_defined_in_flash("osd_aircraft_battery_remaining_control")){
        uint8_t v;
        if ( quan::stm32::flash::get_flash_value("osd_aircraft_battery_remaining_control",v)){
        	osd_aircraft_battery_remaining_page_control=v;
        }
    }

    if ( symtab.is_symbol_name_defined_in_flash("osd_page_select_channel")){
        uint8_t v;
        if ( quan::stm32::flash::get_flash_value("osd_page_select_channel",v)){
        	osd_page_select_channel_number=v;
        }
    }

    if ( symtab.is_symbol_name_defined_in_flash("osd_afcl_horizon_eye_distance")){
        uint32_t v;
        if ( quan::stm32::flash::get_flash_value("osd_afcl_horizon_eye_distance",v)){
        	osd_afcl_horizon_eye_distance=v;
        }
    }
   return true;
}

bool initialise_flash()
{
  if (!quan::stm32::flash::get_app_symbol_table().init()){
      return false;
  }
  return init_values_from_flash();
}
#endif
