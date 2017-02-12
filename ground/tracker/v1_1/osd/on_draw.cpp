
/*
 Copyright (c) 2017 Andy Little 

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

#include <quan/uav/osd/api.hpp>

/*
   every sec or so look at telem errors and notify rate of errors

   maybe draw a bar of errors or good v bad packets good in white / bad in black?
*/

void quan::uav::osd::on_draw()
{
  // draw_text("Hello World again",{-60,30}); 
}
