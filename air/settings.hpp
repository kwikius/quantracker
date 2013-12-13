#ifndef QUANTRACKER_AIR_SETTINGS_HPP_INCLUDED
#define QUANTRACKER_AIR_SETTINGS_HPP_INCLUDED

struct settings{

   enum class altitude_t {gps_alt, baro_alt};

   static altitude_t altitude_src;
};

#endif // QUANTRACKER_AIR_SETTINGS_HPP_INCLUDED
