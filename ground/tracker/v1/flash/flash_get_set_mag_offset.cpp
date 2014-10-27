#include <quan/three_d/vect.hpp>
#include <cstring>
#include <cstdio>
#include <quan/conversion/float_convert.hpp>
#include <quan/conversion/float_to_ascii.hpp>
#include <quan/dynarray.hpp>
#include "flash.hpp"
#include "conv_funcs.hpp"

bool set_flash_use_compass(bool val)
{
   int32_t const sym_index = flash_symtab::get_index("use_compass");
   if (sym_index == -1){
      // shouldnt get here
      return false;
   }
   quan::dynarray<uint8_t> reparray{sizeof (uint8_t),main_alloc_failed};
   if(!reparray.good()){
      return false;
   }
   *reparray.get() = (val == true)?1:0;
   return flash_symtab::write(sym_index,reparray);
}

bool set_flash_mag_offset(quan::three_d::vect<float> const & src)
{
   int32_t const sym_index = flash_symtab::get_index("mag_offsets");
   if (sym_index == -1){
      // shouldnt get here
      return false;
   }
   union {
      uint8_t ar[sizeof (float) * 3];
      float vals[3];
   } uconv;
   for ( uint8_t i = 0U; i < 3U; ++i){
      uconv.vals[i] = src[i];
   }
   quan::dynarray<uint8_t> reparray{sizeof (float) * 3,main_alloc_failed};
   if(!reparray.good()){
      return false;
   }
   memcpy(reparray.get(),uconv.ar,sizeof (float) * 3);
   return flash_symtab::write(sym_index,reparray);
}

bool get_flash_mag_offset(quan::three_d::vect<float> & dest)
{
   // get string from flash
   int32_t const sym_index = flash_symtab::get_index("mag_offsets");
   if (sym_index == -1){
      // shouldnt get here
      return false;
   }
   uint32_t const length = flash_symtab::get_size(sym_index);
   quan::dynarray<uint8_t> reparray{length,main_alloc_failed};
   if(!reparray.good()){
      return false;
   }

   if (!flash_symtab::read(sym_index,reparray)){
      return false;
   }

   union {
      uint8_t ar[sizeof (float) * 3];
      float vals[3];
   } uconv;

   memcpy(uconv.ar,reparray.get(),length);
   for (uint8_t i = 0; i < 3U; ++i){
      dest[i] = uconv.vals[i];
   }
   return true;
}

bool get_flash_use_compass( bool & dest)
{
   int32_t const sym_index = flash_symtab::get_index("use_compass");
   if (sym_index == -1){
      // shouldnt get here
      return false;
   }
   uint32_t const length = flash_symtab::get_size(sym_index);
   quan::dynarray<uint8_t> reparray{length,main_alloc_failed};
   if(!reparray.good()){
      return false;
   }
   if (!flash_symtab::read(sym_index,reparray)){
      return false;
   }
   dest = (*reparray.get() == 1)?true:false;
   return true;
}
