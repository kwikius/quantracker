#ifndef QUANTRACKER_FLASH_CONVERT_HPP_INCLUDED
#define QUANTRACKER_FLASH_CONVERT_HPP_INCLUDED

#include <cstdint>
#include <quan/dynarray.hpp>
#include <quan/three_d/vect.hpp>

template <typename T>
struct flash_convert;

template <> struct flash_convert<bool>{
   static bool text_to_bytestream(quan::dynarray<uint8_t>& dest, quan::dynarray<char> const & src);
   static bool bytestream_to_text(quan::dynarray<char>& dest, quan::dynarray<uint8_t> const & src);
};

template <> struct flash_convert<quan::three_d::vect<float> >{
   static bool text_to_bytestream(quan::dynarray<uint8_t>& dest, quan::dynarray<char> const & src);
   static bool bytestream_to_text(quan::dynarray<char>& dest, quan::dynarray<uint8_t> const & src);
};

#endif
