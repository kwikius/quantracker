#ifndef QUANTRACKER_AIR_OSD_RESOURCES_HPP_INCLUDED
#define QUANTRACKER_AIR_OSD_RESOURCES_HPP_INCLUDED
// after taulabs, openpilot,brainfpv
#include <quan/voltage.hpp>
#include <quan/stm32/gpio.hpp>
#include <quan/stm32/spi.hpp>
#include <quan/stm32/tim.hpp>
#include <quan/stm32/usart.hpp>
#include <quan/stm32/serial_port.hpp>

   typedef quan::stm32::spi2                       video_mux_out_black_spi;
   typedef quan::stm32::spi3                       video_mux_out_white_spi;

   typedef quan::stm32::tim1                       spi_clock_timer;
   typedef quan::stm32::tim2                       video_columns_gate_timer;
   typedef quan::stm32::tim3                       video_rows_line_counter;
   typedef quan::stm32::tim6                       fsk_dac_timer;
#if defined QUAN_OSD_SOFTWARE_SYNCSEP
   typedef quan::stm32::tim9                       sync_sep_timer;
#endif
   typedef quan::stm32::tim10                      video_level_dac_irq_timer;
   typedef quan::stm32::usart3 debug_usart;

   typedef quan::mcu::pin<quan::stm32::gpioa,0>    heartbeat_led_pin; 
   typedef quan::mcu::pin<quan::stm32::gpioa,1>    telem_cmp_enable_pin; // TIM2_CH2 


#if defined QUAN_DISCOVERY
typedef quan::mcu::pin<quan::stm32::gpioc,8>       heartbeat_led_pin; // stm32F0 Discovery blue led
#else
typedef quan::mcu::pin<quan::stm32::gpioa,0>       heartbeat_led_pin; 
#endif
#if !(defined QUAN_OSD_SOFTWARE_SYNCSEP)
#error need to sort pins here
 
  // typedef quan::mcu::pin<quan::stm32::gpioa,2>    video_in_odd_even_pin ; // no special requirements!
#endif
   typedef quan::mcu::pin<quan::stm32::gpioa,2>    frsky_txo_pin;
   typedef quan::mcu::pin<quan::stm32::gpioa,3>    frsky_rxi_pin;
// could change to 
  //typedef quan::mcu::pin<quan::stm32::gpioa,3>  analog_video_in; // ADC123_IN3

  //typedef  quan::mcu::pin<quan::stm32::gpioa,3> test_pin; 

   typedef quan::mcu::pin<quan::stm32::gpioa,4>    fsk_dac_out_pin; 
 //keep pa5 free for stm32 DAC output
   typedef quan::mcu::pin<quan::stm32::gpioa,8>    video_spi_clock ; // TIM1_CH1
   typedef quan::mcu::pin<quan::stm32::gpioa,9>    posdata_txo_pin; 
   typedef quan::mcu::pin<quan::stm32::gpioa,10>   posdata_rxi_pin;

   typedef quan::mcu::pin<quan::stm32::gpioa,15>   video_in_tim2_hsync_pin ; // TIM2_CH1 ( also TIM2_ETR)

   typedef quan::mcu::pin<quan::stm32::gpiob,3>    video_mux_out_white_sck; // SPI3_SCK AF6
   typedef quan::mcu::pin<quan::stm32::gpiob,4>    video_mux_out_white_miso; // SPI3_MISO AF6
   typedef quan::mcu::pin<quan::stm32::gpiob,7>    av_video_rxi; // USART1_RX  fast 10.5 Mbps
  
   typedef quan::mcu::pin<quan::stm32::gpiob,13>   video_mux_out_black_sck; // SPI2_SCK AF5
   typedef quan::mcu::pin<quan::stm32::gpiob,14>   video_mux_out_black_miso; // SPI2_MISO AF5

   // SPI2_TX DMA1 Stream 4 Channel 0
   // SPI3_TX DMA1 Stream 5 Channel 0
   typedef quan::stm32::usart1 av_telem_in_usart;

#if !(defined QUAN_OSD_SOFTWARE_SYNCSEP)
   typedef quan::mcu::pin<quan::stm32::gpioc,6> video_in_vsync_pin ; // TIM3_CH1
#endif

//struct video_in{
   // capture 2nd edge of hsync trigger start gate timer
   typedef quan::mcu::pin<quan::stm32::gpiod,2>  video_in_tim3_hsync_pin; // TIM3_ETR

#if defined QUAN_OSD_SOFTWARE_SYNCSEP
   typedef quan::mcu::pin<quan::stm32::gpioe,5> video_in_hsync_first_edge_pin; // TIM9_CH1
   typedef quan::mcu::pin<quan::stm32::gpioe,6> video_in_hsync_second_edge_pin; // TIM9_CH2
#endif

typedef quan::mcu::pin<quan::stm32::gpiod,8>    debug_txo_pin; // SF:USART3_TX:AF7
typedef quan::mcu::pin<quan::stm32::gpiod,9>    debug_rxi_pin; // SF:UASRT3_RX:AF7

typedef quan::stm32::serial_port<
   debug_usart,500,500,debug_txo_pin,debug_rxi_pin
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
typedef quan::stm32::usart1  posdata_usart;
typedef quan::stm32::usart2  frsky_usart;

struct interrupt_priority{
   static constexpr uint32_t systick_timer = 15;
   static constexpr uint32_t frsky_serial_port= 14;
   static constexpr uint32_t telemetry_input_port = 13;
   static constexpr uint32_t fsk_dac_timer = 12;
};

bool is_transmitter();
bool is_receiver();

void draw_loop();
void do_tasks();



#endif // QUANTRACKER_AIR_OSD_RESOURCES_HPP_INCLUDED
