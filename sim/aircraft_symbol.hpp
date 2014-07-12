#ifndef AIRCRAFT_SYMBOL_HPP_INCLUDED
#define AIRCRAFT_SYMBOL_HPP_INCLUDED

#include <quan/gx/primitives/polyline.hpp>
#include <quan/two_d/vect.hpp>
#include <quan/two_d/rotation.hpp>
#include <quan/three_d/vect.hpp>
#include <quan/length.hpp>
#include <quan/gx/graphics_context.hpp>


// add a scaling so plane is bigger on small maps
struct aircraft_symbol{
    typedef quan::length::mm mm;
    typedef quan::two_d::vect<mm> vect2_mm;
    typedef quan::three_d::vect<mm> vect3_mm;
  aircraft_symbol(vect3_mm const & pos, quan::angle::rad const & heading)
   : m_position(pos), m_heading(heading) { }
    static const size_t num_points = 11;
    void draw(quan::gx::graphics_context<mm> const & gx)const
   {
      vect2_mm pos{m_position.x,m_position.y};
      quan::gx::primitives::polyline<mm> poly{mm{0.5}, quan::gx::rgb::colors::blue}; 
      quan::two_d::rotation rotate{-m_heading};
      for ( size_t i = 0; i < num_points ; ++i){
         poly.push_back(rotate(m_array[i]) + pos);
      }
      gx.draw_polyline(poly);
   }
   
   static vect2_mm m_array[num_points];
   vect3_mm m_position;
   quan::angle::rad m_heading;
};

#endif // AIRCRAFT_SYMBOL_HPP_INCLUDED
