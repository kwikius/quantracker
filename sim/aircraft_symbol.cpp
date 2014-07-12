#include "aircraft_symbol.hpp"
    
quan::two_d::vect<quan::length::mm> 
aircraft_symbol::m_array[aircraft_symbol::num_points] =  
{
   {mm{0},mm{0 + 230}},
   {mm{200},mm{-165 + 230}},
   {mm{600},mm{-410 + 230}},
   {mm{600},mm{-560 + 230}},
   {mm{200},mm{-395 + 230}},
   {mm{0},mm{-450 + 230}},
   {mm{-200},mm{-395 + 230}},
   {mm{-600},mm{-560 + 230}},
   {mm{-600},mm{-410 + 230}},
   {mm{-200},mm{-165 + 230}},
   {mm{0},mm{0 + 230}}
  
};

