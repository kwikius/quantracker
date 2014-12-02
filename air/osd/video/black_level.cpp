#include <quan/stm32/gpio.hpp>
#include <quan/stm32/rcc.hpp>
#include <stm32f4xx.h>
/*

ADC on    typedef quan::mcu::pin<quan::stm32::gpioa,3>  analog_video_in; // ADC123_IN3
option Trigger by EXTI15 injected trigger
  typedef quan::mcu::pin<quan::stm32::gpiob,15> adc_exti_pin ;// Connect to EXTI15
*/
// maybe

/*
   set up exti15 or just use falling edge/ rising edge irqs
*/
// Do sync tip ADC in vsync serrated pulses as are long
// do black level ADC in post_equalising pulses
// both have not much else going on otherwise
// can use long sample times
// if edge trigger have 27 usec which should be enough!
// could do dma to some loc and then average result?
// or just do one of each?
#error redo for boardtype 4
void black_level_a2d_setup()
{
  // TODO ADC port pin
//#######################
   // turn on
    quan::stm32::rcc::get()->apb2enr |= (1 << 10); //( ADC3)
   // reset adc
    quan::stm32::rcc::get()->apb2rstr |= ( 1 << 10 ); // (ADC3)
    quan::stm32::rcc::get()->apb2rstr &= ~( 1 << 10 );
   // set adc clk to 21 MHz
   // PCLK == sysclk/2 == 84 MHz
   // so set /4 --> 21 MHz
  // but maybe *2 so set /8
    ADC->CCR |= (0b11 << 16);
    // channel selection ADC3_IN3
    ADC3->SQR3 = 3;
  // have around 20 usec to get A2D value
  // so sampling time can be mod in SMPR1 set to 56 cycles
  // either 5.6 usec or 2.8 usec
    ADC3->SMPR2 = 0b11;
    ADC3->CR2  |= (1 << 0); // (ADON)

    // To start a conversion
 
}

void start_black_level_conversion()
{
     ADC3->CR2 |= (1 << 30);
}

void update_black_level(uint16_t sync_tip, uint16_t black_level)
{
// TODO
#if 0
  // can also use DAC?
   // 12 bit 
   float const white = sync_tip + (black_level - sync_tip) * 1.0f/0.3f;
   // 12 bit
   float const black = black_level;
   // put white to white pwm
   // put black to black pwm
   float const grey  =  black + 0.25 * ( white - black);
   float const data  = black + ( white - black)/2.f;
   //12 bit pwm  arr// get lowest freq 
#endif
}




