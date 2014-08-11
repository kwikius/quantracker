#ifndef QUANTRACKER_GROUND_V1_COMPASS_HPP_INCLUDED
#define QUANTRACKER_GROUND_V1_COMPASS_HPP_INCLUDED
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

/*
somehow want a token which each bus user must have before they
can go on the bus
*/

struct raw_compass{
   
   static void set_strap(int32_t val);
   static int32_t get_strap();
   static void set_filter(float const & val);
   static void clear();
   static quan::three_d::vect<float> const& get();
   static int32_t update();
   static void init();
   static void request_disable_updating();
   static void enable_updating();
   static bool updating_disabled();
  private:
   static int32_t strap_value;
   static float filter_value;
   static quan::three_d::vect<float> value;
   static bool m_request_disable_updating;
   static bool m_updating_enabled;
   

};

#endif // QUANTRACKER_GROUND_V1_COMPASS_HPP_INCLUDED
