#ifndef QUAN_CPP_PROJECT_TEST_CONV_FUNCS_HPP_INCLUDED
#define QUAN_CPP_PROJECT_TEST_CONV_FUNCS_HPP_INCLUDED

#include <quan/dynarray.hpp>
#include <cstdint>

bool get_flash_mag_offset(quan::three_d::vect<float> & dest);
bool set_flash_mag_offset(quan::three_d::vect<float> const & src);

bool get_flash_use_compass(bool & dest);
bool set_flash_use_compass(bool const & src);

#endif // QUAN_CPP_PROJECT_TEST_CONV_FUNCS_HPP_INCLUDED
