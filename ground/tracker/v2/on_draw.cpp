
#include <cstdio>
#include <quan/uav/osd/api.hpp>
#include <quan/stm32/freertos/millis.hpp>

#include "resources.hpp"
#include "compass.hpp"

void quan::uav::osd::on_draw()
{
   int64_t now = quan::stm32::millis().numeric_value();
   int16_t ms_part = static_cast<int32_t>(now % 1000);
   now /= 1000;
   int32_t s_part = static_cast<int32_t>(now % 60);
   now /= 60;
   int32_t min_part = static_cast<int32_t>(now%60);
   now /= 60;
   int32_t h_part = static_cast<int32_t>(now);
   quan::stm32::set<heartbeat_led_pin>();
   auto font= quan::uav::osd::get_font(0);

   if (font){
      char buf[100];  
      sprintf(buf,"[%2ih:%2im:%2is:%3ims]",
         static_cast<int>(h_part),static_cast<int>(min_part),
         static_cast<int>(s_part),static_cast<int>(ms_part)
      );
      quan::uav::osd::draw_text(buf,{-160,-115},font);

      if ( raw_compass::acquire_mutex(10) == pdTRUE){
         quan::three_d::vect<float> compass_vect = raw_compass::get_raw();
         raw_compass::release_mutex();
         sprintf(buf,"raw compass = [%.3f,%.3f,%.3f]",
               static_cast<double>(compass_vect.x),
                  static_cast<double>(compass_vect.y),
                     static_cast<double>(compass_vect.z)
         );
         quan::uav::osd::draw_text(buf,{-160,100},font);
      }
   }else{
      quan::stm32::set<heartbeat_led_pin>();
   }
 }