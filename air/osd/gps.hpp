#ifndef QUANTRACKER_AIR_GPS_H_INCLUDED
#define QUANTRACKER_AIR_GPS_H_INCLUDED
/*
 Copyright (c) 2003-2014 Andy Little.

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program. If not, see http://www.gnu.org/licenses./
 */

#include "config.hpp"

#ifdef GPS_DEBUG_OUTPUT
#include <iostream>
#include "hardware_serial.hpp"
#endif

//#if defined(ARDUINO)
//#include <Arduino.h>
//#endif
#include <cstdint>
#include <ctype.h>
#include <string.h>

// non blocking stateful gps parser
struct gps {

  public:
   // prob dump this just use ints
   enum fix_type {Fix_not_available,GPS_fix, Differential_fix};
   enum mode_2D3D{Manual,Auto};
   enum fix_mode{Fix_mode_not_available,Fix_mode_2D,Fix_mode_3D};
   enum vtg_mode{Autonomous_vtg,Differential_vtg, Estimated_vtg};
   enum rmc_status {Data_Invalid, Data_Valid};
 
   typedef void (*callback)(gps&);
  // gps(HardwareSerial * sp,callback);
   gps(callback);
   void parse();

private:
  // HardwareSerial * m_sp;
   callback m_callback_fn;
   static const size_t buf_strlen = 11;
   char m_buf[buf_strlen];
   char m_cur_tok;
   size_t m_buf_idx;
   uint16_t m_atom_idx;
   uint8_t m_checksum;

   typedef bool(gps::* const bool_pmf)();
  
   struct field_info{
      const char* const m_name;
      bool_pmf* m_functions;
      uint8_t m_length;
      field_info(const char* const name,bool_pmf* pf, uint8_t length)
      :m_name{name},m_functions(pf), m_length(length){}
   };
   field_info * m_cur_field_info;
   field_info * get_field_info();

   static field_info GGA_info; 
   static const size_t GGA_num_fields= 14; 
   static bool_pmf GGA_cmds[GGA_num_fields];

   static field_info GSA_info;
   static const size_t GSA_num_fields= 17; 
   static bool_pmf GSA_cmds[GSA_num_fields];

   static field_info VTG_info;
   static const size_t VTG_num_fields= 9; 
   static bool_pmf VTG_cmds[VTG_num_fields];

   static field_info RMC_info;
   static const size_t RMC_num_fields= 12; 
   static bool_pmf RMC_cmds[RMC_num_fields];

   static field_info ACK_info;
   static const size_t ACK_num_fields= 2; 
   static bool_pmf ACK_cmds[ACK_num_fields];


public:

   struct cmd_str{
      char m_value[4];
      operator char*(){ return m_value;}
   };

   template <typename V>
   struct var {
      var();
      var (V const& v) ;//: m_value(v),m_changed(true){}
      var & operator = (V const & v);
      V const & get_value() const {return m_value;}
      V & get_value(){return m_value;}
      bool changed(){return m_changed;}
      void clear_change(){ m_changed = false;}
    private:
      V m_value;
      bool m_changed;
   };
   // gps variables
   var<uint32_t> m_utc_time_ms; // R?
   var<int32_t> m_latitude_deg10e7;
   var<int32_t> m_longtitude_deg10e7;
   var<int32_t>  m_altitude_mm;

   var<fix_type> m_fix_type;
   var<uint8_t> m_sats_used;
   var<float>  m_pdop;
   var<float>  m_hdop;
   var<float>  m_vdop;
// redundant  here...?
   var<float>  m_geoidal_sep; 
   var<mode_2D3D> m_mode_2D3D;
   var<fix_mode> m_fix_mode;
   var<int8_t> m_sat_used[12];
   var<float> m_true_heading_deg;
   var<float> m_magnetic_heading_deg;
   var<float> m_groundspeed_m_per_s;
   var<vtg_mode> m_vtg_mode;
   var<rmc_status> m_rmc_status;
   var<uint32_t> m_date;
   var<float> m_magnetic_variation;
   var<cmd_str > m_ack_cmd;
   var<int8_t> m_ack_result;

private:
   typedef void(gps::* pmf)();
   pmf pf_parse;
  
   void reset();
   void in_preamble();
   void in_command_word() ;
   void parse_buf_fn( bool( gps::*pfn)());
   void parse_cmd_fields();

   void update_vars();

   bool get_deg10e7(var<int32_t> & dest);
   bool get_digit(int8_t& dest);
   bool get_int(int32_t& dest);
   bool get_float(float& dest);
   bool get_utc_time();
   bool get_Latitude();
   bool get_sign_of_Latitude();
   bool get_Longtitude();
   bool get_sign_of_Longtitude();
   bool get_position_fix_indicator();
   bool get_sats_used();
   bool get_pdop();
   bool get_hdop();
   bool get_vdop();
   bool get_altitude();
   bool get_meters();
   bool get_geoidal_separation();
   bool get_empty_field();
   bool get_mode_2D3D();
   bool get_fix_mode();
   bool get_sat_used();
   bool get_true_heading();
   bool get_true_heading_t();
   bool get_magnetic_heading();
   bool get_magnetic_heading_m();
   bool get_groundspeed_knot();
   bool get_groundspeed_unit_knot();
   bool get_groundspeed_kmh();
   bool get_groundspeed_unit_kmh();
   bool get_vtg_mode();
   bool get_rmc_status();

   bool get_ack_cmd_id();
   bool get_ack_result_code();

   bool get_date();
   bool get_magnetic_variation();
   bool get_sign_of_magnetic_variation();

   bool do_checksum();

   static uint8_t chdg(char ch);
   static uint8_t chxdg(char ch);
  
   char chksum_read();
};
extern gps the_gps;

#endif // QUANTRACKER_AIR_GPS_H_INCLUDED
