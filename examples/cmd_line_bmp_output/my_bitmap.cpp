

/*
to be included inline
*/

struct my_bitmap_type : quan::uav::osd::basic_bitmap{
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
      , 90, 233, 175, 175
      , 169, 229, 150, 91
      , 170, 166, 229, 150
      , 170, 150, 190, 165
      , 170, 165, 111, 165
      , 154, 166, 107, 165
      , 169, 230, 91, 165
      , 170, 233, 91, 165
      , 170, 249, 91, 165
      , 106, 254, 91, 165
      , 154, 255, 86, 85
      , 233, 191, 101, 149
      , 254, 107, 169, 234
      , 191, 86, 249, 255
      , 106, 85, 169, 191
      , 85, 85, 229, 175
      , 170, 170, 250, 255
      
      };
   } my_bitmap;
  constexpr uint8_t my_bitmap_type::m_data [ 72];