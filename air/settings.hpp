#ifndef QUANTRACKER_AIR_SETTINGS_HPP_INCLUDED
#define QUANTRACKER_AIR_SETTINGS_HPP_INCLUDED

struct settings{

   // source for altitude data sent down telemetry link
   enum class altitude_t {gps_alt, baro_alt};
   static altitude_t altitude_src;

   // protocol to use for sending data down the telemetry link
   enum class protocol_t {none,zapp1, zapp2};
   static protocol_t frsky_protocol;
   static protocol_t fsk_protocol;

};

#endif // QUANTRACKER_AIR_SETTINGS_HPP_INCLUDED
