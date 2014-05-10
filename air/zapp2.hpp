#ifndef QUANTRACKER_AIR_ZAPP2_HPP_INCLUDED
#define QUANTRACKER_AIR_ZAPP2_HPP_INCLUDED

#include <cstdint>
#include <quan/time.hpp>

namespace  zapp2{

  typedef int16_t (*write_fn) (uint8_t const * buf, int16_t len);
  void send_message(zapp2::write_fn f);

  inline constexpr quan::time_<uint32_t>::ms get_update_event_timing()
  {return quan::time_<uint32_t>::ms{83U};}

}

#endif // QUANTRACKER_AIR_ZAPP2_HPP_INCLUDED
