/***************************************************************
 * Name:      main_frame.cpp
 * Purpose:   Code for Application Frame
 * Author:    Andy (kwikius@yahoo.com)
 * Created:   2013-03-06
 * Copyright: Andy (http://www.softpainter.org)
 * License:
 **************************************************************/

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include "main_frame.h"
#include "bytestuff.hpp"
#include "cobs.hpp"
#include "document.hpp"
#include "sp_in_thread.hpp"
#include "aircraft.hpp"

#include "app.h"
#include "splitter.hpp"
#include "sp_in_thread.hpp"

//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__WXMAC__)
        wxbuild << _T("-Mac");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

BEGIN_EVENT_TABLE(main_frame, wxFrame)
    EVT_CLOSE(main_frame::OnClose)
    EVT_MENU(idMenuQuit, main_frame::OnQuit)
    EVT_MENU(idMenuAbout, main_frame::OnAbout)
    EVT_TIMER(idTimer, main_frame::OnTimer)
END_EVENT_TABLE()

main_frame::main_frame(wxFrame *frame, const wxString& title)
    : wxFrame(frame, -1, title)
{
     
     this->m_splitter = new splitter(this);
#if wxUSE_MENUS
    // create a menu bar
    wxMenuBar* mbar = new wxMenuBar();
    wxMenu* fileMenu = new wxMenu(_T(""));
    fileMenu->Append(idMenuQuit, _("&Quit\tAlt-F4"), _("Quit the application"));
    mbar->Append(fileMenu, _("&File"));

    wxMenu* helpMenu = new wxMenu(_T(""));
    helpMenu->Append(idMenuAbout, _("&About\tF1"), _("Show info about this application"));
    mbar->Append(helpMenu, _("&Help"));

    SetMenuBar(mbar);
#endif // wxUSE_MENUS

#if wxUSE_STATUSBAR
    // create a status bar with some information about the used wxWidgets version
    CreateStatusBar(2);
    SetStatusText(_("Hello Code::Blocks user!"),0);
    SetStatusText(wxbuildinfo(short_f), 1);
#endif // wxUSE_STATUSBAR

    Timer= new wxTimer{this,idTimer};
    // update rate of 1/5th sec
    Timer->Start(200,wxTIMER_CONTINUOUS); 

    m_sp_in_thread = new sp_in_thread(this);
    m_sp_in_thread->Create();
    m_sp_in_thread->Run();

}

main_frame::~main_frame()
{}

bool main_frame::Destroy()
{
   {
      wxCriticalSectionLocker lock(m_thread_CS);
      if ( m_sp_in_thread != nullptr){
         m_sp_in_thread->Delete();
      };
   }

   for (;;)
   {
      wxCriticalSectionLocker lock(m_thread_CS);
      if ( m_sp_in_thread == nullptr){
         break;
      }
   }
   return wxFrame::Destroy();
}

void main_frame::OnClose(wxCloseEvent &event)
{
    Destroy();
}

void main_frame::OnQuit(wxCommandEvent &event)
{
    Destroy();
}

void main_frame::OnAbout(wxCommandEvent &event)
{
    wxString msg = wxbuildinfo(long_f);
    wxMessageBox(msg, _("Welcome to..."));
}

/*
 Timer represents the update rate of the  airborne telemetry unit
*/
void main_frame::OnTimer(wxTimerEvent &event)
{
   if (wxGetApp().have_sp()){
      auto & app = wxGetApp();
      auto doc = app.get_document();
      update_aircraft_gps_position( doc->get_aircraft_gps_position<quan::angle::deg, quan::length::m>());

      if ( m_splitter->m_panel->want_cobs_protocol()){
         COBS_send_message();
      }else{
         ByteStuff_send_message();
      }
   }
}
