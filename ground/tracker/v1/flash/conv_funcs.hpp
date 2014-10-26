#ifndef QUAN_CPP_PROJECT_TEST_CONV_FUNCS_HPP_INCLUDED
#define QUAN_CPP_PROJECT_TEST_CONV_FUNCS_HPP_INCLUDED

#include <quan/dynarray.hpp>
#include <cstdint>

/*
  actually these generic functions so could move to quan
  Needs proper API
*/

bool is_defined_flash_mag_offset();
bool get_flash_mag_offset(quan::three_d::vect<float> & dest);
bool set_flash_mag_offset(quan::three_d::vect<float> const & src);

bool is_defined_flash_use_compass();
bool get_flash_use_compass(bool & dest);
bool set_flash_use_compass(bool const & src);

bool rep_to_cstring_Vect3F(quan::dynarray<char>& dest, quan::dynarray<uint8_t> const & src);
bool cstring_to_rep_Vect3F(quan::dynarray<uint8_t>& dest, quan::dynarray<char> const & src);

bool rep_to_cstring_Bool(quan::dynarray<char>& dest, quan::dynarray<uint8_t> const & src);
bool cstring_to_rep_Bool(quan::dynarray<uint8_t>& dest, quan::dynarray<char> const & src);

#endif // QUAN_CPP_PROJECT_TEST_CONV_FUNCS_HPP_INCLUDED
