
/*
 Copyright (c) 2013 Andy Little 

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

#include <quan/stm32/tim.hpp>
#include "azimuth.hpp"

void azimuth::motor::enable()
{
   m_enabled = true;
}

void azimuth::motor::disable()
{
   m_enabled = false;
}

bool azimuth::motor::enabled()
{
   return m_enabled == true;
}

int32_t azimuth::motor::get_actual_position()
{
   return static_cast<int32_t>(azimuth::encoder::counter::get()->cnt);
}

int32_t azimuth::motor::get_target_position()
{
   return m_target_position;
}

void  azimuth::motor::set_target_position(uint32_t val)
{
   m_target_position = static_cast<int32_t>(val) % static_cast<int32_t>(azimuth::encoder::counts_rev());
}

int32_t azimuth::motor::bearing_to_encoder( quan::angle::deg bearing)
{
     while( bearing < quan::angle::deg{0}){
         bearing += quan::angle::deg{360};
     }

     while ( bearing > quan::angle::deg{360}){
         bearing -= quan::angle::deg{360};
     }
  // if (reverse) ...
     auto const norm_angle = quan::angle::deg{360} - bearing;
     int32_t const val =  static_cast<int32_t>( norm_angle * (azimuth::encoder::counts_rev() / quan::angle::deg{360}));
     return val % static_cast<int32_t>(azimuth::encoder::counts_rev());
}

quan::angle::deg azimuth::motor::encoder_to_bearing(uint32_t encoder_val)
{
   // do modulo?
  // encoder_val %= azimuth::encoder::counts_rev() ;
   quan::angle::deg const raw_angle = (encoder_val * quan::angle::deg{360}) / azimuth::encoder::counts_rev() ;
   //if reverse...
   return  quan::angle::deg{360} - raw_angle;
}

void azimuth::motor::set_azimuth(quan::angle::deg angle)
{
     while( angle < quan::angle::deg{0}){
         angle += quan::angle::deg{360};
     }

     while ( angle > quan::angle::deg{360}){
         angle -= quan::angle::deg{360};
     }
  // if (reverse) ...
      auto const norm_angle = quan::angle::deg{360} - angle;
      set_target_position( static_cast<uint32_t>( norm_angle * (azimuth::encoder::counts_rev() / quan::angle::deg{360}) ) );
}

quan::angle::deg azimuth::motor::get_target_azimuth()
{
   auto const raw_angle = (get_target_position() * quan::angle::deg{360} ) / azimuth::encoder::counts_rev() ;
   //if reverse...
   return raw_angle - quan::angle::deg{360};
}

using namespace quan::stm32;

namespace{

   void update_duty_cycle(float val)
   {
      // to raise duty cycle
      // set a direction value for irq to update at next end of cycle
      // and a flag
      auto const direction = (val < 0) ? false:true;
      float const abs_val = fabs(val);
      float const max_capped_val = fminf( abs_val, azimuth::motor::m_max_duty_cycle_percent);
      float const min_capped_val = ( max_capped_val > azimuth::motor::get_min_duty_cycle()) ? max_capped_val : 0 ;

      main_loop::timer::get()->ccr1 = static_cast<uint16_t>(min_capped_val* main_loop::max_count);
      // update the direction too.
      if (direction == true){
         apply<azimuth::motor::direction_pin, gpio::ostate::high>();
         apply<azimuth::motor::not_direction_pin, gpio::ostate::low>();
      }else{
         apply<azimuth::motor::not_direction_pin, gpio::ostate::high>();
         apply<azimuth::motor::direction_pin, gpio::ostate::low>();
      }
   }

   constexpr int32_t one_rev = static_cast<int32_t>(azimuth::encoder::counts_rev());
   // if loop is 50 Hz then should be ok here
   constexpr int32_t near_count = one_rev / 4;

   inline bool overflow(int32_t old_val,int32_t new_val)
   {
      return  (old_val > (one_rev - near_count)) && ( new_val < near_count) ;
   }

   inline int32_t get_speed(int32_t old_val, int32_t new_val)
   {
      if (overflow(old_val,new_val)){
         return (one_rev - old_val) + new_val;
      }else {
         if (overflow(new_val,old_val)){
            return new_val - ( old_val + one_rev );
         }else{
            return new_val - old_val;
         }
      }
   }

   inline int32_t get_distance(int32_t cur, int32_t targ)
   {
      int32_t const val = targ - cur;
      if ( val > ( one_rev / 2 ) ){
         return val - one_rev;
      }else {
         if ( val < -one_rev/2  ){
            return val + one_rev;
         }else{
            return val;
         }
      }
   }
} // namspace

//always in interrupt
/*

*/
void azimuth::motor::do_calc()
{
   auto const oldpos = m_last_recorded_position;
   m_last_recorded_position = get_actual_position();

   // rotational speed
   auto const speed = get_speed(oldpos,m_last_recorded_position);
   // angle from last position to next position
   auto const dist = get_distance(m_last_recorded_position,m_target_position);
   // could put these into 
   static float kI = 0;
   static float kI_incr = 0.00002;
   constexpr int32_t min_dist = 5;
   constexpr int32_t min_speed = 1;
     //  integral term should only be incred when enables
     // and should be capped
    if ( ( abs(dist) > min_dist ) &&  ( abs(speed) < min_speed )  ){
      kI +=  kI_incr * dist;
    }else {
      kI = 0.f;
    }

   if ( enabled()){
      update_duty_cycle( m_kP * dist - m_kD * speed  + kI);
   }
   else{
      update_duty_cycle( 0.f );
   }
}
