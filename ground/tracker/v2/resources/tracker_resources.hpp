#ifndef QUANTRACKER_GROUND_TRACKER_V2_TRACKER_RESOURCES_HPP_INCLUDED
#define QUANTRACKER_GROUND_TRACKER_V2_TRACKER_RESOURCES_HPP_INCLUDED


/*
non OSD resources used by the tracker
also add some leds
push buttons etc

--- pan motor
       motor_direction           PC14
       not_motor_direction       PC15
       motor pwm  ---            TIM10-Ch1 PB8    
       qdrtA --- needs a timer   TIM4-CH1 PB6
       qdrtB --- needs a timer   TIM4-CH2 PB7
      ( for qdrt either lose a sp or a i2c )
--- tilt servo
       tilt_servo_pwm            TIM11-CH1 PB9
--- sliprings_sp
       tx                        Mavlink port TX
       rx                        Mavlink Port Rx
--- frsky/mavlink telem 
       tx                        USART4-TXO PA0
       rx                        USART4-RXI PA1
--- magnetometer (baro)
       scl                       I2C3-SCL PA8
       sda                       I2C3-SDA PC9
       rdy                       PC5
--- MPU6000 spi
       MISO                      PB4
       MOSI                      PB5
       SCK                       PB3
--- GPS
       tx                        Mavlink Port TX2
       rx                        Mavlink Port RX2
--- fsk
       audioL  --- analog in     PC0
       audioR  --- analog in     PC1
---  RC out
       pwm                       TIM1-CH4   PA11  
--- battery level warning
      batt_level                 PB1

--- serial LCD inteface
     tx                          USART5-TXO  PC12
*/


#endif // QUANTRACKER_GROUND_TRACKER_V2_TRACKER_RESOURCES_HPP_INCLUDED
