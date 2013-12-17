#include "fsk.hpp"
#include "zapp2.hpp"
#include "events.hpp"
#include "settings.hpp"

namespace {

    int16_t fsk_output (uint8_t const * buf, int16_t len)
   {
      fsk::write(reinterpret_cast<char const *>(buf),len);
      return len;
   }
}

namespace fsk{ namespace zapp2{

   void send_message()
   {
      ::zapp2::send_message(fsk_output);
   }

}}

namespace {

   periodic_event zapp2_event{
      zapp2::get_update_event_timing(),
      fsk::zapp2::send_message,true
   };
}

namespace fsk{ namespace zapp2{

   void setup_event()
   {
      ::set_event(::event_index::fsk,&zapp2_event);
   }

}}

namespace fsk{

   void send_message()
   {
      switch (settings::fsk_protocol){
         case settings::output_protocol_t::zapp1:
         //   zapp1::frsky_send_message();
            break;
         case settings::output_protocol_t::zapp2:
            fsk::zapp2::send_message();
            break;
         default:
            break;
      }
   }

   void setup_event()
   {
      switch (settings::fsk_protocol){
         case settings::output_protocol_t::zapp1:
         //   zapp1::setup_frsky_event();
            break;
         case settings::output_protocol_t::zapp2:
            fsk::zapp2::setup_event();
            break;
         default:
            break;
      }
   }

}//fsk