#ifndef QUANTRACKER_SIM_DRAWING_HPP_INCLUDED
#define QUANTRACKER_SIM_DRAWING_HPP_INCLUDED

#include <quan/gx/drawing.hpp>
#include <quan/length.hpp>

struct drawing : quan::gx::drawing<quan::length::mm>{
   typedef quan::length::mm mm;;
   using quan::gx::drawing<mm>::vect;
   drawing(){}
   vect get_size() const;
   vect get_origin() const {return vect{mm{0},mm{0}};}
};


#endif
