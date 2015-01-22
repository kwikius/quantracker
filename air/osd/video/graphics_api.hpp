#ifndef OSD_GRAPHICS_API_HPP_INCLUDED
#define OSD_GRAPHICS_API_HPP_INCLUDED
/*
 Copyright (c) 2013 -2015 Andy Little 

 With Grateful Acknowledgements to the prior work of:
   Sami Korbonen(Openpilot.org)
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

#include <quan/two_d/vect.hpp>
#include "bitmap.hpp"

enum class color{ grey = 0, white = 1, black = 2, transparent = 3};

void draw_line_wu(quan::two_d::vect<float> const & p0_in, quan::two_d::vect<float> const & p1_in);
void draw_line_bres(quan::two_d::vect<float> const & p0_in, quan::two_d::vect<float> const & p1_in, color c);
void draw_text(quan::two_d::vect<float> const & p0_in, const char* str);
void set_white_pixel(quan::two_d::vect<int32_t> const & px);
void set_black_pixel(quan::two_d::vect<int32_t> const & px);
void set_grey_pixel(quan::two_d::vect<int32_t> const & px);

void set_pixel(quan::two_d::vect<int32_t> const & px, color c);

struct abc_bitmap<uint8_t>* get_font_char( char c);
void output_bitmap(abc_bitmap<uint8_t>* pic, quan::two_d::vect<int32_t> const & pos);
/*
todo arc/ circle etc
*/

void on_draw();

#endif // OSD_GRAPHICS_API_HPP_INCLUDED
