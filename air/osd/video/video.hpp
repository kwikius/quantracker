#ifndef QUAN_STM32_OSD_VIDEO_HPP_INCLUDED
#define QUAN_STM32_OSD_VIDEO_HPP_INCLUDED

#include <cstdint>
#include <quan/two_d/vect.hpp>
#include <quan/time.hpp>
#include "video_setup.hpp"
#include "video_buffer.hpp"
#include <quan/small_rational.hpp>
// after openpilot oposd etc
struct video_t{
   // full visible screen width and height not including any non osd border
   // storage aspect_ratio
   // display_aspect_ratio Usually 4:3
   //

   static quan::two_d::vect<uint32_t> get_display_size_px()
   {
     return {get_display_size_x_px(),get_display_size_y_px()};
   }
   // full visible number of columns
   // adjusted to multiple of 8
   static uint32_t get_display_size_x_px()
   {
      return (video_cfg::columns::osd::get_num_pixels()/8U)*8U;
   }
   // full visible number of rows
   static uint32_t get_display_size_y_px()
   {
       return video_cfg::rows::osd::get_visible_length();
   }
   // dump last pixels if not a multiple of 8
   static uint32_t get_display_size_x_bytes()
   {
       return get_display_size_x_px() / 8U;
   }

};

#endif // QUAN_STM32_OSD_VIDEO_H_INCLUDED
