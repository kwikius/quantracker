#ifndef QUANTRACKER_FLASH_FLASH_TYPE_TAGS_HPP_INCLUDED
#define QUANTRACKER_FLASH_FLASH_TYPE_TAGS_HPP_INCLUDED

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
// local
enum flash_type_tags {  Vect3F, Bool};

// app level create unique integer tags for each type in the eeprom structure
// accessible using these templates
template <typename T> struct type_to_id;

template <> struct type_to_id<quan::three_d::vect<float> > {
   static constexpr uint32_t value = flash_type_tags::Vect3F;
};
template <> struct type_to_id<bool> {
   static constexpr uint32_t value = flash_type_tags::Bool;
};

template <uint32_t id> struct id_to_type;

template <> struct id_to_type<flash_type_tags::Vect3F>{
   typedef quan::three_d::vect<float> type;
};

template <> struct id_to_type<flash_type_tags::Bool>{
   typedef bool type;
};



#endif // QUANTRACKER_FLASH_FLASH_TYPE_TAGS_HPP_INCLUDED
