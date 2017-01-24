
//Generated by OSDMaker foraerflite FC

//https://github.com/kwikius/quantracker
//https://github.com/kwikius/osd_maker

#include <quan/uav/osd/api.hpp>
#include <quan/uav/osd/basic_bitmap.hpp>

namespace {

 struct home_image_type : quan::uav::osd::basic_bitmap{
    uint8_t const * get_data()const
    {
       return m_data;
    }
    quan::uav::osd::size_type get_size()const
    {
       return {16,12};
    }
 private:
    static constexpr uint8_t m_data[48] = {
      255, 191, 254, 255
      , 255, 43, 232, 255
      , 191, 130, 130, 254
      , 43, 40, 40, 232
      , 130, 2, 128, 130
      , 42, 0, 0, 168
      , 47, 170, 168, 248
      , 47, 150, 152, 248
      , 47, 150, 152, 248
      , 47, 150, 168, 248
      , 47, 150, 0, 248
      , 175, 170, 170, 250
      
      };
   } home_image;
  constexpr uint8_t home_image_type::m_data [ 48];

 struct satellite_image_type : quan::uav::osd::basic_bitmap{
    uint8_t const * get_data()const
    {
       return m_data;
    }
    quan::uav::osd::size_type get_size()const
    {
       return {15,19};
    }
 private:
    static constexpr uint8_t m_data[72] = {
      175, 234, 255, 255
      , 10, 232, 175, 175
      , 168, 224, 130, 11
      , 170, 162, 224, 130
      , 170, 130, 190, 160
      , 170, 160, 47, 160
      , 138, 162, 43, 160
      , 168, 226, 11, 160
      , 170, 232, 11, 160
      , 170, 248, 11, 160
      , 42, 254, 11, 160
      , 138, 255, 2, 0
      , 232, 191, 32, 128
      , 254, 43, 168, 234
      , 191, 2, 248, 255
      , 42, 0, 168, 191
      , 0, 0, 224, 175
      , 170, 170, 250, 255
      
      };
   } satellite_image;
  constexpr uint8_t satellite_image_type::m_data [ 72];

 struct altitude_symbol_type : quan::uav::osd::basic_bitmap{
    uint8_t const * get_data()const
    {
       return m_data;
    }
    quan::uav::osd::size_type get_size()const
    {
       return {12,9};
    }
 private:
    static constexpr uint8_t m_data[27] = {
      171, 255, 255, 138
      , 254, 255, 34, 234
      , 255, 2, 162, 170
      , 34, 162, 128, 170
      , 162, 162, 255, 2
      , 226, 255, 170, 226
      , 255, 255, 234
      };
   } altitude_symbol;
  constexpr uint8_t altitude_symbol_type::m_data [ 27];

 struct home_arrow_type : quan::uav::osd::basic_bitmap{
    uint8_t const * get_data()const
    {
       return m_data;
    }
    quan::uav::osd::size_type get_size()const
    {
       return {19,24};
    }
 private:
    static constexpr uint8_t m_data[114] = {
      255, 255, 234, 255
      , 255, 255, 191, 248
      , 255, 255, 255, 43
      , 250, 255, 255, 255
      , 2, 254, 255, 255
      , 175, 128, 254, 255
      , 255, 11, 128, 255
      , 255, 191, 2, 160
      , 255, 255, 47, 0
      , 224, 255, 255, 10
      , 0, 232, 255, 191
      , 0, 0, 248, 255
      , 43, 0, 0, 250
      , 255, 130, 2, 10
      , 254, 175, 160, 128
      , 130, 254, 11, 40
      , 160, 128, 191, 2
      , 10, 40, 160, 47
      , 128, 170, 10, 224
      , 10, 160, 170, 2
      , 168, 0, 40, 160
      , 0, 168, 10, 10
      , 40, 168, 254, 130
      , 2, 10, 254, 191
      , 160, 128, 130, 255
      , 47, 0, 0, 224
      , 255, 11, 0, 0
      , 248, 255, 170, 170
      , 170, 254
      };
   } home_arrow;
  constexpr uint8_t home_arrow_type::m_data [ 114];

 constexpr quan::uav::osd::bitmap_ptr bitmap_array[] =
   {
    &home_image
    ,&satellite_image
    ,&altitude_symbol
    ,&home_arrow
   };
} // namespace

quan::uav::osd::bitmap_ptr
quan::uav::osd::get_bitmap(uint32_t id)
{
constexpr uint32_t size = sizeof(bitmap_array)/sizeof(quan::uav::osd::bitmap_ptr);
  if ( id < size){
     return bitmap_array[id];
  }else{
     return nullptr;
  }
}