
#include <quan/three_d/vect.hpp>
#include "../compass.hpp"
#include "../serial_ports.hpp"
#include "flash.hpp"
#include "flash_convert.hpp"
#include "to_from_flash.hpp"

/*
 get values from flash for startup config
 
*/

 bool init_values_from_flash()
 {
    bool mag_offsets_defined = flash_symtab::is_defined("mag_offsets");
    if ( mag_offsets_defined){
      quan::three_d::vect<float> mag_offset;
      if(get_flash_mag_offset(mag_offset)){
         raw_compass::set_mag_offset(mag_offset);
      }else{
         debug::serial_port::write("get flash mag offset failed\n");
         return false;
      }
    }
    if( mag_offsets_defined && flash_symtab::is_defined("use_compass")){
       bool use_compass;
       if(get_flash_use_compass(use_compass)){
         raw_compass::set_use_compass(use_compass);
      }else{
         debug::serial_port::write("get flash use compass failed\n");
         return false;
      }
    }else{ // use default
      raw_compass::set_use_compass(false);
    }
   return true;
 }