#ifndef QUANTRACKER_FLASH_FLASH_TYPE_TAGS_HPP_INCLUDED
#define QUANTRACKER_FLASH_FLASH_TYPE_TAGS_HPP_INCLUDED

#include <quan/three_d/vect.hpp>

// each type in flash needs a typeid
// these are zero based and are used as indexes
// in array
enum flash_type_tags { Vect3F, Bool};

// app level create unique integer tags for each type in the flash
// accessible using these templates
template <typename T> struct type_to_id;
template <uint32_t id> struct id_to_type;

template <> struct type_to_id<quan::three_d::vect<float> > {
   static constexpr uint32_t value = flash_type_tags::Vect3F;
};
template <> struct id_to_type<flash_type_tags::Vect3F>{
   typedef quan::three_d::vect<float> type;
};
//---------
template <> struct type_to_id<bool> {
   static constexpr uint32_t value = flash_type_tags::Bool;
};
template <> struct id_to_type<flash_type_tags::Bool>{
   typedef bool type;
};
//---

#endif // QUANTRACKER_FLASH_FLASH_TYPE_TAGS_HPP_INCLUDED
