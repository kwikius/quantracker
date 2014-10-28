#ifndef OSD_GRAPHICS_API_HPP_INCLUDED
#define OSD_GRAPHICS_API_HPP_INCLUDED

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
void draw_setup();
void draw_loop();
bool on_draw();

#endif // OSD_GRAPHICS_API_HPP_INCLUDED
