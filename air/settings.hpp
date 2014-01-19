#ifndef QUANTRACKER_AIR_SETTINGS_HPP_INCLUDED
#define QUANTRACKER_AIR_SETTINGS_HPP_INCLUDED

struct settings{

   // source for altitude data sent down telemetry link
   // since mavlink has both baro_alt and gps_alt
   enum class altitude_t {gps_alt, baro_alt};
   static altitude_t altitude_src;

   // protocol to use for sending data down the telemetry link
   // choose zapp1 for compatibility
   enum class output_protocol_t {none,zapp1, zapp2};
   static output_protocol_t frsky_protocol;
   // must be i none or zapp2
   static output_protocol_t fsk_protocol;

   // what is the data source
   enum class data_source_t {mavlink, gps};
   static data_source_t  data_source;

};

#endif // QUANTRACKER_AIR_SETTINGS_HPP_INCLUDED
