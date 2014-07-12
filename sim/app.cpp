/***************************************************************
 * Name:      quantracker_simApp.cpp
 * Purpose:   Code for Application Class
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

#include "app.h"
#include "main_frame.h"
#include "panel.hpp"
#include "document.hpp"
#include "view.hpp"
#include "splitter.hpp"

IMPLEMENT_APP(quantracker_simApp);


bool quantracker_simApp::OnInit()
{
   
    m_frame = new main_frame(0L, _("Quantracker Sim"));

    m_frame->Show();
    
    return true;
}

view* quantracker_simApp::get_view()const 
{ return m_frame->m_splitter->m_view;}

quantracker_simApp::quantracker_simApp()
: m_frame{nullptr}
, m_document{new document}
, m_sp{nullptr}
{} 

panel * quantracker_simApp::get_panel() const
{
   assert( 
      (m_frame !=nullptr) && 
      (m_frame->m_splitter != nullptr) &&
      ( m_frame->m_splitter->m_panel != nullptr) 
   );
   return m_frame->m_splitter->m_panel;
}


quantracker_simApp::~quantracker_simApp()
{
   delete m_document;
}
