#ifndef QUANTRACKER_FLASH_FLASH_TYPE_TAGS_HPP_INCLUDED
#define QUANTRACKER_FLASH_FLASH_TYPE_TAGS_HPP_INCLUDED

enum flash_type_tags {  Vect3F, Bool};

// app level create unique integer tags for each type in the eeprom structure
// accessible using these templates
template <typename T> struct get_flash_type_tag;

template <> struct get_flash_type_tag<quan::three_d::vect<float> > {
   static constexpr uint32_t value = flash_type_tags::Vect3F;
};
template <> struct get_flash_type_tag<bool> {
   static constexpr uint32_t value = flash_type_tags::Bool;
};

#endif // QUANTRACKER_FLASH_FLASH_TYPE_TAGS_HPP_INCLUDED
