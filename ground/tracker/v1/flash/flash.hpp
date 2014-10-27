#ifndef QUANTRACKER_FLASH_FLASH_HPP_INCLUDED
#define QUANTRACKER_FLASH_FLASH_HPP_INCLUDED

#include <cstdint>
#include <quan/dynarray.hpp>
#include <quan/three_d/vect.hpp>
#include <quan/stm32/flash.hpp>
#include "flash_error.hpp"

namespace flash_symtab{

      // get the index of the symbol name in the dynarray
      int32_t get_index( quan::dynarray<char> const & symbol_name);
      // get index of the symbol name cstring
      int32_t get_index ( const char* symbol_name);
      // return the name of the index
      const char* get_name(uint16_t symindex);
      // get the storage size of the index
      uint16_t get_size(uint16_t sym_index);
      // get the text info blurb about the index
      const char* get_info(uint16_t symindex);
      // is it readonly
       // TODO sort force writing
      bool get_readonly_status(uint16_t symidx,bool & result);
       // get the symbol value to a user string representation
      bool read_to_string(uint16_t symbol_index, quan::dynarray<char> & value);
      // write from text rep
      bool write_from_string (uint16_t symbol_index,quan::dynarray<char> const & value);
      // write to flash as byte stream in symbol_value
      bool write(uint16_t symidx, quan::dynarray<uint8_t> const & symbol_value);
     // ideally symbol_value is prepared with a size from get_size
      // read symbol to byte stream in symbol value
      bool read (uint16_t symidx, quan::dynarray<uint8_t> & symbol_value);
      // does the given symbol index exist
      bool exists(uint16_t symidx);
      // is the symbol defined in flash
      bool is_defined(const char* symbol_name);
      uint16_t get_num_elements();
      // convesrion from user text to byte stream
      typedef bool (*pfn_text_to_bytestream)(quan::dynarray<uint8_t>& dest, quan::dynarray<char> const & src);
      // conversion from byte_stream to user text rep
      typedef bool (*pfn_bytestream_to_text)(quan::dynarray<char>& dest, quan::dynarray<uint8_t> const & src);
      // return the string to rep stream fun for the index
      pfn_text_to_bytestream get_text_to_bytestream_fun( uint16_t symidx);
      // return the rep stream to string fun for the index
      pfn_bytestream_to_text get_bytestream_to_text_fun( uint16_t symidx);
      // at startup check that flash is initialised
      // and then read symbols to their runtime places from flash
      bool init();

}//namespace flash_symtab

#endif // QUANTRACKER_FLASH_FLASH_HPP_INCLUDED
