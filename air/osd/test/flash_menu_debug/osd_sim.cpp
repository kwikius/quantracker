
#include <cstring>
#include <cassert>
#include <iostream>
#include <string>
#include <fstream>
#include <quan/utility/fifo.hpp>
#include <quan/error.hpp>
#include <quan/user.hpp>
#include <quan/stm32/detail/flash.hpp>

namespace {

   uint32_t page_size = 0;

   uint8_t* pageA_array = nullptr;
   uint8_t* pageB_array  = nullptr;

   uint8_t* page1_array = nullptr;
   uint8_t* page2_array = nullptr;
}

bool init_flash_pages(uint32_t page_size_in)
{
    pageA_array = (uint8_t*) malloc(page_size_in);
    pageB_array = (uint8_t*) malloc(page_size_in);

    assert( ((pageA_array != nullptr) && (pageB_array!= nullptr)) && " invalid memory");
    page_size = page_size_in;
 // The arrays relative positions in memory can vary dependednt on optimisation level
    page1_array = (pageA_array < pageB_array)? pageA_array: pageB_array;
    page2_array = (pageA_array < pageB_array)? pageB_array: pageA_array;
    return true;
}

bool quan::stm32::flash::detail::erase(int32_t page_id)
{
   if ( page_id == 1){
      memset(page1_array,0xFF,page_size);
      return true;
   }
  
   if (page_id == 2){
      memset(page2_array,0xFF,page_size);
      return true;
   }
   
   return false;
}

uint32_t quan::stm32::flash::detail::get_page_size(int32_t page_num)
{
   if( (page_num == 1) || (page_num == 2)){
     return page_size;
   }else{
     return 0U;
   }
}

void * quan::stm32::flash::detail::get_page_address(int32_t page_id)
{
   if ( page_id ==1){
      return page1_array;
   }
   if ( page_id ==2){
      return page2_array;
   }
   return nullptr;
}

namespace {

    bool check_valid_flash_address(uint8_t * p)
   {
      return ( p >= ((uint8_t*)quan::stm32::flash::detail::get_page_address(1)))
         && ( p < (((uint8_t*)quan::stm32::flash::detail::get_page_address(2)) 
            + quan::stm32::flash::detail::get_page_size(2)))
      ;
   }
}

bool quan::stm32::flash::detail::write 
(const volatile uint8_t* dest, const uint8_t* buf, int32_t bytes)
{
   if (( dest == nullptr) || (buf == nullptr) || (bytes == 0)){
      quan::error(quan::detail::function_id::stm32_flash_write,quan::detail::unexpected_nullptr);
      return false;
   }
   if ( !(check_valid_flash_address((uint8_t*)dest) && (check_valid_flash_address((uint8_t*)dest + bytes -1)))){
      quan::error(quan::detail::function_id::stm32_flash_write,quan::detail::stm32_invalid_flash_page);
#if defined QUAN_FLASH_DEBUG
    std::cout << "dest = " << (int) dest << " numbytes = " << bytes <<'\n';
    std::cout << "page 1 = " << (int) quan::stm32::flash::detail::get_page_address(1) <<'\n';
#endif
      return false;
   }else{
      memcpy((uint8_t*)(dest),buf, bytes);
      return true;
   }
}

bool quan::stm32::flash::detail::read 
   (uint8_t* buf,const volatile uint8_t * src, int32_t bytes)
{
   if ( (src == nullptr) || (buf == nullptr) || bytes ==0){
      quan::error(quan::detail::function_id::stm32_flash_read,quan::detail::unexpected_nullptr);
      return false;  
   }
   if ( ! ( check_valid_flash_address((uint8_t*)src) && (check_valid_flash_address((uint8_t*)src + bytes -1)))){

      quan::error(quan::detail::function_id::stm32_flash_read,quan::detail::stm32_invalid_flash_page);
      return false;  
   }
   memcpy (buf, (uint8_t const *) src,bytes);
   return true;
}
//nothing to do here ?? 
bool init_values_from_flash()
{
   return false;
}

void quan::user_message(char const* text)
{
   std::cout <<  text ;
}

namespace {

   const char* get_function_name(int32_t function_id)
   {
      switch (function_id){
         case  quan::detail::stm32_flash_read_symbol:
         return "quan::stm32::flash_read_symbol";
         case  quan::detail::stm32_flash_write_symbol:
         return "quan::stm32::flash_write_symbol";
         case  quan::detail::stm32_flash_get_write_count:
         return "quan::stm32::flash_get_write_count";
         case quan::detail::stm32_ll_flash_get_sym_ptr:
         return "stm32_ll_flash_get_sym_ptr";
         case quan::detail::stm32_ll_flash_find_end_records:
         return "stm32_ll_flash_find_end_records";
         case quan::detail::stm32_flash_text_to_type:
         return "stm32_flash_text_to_type";
         case quan::detail::stm32_flash_type_to_text:
         return "stm32_flash_type_to_text";
         case quan::detail::stm32_flash_symtab_init:
         return "stm32_flash_symtab_init";
         case quan::detail::stm32_flash_bytestream_to_type:
         return "stm32_flash_bytestream_to_type";
         case quan::detail::stm32_flash_validate:
         return "stm32_flash_validate";
         case quan::detail::stm32_flash_write:
         return "stm32_flash_write";
         case quan::detail::stm32_flash_read:
         return "stm32_flash_read";
         case quan::detail::function_id_user:
         return "function_id_user";
         default:
         return "unknown function";
      }
   }

   const char* get_error_string(int32_t error_id)
   {
      switch (error_id){
         case  quan::detail::stm32_flash_page_corrupted:
         return "flash page corrupted";
         case quan::detail::invalid_storage_size:
         return "invalid storage size";
         case quan::detail::out_of_flash_memory:
         return "out of flash memory";
         case quan::detail::out_of_heap_memory:
         return "out of heap memory";
         case quan::detail::stm32_flash_invalid_symbol_index:
         return "invalid symbol index";
         case quan::detail::stm32_flash_symbol_not_found:
         return "stm32 flash symbol not found";
         case quan::detail::recursion_too_deep:
         return "recursion too deep";
         case quan::detail::unterminated_string_constant:
         return "unterminated string constant";
         case quan::detail::stm32_invalid_flash_page:
         return "invalid flash page";
         case quan::detail::bad_float_range:
         return "bad float range";
         case quan::detail::bad_int_range:
         return "bad int range";
         case quan::detail::stm32_flash_erase_page_failed:
         return "flash_erase page failed";
         case quan::detail::stm32_unknown_flash_error:
         return "unknown_flash_error";
         case quan::detail::stm32_flash_incorrect_type:
         return "flash incorrect type";
         case quan::detail::unexpected_nullptr:
         return "unexpected nullptr";

         default:
         return "unknown error";
      }
   }

   struct error_info{
      int32_t function_id;
      int32_t error_id;
   };

   quan::fifo<error_info,16> error_fifo;
}// ~namespace

// should prob be out of flash errors...
void quan::error(int32_t function_id, int32_t error_id)
{
    if (error_fifo.is_full()){
      error_fifo.get();
    }
    error_fifo.put({function_id,error_id});
}

uint32_t quan::get_num_errors()
{
   return error_fifo.num_in_buffer();
}

void quan::report_errors()
{
   while ( ! error_fifo.is_empty()){
      error_info const & info = error_fifo.get();
      user_message("in fn ");
      user_message(get_function_name(info.function_id));
      user_message( " : ");
      user_message(get_error_string(info.error_id));
      user_message("\n");
   }
}

void quan::user_error (char const * str) {
   user_message("error : ");
   user_message(str);
   user_message("\n");
}

char quan::user_get()
{
   char ch;
   std::cin.get(ch);
   return ch;
}

uint32_t quan::user_in_avail()
{
   return 1;
}

void quan::user_flush_sptx()
{
   
}

namespace {

   void write_page(std::ostream & out, uint8_t* page, uint32_t size){
       uint32_t constexpr cols = 10;
       uint32_t cur_col = 0;
       for (uint32_t i = 0; i < size; ++i){
       if ( i != 0){
         out << ",";
       }else{
         out << "      ";
       }
       out << (uint32_t) page[i];
       if ( ++cur_col == cols){
         out << "\n      ";
         cur_col = 0;
       }
     }
   }
}
void write_to_file(std::string const & filename)
{
 
   if( page_size !=  0x4000){
      std::cout << "need 0x4000 page_size..\n";
      return;
   }
   std::ofstream out(filename);
   out << "#include <cstdint>\n";
   out << "   static __attribute__((section(\".rodata\"),used)) "; 
   out << "   uint8_t const flash_variables_array["<< (page_size * 2) << "] = {\n";
   write_page(out,page1_array, page_size);
   out << ",\n//---------------------------------------------\n";
   write_page(out,page2_array,page_size);
   out << "\n   };\n";

}


#if 0
// binary
namespace {

   void write_page(uint8_t* page, std::ostream & out)
   {
      for (uint32_t i = 0; i < page_size; ++i){
        out << page[i];
      }
   }
}

void write_to_file(std::string const & filename)
{
   if( page_size !=  0x4000){
      std::cout << "need 0x4000 page_size..\n";
      return;
   }
   std::ofstream out(filename + ".bin" , std::ios::binary);
 
   write_page(page1_array, out);
   write_page(page2_array, out);
}
#endif





