
#include "graphics_api.hpp"
#include "video_buffer.hpp"
#include "video_cfg.hpp"
#include <quan/stm32/push_pop_fp.hpp>

//<black, white> 0 = on
void set_white_pixel(quan::two_d::vect<int32_t> const & px)
{
   video_buffers::osd::xy_to_buf(px,0b01);
}

void set_black_pixel(quan::two_d::vect<int32_t>const & px)
{
   video_buffers::osd::xy_to_buf(px,0b10);
}

void set_grey_pixel(quan::two_d::vect<int32_t> const & px)
{
   video_buffers::osd::xy_to_buf(px,0b00);
}

void set_pixel(quan::two_d::vect<int32_t> const & px, color c)
{
   video_buffers::osd::xy_to_buf(px,static_cast<uint8_t>(c));
}

void output_bitmap(abc_bitmap<uint8_t>* pic, quan::two_d::vect<int32_t> const & pos)
{
   if ( !pic){
      return ;
   }
   quan::two_d::vect<uint32_t> size_px = pic->get_size_px();
   uint32_t const bits_px = pic->get_bits_px();
   uint8_t const * data = pic->get_data();
   for ( uint32_t y = 0U; y < size_px.y; ++y){
      uint32_t const y_bit_pos = y *size_px.x * bits_px;
      for ( uint32_t x = 0U; x < size_px.x; ++x){
         uint32_t const px_pos = y_bit_pos + x * bits_px;
         uint32_t const byte_pos = px_pos / 8U;
         uint32_t const bit_pos = px_pos % 8U;
         uint8_t const val = static_cast<uint8_t>((data[byte_pos] >> bit_pos) & 0b11)  ;
         if (val != 0b11) {
            set_pixel(pos + quan::two_d::vect<int32_t>{x,y},static_cast<color>(val));
         }
      }
   }
}

void draw_text(quan::two_d::vect<float> const & p0_in, const char* str)
{
   if (str){
      quan::two_d::vect<int32_t> pos{p0_in.x + 0.5f, p0_in.y +0.5f};
      for (const char* ptr = str; *ptr != '\0'; ++ptr) {
         abc_bitmap<uint8_t>* fontch = get_font_char (*ptr);
         if (fontch) {
            output_bitmap (fontch,pos);
            pos.x += fontch->get_size_px().x;
         }
      }
   }
}

namespace{

int32_t ipart( float const & in)
{
   float int_part;
   std::modf(in, &int_part);
   return static_cast<int32_t>(int_part);
}
int32_t round( float const & in)
{
   return ipart(in + 0.5f);
}

float fpart(float const & in)
{
   float int_part;
   return std::modf(in, &int_part);
}

float rfpart( float const & in)
{
   return 1.f - fpart(in);
}

color get_color(float const & value)
{
#if (1)
   if ( value > 2.f/3){
      return color::black;
   }else{
      if (value > 1.f/3){
         return color::grey;
      }else{
         return color::white;
      }
   }
#else
 if ( value > 0.5f){
      return color::black;
   }else{
     return color::grey;
   }

#endif
}

}//namespace

void draw_line_bres(quan::two_d::vect<float> const & p0_in, quan::two_d::vect<float> const & p1_in, color c)
{

 //  quan::stm32::push_FPregs();

	// Based on http://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
	bool const steep = fabs(p1_in.y - p0_in.y) > fabs(p1_in.x - p0_in.x);

  // quan::stm32::pop_FPregs();


   quan::two_d::vect<float> p0;
   quan::two_d::vect<float> p1;
	if (!steep) {
      p0 = p0_in;
      p1 = p1_in;
   }else{
       p0 = quan::two_d::vect<float>{  p0_in.y,p0_in.x};
       p1 = quan::two_d::vect<float>{  p1_in.y,p1_in.x};
   }

   if ( p0.x > p1.x){
      quan::two_d::vect<float> const tmp = p0;
      p0 = p1;
      p1 = tmp;
   }

	float const deltax = p1.x - p0.x;
	float const deltay = fabs(p1.y - p0.y);
	float error = deltax / 2.f;
	int32_t const ystep = (p0.y < p1.y)?1:-1;
	int32_t y = static_cast<int32_t>(p0.y + 0.5f);

	for (int32_t x = static_cast<int32_t>(p0.x +0.5f), end = static_cast<int32_t>(p1.x + 0.5f);x < end ; ++x) {
		if (!steep) {
         set_pixel({x,y},c);
          asm volatile ("nop":::);
      }else{
			set_pixel({y,x},c);
           asm volatile ("nop":::);
		}
		error -= deltay;
		if (error < 0.f) {
			y     += ystep;
			error += deltax;
		}
	}
}

void draw_line_wu(
      quan::two_d::vect<float> const & p0_in, quan::two_d::vect<float> const & p1_in)
{
   bool const steep = abs(p1_in.y-p0_in.y) > abs(p1_in.x- p0_in.x);
   quan::two_d::vect<float> p0;
   quan::two_d::vect<float> p1;
   if (steep){
      p0 = quan::two_d::vect<float>{p0_in.y, p0_in.x};
      p1 = quan::two_d::vect<float>{p1_in.y, p1_in.x};
   }else{
      p0 = p0_in;
      p1 = p1_in;
   }
   if ( p0.x > p1.x){
      auto temp = p0;
      p0 = p1;
      p1 = temp;
   }
   auto delta = p1 - p0;
   auto gradient = delta.y/delta.x;

   // int
   int32_t xend = round(p0.x);
   // float
   auto yend = p0.y + gradient *(xend - p0.x);
   // float
   auto xgap = rfpart(p0.x + 0.5f);
   //int
   int32_t xpxl1 = xend;
   // int
   int32_t ypxl1 = ipart(yend);
 
   if ( steep ){
      set_pixel(quan::two_d::vect<int32_t>{ypxl1,xpxl1},get_color(rfpart(yend) * xgap));
      set_pixel(quan::two_d::vect<int32_t>{ypxl1+1,xpxl1},get_color(fpart(yend) * xgap));
   }else{
      set_pixel(quan::two_d::vect<int32_t>{xpxl1,ypxl1},get_color(rfpart(yend) * xgap));
      set_pixel(quan::two_d::vect<int32_t>{xpxl1+1,ypxl1},get_color(fpart(yend) * xgap));
   }

   auto intery = yend + gradient;
//int
   xend = round(p1.x);
   yend = p1.y + gradient * (xend - p1.x);
   xgap = fpart(p1.x + 0.5f);
//int
   auto xpxl2 = xend;
//int
   auto ypxl2 = ipart(yend);
   if (steep ){
      set_pixel(quan::two_d::vect<int32_t>{ypxl2,xpxl2},get_color(rfpart(yend) * xgap));
      set_pixel(quan::two_d::vect<int32_t>{ypxl2+1,xpxl2},get_color(fpart(yend) * xgap));
   }else{
      set_pixel(quan::two_d::vect<int32_t>{xpxl2,ypxl2},get_color(rfpart(yend) * xgap));
      set_pixel(quan::two_d::vect<int32_t>{xpxl2+1,ypxl2},get_color(fpart(yend) * xgap));
   }

   // main loop
   for ( int32_t x = xpxl1+1; x < xpxl2; ++x){
      if (steep){
         set_pixel(quan::two_d::vect<int32_t>{ipart(intery),x},get_color(rfpart(intery)));
         set_pixel(quan::two_d::vect<int32_t>{ipart(intery) +1,x},get_color(fpart(intery)));
      }else{
         set_pixel(quan::two_d::vect<int32_t>{x,ipart(intery)},get_color(rfpart(intery)));
         set_pixel(quan::two_d::vect<int32_t>{x,ipart(intery) +1,},get_color(fpart(intery)));
      }
      intery += gradient;
   }
}



