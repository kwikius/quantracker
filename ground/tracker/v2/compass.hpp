#ifndef QUANTRACKER_GROUND_V2_COMPASS_HPP_INCLUDED
#define QUANTRACKER_GROUND_V2_COMPASS_HPP_INCLUDED
/*
 Copyright (c) 2003-2014 Andy Little.

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program. If not, see http://www.gnu.org/licenses./
 */

#include <quan/three_d/vect.hpp>
#include <quan/angle.hpp>

#include "FreeRTOS.h"
#include <task.h>
#include <semphr.h>

struct raw_compass{
   
   static void set_strap(int32_t val);
   static int32_t get_strap();
   static void set_filter(float const & val);
   static void clear();
   static quan::three_d::vect<float> get_vect();
   static quan::three_d::vect<float> const& get_raw();
   static quan::three_d::vect<float> const& get_offset();
   static bool get_use_compass() { return get_compass_offset_set() && m_use_compass;}
   static int32_t update();
   static void init();
   static void request_disable_updating();
   static void enable_updating();
   static bool updating_disabled();
   static void set_use_compass( bool val);
   static void set_mag_offset(quan::three_d::vect<float> const & offset);
   static bool get_bearing( quan::angle::deg & bearing_out); 
   static bool get_compass_offset_set(){return m_compass_offset_set;}
   static bool acquire_mutex(TickType_t t);
   static void release_mutex();
  private:
   static int32_t m_strap_value;
   static float m_filter_value;
   // value without offsets
   static quan::three_d::vect<float> m_raw_value;
   static bool m_request_disable_updating;
   static bool m_updating_enabled;
   static bool m_use_compass;
   static quan::three_d::vect<float> m_offset;
   static bool m_compass_offset_set;
   static SemaphoreHandle_t m_mutex;
  
   

};

#endif // QUANTRACKER_GROUND_V2_COMPASS_HPP_INCLUDED
