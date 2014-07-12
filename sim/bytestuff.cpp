/*
 Copyright (c) 2012 - 2013 Andy Little 

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

/*
  The original ByteStuff protocol
   create a message in ByteStuff format
   Each message is 7 bytes long
########################################
   Frame Character is really from the higher level protocol
   message[0] is the  Frame character
###############################
   message[1] is the message id
   message[2:5] is the value (in little endian format?)
####################################
   Checksum is really from higher level protocol
   message[6] is the checksum
*/

/*

 stage 1 make a message from the data,  which variable it is and new value
 stage 2 put the message packet in a frame. Add the framing header byte and checksum
 stage 3 put message into the higher level protocol with escapes byte stuffing
 stage 4 put the bytestuffed message into FrSky RcTx telemetry output format

*/

#include "frsky.hpp"
#include "bytestuff.hpp"
#include "aircraft.hpp"

namespace {

   struct VariableID {

       static const uint8_t GPSLAT   =       0x1; // deg to int32_t
       static const uint8_t GPSLON   =       0x2; // deg to int32_t
       static const uint8_t AIRSPEED =       0x7; // from pitot
       // just change to altitude and send the best?
       static const uint8_t BAROALT =        0x8;
   };
  
   struct ByteStuff{

       static const uint8_t header_value =   0x5e;
       static const uint8_t escape_value =   0x5d;
   /*
    len is length of buf including checksum, but checksum is last byte in buf so
   last byte in Len not included
   */
      template <uint8_t Len>
      static uint8_t checksum(uint8_t * ar)
      {
          uint8_t sum = static_cast<uint8_t>(ar[0]);
          for ( size_t i = 1; i < (Len-1); ++i) {
              uint16_t const sum1 = sum  + static_cast<uint16_t>( static_cast<uint8_t>(ar[i]));
              sum += static_cast<uint8_t>(sum1 & 0xff)  + static_cast<uint8_t>((sum1 >> 8) && 0xff);
          }
          return static_cast<uint8_t>(sum);
      }

      template<typename T>
      struct serialise {
          serialise(uint8_t msg_id, T const & val);
          static const size_t length = sizeof(T) + 3;
          typedef uint8_t (&msg_buf_t)[length];
          msg_buf_t get() {
              return m_msg_buf;
          }
          serialise & operator = (T const & val);
      private:
          typedef union {
              uint8_t m_array[sizeof(T)];
              T m_raw_value;
          } converter;
          uint8_t      m_msg_buf[length];
      };

      
      static quan::fifo<uint8_t,1000> buffer;

   // write data into the buffer in the higher level bytestuff protocol
   // packet is assumed to be len bytes long
   // start_of_frame is true if its the start of a new higher level frame
   // the frame is then in
      static int16_t stuff(uint8_t * buf, int16_t len, bool start_of_frame)
      {

         int16_t pos = 0;
         int16_t count = 0;
         if(start_of_frame){
           // though the header is in buf[0] we know what it is so use a constant here...
          buffer.put(buf[0]);
           ++pos;
           ++count;
         }
         for( ; pos < len; ++pos){
             uint8_t ch = buf[pos];
             if ( (ch == header_value) || (ch == escape_value) ){
                uint8_t ar[]= {escape_value, static_cast<uint8_t>(ch ^ 0x60)};
               // sp->write(ar,2);
                buffer.put(ar[0]);
                buffer.put(ar[1]);
                count += 2;
             }else{
               // sp->write(buf + pos,1);
               buffer.put(buf[pos]);
                ++count;
             }
         }
         return count;
      }

     static  void send_message();

   }; // ByteStuff struct
    
   quan::fifo<uint8_t,1000> ByteStuff::buffer;
  
   template<typename T>
   ByteStuff::serialise<T>::serialise(uint8_t msg_id, T const & val)
   {
       m_msg_buf[0] = ByteStuff::header_value;
       m_msg_buf[1] = msg_id;
    
       converter conv;
       conv.m_raw_value = val;
    
       for (size_t i = 0; i < sizeof(T); ++i) {
           m_msg_buf[2 + i] = conv.m_array[i];
       }
       m_msg_buf[length -1] = ByteStuff::checksum<length>(m_msg_buf);
   }
    
   template <typename T>
   ByteStuff::serialise<T> & ByteStuff::serialise<T>::operator = (T const & val)
   {
       converter conv;
       conv.m_raw_value = val;
    
       for (size_t i = 0; i < sizeof(T); ++i) {
           m_msg_buf[2 + i] = conv.m_array[i];
       }
       m_msg_buf[length -1] = ByteStuff::checksum<length>(m_msg_buf);
       return *this;
   }
    
   ByteStuff::serialise<int32_t> lat_msg(VariableID::GPSLAT,0);
   ByteStuff::serialise<int32_t> lon_msg(VariableID::GPSLON,0);
   ByteStuff::serialise<int32_t> baroalt_msg(VariableID::BAROALT,0);
   ByteStuff::serialise<int32_t> airspeed_msg(VariableID::AIRSPEED,0);
    /*
    angle is +- deg  - 180 to 180
    convert to angle of +- deg * 1e7
   */
   inline int32_t normalise(quan::angle_<int32_t>::deg10e7 const & in)
   {
       constexpr int64_t ang_max  = 1800000000LL;
       return static_cast<int32_t>(in.numeric_value() % ang_max);
   }
   /* 
   max FrSky update rate is 1200 baud. If this is called at 50 Hz then
   works out that can send only 7 bytes over 3 cycles.
   so send 2, 2, then 3
    gives update rate for all variables of around 5 Hz

   or do one every 7 ms?

   The following functions are put in an array and called in rotation
     First func fro var does setup, others just put out some bytes
     called in 50 Hz loop so as not to exceed max data rate
   */
   // Latitude

   //return actual num of uint8_ts written in escapes
   int16_t update_lat_msg1()
   {
       lat_msg = normalise(get_aircraft_gps_position().lat);
       return ByteStuff::stuff(lat_msg.get(), 2, true);
   }
   int16_t update_lat_msg2()
   {
       return ByteStuff::stuff(lat_msg.get() + 2, 2, false);
   }
   int16_t update_lat_msg3()
   {
      return ByteStuff::stuff(lat_msg.get() + 4, 3, false);
   }

   //longtitude
   int16_t update_lon_msg1()
   {
      lon_msg = normalise(get_aircraft_gps_position().lon);
      return ByteStuff::stuff(lon_msg.get(), 2, true);
   }

   int16_t update_lon_msg2()
   {
      return ByteStuff::stuff(lon_msg.get() + 2, 2, false);
   }
   int16_t update_lon_msg3()
   {
      return ByteStuff::stuff(lon_msg.get() + 4, 3, false);
   }

   //altitude
   int16_t update_baroalt_msg1()
   {
      baroalt_msg = static_cast<int32_t>(get_aircraft_gps_position().alt.numeric_value());
      return ByteStuff::stuff(baroalt_msg.get(),2,true);
   }
   int16_t update_baroalt_msg2()
   {
      return ByteStuff::stuff(baroalt_msg.get() + 2, 2, false);
   }
   int16_t update_baroalt_msg3()
   {
      return ByteStuff::stuff(baroalt_msg.get() + 4, 3,false);
   }

   //airspeed
   int16_t update_airspeed_msg1()
   {

//TODO#########################
        // airspeed_msg = static_cast<int32_t>(the_aircraft.airspeed.numeric_value());
      airspeed_msg = static_cast<int32_t>(0);
//##########################
     return ByteStuff::stuff(airspeed_msg.get(),2,true);
   }

   int16_t update_airspeed_msg2()
   {
      return ByteStuff::stuff(airspeed_msg.get() + 2, 2, false);
   }
   int16_t update_airspeed_msg3()
   {
      return ByteStuff::stuff(airspeed_msg.get() + 4, 3,false);
   }

   typedef int16_t(*msgfun)();
    
   msgfun msgfuns[] = {
       update_lat_msg1,
       update_lat_msg2,
       update_lat_msg3,
       update_lon_msg1,
       update_lon_msg2,
       update_lon_msg3,
       update_baroalt_msg1,
       update_baroalt_msg2,
       update_baroalt_msg3,
       update_airspeed_msg1,
       update_airspeed_msg2,
       update_airspeed_msg3
   };

   // call functions to output data in sequence
// called at 1 call every 1/50th sec
   void ByteStuff::send_message()
   {

      static uint8_t idx = 0;
      static uint8_t byte_idx = 0;
      static int16_t write_count = 0;

      while (write_count <= 0 ){ // ready for more comms
       // call current fun
       write_count += msgfuns[idx]();
       //and update to next fun
       idx = (idx + 1) % (sizeof(msgfuns)/sizeof(msgfun));
      }
      // number of bytes allowed is 7  per 60 msec ( each call happens every 20 msec)
      // so split the bytes into 2,2,3 allowed each time.
      byte_idx = (byte_idx + 1 ) % 3; 
      int const bytes_gone = (byte_idx == 2)?3:2;
      write_count -= bytes_gone;
      // cap the read ahead...
      if( write_count < -64){
       write_count = -64;
      }
      // while ( ByteStuff::buffer.num_in_buffer()){
      auto & app =wxGetApp();
      assert(app.have_sp());
      send_as_frsky_Telemetry_from_RcTx(ByteStuff::buffer, app.get_sp());

   }

}//namespace

void ByteStuff_send_message()
{
   ByteStuff::send_message();
}


