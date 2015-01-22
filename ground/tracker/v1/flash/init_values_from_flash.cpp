/*
 Copyright (c) 2013 -2015 Andy Little 

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program. If not, see <http://www.gnu.org/licenses/>
*/
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