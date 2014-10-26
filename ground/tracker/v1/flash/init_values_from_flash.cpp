
#include <quan/three_d/vect.hpp>
#include "conv_funcs.hpp"
#include "../compass.hpp"
#include "../serial_ports.hpp"

 bool init_values_from_flash()
 {
   // for each flash var read it to its runtime loc acccording to logic!
    // is defined in table but not in flash!
    if ( is_defined_flash_mag_offset()){
      quan::three_d::vect<float> mag_offset;
      if(get_flash_mag_offset(mag_offset)){
         raw_compass::set_mag_offset(mag_offset);
      }else{
         debug::serial_port::write("get flash mag offset failed\n");
         return false;
      }
    }
    if( is_defined_flash_mag_offset() && is_defined_flash_use_compass()){
       bool use_compass;
       if(get_flash_use_compass(use_compass)){
         raw_compass::set_use_compass(use_compass);
      }else{
         debug::serial_port::write("get flash use compass failed\n");
         return false;
      }
    }else{
      raw_compass::set_use_compass(false);
    }
   return true;
 }