/*
 Copyright (c) 2003-2014 Andy Little.

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program. If not, see http://www.gnu.org/licenses./
 */

#include "view.hpp"
#include "panel.hpp"
#include "frsky.hpp"
#include "events.hpp"
#include "window_ids.hpp"
#include "document.hpp"
#include <quan/gx/primitives/simple_line.hpp>

#include "aircraft_symbol.hpp"

BEGIN_EVENT_TABLE(view,wxWindow)

 EVT_PAINT(view::OnPaint)
 EVT_SIZE(view::OnSize)
 EVT_SCROLLWIN(view::OnScroll)
 EVT_LEFT_DOWN(view::OnMouseLeftDown)
 EVT_LEFT_UP(view::OnMouseLeftUp)
 EVT_MOTION(view::OnMouseMove)

END_EVENT_TABLE()

view::view(wxWindow* parent)
: wxWindow(parent, wxID_ANY)
,m_aircraft_selected{false}

{
    window_ids::view = this->GetId();
    this->SetWindowStyle(wxVSCROLL | wxHSCROLL);
    this->SetScrollbar(wxVERTICAL,50,10,110);
    this->SetScrollbar(wxHORIZONTAL,50,10,110);
    this->m_drawing_view.set_scale(0.01);
    auto doc = wxGetApp().get_document();
    m_last_aircraft_position = doc->get_aircraft_xyz_position();

//   update_aircraft_gps_position(doc->get_aircraft_gps_position<
//      quan::angle::deg10e7,
//      quan::length::m
//   >());
}

void view::set_scale(double const & v)
{
   m_drawing_view.set_scale(v);
  this->Refresh();
}

void view::OnPaint(wxPaintEvent & event)
{
   wxPaintDC dc(this);
   dc.SetBackground(* wxWHITE_BRUSH); // sets background brush but doesnt clear
   dc.Clear(); //       need to invoke to clear using current background brush
   
   quan::gx::wxwidgets::graphics_context wc{
      &dc, 
      &this->m_drawing,
      &this->m_drawing_view,
      &this->m_device_window
   };

   quan::two_d::box<mm> tracker{mm{-250},mm{250},mm{250},mm{-250}};
   wc.draw_box({tracker});

    

   auto doc = wxGetApp().get_document();
   auto ap = doc->get_aircraft_xyz_position();
   aircraft_symbol sym{ap,doc->get_aircraft_heading()};
   sym.draw(wc);

   vect2_mm ap1{ap.x,ap.y};
   quan::gx::primitives::simple_line<mm> loc {
      {mm{0},mm{0}},ap1,mm{1},
      quan::gx::rgb::colors::silver
   };
   wc.draw_line(loc);
}

// update device_window size data when size changes
void view::OnSize(wxSizeEvent & event)
{
   this->GetClientSize(
         &this->m_device_window.m_size_px.x,
         &this->m_device_window.m_size_px.y
   );
   
   // find window size in mm by getting size of pixel on display
   // may be an ondisplay change function so dont need to continaully update
   vect2_i display_size_in_px;
   wxDisplaySize(&display_size_in_px.x,&display_size_in_px.y);

   wxSize detail_display_size_mm = wxGetDisplaySizeMM();
   vect2_mm display_size_in_mm{mm{detail_display_size_mm.x},mm{detail_display_size_mm.y}};
   
   this->m_device_window.m_size_mm.x 
   = (this->m_device_window.m_size_px.x * display_size_in_mm.x) / display_size_in_px.x;
   this->m_device_window.m_size_mm.y 
   = (this->m_device_window.m_size_px.y * display_size_in_mm.y) / display_size_in_px.y;
  
   this->Refresh();

}

void view::OnScroll(wxScrollWinEvent & event)
{
  if (event.GetOrientation() == wxHORIZONTAL){
     this->OnHScroll(event);
  }else{
     this->OnVScroll(event);
  }
  this->Refresh();
}

void view::OnHScroll(wxScrollWinEvent & event)
{
   this->m_drawing_view.set_x_scroll_ratio((event.GetPosition() - 50 )/100.0); 
}

void view::OnVScroll(wxScrollWinEvent & event)
{
   this->m_drawing_view.set_y_scroll_ratio( -(event.GetPosition() - 50 )/100.0);
}

void view::OnMouseLeftDown(wxMouseEvent & event)
{
   event.Skip();

   quan::gx::wxwidgets::graphics_info_context ic{
      &this->m_drawing,
      &this->m_drawing_view,
      &this->m_device_window
   };

   // in drawing 
   auto mouse_down_pos = ic.device_to_drawing({event.GetX(),event.GetY()});
   auto doc = wxGetApp().get_document();
   auto aircraft_pos =  doc->get_aircraft_xyz_position();
   auto diff = abs(ic.scale_drawing_to_view(mouse_down_pos - vect2_mm{aircraft_pos.x,aircraft_pos.y}));

   auto click_bucket = doc->get_click_bucket();

  if ( (diff.x < click_bucket) && (diff.y  < click_bucket)){
     m_aircraft_selected = true;
  }

}
void view::OnMouseLeftUp(wxMouseEvent & event)
{
  m_aircraft_selected = false;
}

void view::OnMouseMove(wxMouseEvent & event)
{
   if ( m_aircraft_selected && event.Dragging()){

      quan::gx::wxwidgets::graphics_info_context ic{
         &this->m_drawing,
         &this->m_drawing_view,
         &this->m_device_window
      };
      
      auto mouse_pos = ic.device_to_drawing({event.GetX(),event.GetY()});
      auto & app = wxGetApp();
      auto doc = app.get_document();
      m_last_aircraft_position = doc->get_aircraft_xyz_position();
      vect2_mm last_xy {m_last_aircraft_position.x,m_last_aircraft_position.y};
      if ( mouse_pos != last_xy){
         doc->set_aircraft_xyz_position({mouse_pos.x,mouse_pos.y,m_last_aircraft_position.z});
         auto diff = mouse_pos - last_xy;
         auto angle = quan::atan2(diff.y,diff.x);
         doc->set_aircraft_heading(quan::angle::deg{90} - angle);
//         update_aircraft_gps_position(doc->get_aircraft_gps_position<
//            quan::angle::deg10e7,
//            quan::length::m
//         >());

         QuanTrackerSimEvent x{wxEvent_AircraftPositionChanged};
         app.get_panel()->AddPendingEvent(x);
         this->Refresh();
      }
   }
}


