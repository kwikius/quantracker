#ifndef QUANTRACKER_GROUND_V1_1_AZIMUTH_ENCODER_HPP_INCLUDED
#define QUANTRACKER_GROUND_V1_1_AZIMUTH_ENCODER_HPP_INCLUDED

#include "../system/tracker_resources.hpp"

 struct azimuth_encoder{

      typedef  azimuth_encoder_ch1_pin    ch1_pin;
      typedef  azimuth_encoder_ch2_pin    ch2_pin;
      typedef  azimuth_quadrature_counter  counter;
     
      static uint32_t get_steps_per_revolution(){ return m_steps_per_revolution;}
      static bool is_indexed() { return m_is_indexed;}
      static uint32_t get_count() 
      {
         return counter::get()->cnt;
      }
      static bool set_index( uint32_t value);
    private:
        
        static void un_set_index() { m_is_indexed = false;}
        static void set_steps_per_revolution(uint32_t val)
        { 
           // check greater than 0. In fact check greater than some minimum e.g 360
            m_steps_per_revolution = val;
        }
        friend struct      azimuth_servo;
        static void        setup();
        static uint32_t    m_steps_per_revolution;
        static bool        m_is_indexed;
   };

#endif // QUANTRACKER_GROUND_V1_1_AZIMUTH_ENCODER_HPP_INCLUDED
