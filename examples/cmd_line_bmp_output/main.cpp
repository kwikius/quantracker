
#include <quan/uav/osd/basic_bitmap.hpp>
#include <quan/uav/osd/detail/output_image.hpp>

/*
 example that outputs a rendition of an OSD bitmap to stdout
*/

namespace{

/* 
  wrapping in anonymous namespace is the preferred style
  for larger projects as it avoids adding names globally.
*/
  // look in the source file to see how it's composed
  #include "my_bitmap.cpp"

}

#include <iostream>

int main()
{
   quan::uav::osd::detail::output_image(&my_bitmap,std::cout);
}

