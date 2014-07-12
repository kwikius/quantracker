
#include "sp_in_thread.hpp"
#include "app.h"
#include <cstring>
#include <quan/conversion/float_convert.hpp>
#include "document.hpp"
#include "events.hpp"
#include "panel.hpp"
#include "main_frame.h"

sp_in_thread::sp_in_thread(main_frame * m)
:wxThread{wxTHREAD_DETACHED}, m_main_frame(m)
{

}

sp_in_thread::~sp_in_thread()
{

}


/*
#ifdef DEBUG
   char buf[100];
   sprintf(buf,"distance = %f m\n",static_cast<double>(distance.numeric_value()));
   debug::serial_port::write( buf);
   sprintf(buf,"bearing = %f deg\n",static_cast<double>(bearing.numeric_value()));
   debug::serial_port::write( buf);
   sprintf(buf,"elevation = %f deg\n",static_cast<double>(elevation.numeric_value()));
   debug::serial_port::write( buf);
#endif
*/
namespace {
   char buffer [1000];
   size_t  idx = 0;
   void parse_buffer()
   {  
     switch (buffer[0]){
         case 'd':{
            if (strncmp(buffer,"distance = ",11) ==0){
               float v = quan::convert<float>(&buffer[11]);
               auto & app = wxGetApp();
               app.get_document()->m_remote_tracker_params.distance = quan::length::m{v};
               QuanTrackerSimEvent x{wxEvent_RemoteDistanceChanged};
               app.get_panel()->AddPendingEvent(x);
            }
         }
         break;  
         case 'b' :{
            if (strncmp(buffer,"bearing = ",10) ==0){
               
               float v = quan::convert<float>(&buffer[10]);
               auto & app = wxGetApp();
               app.get_document()->m_remote_tracker_params.bearing = quan::angle::deg{v};
               QuanTrackerSimEvent x{wxEvent_RemoteBearingChanged};
               app.get_panel()->AddPendingEvent(x);
            }
         }
         break;
         case 'e':{
            if (strncmp(buffer,"elevation = ",12) ==0){
               float v = quan::convert<float>(&buffer[12]);
               auto & app = wxGetApp();
               app.get_document()->m_remote_tracker_params.elevation = quan::angle::deg{v};
               QuanTrackerSimEvent x{wxEvent_RemoteElevationChanged};
               app.get_panel()->AddPendingEvent(x);
            }
         }
         break;
         default: 
         break;
     }
    
   }
}
void * sp_in_thread::Entry()
{
   while ( !this->TestDestroy()){
      if ( idx == 999){
         idx = 0;
      }
      auto & app = wxGetApp();
      bool new_data = false;
      char ch;
      { // CS
         wxCriticalSectionLocker lock(app.m_sp_CS);
         if ( app.have_sp() ){
            auto sp = app.get_sp();
            if ( sp->in_avail() ){
               unsigned  char uch;
               sp->read(&uch,1);
               ch = (char) uch;
               new_data = true;
            }
         }
      }// ~CS
      if (new_data){
         if ( ch == '\n'){
            buffer[idx] = '\0';
            idx = 0;
            parse_buffer();
         }else{
            buffer[idx] = ch;
            ++idx;
         }
      }
   }
   wxCriticalSectionLocker(m_main_frame->m_thread_CS);
   m_main_frame->m_sp_in_thread = nullptr;
   return nullptr;
}