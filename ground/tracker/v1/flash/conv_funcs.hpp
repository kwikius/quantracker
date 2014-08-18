#ifndef QUAN_CPP_PROJECT_TEST_CONV_FUNCS_HPP_INCLUDED
#define QUAN_CPP_PROJECT_TEST_CONV_FUNCS_HPP_INCLUDED

#include <quan/dynarray.hpp>
#include <cstdint>

bool rep_to_cstring_MagVect3F(quan::dynarray<char>& dest, quan::dynarray<uint8_t> const & src);
bool cstring_to_rep_MagVect3F(quan::dynarray<uint8_t>& dest, quan::dynarray<char> const & src);

bool rep_to_cstring_Char(quan::dynarray<char>& dest, quan::dynarray<uint8_t> const & src);
bool cstring_to_rep_Char(quan::dynarray<uint8_t>& dest, quan::dynarray<char> const & src);

bool rep_to_cstring_CharAr8(quan::dynarray<char>& dest, quan::dynarray<uint8_t> const & src);
bool cstring_to_rep_CharAr8(quan::dynarray<uint8_t>& dest, quan::dynarray<char> const & src);

#endif // QUAN_CPP_PROJECT_TEST_CONV_FUNCS_HPP_INCLUDED
