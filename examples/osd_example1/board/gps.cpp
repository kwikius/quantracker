/*
 Copyright (c) 2012 - 2015 Andy Little 

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

#include "config.hpp"
#include "gps.hpp"
#include "serial_ports.hpp"
#include <quan/conversion/float_convert.hpp>

/*
even though many fields arent used, they are parsed
If any field is invalid the whole message is rejected
This gives high quality error checking. I would rather have no data than bad data.

Some of the messages arent required and will probably be removed to save rom space
*/

/*
 useful to get latitude, longtitude and altitude, number of sats  
really only need this message, except for RMC to check data valid
 
*/
namespace {
   typedef posdata_sp::serial_port gps_sp;
}

gps::bool_pmf gps::GGA_cmds[gps::GGA_num_fields] 
={
   &gps::get_utc_time
   ,&gps::get_Latitude
   ,&gps::get_sign_of_Latitude
   ,&gps::get_Longtitude
   ,&gps::get_sign_of_Longtitude
   ,&gps::get_position_fix_indicator
   ,&gps::get_sats_used
   ,&gps::get_hdop                   // opt
   ,&gps::get_altitude               // opt
   ,&gps::get_meters                
   ,&gps::get_geoidal_separation    // opt
   ,&gps::get_meters                // opt       
   ,&gps::get_empty_field           // get age_of_diff_corr
   ,&gps::get_empty_field           // DGPS reference station id
};

/*
redundant unless want the hdop, vdop etc
*/
gps::bool_pmf gps::GSA_cmds[gps::GSA_num_fields] 
={
   &gps::get_mode_2D3D
  ,&gps::get_fix_mode
  ,&gps::get_sat_used
  ,&gps::get_sat_used
  ,&gps::get_sat_used
  ,&gps::get_sat_used
  ,&gps::get_sat_used
  ,&gps::get_sat_used
  ,&gps::get_sat_used
  ,&gps::get_sat_used
  ,&gps::get_sat_used
  ,&gps::get_sat_used
  ,&gps::get_sat_used
  ,&gps::get_sat_used
  ,&gps::get_pdop
  ,&gps::get_hdop
  ,&gps::get_vdop
};
/*
redundant for the tracker, though useful for OSD
*/
gps::bool_pmf gps::VTG_cmds[gps::VTG_num_fields] 
={
  &gps::get_true_heading
  ,&gps::get_true_heading_t
  ,&gps::get_magnetic_heading
  ,&gps::get_magnetic_heading_m
  ,&gps::get_groundspeed_knot
  ,&gps::get_groundspeed_unit_knot
  ,&gps::get_groundspeed_kmh
  ,&gps::get_groundspeed_unit_kmh
  ,&gps::get_vtg_mode
};
/*
Doesnt provide altitude so not very useful
*/

gps::bool_pmf gps::RMC_cmds[gps::RMC_num_fields] 
={
    &gps::get_utc_time
   ,&gps::get_rmc_status
   ,&gps::get_Latitude
   ,&gps::get_sign_of_Latitude
   ,&gps::get_Longtitude
   ,&gps::get_sign_of_Longtitude
   ,&gps::get_groundspeed_knot
   ,&gps::get_true_heading
   ,&gps::get_date
   ,&gps::get_magnetic_variation
   ,&gps::get_sign_of_magnetic_variation
   ,&gps::get_vtg_mode
};

gps::bool_pmf gps::ACK_cmds[gps::ACK_num_fields] 
={
    &gps::get_ack_cmd_id,
    &gps::get_ack_result_code
};

gps::field_info gps::GGA_info("GGA",gps::GGA_cmds,gps::GGA_num_fields);
gps::field_info gps::GSA_info("GSA",gps::GSA_cmds,gps::GSA_num_fields);
gps::field_info gps::VTG_info("VTG", gps::VTG_cmds,gps::VTG_num_fields);
gps::field_info gps::RMC_info("RMC",gps::RMC_cmds,gps::RMC_num_fields);
gps::field_info gps::ACK_info("ACK",gps::ACK_cmds,gps::ACK_num_fields);

//-------------------------------

gps::gps(gps::callback fn)
   :
    m_callback_fn(fn)
   ,m_buf_idx(0)
   ,m_atom_idx(0)
   ,m_checksum(false)
   ,m_utc_time_ms(0)
   ,m_latitude_deg10e7(0)
   ,m_longtitude_deg10e7(0)
   ,m_altitude_mm(0)
   ,m_fix_type(Fix_not_available)
   ,m_sats_used(0)
   ,m_pdop(1.f)
   ,m_hdop(1.f)
   ,m_vdop(1.f)
   ,m_geoidal_sep(0.f)
   ,m_mode_2D3D(Manual) // check this
   ,m_fix_mode(Fix_mode_not_available)
   ,pf_parse(&gps::in_preamble)
{}

uint8_t gps::chdg(char ch)
{ 
   return ch - '0';
}

uint8_t gps::chxdg(char ch)
{ 
   return ((ch >= 'A') && (ch <= 'F'))
   ? (ch - 'A' + 10)
   : (((ch >= 'a') && (ch <= 'f')) 
      ? (ch - 'a' + 10)
      : (ch - '0'));
}

bool gps::get_ack_cmd_id()
{
   if ( strlen(m_buf) != 3){
   //    asynch_tx_write("cmd id =");
    //     asynch_tx_write(m_buf);
     //    asynch_tx_write("\n");
      return false;
   }
   strcpy(m_ack_cmd.get_value(),m_buf);
   return true;
}

bool gps::get_ack_result_code()
{
   int8_t result;
   if( ! get_digit(result)){
     //  asynch_tx_write(m_buf);
     //  asynch_tx_write(" not dig\n");
      return false;
   }
   if ((result >=0) && ( result <=3 )){
      m_ack_result = result;
      return true;
   }
   // asynch_tx_write("range\n");
   return false;
}

bool gps::get_pdop()
{
   float result;
   if (!get_float(result)){
      return false;
   }
   m_pdop = result;
   return true;
}

bool gps::get_rmc_status()
{
   if ( strlen(m_buf) !=1){
      return false;
   }
   if (m_buf[0] == 'V'){
      m_rmc_status = Data_Invalid;
      return true;
   }
   if (m_buf[0] == 'A'){
      m_rmc_status = Data_Valid;
      return true;
   }
   return false;
}

bool gps::get_hdop()
{
   float result;
   if (!get_float(result)){
      return false;
   }
   m_hdop = result;
   return true;
}

bool gps::get_vdop()
{
   float result;
   if (!get_float(result)){
      return false;
   }
   m_vdop = result;
   return true;
}

bool gps::get_true_heading()
{
   if (strlen(m_buf) ==0){ // empty field ok?
      return true;
   }
   float result;
   if (!get_float(result)){
      return false;
   }
   m_true_heading_deg = result;
   return true;
}

bool gps::get_magnetic_heading()
{
   if (strlen(m_buf) ==0){ // empty field ok?
      return true;
   }
   float result;
   if (!get_float(result)){
      return false;
   }
   m_magnetic_heading_deg = result;
   return true;
}

bool gps::get_groundspeed_knot()
{

   float result;
   if (!get_float(result)){
      return false;
   }
   m_groundspeed_m_per_s = result * 0.514444444;
   return true;
}

bool gps::get_groundspeed_kmh()
{
   float result;
   if (!get_float(result)){
      return false;
   }

   m_groundspeed_m_per_s = result * 0.277777778;
   return true;;
 
}

bool gps::get_groundspeed_unit_knot()
{
   if ( strlen(m_buf) !=1){
      return false;
   }
   return (m_buf[0] == 'N');
}

bool gps::get_groundspeed_unit_kmh()
{
   if ( strlen(m_buf) !=1){
      return false;
   }
   return (m_buf[0] == 'K');
}

bool gps::get_true_heading_t()
{
   if ( strlen(m_buf) !=1){
      return false;
   }
   return (m_buf[0] == 'T');
}

bool gps::get_magnetic_heading_m()
{
   if ( strlen(m_buf) !=1){
      return false;
   }
   return (m_buf[0] == 'M');
}

bool gps::get_altitude()
{
   float result;
   if (!get_float(result)){
      return false;
   }
   m_altitude_mm = static_cast<int32_t>(result*1000);
   return true;
}

bool gps::get_meters()
{
   if ( strlen(m_buf) !=1){
      return false;
   }
   return (m_buf[0] == 'M');
}

bool gps::get_geoidal_separation()
{
   float result;
   if (!get_float(result)){
      return false;
   }
   m_geoidal_sep = result;
   return true;
}

bool gps::get_empty_field()
{
   return true;
}

bool gps::get_sats_used()
{
   int32_t result;
   if (! get_int(result)){
      return false;
   }
   if ( result > 127){
      return false;
   }
   m_sats_used = result;
   return true;
}

void gps::parse_cmd_fields()
{
   if (m_cur_tok == '*'){
      // dont eval til checksum arrived
     // if ( gps_sp::in_avail() < 2){
        if( gps_sp::in_avail() <2){
         return;
      }
      if (do_checksum() == true){
         update_vars();
      //   asynch_tx_write(m_cur_field_info->m_name);
      //   asynch_tx_write(" - cmd sxs\n");
#ifdef GPS_DEBUG_OUTPUT
      }else{
         std::cout << "parse failed\n";
#endif
      }

      reset();
   }else{
   // check length here
      if( m_atom_idx < m_cur_field_info->m_length ){
         parse_buf_fn(m_cur_field_info->m_functions[m_atom_idx]); 
      }
      else{
#ifdef GPS_DEBUG_OUTPUT
         std::cout << "in parse_cmd_fields - no end of fields\n";
#endif
         reset();
      }
   }
}

void gps::parse() 
{  
   (this->*pf_parse)();
}

gps::field_info* gps::get_field_info()
{
  if(strcmp(m_buf,"GPGGA")==0){return &GGA_info;}
  if(strcmp(m_buf,"GPGSA")==0){ return &GSA_info;}
//      if(strcmp(m_buf,"GPGSV")==0){ return 0;}
  if(strcmp(m_buf,"GPRMC")==0){ return &RMC_info;}
  if(strcmp(m_buf,"GPVTG")==0){ return &VTG_info;}
  if(strcmp(m_buf,"PMTK001")==0){ return &ACK_info;}
   // unknown other cmd
   return 0;
}

char gps::chksum_read()
{
  // char ch = gps_sp::get();
   char ch = gps_sp::get();
   m_checksum ^= ch;
   return ch;
}

void gps::update_vars()
{
  if (m_callback_fn !=0){
    m_callback_fn(*this);
  }
}

bool gps::do_checksum()
{
  char ck1 = gps_sp::get();
  char ck2 = gps_sp::get();
  if (! isxdigit(ck1) || !isxdigit(ck2) ){
#ifdef GPS_DEBUG_OUTPUT
    std::cout << "in checksum - values arent digits\n";
#endif
   return false;
  }
//#ifdef GPS_DEBUG_OUTPUT
//  std::cout << "input checksum = " << ck1 << ck2 << "\n";
//#endif
  int lcksum = (chxdg(ck1) << 4) | chxdg(ck2);
  
  bool result = lcksum == m_checksum;
#ifdef GPS_DEBUG_OUTPUT
  if ( result ){
  //   std::cout << "checksum succeeded\n";
  }else{
    // std::cout << "computed checksum = " << std::hex << static_cast<int>(m_checksum) << "\n";
    // std::cout << "locally computed checksum  = "<< std::hex << lcksum << "\n";
     std::cout << "checksum failed\n";
  }
#endif
  return result;
}

void gps::reset()
{

   m_buf_idx = 0;
   m_atom_idx = 0;
   m_checksum = 0;
   m_cur_tok = '\0';
   pf_parse = &gps::in_preamble;
   return;
}

void gps::in_preamble()
{
  // asynch_tx_write("pre\n");
   while( gps_sp::in_avail()){
      if(gps_sp::get() == '$'){
       // asynch_tx_write("->cmd wrd\n");
        m_checksum = 0;
        pf_parse = &gps::in_command_word;
        return;
      }
   }
}

void gps::in_command_word()
{
   while (gps_sp::in_avail()){
      char ch = chksum_read();
      if ( isalnum(ch)){
         m_buf[m_buf_idx] = ch;
         if(++m_buf_idx == buf_strlen){
            // asynch_tx_write( "cmd wrd too lng\n");
            reset();
            return;
         }
      }else {
         if ( ch == ','){
            m_buf[m_buf_idx] = '\0';
            m_buf_idx = 0;
            m_atom_idx = 0;
            m_cur_tok = 0;
            m_cur_field_info = get_field_info();
            if (m_cur_field_info == 0){
               // asynch_tx_write(m_buf);
              // asynch_tx_write(" - not cmd str\n");
               reset();
               return;
            }else{
             //  asynch_tx_write("->prs cmdflds\n");
               pf_parse = &gps::parse_cmd_fields;
               return;
            }
         }else{
         //   asynch_tx_write("cmd wrd expct comma\n");
          //  asynch_tx_write("got \'");
         //   asynch_tx_write_byte(ch);
          //  asynch_tx_write("\'\n");
            reset();
            return;
         }
      }
   }
}

void gps::parse_buf_fn( bool( gps::*pfn)())
{
  
  while (gps_sp::in_avail()){
      char ch = gps_sp::get();
      switch (ch){
         case ',' :
             m_checksum ^= ch;
          
            //ok fall through
         case '*' :
            m_cur_tok = ch;
            m_buf[m_buf_idx] = '\0';

            // skip empty field or parse the buffer
            if ( (m_buf_idx == 0 ) || (this->*pfn)() ){
               ++m_atom_idx; // move to next field
               m_buf_idx = 0; //reset to start of buffer
            }else{
#ifdef GPS_DEBUG_OUTPUT
               std::cout << "in parse_buf_fn - pfn failed\n";
#endif
            // asynch_tx_write("prs buf fn fail\n");
               reset();
            }
         return;
         default:
            m_checksum ^= ch;
            m_buf[m_buf_idx] = ch;
            if (++m_buf_idx == buf_strlen){
#ifdef GPS_DEBUG_OUTPUT
               std::cout << "in parse_buf_len - bufferstring too long\n";
#endif
               reset();
               return;
            }
         break;
      }
   }
}

bool gps::get_deg10e7(var<int32_t> & dest)
{
    //expected input format dddmm.mmmm
   int16_t buflen = strlen(m_buf);
   if ( buflen < 7){
#ifdef GPS_DEBUG_OUTPUT
      std::cout << "in get_deg10e7 - string too short\n";
#endif
      return false;
   }
   // get decimal point
   int16_t lenb4dp = 0;
   for ( ;;++lenb4dp){
      if ( lenb4dp == buflen){
#ifdef GPS_DEBUG_OUTPUT
         std::cout << "in get_deg10e7 - hit end of buffer\n";
#endif
         return false;
      }
      if(m_buf[lenb4dp] == '.'){
         break;
      }
   }
   int16_t degrees = 0;
   int16_t scale = 1;
   // get degrees part
   for ( int16_t pos  = lenb4dp - 3; pos >=0; --pos){
      char ch = m_buf[pos];
      if( !isdigit(ch)){
#ifdef GPS_DEBUG_OUTPUT
         std::cout << "in get_deg10e7 - degrees part not digit\n";
#endif
         return false;
      }
      degrees += chdg(ch) * scale;
      scale *= 10;
   }
    int16_t minutes = 0;
    scale = 1;
    for ( int16_t pos  = lenb4dp - 1; pos > lenb4dp -3 ; --pos){
      char ch = m_buf[pos];
      if( !isdigit(ch)){
#ifdef GPS_DEBUG_OUTPUT
               std::cout << "in get_deg10e7 - minutes part not digit\n";
#endif
         return false;
      }
      minutes += chdg(ch) *scale;
      scale *= 10;
    }
    // up to 4 places in minutes * 1e-4
    int16_t fractional_minutes = 0;
    scale = 1000;
    for (int pos = lenb4dp+1; pos < buflen; ++pos)
    {
      char ch = m_buf[pos];
      if( !isdigit(ch)){
#ifdef GPS_DEBUG_OUTPUT
               std::cout << "in get_deg10e7 - fractional minutes part not digit\n";
#endif
         return false;
      }
      fractional_minutes += chdg(ch) * scale;
      scale /= 10;
    }
    dest = static_cast<int32_t>( (degrees * 1.e7 + ( minutes * 1.e6  + fractional_minutes * 1.e2) / 6 )  + 0.5) ;
    return true;
}

bool gps::get_utc_time()
{

   // input format : hhmmss.sss
   int16_t buflen = strlen(m_buf);
   if ( buflen < 6){
#ifdef GPS_DEBUG_OUTPUT
       std::cout << "in get_utc_time - buflen too short\n";
#endif
      return false;
   }
   for ( uint8_t i =0; i < 6; ++i){
      if (!isdigit(m_buf[i])){
#ifdef GPS_DEBUG_OUTPUT
         std::cout << "in get_utc_time - expected digit in hhmmss\n";
#endif
         return false;
      }
   }
   uint16_t const hrs  = chdg(m_buf[0]) * 10  + chdg(m_buf[1]);
   uint16_t const mins = chdg(m_buf[2]) * 10  + chdg(m_buf[3]) + hrs * 60;
   uint32_t const secs = chdg(m_buf[4]) * 10  + chdg(m_buf[5]) + mins * 60;
   uint32_t ms = secs * 1000;

   int len_remaining = buflen - 7;
   if (len_remaining > 3){ len_remaining = 3;}
   int mul = 100;
   for ( int i = 0; i < len_remaining; ++i){
      char ch = m_buf[7 + i];
      if( !isdigit(ch)){
#ifdef GPS_DEBUG_OUTPUT
          std::cout << "in get_utc_time - expected digit in .sss\n";
#endif
         return false;
      }
      ms += chdg(ch) * mul;
      mul /= 10;
   }
   m_utc_time_ms = ms;
   return true;
}

bool gps::get_Latitude()
{
   bool result = get_deg10e7(m_latitude_deg10e7) == true;
#ifdef GPS_DEBUG_OUTPUT
   if(! result){
      std::cout << "in get_Latitude - fail\n";
   }
#endif
   return result;
}

bool gps::get_sign_of_Latitude()
{
   if ( strlen(m_buf) !=1){
#ifdef GPS_DEBUG_OUTPUT
      std::cout << "in get_sign_of_Latitude - buf len ! 1\n";
#endif
      return false;
   }
   if (m_buf[0] == 'N'){
      return true;
   }
   if (m_buf[0] == 'S'){
      m_latitude_deg10e7 = m_latitude_deg10e7.get_value() * -1;
      return true;
   }
#ifdef GPS_DEBUG_OUTPUT
      std::cout << "in get_sign_of_Latitude - cah isnt N or S\n";
#endif
   return false;
}

bool gps::get_Longtitude()
{
   return get_deg10e7(m_longtitude_deg10e7)== true;
}

bool gps::get_sign_of_Longtitude()
{
   if ( strlen(m_buf) !=1){
      return false;
   }
   if (m_buf[0] == 'E'){
      return true;
   }
   if (m_buf[0] == 'W'){
      m_longtitude_deg10e7 = m_longtitude_deg10e7.get_value() * -1;
      return true;
   }
   return false;
}

bool gps::get_mode_2D3D()
{
   if ( strlen(m_buf) !=1){
      return false;
   }
   if (m_buf[0] == 'M'){
      m_mode_2D3D = Manual;
      return true;
   }
   if (m_buf[0] == 'A'){
      m_mode_2D3D = Auto;
      return true;
   }
   return false;
}

bool gps::get_fix_mode()
{
   if ( strlen(m_buf) !=1){
      return false;
   }
   if (m_buf[0] == '1'){
      m_fix_mode = Fix_mode_not_available;
      return true;
   }
   if (m_buf[0] == '2'){
      m_fix_mode = Fix_mode_2D;
      return true;
   }
   if (m_buf[0] == '3'){
      m_fix_mode = Fix_mode_3D;
      return true;
   }
   return false;
}

bool gps::get_vtg_mode()
{
   if ( strlen(m_buf) !=1){
      return false;
   }
   if (m_buf[0] == 'A'){
      m_fix_mode = Fix_mode_not_available;
#ifdef GPS_DEBUG_OUTPUT
      std::cout << "Fix mode na\n";
#endif
      return true;
   }
   if (m_buf[0] == 'D'){
      m_fix_mode = Fix_mode_2D;
#ifdef GPS_DEBUG_OUTPUT
      std::cout << "Fix mode 2D\n";
#endif
      return true;
   }
   if (m_buf[0] == 'E'){
      m_fix_mode = Fix_mode_3D;
#ifdef GPS_DEBUG_OUTPUT
      std::cout << "Fix mode 3d\n";
#endif
      return true;
   }
   return false;

}

bool gps::get_date()
{
   if( strlen(m_buf) != 6){
      return false;
   }
   uint32_t date = 0;
   for ( int i =0; i <6; ++i){
      char ch = m_buf[i];
      if ( !isdigit(ch)){
         return false;
      }
      date |= ( chdg(ch) << (4 *(5-i)));
   }
   m_date = date;
   return true;
}

bool gps::get_magnetic_variation()
{
      // may be null field?
   float mag=0;
   if( !get_float(mag)){
      return false;
   }
   m_magnetic_variation = mag;
   return true;
}

bool gps::get_sign_of_magnetic_variation()
{

   if ( strlen(m_buf) !=1){
      return false;
   }
   if (m_buf[0] == 'E'){
      return true;
   }
   if (m_buf[0] == 'W'){
      m_magnetic_variation = m_magnetic_variation.get_value() * -1;
      return true;
   }
   return false;
}

bool gps::get_sat_used()
{

  int32_t sat_used = -1; // defualt to unused
  // many of these are empty fields
  if (strlen(m_buf) > 0){
     if (!get_int(sat_used)){
       return false;
     }
  }
  uint16_t const sat_idx = m_atom_idx -2;
// shoulnt get here
  if(sat_idx > 11){
    return false;
  }
#ifdef GPS_DEBUG_OUTPUT
  std::cout << "sat" << sat_idx << " channel =  " << sat_used << "\n";
#endif
  m_sat_used[sat_idx] = sat_used;
  return true;
}

bool gps::get_position_fix_indicator()
{

   int8_t result;
   if (!get_digit(result)){
      return false;
   }
   switch ( result ){
      case 0:
         m_fix_type = Fix_not_available;
         return true;
      case 1:
         m_fix_type = GPS_fix;
         return true;
      case 3:
         m_fix_type = Differential_fix;
         return true;
      default:
         return false;
   }
   return false;
}


bool gps::get_digit(int8_t& dest)
{
   if ( strlen(m_buf) !=1){
      return false;
   }
   if ( !isdigit(m_buf[0])){
      return false;
   }
   dest = chdg(m_buf[0]);
   return true;
}

bool gps::get_int(int32_t& dest)
{
   int buflen = strlen(m_buf);
   if ( buflen < 1){
      return false;
   }
   int32_t result=0;
   int32_t scale = 1;
   for (int i = buflen -1; i >=0; --i){
      if ( !isdigit(m_buf[i])){
         return false;
      }
      result += chdg(m_buf[i]) * scale;
      scale *= 10;
   }
   dest = result;
   return true;
}

bool gps::get_float(float& dest)
{
   quan::detail::converter<float,char*> conv;
   float result = conv(m_buf);
   if ( conv.get_errno() != 0){
      return false;
   }else{
      dest = result;
      return true;
   }
}

//-----------------

template <typename V>
gps::var<V>::var() : m_value(V()),m_changed{false}{}

template <typename V>
gps::var<V>::var(V const & v) : m_value(v),m_changed{false}{}

template <typename V>
gps::var<V>& gps::var<V>::operator = (V const & v)
{
   m_value = v;
   m_changed = true;
   return *this;
}

template struct gps::var<uint32_t>;
template struct gps::var<int32_t>;
template struct gps::var<gps::fix_type>;
template struct gps::var<gps::fix_mode>;
template struct gps::var<gps::mode_2D3D>;
template struct gps::var<gps::vtg_mode>;
template struct gps::var<uint8_t>;
template struct gps::var<int8_t>;
template struct gps::var<float>;
template struct gps::var<gps::cmd_str>;

   void setup_GPS()
   {
   
// need to scan the various baudrates
// coudl start at top?
#if 0
      serial_port.begin(38400); 

      //do MTK factory cold start 
      serial_port.write("$PMTK104*37\r\n");
      serial_port.flush();
      //reset NMEA to default output
      serial_port.write("$PMTK314,-1*04\r\n");
      serial_port.flush();

     //send only VTG and GGA once every fix
     // (seems rmc is sent also)
      serial_port.write("$PMTK314,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0*28\r\n");	
      serial_port.flush();

      //set NMEA update rate to 5Hz
      serial_port.write("$PMTK220,200*2C\r\n");
      serial_port.flush();

      //MTK set fix interval to 200 ms
      serial_port.write("$PMTK300,200,0,0,0,0*2F\r\n");
      serial_port.flush();

      serial_port.end();
      serial_port.begin(38400); 
#endif
// ardupilot
  //  serial_port.write("$PMTK314,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0*28\r\n");	// GGA & VTG once every fix
//    "$PMTK330,0*2E\r\n"										// datum = WGS84
//    "$PMTK313,1*2E\r\n"									// SBAS on
//    "$PMTK301,2*2E\r\n"									// use SBAS data for DGPS
//    "");
//      //MTK  set NMEA update rate to 5Hz
//      serial_port.write("$PMTK220,200*2C\r\n");
//      if (get_ack("220",300)){
//          asynch_tx_write("set rate to 5Hz\n");
//      }
     
//      return true;
      //MTK set fix interval to 200 ms
      // recalc ck for 200 ms
//      serial_port.write("$PMTK300,200,0,0,0,0*2F\r\n");
//      if (get_ack("300",300)){
//        asynch_tx_write("set fix interval to 200 ms\n");
//      }
     
      //MTK only send GGA
//      serial_port.write("$PMTK314,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29\r\n");
//      if (! get_ack("314",300)){
//         asynch_tx_write("set required data\n");
//      }
//     
//      // try to speed up to 38400 baud
//     // 3329 supports baud rates of 4800/9600/38400/57600/115200
//      serial_port.write("$PMTK251,38400*27\r\n");
//      if (get_ack("251",300)){
//         serial_port.end();
//         serial_port.begin(38400);
//         return true;
//      }
 //     return false;

   }
