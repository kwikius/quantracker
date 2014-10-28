
#ifndef QUAN_OSD_BOARD_TYPE
   #error which board?
#else
   #if QUAN_OSD_BOARD_TYPE == 1
      #pragma message "for board 1"
   #else 
      #if QUAN_OSD_BOARD_TYPE == 2
         #pragma message "for board 2"
      #else
         #if QUAN_OSD_BOARD_TYPE == 3
            #pragma message "for board 3"
         #else
            #error unknown board
         #endif
      #endif
   #endif
#endif

#include <cstdint>
#include "resources.hpp"
#include "graphics_api.hpp"
#include <quan/stm32/serial_port.hpp>
// n.b need tasking switching here!

int main()
{
#if (QUAN_OSD_BOARD_TYPE == 2 )  || (QUAN_OSD_BOARD_TYPE == 3 )
   
    serial_port::write("OSD startup\n");

    constexpr uint8_t dac_sync_idx = 0;
    constexpr uint8_t dac_black_idx = 1;
    constexpr uint8_t dac_white_idx = 2;
    constexpr uint8_t dac_data_idx = 3;

//0.16 too low
//0.18 worksbut not great
// 0.2 works but not great
// 0.24 works but not great
// 0.26 ok but not at bottom
//0.27
//0.28 look v good at top bu not bottom
//0.29
//0.3 too high

   // Dac_write (dac_sync_idx, quan::voltage::V{0.4f}, 0);
    Dac_write (dac_black_idx, quan::voltage::V{0.64f}, 0); // 0.64
    Dac_write (dac_white_idx, quan::voltage::V{2.04f} , 0); // 2.04
    Dac_write (dac_data_idx, quan::voltage::V{1.2f}, 1);
#endif
   while (1){
     draw_loop();
   }
}