/*
 Copyright (c) 2013 -2015 Andy Little 

 With Grateful Acknowledgements to the prior work of:
   Sami Korhonen(Openpilot.org)
   taulabs ( taulabs.com) 
   brainFPV ( brainfpv.com)
   Thomas Oldbury (super-osd.com)

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program. If not, see <http://www.gnu.org/licenses/>
*/
#include "../aircraft.hpp"

//#include "graphics_api.hpp"
#include <quan/uav/osd/api.hpp>
#include <quan/uav/osd/features_api.hpp>
#include <quan/uav/osd/basic_bitmap.hpp>
#include <quan/uav/osd/basic_font.hpp>
#include <quan/uav/osd/get_aircraft_position.hpp>
#include <quan/uav/osd/get_home_position.hpp>
#include <quan/uav/osd/get_aircraft_attitude.hpp>
#include <quan/uav/osd/get_aircraft_heading.hpp>

#include "video_buffer.hpp"
#include "video_cfg.hpp"
#include "video_cfg.hpp"

   quan::uav::osd::pxp_type  
   quan::uav::osd::transform_to_raw(pxp_type const & pos)
   {
        auto const ds = video_cfg::get_display_size_px();
        return {pos.x + static_cast<int32_t>(ds.x)/2 ,static_cast<int32_t>(ds.y)/2 - pos.y};
   }

   quan::uav::osd::pxp_type 
   quan::uav::osd::transform_from_raw(pxp_type const & raw_pos)
   {
       auto const ds = video_cfg::get_display_size_px();
       return {raw_pos.x - static_cast<int32_t>(ds.x)/2 ,
               static_cast<int32_t>(ds.y) /2 - raw_pos.y};
   }
   void quan::uav::osd::set_pixel_raw(pxp_type const & px,colour_type c)
   {
      video_buffers::osd::xy_to_buf(px,static_cast<uint8_t>(c));
   }
//#########################TODO NO Floodfill yet!####################
   quan::uav::osd::colour_type    
   quan::uav::osd::get_pixel_raw(pxp_type const & px)
   {
      return  quan::uav::osd::colour_type::transparent;// get_device().get_pixel_raw(px);
   }
//#############################################

   quan::uav::osd::colour_type 
   quan::uav::osd::get_pixel_raw(bitmap_ptr bmp,pxp_type const & pos)
   {
      return bmp->get_pixel_colour(pos);
   }
#if 0
  ####################TODO#####################
   void 
   quan::uav::osd::set_clip_rect(pxp_type const & minimums, pxp_type const & maximums)
   {
       get_device().set_clip_rect(minimums,maximums);
   }
#endif
   quan::uav::osd::size_type
   quan::uav::osd::get_display_size()
   {
      return {
         static_cast<int32_t>(video_cfg::get_display_size_x_px()),
         static_cast<int32_t>(video_cfg::get_display_size_y_px())
      };
   }
        // fonts
  
   quan::uav::osd::pxp_type 
   quan::uav::osd::get_size( font_ptr p)
   {
      return p->get_size();
   }

   quan::uav::osd::bitmap_ptr 
   quan::uav::osd::get_char(font_ptr f, int32_t i)
   {
      return f->get_char_at(i);
   }

   quan::uav::osd::pxp_type 
   quan::uav::osd::get_size( bitmap_ptr p)
   {
      return p->get_size();
   }

// prob should put these in separate file ....

   quan::uav::osd::position_type
   quan::uav::osd::get_aircraft_position()
    {
       the_aircraft.mutex_acquire(); 
         quan::uav::osd::position_type const p = the_aircraft.location;
       the_aircraft.mutex_release();
       return p;
    }

    quan::uav::osd::position_type 
    quan::uav::osd::get_home_position()
    {
      the_aircraft.mutex_acquire();
         quan::uav::osd::position_type const p = the_aircraft.home_location;
      the_aircraft.mutex_release();
      return p;
    }

    quan::uav::osd::attitude_type
    quan::uav::osd::get_aircraft_attitude()
    {
       the_aircraft.mutex_acquire();
         quan::uav::osd::attitude_type const a= the_aircraft.attitude;
       the_aircraft.mutex_release();
       return a;
    }

    quan::uav::osd::angle_type 
    quan::uav::osd::get_aircraft_heading()
    {
      // atomic
        return the_aircraft.heading;
    }

    quan::velocity_<float>::m_per_s
     quan::uav::osd::get_airspeed()
     {
       // atomic
         return the_aircraft.airspeed;
     }

    quan::velocity_<float>::m_per_s
     quan::uav::osd::get_groundspeed()
     {
       // atomic
         return the_aircraft.groundspeed;
     }

   bool quan::uav::osd::position_is_good()
   {
      // atomic
      return (the_aircraft.gps.fix_type > 1);
   }

   namespace {
      bool home_position_set = false;
   }

   bool quan::uav::osd::home_position_is_set()
   {
      return home_position_set == true;
   }

   // even with force position must be good
   // use force to set home pos in flight
   bool quan::uav::osd::set_home_position(position_type const & p, bool force)
   {
       if ( ((home_position_set == false) || (force == true)) &&
           (quan::uav::osd::position_is_good()) ){
          the_aircraft.mutex_acquire();
            the_aircraft.home_location = p;
          the_aircraft.mutex_release();
          home_position_set = true;
          return true;
       }else{
          return false;
       }
   }

   uint8_t quan::uav::osd::read_gps_fix_type()
   {
      //atomic
      return the_aircraft.gps.fix_type;

   }

   uint8_t quan::uav::osd::read_gps_num_sats()
   {
       // atomic
       return the_aircraft.gps.num_sats;
   }
   quan::length_<uint16_t>::cm
   quan::uav::osd::read_gps_hdop()
   {
      //atomic
      return the_aircraft.gps.hdop;

   }
   // for now!
   uint8_t quan::uav::osd::get_num_batteries()
   {
      return 1;
   }
   const char* const quan::uav::osd::get_battery_name(uint8_t n)
   {
      return "Flt1";
   }

   quan::voltage_<float>::V  quan::uav::osd::get_battery_voltage(uint8_t n)
   {
      return the_aircraft.battery_voltage;
   }

   quan::current_<float>::A  quan::uav::osd::get_battery_current(uint8_t n)
    {
       return the_aircraft.battery_current;
    }

   float quan::uav::osd::get_battery_remaining(uint8_t n)
    {
       return the_aircraft.battery_remaining;
    }


   uint8_t quan::uav::osd::get_base_mode()
    {
       //atomic
       return the_aircraft.base_mode;

    }

   uint8_t quan::uav::osd::get_custom_mode()
    {
       //atomic
       return the_aircraft.custom_mode;

    }

   uint16_t quan::uav::osd::get_rssi()
   {
      return the_aircraft.rc_raw_rssi;
   }

   uint16_t quan::uav::osd::get_rc_raw_chan(uint8_t channel)
   {
      return the_aircraft.rc_raw_chan[channel];
   }

   quan::length_<float>::m
   quan::uav::osd::get_baro_alt()
        {
          // atomic
            return the_aircraft.baro_alt;
        }
