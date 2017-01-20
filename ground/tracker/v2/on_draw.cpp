
#include <cstdio>
#include <quan/uav/osd/api.hpp>
#include <quan/stm32/freertos/millis.hpp>

#include "resources.hpp"
#include "compass.hpp"

namespace {

quan::uav::osd::font_ptr font = nullptr;
}

void quan::uav::osd::on_draw()
{
   if ( font == nullptr){
      font = quan::uav::osd::get_font(0);
   }
   static quan::time_<int64_t>::ms prev_time{0};
   quan::time_<int64_t>::ms time_now = quan::stm32::millis();
   if ((time_now - prev_time) > quan::time_<int64_t>::ms{250}){
      prev_time = time_now;
     // quan::stm32::complement<heartbeat_led_pin>();
   }
   int64_t now = time_now.numeric_value();
   int16_t ms_part = static_cast<int32_t>(now % 1000);
   now /= 1000;
   int32_t s_part = static_cast<int32_t>(now % 60);
   now /= 60;
   int32_t min_part = static_cast<int32_t>(now%60);
   now /= 60;
   int32_t h_part = static_cast<int32_t>(now);

   char buf[100];  
   sprintf(buf,"[%2ih:%2im:%2is:%3ims]",
      static_cast<int>(h_part),static_cast<int>(min_part),
      static_cast<int>(s_part),static_cast<int>(ms_part)
   );
   quan::uav::osd::draw_text(buf,{-150,-55},font);
   vTaskSuspendAll();
   if ( raw_compass::acquire_mutex(1) == pdTRUE){
      quan::three_d::vect<float> compass_vect = raw_compass::get_raw();
      raw_compass::release_mutex();
      xTaskResumeAll();
      char constexpr dim[] = {'x','y','z'};
      for ( uint32_t i = 0; i < 3; ++i){
         sprintf(buf,"%c = %.3f",dim[i],static_cast<double>(compass_vect[i]));
         pxp_type pos{-160,80 - ((get_size(font).y +4) * i) };
         quan::uav::osd::draw_text(buf,pos,font);
      }
   }else{
      xTaskResumeAll();
   }
 }