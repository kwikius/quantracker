#ifndef RESOURCES_HPP_INCLUDED1
#define RESOURCES_HPP_INCLUDED1
#include <quan/voltage.hpp>
// after taulabs, openpilot,brainfpv
#include <quan/stm32/gpio.hpp>
#include <quan/stm32/spi.hpp>
#include <quan/stm32/tim.hpp>
#include <quan/stm32/usart.hpp>
#include <quan/stm32/serial_port.hpp>

// PB2 BOOT1

/*
typedef quan::stm32::tim3 rows::line_counter; 16 bit // POSS move
// need to have 3 compare channels available
  TIM3_ETR used to clock counter  ( hsync)
   could use :
      TIM8_CH2  on PC7 cant go more than 3 V, but ok here for hsync if comparator on 3V
      TIM12_CH2 on PB15
 in software sync sep mode

typedef quan::stm32::tim2 columns::gate_timer; 
   TIM2 CH2 -> OUTPUT pwm output for enabling comp for rx also TRGO for pixel_timer
rising or falling dependent on version
   TIM2_CH1 -> hsync input self triggers timer  (TI1)
   on rising edge if QUAN_OSD_BOARD == 1
   on faling edge if QUAN_OSD_BOARD == 2
   
 must be able to trigger TIM1 ( 5,4 ) with ch1,2,3,4. 
    poss PB7 TIM4_CH2 (have to move vid telem usart) -> hsync TI1
    poss PB8 TIM4_CH3  ->  usart telem  comp and TRGO
    
  // is hysnc input
 // output PWM on 1 channel must be 
 
typedef quan::stm32::tim1 spi_clock::timer;
*/

/*
   TIM2_CH2
   PWM output 
*/
  typedef quan::mcu::pin<quan::stm32::gpioa,1> telem_cmp_enable_pin; // TIM2_CH2 
  typedef  quan::mcu::pin<quan::stm32::gpioa,3> test_pin; 
// could change to 
  //typedef quan::mcu::pin<quan::stm32::gpioa,3>  analog_video_in; // ADC123_IN3

// keep pa4 free for stm32 DAC output
//keep pa5 free for stm32 DAC output
//PA6 free sort of has disco connections
// PA7  free sort of has disco connections
// PA8 used by quantracker
 // PA9  has LED on it

// nb by coupling usart to white spi. can maybe add TX pin
// looks like could work direct by writing sp output to
// white spi line on mux
// faster and less memory than calcing data state
  typedef quan::mcu::pin<quan::stm32::gpiob,7> av_video_rxi; // USART1_RX  fast 10.5 Mbps

  typedef quan::stm32::usart1 av_telem_in_usart;


struct video_mux{

   // Black is make bit0 low
   // white is make bit1 low
   // grey is make both low
   // transparent is keep both high
   struct out_black{
      typedef quan::mcu::pin<quan::stm32::gpiob,13> sck; // SPI2_SCK AF5
      typedef quan::mcu::pin<quan::stm32::gpiob,14> miso; // SPI2_MISO AF5
      typedef quan::stm32::spi2 spi;
      // SPI2_TX DMA1 Stream 4 Channel 0
   };

    struct out_white{
      typedef quan::mcu::pin<quan::stm32::gpiob,3> sck; // SPI3_SCK AF6
      typedef quan::mcu::pin<quan::stm32::gpiob,4> miso; // SPI3_MISO AF6
      typedef quan::stm32::spi3 spi;
      // SPI3_TX DMA1 Stream 5 Channel 0
   };
   typedef quan::mcu::pin<quan::stm32::gpioa,8>  spi_clock ; // TIM1_CH1
};

struct video_in{
   // capture 2nd edge of hsync trigger start gate timer
   typedef quan::mcu::pin<quan::stm32::gpioa,15> tim2_hsync_pin ; // TIM2_CH1 ( also TIM2_ETR)
   // clock line counter
   typedef quan::mcu::pin<quan::stm32::gpiod,2>  tim3_hsync_pin; // TIM3_ETR

#if defined QUAN_OSD_SOFTWARE_SYNCSEP
   typedef quan::mcu::pin<quan::stm32::gpioe,5> hsync_first_edge_pin; // TIM9_CH1
   typedef quan::mcu::pin<quan::stm32::gpioe,6> hsync_second_edge_pin; // TIM9_CH2
    typedef quan::mcu::pin<quan::stm32::gpioa,2> test_pin ; 
#else
   typedef quan::mcu::pin<quan::stm32::gpioc,6> vsync_pin ; // TIM3_CH1
   typedef quan::mcu::pin<quan::stm32::gpioa,2> odd_even_pin ; 
#endif
};

typedef quan::stm32::tim10 dac_irq_timer;
typedef quan::stm32::usart3 debug_usart;
typedef quan::mcu::pin<quan::stm32::gpiod,8>    txo_pin; // SF:USART3_TX:AF7
typedef quan::mcu::pin<quan::stm32::gpiod,9>    rxi_pin; // SF:UASRT3_RX:AF7

typedef quan::stm32::serial_port<
   debug_usart,500,500,txo_pin,rxi_pin
> serial_port;


typedef quan::mcu::pin<quan::stm32::gpiod,12>   green_led_pin;   // green led on Discovery
typedef quan::mcu::pin<quan::stm32::gpiod,13>   orange_led_pin;       // orange led on Discovery
typedef quan::mcu::pin<quan::stm32::gpiod,14>   red_led_pin;  // red led on Discover
typedef quan::mcu::pin<quan::stm32::gpiod,15>   blue_led_pin;       // blue led on Discover

#if (QUAN_OSD_BOARD_TYPE == 2 )  || (QUAN_OSD_BOARD_TYPE == 3 )
void Dac_setup();
// for 8 bit only msbyte of val is used
// code is 00 write to specific reg but dont update
// 1 is write to specific reg and update outputs
// 2 is write tao all registers and update outputs
// 3 is power down outputs
// really only 0 1nd 1 are useful
void Dac_write(uint8_t ch, quan::voltage::V const & vout, uint8_t code);

  typedef quan::mcu::pin<quan::stm32::gpiod,1> av_dac_nsync;
  typedef quan::mcu::pin<quan::stm32::gpiod,3> av_dac_data;
  typedef quan::mcu::pin<quan::stm32::gpiod,7> av_dac_clk;

//typedef quan::stm32::usart3 debug_usart;
//typedef quan::mcu::pin<quan::stm32::gpiod,8>    debug_txo_pin; // SF:USART3_TX:AF7
//typedef quan::mcu::pin<quan::stm32::gpiod,9>    debug_rxi_pin; // SF:UASRT3_RX:AF7
//
//   typedef quan::stm32::serial_port<
//      debug_usart,500,500,txo_pin,rxi_pin
//   > serial_port;
#endif

bool is_transmitter();
bool is_receiver();
/*
   using timers 1,2 3 and 9,10

   conflict with quantracker so move timer 2 to timer 5, timer 3 to tim4
*/

#endif // RESOURCES_HPP_INCLUDED
