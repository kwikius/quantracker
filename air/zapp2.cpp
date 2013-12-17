
#include <quan/uav/fletcher16.hpp>
#include <quan/uav/cobs/protocol.hpp>
#include "config.hpp"
#include "aircraft.hpp"
#include "zapp2.hpp"
#include "settings.hpp"

/*
 The ZAPP2 protocol
*/


namespace { 

   struct msgID {

      static const uint8_t Latitude     =    0x1; // deg10e7 to int32_t
      static const uint8_t Longtitude   =    0x2; // deg10e7 to int32_t
      static const uint8_t Altitude     =    0x3; // altitude in mm 
   };

   template<typename T,uint8_t ID>
   struct create_message {
       create_message(T const & val);
       static const size_t encoded_length = sizeof(T) + 5;
       typedef uint8_t (&msg_buf_t)[encoded_length];
       typedef uint8_t const (&const_msg_buf_t)[encoded_length];
       const_msg_buf_t get()const {
           return m_msg_buf;
       }
   private:
       uint8_t m_msg_buf[encoded_length];
   };
    
   template<typename T,uint8_t ID>
   create_message<T,ID>::create_message(T const & val)
   {
       uint8_t buf[sizeof(T) + 3];
       buf[0] = ID;
       for (size_t i = 0; i < sizeof(T); ++i) {
          buf[i+1] = static_cast<uint8_t>((val >> (8U*i)) & 0xFF);
       }
       uint16_t ck = quan::uav::fletcher16(buf,sizeof(T)+1);
       buf[sizeof(T)+1] = static_cast<uint8_t>(ck && 0xFF) ;
       buf[sizeof(T)+2] = static_cast<uint8_t>((ck && 0xFF00) >> 8) ;
       this->m_msg_buf[0] = 0; // framing byte
       quan::uav::cobs::encode(buf,sizeof(T)+3,this->m_msg_buf+1);
   }

   inline int32_t normalise_angle(quan::angle_<int32_t>::deg10e7 const & in)
   {
       constexpr int64_t ang_max  = 1800000000LL;
       return static_cast<int32_t>(in.numeric_value() % ang_max);
   }

   // latitude
   int16_t update_lat_msg(zapp2::write_fn f)
   {
       uint32_t const lat = normalise_angle(the_aircraft.location.gps_lat);
       typedef create_message<uint32_t,msgID::Latitude> msg_type;
       msg_type lat_msg(lat);
       return f(lat_msg.get(),msg_type::encoded_length);
   }
  
   //longtitude
   int16_t update_lon_msg(zapp2::write_fn f)
   {
       uint32_t const lon = normalise_angle(the_aircraft.location.gps_lon);
       typedef create_message<uint32_t,msgID::Longtitude> msg_type;
       msg_type lon_msg(lon);
       return f(lon_msg.get(),msg_type::encoded_length);
   }
   // altitude
   int16_t update_alt_msg(zapp2::write_fn f)
   {
      uint32_t const alt = 
      ( ::settings::altitude_src == ::settings::altitude_t::gps_alt)
         ? static_cast<uint32_t>(the_aircraft.location.gps_alt.numeric_value())
         : static_cast<uint32_t>(static_cast<int32_t>(the_aircraft.baro_alt.numeric_value() * 1000));
       typedef create_message<uint32_t,msgID::Altitude> msg_type;
       msg_type alt_msg(alt);
       return f(alt_msg.get(),msg_type::encoded_length);
   }

   typedef int16_t(*msgfun)(zapp2::write_fn);
    
   msgfun msgfuns[] = {
       update_lat_msg,
       update_lon_msg,
       update_alt_msg,
   };

}//namespace

namespace zapp2{

   void send_message(zapp2::write_fn f)
   {
      static uint8_t idx = 0;
      //call cur fun
      msgfuns[idx](f);
      //and update to next fun
      idx = (idx + 1) % (sizeof(msgfuns)/sizeof(msgfun));
   }

}

