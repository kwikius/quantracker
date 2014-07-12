#include "panel.hpp"
#include <wx/wx.h>
#include <quan/serial_port.hpp>
#include "app.h"
#include <quan/gx/wxwidgets/from_wxString.hpp>
#include "window_ids.hpp"
#include "events.hpp"
#include "document.hpp"
#include "main_frame.h"
#include "view.hpp"

/*
 add lat lon distance etc

 add info from tracker v info computed locally
*/
panel::panel (wxWindow* parent)
//: wxPanel{parent}
   :  wxScrolledWindow {parent}
,BtnConnect {nullptr}
,PortText {nullptr}
,LatText {nullptr}
,LonText {nullptr}
,AltText {nullptr}
,AltitudeSlider {nullptr}
,ProtocolChooser {nullptr}
{
   // add scrolling
   window_ids::panel = this->GetId();
   
   auto vert_sizer = new wxBoxSizer (wxVERTICAL);
   
   auto sp_sizer =  new wxFlexGridSizer (2,2,4,4);
   auto port_label = new wxStaticText{this,wxID_ANY, wxT ("serial port"),wxDefaultPosition, wxDefaultSize, 0};
   sp_sizer->Add (port_label,0,wxALL,5);
   PortText = new wxTextCtrl{this,wxID_ANY, wxT ("/dev/ttyUSB0"), wxDefaultPosition,wxSize (200,wxDefaultSize.GetHeight()) };
   sp_sizer->Add (PortText,0,wxALL,5);

   wxString choices[] = {wxT ("COBS"),wxT ("ByteStuff") };
   ProtocolChooser = new wxRadioBox (this,idProtocolChooser,wxT ("Protocol"),wxDefaultPosition,wxDefaultSize,2,choices);
   //check its set to COBS
   ProtocolChooser->SetSelection (0);
   sp_sizer->Add (ProtocolChooser);

   BtnConnect = new wxButton (this, idBtnConnect, wxT ("Connect"), wxDefaultPosition, wxSize (100,wxDefaultSize.GetHeight()), 0);
   sp_sizer->Add (BtnConnect,0,wxALL,5);
   vert_sizer->Add (sp_sizer);
   
   auto vert_spacer1 =  new wxPanel{this,0,0, 1, 1, 0,  wxT ("") };
   vert_sizer->Add (vert_spacer1,0,wxALL,10);
   
   auto min_scale = 1;
   auto max_scale = 20;
   auto scale_sizer = new wxFlexGridSizer (2,2,4,4);
   
   auto scale_spacer =  new wxPanel{this,0,0, 1, 1, 0,  wxT ("") };
   scale_sizer->Add (scale_spacer,0,wxALL,5);
   
   ScaleText =new wxTextCtrl{
      this,wxID_ANY,
      wxT ("0.01"),
      wxDefaultPosition,wxSize (100,wxDefaultSize.GetHeight())
   };
   
   ScaleSlider = new wxSlider (this,idScaleSlider,min_scale,min_scale,max_scale, wxDefaultPosition,wxSize (200,wxDefaultSize.y),
                               wxSL_HORIZONTAL ,wxDefaultValidator,wxT ("Scale"));
                               
   scale_sizer->Add (ScaleText,0,wxALL,5);
   
   auto scale_label = new wxStaticText{this,wxID_ANY, wxT ("map scale"),wxDefaultPosition, wxDefaultSize, 0};
   scale_sizer->Add (scale_label,0,wxALL,5);
   
   scale_sizer->Add (ScaleSlider,0,wxALL,5);
   vert_sizer->Add (scale_sizer);
   
   auto & app = wxGetApp();
   auto doc = app.get_document();
   auto aircraft_gps_position = doc->get_aircraft_gps_position<quan::angle::deg, quan::length::m>();
   
   auto gps_params_sizer = new wxFlexGridSizer (3,2,4,4);
   
   auto lat_label = new wxStaticText{this,wxID_ANY, wxT ("latitude"),wxDefaultPosition, wxDefaultSize, 0};
   LatText = new wxTextCtrl{
      this,wxID_ANY,
      wxString::Format (wxT ("%f deg"),aircraft_gps_position.lat.numeric_value()),
      wxDefaultPosition,wxSize (200,wxDefaultSize.GetHeight())
   };
   gps_params_sizer->Add (lat_label,0,wxALL,5);
   gps_params_sizer->Add (LatText,0,wxALL,5);
   
   auto lon_label = new wxStaticText{this,wxID_ANY, wxT ("longtitude"),wxDefaultPosition, wxDefaultSize, 0};
   LonText = new wxTextCtrl{
      this,wxID_ANY,
      wxString::Format (wxT ("%f deg"),aircraft_gps_position.lon.numeric_value()),
      wxDefaultPosition,wxSize (200,wxDefaultSize.GetHeight())
   };
   gps_params_sizer->Add (lon_label,0,wxALL,5);
   gps_params_sizer->Add (LonText,0,wxALL,5);
   
   auto alt_label = new wxStaticText{this,wxID_ANY, wxT ("plane sea lvl alt"),wxDefaultPosition, wxDefaultSize, 0};
   AltText = new wxTextCtrl{
      this,wxID_ANY,
      wxString::Format (wxT ("%f m"),aircraft_gps_position.alt.numeric_value()),
      wxDefaultPosition,wxSize (200,wxDefaultSize.GetHeight())
   };
   gps_params_sizer->Add (alt_label,0,wxALL,5);
   gps_params_sizer->Add (AltText,0,wxALL,5);
   
   vert_sizer->Add (gps_params_sizer);
   
   auto vert_spacer2 =  new wxPanel{this,0,0, 1, 1, 0,  wxT ("") };
   vert_sizer->Add (vert_spacer2,0,wxALL,10);
   
   auto pos_params_sizer = new wxFlexGridSizer (3,2,4,4);
   
   auto local_distance_label = new wxStaticText{this,wxID_ANY, wxT ("actual distance"),wxDefaultPosition, wxDefaultSize, 0};
   LocalDistanceText =  new wxTextCtrl{
      this,wxID_ANY,
      wxT ("0 m"),
      wxDefaultPosition,wxSize (200,wxDefaultSize.GetHeight())
   };
   pos_params_sizer->Add (local_distance_label,0,wxALL,5);
   pos_params_sizer->Add (LocalDistanceText,0,wxALL,5);
   
   auto remote_distance_label = new wxStaticText{this,wxID_ANY, wxT ("remote read dist"),wxDefaultPosition, wxDefaultSize, 0};
   RemoteDistanceText =  new wxTextCtrl{
      this,wxID_ANY,
      wxT ("0 m"),
      wxDefaultPosition,wxSize (200,wxDefaultSize.GetHeight())
   };
   pos_params_sizer->Add (remote_distance_label,0,wxALL,5);
   pos_params_sizer->Add (RemoteDistanceText,0,wxALL,5);
   
   auto local_bearing_label = new wxStaticText{this,wxID_ANY, wxT ("actual bearing"),wxDefaultPosition, wxDefaultSize, 0};
   LocalBearingText =  new wxTextCtrl{
      this,wxID_ANY,
      wxT ("0 deg"),
      wxDefaultPosition,wxSize (200,wxDefaultSize.GetHeight())
   };
   pos_params_sizer->Add (local_bearing_label,0,wxALL,5);
   pos_params_sizer->Add (LocalBearingText,0,wxALL,5);
   
   auto remote_bearing_label = new wxStaticText{this,wxID_ANY, wxT ("remote read bear"),wxDefaultPosition, wxDefaultSize, 0};
   RemoteBearingText =  new wxTextCtrl{
      this,wxID_ANY,
      wxT ("0 deg"),
      wxDefaultPosition,wxSize (200,wxDefaultSize.GetHeight())
   };
   pos_params_sizer->Add (remote_bearing_label,0,wxALL,5);
   pos_params_sizer->Add (RemoteBearingText,0,wxALL,5);
   
   auto local_elevation_label = new wxStaticText{this,wxID_ANY, wxT ("actual elevation"),wxDefaultPosition, wxDefaultSize, 0};
   LocalElevationText =  new wxTextCtrl{
      this,wxID_ANY,
      wxT ("0 deg"),
      wxDefaultPosition,wxSize (200,wxDefaultSize.GetHeight())
   };
   pos_params_sizer->Add (local_elevation_label,0,wxALL,5);
   pos_params_sizer->Add (LocalElevationText,0,wxALL,5);
   
   auto remote_elevation_label = new wxStaticText{this,wxID_ANY, wxT ("remote read elev"),wxDefaultPosition, wxDefaultSize, 0};
   RemoteElevationText =  new wxTextCtrl{
      this,wxID_ANY,
      wxT ("0 deg"),
      wxDefaultPosition,wxSize (200,wxDefaultSize.GetHeight())
   };
   pos_params_sizer->Add (remote_elevation_label,0,wxALL,5);
   pos_params_sizer->Add (RemoteElevationText,0,wxALL,5);
   vert_sizer->Add (pos_params_sizer);
   
   auto vert_spacer3 =  new wxPanel{this,0,0, 1, 1, 0,  wxT ("") };
   vert_sizer->Add (vert_spacer3,0,wxALL,10);
   
   auto horz_sizer = new wxBoxSizer (wxHORIZONTAL);
   auto min_alt = 0; // meters
   auto max_alt = 300; //meters
   AltitudeSlider = new wxSlider (this,idAltitudeSlider,min_alt,min_alt,max_alt, wxDefaultPosition,wxSize (wxDefaultSize.x, 300),
                                  wxSL_VERTICAL | wxSL_INVERSE |  wxSL_LABELS ,wxDefaultValidator,wxT ("Altitude"));
                                  
   horz_sizer->Add (AltitudeSlider,0,wxALL,5);
   horz_sizer->Add (vert_sizer);
   this->SetSizer (horz_sizer);
   this->Layout();
   horz_sizer->Fit (this);
   
   // this part makes the scrollbars show up
   this->FitInside(); // ask the sizer about the needed size
   this->SetScrollRate (5, 5);
}

BEGIN_EVENT_TABLE (panel, wxPanel)
   EVT_BUTTON (idBtnConnect, panel::OnConnectDisconnect)
   EVT_CUSTOM (wxEvent_AircraftPositionChanged,wxID_ANY,panel::OnAircraftPositionChanged)
   EVT_CUSTOM (wxEvent_RemoteDistanceChanged,wxID_ANY,panel::OnRemoteDistanceChanged)
   EVT_CUSTOM (wxEvent_RemoteBearingChanged,wxID_ANY,panel::OnRemoteBearingChanged)
   EVT_CUSTOM (wxEvent_RemoteElevationChanged,wxID_ANY,panel::OnRemoteElevationChanged)
   EVT_COMMAND_SCROLL (idAltitudeSlider,panel::OnAltitudeSlider)
   EVT_COMMAND_SCROLL (idScaleSlider,panel::OnScaleSlider)
   EVT_RADIOBOX (idProtocolChooser,panel::OnProtocolChanged)
END_EVENT_TABLE()


void panel::OnScaleSlider (wxScrollEvent & event)
{
   auto v = ScaleSlider->GetValue();
   double scale = 0.01 / v;
   wxGetApp().get_view()->set_scale (scale);
   ScaleText->ChangeValue (wxString::Format (wxT ("%f"),scale));
}

void panel::OnAltitudeSlider (wxScrollEvent & event)
{
   quan::length::m alt {AltitudeSlider->GetValue() };
   auto doc = wxGetApp().get_document();
   
   doc->set_aircraft_alt (alt);
   
   auto gps_pos = doc->get_aircraft_gps_position<quan::angle::deg,quan::length::m>();
   
   AltText->ChangeValue (wxString::Format (wxT ("%f m"),gps_pos.alt.numeric_value()));
}

void panel::OnConnectDisconnect (wxCommandEvent &event)
{
   auto & app = wxGetApp();
   app.m_sp_CS.Enter();
   if (!app.have_sp())
   {
      app.m_sp_CS.Leave();
      std::string text = quan::gx::wxwidgets::from_wxString<char> (PortText->GetValue());
      // do a test open of the port
      auto testsp = new quan::serial_port (text.c_str());
      testsp->init();
      if (testsp->good()) {
         app.m_sp_CS.Enter();
         wxGetApp().set_sp (testsp);
         OnProtocolChanged();
         app.m_sp_CS.Leave();
         testsp =0;
         BtnConnect->SetLabel (wxT ("Disconnect"));
         // grey ProtocolChooser
         ProtocolChooser->Enable(0,false);
         ProtocolChooser->Enable(1,false);
         return;
      }
      else {
         delete testsp;
         wxString msg = wxT ("Open \"") + wxString::FromAscii (text.c_str()) + wxT ("\" failed. Check port.");
         wxMessageBox (msg,wxT ("Connect Error"));
         return;
      }
   } else { // want disconnect
      {
         app.close_sp();
         app.m_sp_CS.Leave();
      }
      // ungrey ProtocolChooser
      ProtocolChooser->Enable(0,true);
      ProtocolChooser->Enable(1,true);
      BtnConnect->SetLabel (wxT ("Connect"));
   }
}

void  panel::OnAircraftPositionChanged (wxEvent& event)
{
   auto & app = wxGetApp();
   auto doc = app.get_document();
   auto aircraft_gps_position = doc->get_aircraft_gps_position<quan::angle::deg, quan::length::m>();
   LatText->ChangeValue (wxString::Format (wxT ("%f deg"),aircraft_gps_position.lat.numeric_value()));
   LonText->ChangeValue (wxString::Format (wxT ("%f deg"),aircraft_gps_position.lon.numeric_value()));
   AltText->ChangeValue (wxString::Format (wxT ("%f m"),aircraft_gps_position.alt.numeric_value()));
   
   auto tp = doc->get_tracker_params();
   
   LocalDistanceText->ChangeValue (wxString::Format (wxT ("%f m"),tp.distance.numeric_value()));
   LocalBearingText->ChangeValue (wxString::Format (wxT ("%f deg"),tp.bearing.numeric_value()));
   LocalElevationText->ChangeValue (wxString::Format (wxT ("%f deg"),tp.elevation.numeric_value()));
}

void panel::OnRemoteDistanceChanged (wxEvent& event)
{
   auto & app = wxGetApp();
   auto doc = app.get_document();
   auto dist = doc->m_remote_tracker_params.distance;
   RemoteDistanceText->ChangeValue (wxString::Format (wxT ("%f m"),dist.numeric_value()));
}

void panel::OnRemoteBearingChanged (wxEvent& event)
{
   auto & app = wxGetApp();
   auto doc = app.get_document();
   auto bearing = doc->m_remote_tracker_params.bearing;
   if (bearing < quan::angle::deg {0}) {
      bearing += quan::angle::deg {360};
   }
   RemoteBearingText->ChangeValue (wxString::Format (wxT ("%f deg"),bearing.numeric_value()));
}

void panel::OnRemoteElevationChanged (wxEvent& event)
{
   auto & app = wxGetApp();
   auto doc = app.get_document();
   auto elevation = doc->m_remote_tracker_params.elevation;
   RemoteElevationText->ChangeValue (wxString::Format (wxT ("%f deg"),elevation.numeric_value()));
}

void panel::OnProtocolChanged ()
{
   auto & app = wxGetApp();
  
   if (this->want_cobs_protocol()) {
     app.get_main_frame()->Timer->Start(200,wxTIMER_CONTINUOUS); 
     app.get_sp()->set_baud(1200);
   } else {
     app.get_main_frame()->Timer->Start(20,wxTIMER_CONTINUOUS); 
     app.get_sp()->set_baud(9600);
   }
}

void panel::OnProtocolChanged (wxCommandEvent& )
{
   // this->OnProtocolChanged();
}

