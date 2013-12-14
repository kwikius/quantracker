#ifndef QUANTRACKER_AIR_SETTINGS_HPP_INCLUDED
#define QUANTRACKER_AIR_SETTINGS_HPP_INCLUDED

struct settings{

   // source for altitude data sent down telemetry link
   enum class altitude_t {gps_alt, baro_alt};
   static altitude_t altitude_src;

   // protocol to use for sending data down the telemetry link
   enum class output_protocol_t {none,zapp1, zapp2};
   static output_protocol_t frsky_protocol;
   static output_protocol_t fsk_protocol;

   // what is the data source
   enum class data_source_t {mavlink, gps};
   static data_source_t  data_source;

};

#endif // QUANTRACKER_AIR_SETTINGS_HPP_INCLUDED
