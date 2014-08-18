#ifndef CPP_PROJECTS_TEST_FLASH_IMPL_HPP_INCLUDED
#define CPP_PROJECTS_TEST_FLASH_IMPL_HPP_INCLUDED

#include <cstdint>
#include <quan/dynarray.hpp>
#include <quan/stm32/flash.hpp>

#include "flash_error.hpp"

namespace flash_symtab{

      int32_t get_index( quan::dynarray<char> const & symbol_name);
      const char* get_name(uint16_t symindex);
      uint16_t get_size(uint16_t sym_index);
      const char* get_info(uint16_t symindex);
      bool get_readonly_status(uint16_t symidx,bool & result);
      bool read_to_string(uint16_t symbol_index, quan::dynarray<char> & value);
      bool write_from_string (uint16_t symbol_index,quan::dynarray<char> const & value);
      bool write(uint16_t symidx, quan::dynarray<uint8_t> const & symbol_value);
     // ideally symbol_value is prepared with a size from get_flash_symbol_size
      bool read (uint16_t symidx, quan::dynarray<uint8_t> & symbol_value);
      bool exists(uint16_t symidx);
  
      uint16_t get_num_elements();
      typedef bool (*pfn_cstring_to_rep)(quan::dynarray<uint8_t>& dest, quan::dynarray<char> const & src);
      typedef bool (*pfn_rep_to_cstring)(quan::dynarray<char>& dest, quan::dynarray<uint8_t> const & src);

      pfn_cstring_to_rep get_string_to_rep_fun( uint16_t symidx);
      pfn_rep_to_cstring get_rep_to_string_fun( uint16_t symidx);
      bool init();

}//namespace flash_symtab

#endif // CPP_PROJECTS_TEST_FLASH_IMPL_HPP_INCLUDED
