#ifndef QUANTRACKER_GROUND_V1_1_AZIMUTH_ENCODER_HPP_INCLUDED
#define QUANTRACKER_GROUND_V1_1_AZIMUTH_ENCODER_HPP_INCLUDED

/*
 Copyright (c) 2017 Andy Little 

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

#include <quan/angle.hpp>
#include "../system/tracker_resources.hpp"

struct azimuth_encoder{

   typedef  azimuth_encoder_ch1_pin    ch1_pin;
   typedef  azimuth_encoder_ch2_pin    ch2_pin;
   typedef  azimuth_quadrature_counter  counter;

   static uint32_t get_steps_per_revolution(){ return m_steps_per_revolution;}

   static bool is_indexed()  { return m_is_indexed;}
   static uint32_t get_index() { return counter::get()->cnt; }
   static bool set_index( uint32_t value);

   static quan::angle::rad angle_of_one_step() { return quan::angle::two_pi / get_steps_per_revolution();}

   static uint32_t bearing_to_encoder(quan::angle::rad const & angle_in)
   {
      return static_cast<uint32_t>(
      ( unsigned_modulo(angle_in) * get_steps_per_revolution() ) / quan::angle::two_pi
      );
   }
   // precondition: encoder_index is < azimuth_encoder::get_steps_per_revolution()
   static quan::angle::rad encoder_to_bearing(uint32_t encoder_index)
   {
      return (encoder_index * quan::angle::two_pi) / get_steps_per_revolution();
   }

private:
   static void un_set_index() { m_is_indexed = false;}
   static void set_steps_per_revolution(uint32_t val)
   { 
      // check greater than some minimum e.g 3 stesps per revolution
      m_steps_per_revolution = val;
   }
   friend struct      azimuth_servo;
   static void        setup();
   static uint32_t    m_steps_per_revolution;
   static bool        m_is_indexed;
};

#endif // QUANTRACKER_GROUND_V1_1_AZIMUTH_ENCODER_HPP_INCLUDED
