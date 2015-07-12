/*
 Copyright (c) 2012 - 2014 Andy Little 

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

#include <stm32f4xx.h>
#include <quan/stm32/rcc.hpp>
#include <quan/stm32/f4/exti/set_exti.hpp>
#include <quan/stm32/f4/syscfg/module_enable_disable.hpp>
#include <quan/atan2.hpp>
#include <quan/three_d/vect.hpp>
#include <quan/constrain.hpp>
#include "resources.hpp"
#include "compass.hpp"

SemaphoreHandle_t raw_compass::m_mutex = NULL;
quan::three_d::vect<float> raw_compass ::m_raw_value = {0.f,0.f,0.f};
int32_t raw_compass::m_strap_value = 0;
float raw_compass::m_filter_value = 0.1f;
bool raw_compass::m_request_disable_updating = false;
bool raw_compass::m_updating_enabled = true;
bool raw_compass::m_use_compass = false;
bool raw_compass::m_compass_offset_set = false;
quan::three_d::vect<float> raw_compass::m_offset = {0.f,0.f,0.f};

SemaphoreHandle_t get_mag_ready_semaphore();

bool raw_compass::get_bearing( quan::angle::deg & bearing_out)
{
   if ( ! m_compass_offset_set){
      return false;
   }
  
   quan::three_d::vect<float> v = get_vect();
   quan::angle::deg result = quan::atan2(v.y, v.x) + quan::angle::pi/2;
   while (result < quan::angle::deg{0}){
      result += quan::angle::deg{360};
   }
   bearing_out = result;
   return true;
}

void raw_compass::set_mag_offset(quan::three_d::vect<float> const & offset)
{
   m_offset = offset;
   m_compass_offset_set = true;
}

void raw_compass::set_use_compass( bool val)
{
   m_use_compass = val;
}

void raw_compass::set_strap(int32_t val)
{
   switch(val){
      case 0:
      case 1:
      case -1:
      m_strap_value = val;
      break;
      default:
      break;
   }
}

int32_t raw_compass::get_strap()
{
   return m_strap_value;
}

void raw_compass::set_filter(float const & val)
{
   m_filter_value = quan::constrain(val,0.f,1.f);
}

void raw_compass::clear()
{
   m_raw_value = {0.f,0.f,0.f};
}

quan::three_d::vect<float> const& raw_compass::get_raw()
{
   return m_raw_value ;
}

quan::three_d::vect<float> const& raw_compass::get_offset()
{
   return m_offset ;
}

quan::three_d::vect<float> raw_compass::get_vect()
{
   return m_raw_value - m_offset;
}

int32_t  ll_update_mag(quan::three_d::vect<int16_t> & result_out,int32_t strap);

void raw_compass::request_disable_updating()
{
   if ( m_updating_enabled){
      m_request_disable_updating = true;
   }
}

void raw_compass::enable_updating()
{
   m_request_disable_updating = false;
   m_updating_enabled = true;
}

int32_t raw_compass::update()
{
   if (m_updating_enabled){
      quan::three_d::vect<int16_t> result;
      int res = ::ll_update_mag(result,raw_compass::get_strap());
      if ( res == 1){
         taskENTER_CRITICAL();
         if ( raw_compass::acquire_mutex(1) == pdTRUE){

            raw_compass::m_raw_value 
               = raw_compass::m_raw_value * (1.f - raw_compass::m_filter_value) 
                  + result * raw_compass::m_filter_value;
            raw_compass::release_mutex();
         }
         taskEXIT_CRITICAL();
         // Disable updating here because its the end of a cycle so nice and neat
         if ( m_request_disable_updating){
            m_request_disable_updating = false;
            m_updating_enabled = false;
         }
      }
      return res;
   }else{
      return false;
   }
}

namespace {

//   void setup_mag_ready_irq()
//   {
//      quan::stm32::module_enable<quan::stm32::syscfg>(); 
//      quan::stm32::set_exti_syscfg<mag_rdy_exti_pin>();
//      quan::stm32::set_exti_falling_edge<mag_rdy_exti_pin>();
//      NVIC_SetPriority(I2C3_EV_IRQn,local_interrupt_priority::exti_mag_rdy);
//      quan::stm32::nvic_enable_exti_irq<mag_rdy_exti_pin>();
//      quan::stm32::module_enable<mag_rdy_exti_pin::port_type>();
//      quan::stm32::apply<
//         mag_rdy_exti_pin
//         , quan::stm32::gpio::mode::input
//         , quan::stm32::gpio::pupd::none // make this pullup ok as mag is on 3v?
//      >();
      
      //quan::stm32::enable_exti_interrupt<mag_rdy_exti_pin>();
  // }
}

bool raw_compass::acquire_mutex(TickType_t ticks)
{
   if ( m_mutex != NULL){
     return xSemaphoreTake(m_mutex,ticks) == pdTRUE;
   }else{
      return false;
   }
}

void raw_compass::release_mutex()
{
    xSemaphoreGive(m_mutex);
}

void raw_compass::init()
{
   m_mutex = xSemaphoreCreateMutex();
  // setup_mag_ready_irq();
   NVIC_SetPriority(I2C3_EV_IRQn,local_interrupt_priority::i2c_mag_evt);
   i2c_mag_port::init(false,false);
}

extern "C" void I2C3_EV_IRQHandler() __attribute__ ((interrupt ("IRQ")));
extern "C" void I2C3_EV_IRQHandler()
{     

   static_assert(std::is_same<i2c_mag_port::i2c_type, quan::stm32::i2c3>::value,"incorrect port irq");
   i2c_mag_port::handle_irq();
}

extern "C" void I2C3_ER_IRQHandler() __attribute__ ((interrupt ("IRQ")));
extern "C" void I2C3_ER_IRQHandler()
{
   static_assert(std::is_same<i2c_mag_port::i2c_type, quan::stm32::i2c3>::value,"incorrect port irq");
   uint32_t const sr1 = i2c_mag_port::i2c_type::get()->sr1.get();
   i2c_mag_port::i2c_type::get()->sr1.set(sr1 & 0xFF); 
   i2c_mag_port::i2c_errno = i2c_mag_port::errno_t::i2c_err_handler;
}


