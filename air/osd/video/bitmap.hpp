#ifndef OSD_BITMAP_HPP_INCLUDED
#define OSD_BITMAP_HPP_INCLUDED

#include <cstdint>
#include <quan/two_d/vect.hpp>
 
// data is of length (size_px. x * size_px.y )/ 8;
 
// virtual bitmap?
template <typename DataType>
struct abc_bitmap {
   virtual quan::two_d::vect<uint32_t> get_size_px() const =0;
   virtual uint32_t get_bits_px() const=0;
   virtual DataType const * get_data() const=0;
   uint32_t get_data_length()
   {
      quan::two_d::vect<uint32_t> const sz = get_size_px();
      return sz.x * sz.y * get_bits_px();
   }
   virtual ~abc_bitmap() {}
};
 
template <typename DataType, uint32_t X_size, uint32_t Y_size,uint32_t Bits_px>
struct static_image : abc_bitmap<DataType> {
   quan::two_d::vect<uint32_t> get_size_px() const {
      return {X_size,Y_size};
   }
   uint32_t get_bits_px() const {
      return Bits_px;
   }
};

#endif // OSD_BITMAP_HPP_INCLUDED
