#ifndef QUANTRACKER_GROUND_V1_COMPASS_HPP_INCLUDED
#define QUANTRACKER_GROUND_V1_COMPASS_HPP_INCLUDED

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
