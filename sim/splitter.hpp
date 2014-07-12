#ifndef QUANTRACKER_SPLITTER_HPP_INCLUDED
#define QUANTRACKER_SPLITTER_HPP_INCLUDED


#include <wx/splitter.h>

#include "panel.hpp"
#include "view.hpp"

struct splitter : wxSplitterWindow{
   splitter(wxWindow* parent);

   view * m_view;
   panel* m_panel;
   
};


#endif // QUANTRACKER_SPLITTER_HPP_INCLUDED
