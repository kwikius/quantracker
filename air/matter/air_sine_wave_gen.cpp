

#include "../fsk_params.hpp"
#include <quan/constants/constant.hpp>
#include <fstream>
#include <quan/frequency.hpp>
#include <iostream>

void show_values()
{
   std::cout << "tim6 bus frequency   = " << fsk_params::tim6_bus_freq <<'\n';
   std::cout << "low freq steps        = " << fsk_params::low_freq_steps << '\n';
   std::cout << "high freq steps       = " << fsk_params::high_freq_steps << '\n';
   std::cout << "dac write frequency   = " << fsk_params::dac_write_freq <<'\n';
   std::cout << "time between writes   = " << (1.e6 / fsk_params::dac_write_freq) << "us\n";
   std::cout << "dac steps / hf cycle  = " << fsk_params::dac_write_freq / fsk_params::F_high <<'\n';
   std::cout << "dac steps / lf cycle  = " << fsk_params::dac_write_freq / fsk_params::F_low <<'\n';
   std::cout << "clks between writes   = " << fsk_params::clks_per_dac_write << "\n";
   std::cout << "num array elems       = " << fsk_params::num_sin_values <<'\n';
   std::cout << "samples per_bit       = " << fsk_params::samples_per_bit << '\n';
   std::cout << "samples per byte sent = " << (10 * fsk_params::samples_per_bit) << '\n';
   std::cout << "time per byte         = " << (1.e6 / 120 ) << "us\n";
}

namespace{

   fsk_params::dac_data_type sin_array[fsk_params::num_sin_values];

   void setup_array()
   {  
      uint32_t const n = fsk_params::num_sin_values;
      for ( auto i = 0UL ; i < n; ++i){
         double const sin_angle = sin( 2.0 * quan::constant::pi * i / static_cast<double>( n) );
         double intval = fsk_params::dac_max_value * ( 1.0 + (sin_angle * fsk_params::dac_pp_voltage) / fsk_params::supply_voltage )/ 2.0 ;
         if( ( intval < 0) || ( intval > fsk_params::dac_max_value) ){
           std::cout << intval <<  " array value range\n";
         }
         sin_array[i] = static_cast<fsk_params::dac_data_type>(intval);
      
      }
   }

   uint32_t cur_pos = 0;
   uint32_t t =0;

   void output_mark(std::ostream & os)
   {
      for ( uint32_t i = 0; i  < fsk_params::samples_per_bit;++i){
        cur_pos = (cur_pos + fsk_params::low_freq_steps) % fsk_params::num_sin_values;
        os << (++t) << "," << sin_array[cur_pos] <<'\n';
      }
   }

   void output_space(std::ostream & os)
   {
      for ( uint32_t i = 0; i  < fsk_params::samples_per_bit;++i){
         cur_pos = (cur_pos + fsk_params::high_freq_steps) % fsk_params::num_sin_values;
        os << (++t) << "," << sin_array[cur_pos] <<'\n';
      }
   }
}


void output_data (std::ostream & os, uint8_t val)
{
   output_space(os);
   for (uint32_t i =0;i < 8;++i){
     if ( val  & (1 << i) ){
      output_mark(os);
     }else {
         output_space(os);
     }
   }
   output_mark(os);
  
}

void output_file()
{
   const char outfile [] = "../sin_array.txt";
   std::ofstream out(outfile);
   out << "{";
   for(uint32_t i = 0; i < fsk_params::num_sin_values; ++i){
    
      if ( i != 0){
         if ( (i % 10) == 0){
            out << "\n";
         }
         out << ", ";
      }
      out << sin_array[i];
   }
   out << "};\n";
   std::cout << "sine array written to " << outfile <<'\n';
}

void calc()
{
#if 0
     // quan::frequency::Hz dac_write_freq{fsk_params::dac_write_freq};
      double  mark_angle = (static_cast<double>(fsk_params::low_freq_steps) * fsk_params::samples_per_bit ) / fsk_params::num_sin_values;
      double  space_angle = (static_cast<double>(fsk_params::high_freq_steps) * fsk_params::samples_per_bit ) / fsk_params::num_sin_values;

     std::cout << "mark angle = " << mark_angle <<'\n';
     std::cout << "space angle = " << space_angle <<'\n';
#endif
     double freq = fsk_params::dac_write_freq / fsk_params::samples_per_bit;
     std::cout << "freq = " << freq << "\n";
     std::cout << "ratio = " << (static_cast<double>(fsk_params::F_high) / fsk_params::F_low) <<'\n';

}

int main()
{
   setup_array();

   output_file();

   calc();

   show_values();

}

