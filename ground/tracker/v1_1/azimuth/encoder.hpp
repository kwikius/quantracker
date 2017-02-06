#ifndef QUANTRACKER_GROUND_V1_1_AZIMUTH_ENCODER_HPP_INCLUDED
#define QUANTRACKER_GROUND_V1_1_AZIMUTH_ENCODER_HPP_INCLUDED

#include <quan/angle.hpp>
#include "../system/tracker_resources.hpp"

 struct azimuth_encoder{

      typedef  azimuth_encoder_ch1_pin    ch1_pin;
      typedef  azimuth_encoder_ch2_pin    ch2_pin;
      typedef  azimuth_quadrature_counter  counter;
     
      static uint32_t get_steps_per_revolution(){ return m_steps_per_revolution;}

      static bool is_indexed()  { return m_is_indexed;}
      static uint32_t get_index() { return counter::get()->cnt; }
      static bool set_index( uint32_t value);

      static uint32_t bearing_to_encoder(quan::angle::rad const & angle_in)
      {
         return static_cast<uint32_t>(
            ( unsigned_modulo(angle_in) * azimuth_encoder::get_steps_per_revolution() ) / quan::angle::two_pi
         );
      }
      // precondition: encoder_index is < azimuth_encoder::get_steps_per_revolution()
      static quan::angle::rad encoder_to_bearing(uint32_t encoder_index)
      {
         return (encoder_index * quan::angle::two_pi) / azimuth_encoder::get_steps_per_revolution();
      }

    private:
        
        static void un_set_index() { m_is_indexed = false;}
        static void set_steps_per_revolution(uint32_t val)
        { 
           // check greater than some minimum e.g 3 stesps per revolution
            m_steps_per_revolution = val;
        }
        friend struct      azimuth_servo;
        static void        setup();
        static uint32_t    m_steps_per_revolution;
        static bool        m_is_indexed;
   };

#endif // QUANTRACKER_GROUND_V1_1_AZIMUTH_ENCODER_HPP_INCLUDED
