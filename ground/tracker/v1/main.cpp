
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

#include "switch_input.hpp"
#include "events.hpp"
#include "telemetry.hpp"
#include "azimuth.hpp"
#include "compass.hpp"
#include <quan/three_d/vect.hpp>


extern "C" void setup();

int main()
{
<<<<<<< Updated upstream
   setup();

=======
>>>>>>> Stashed changes
   for (;;){  
      // timer events
      if ( azimuth_pwm_calc_event.signalled()){
         azimuth::motor::do_calc();
         azimuth_pwm_calc_event.clear();
      }

      if(ms1_event.signalled()){
         switches_update();
         ms1_event.clear();
      }

      if(ms20_event.signalled()){
         on_20ms_event();
         ms20_event.clear();
      }
      // asynch events
      telemetry::parse_input();
      raw_compass::update();
   }
}

   
