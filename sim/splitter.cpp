#include "splitter.hpp"

splitter::splitter(wxWindow* parent) : wxSplitterWindow(parent, wxID_ANY)
{
   SetSashGravity(1);
   SetSashPosition(2000);
   SetMinimumPaneSize(400);
   m_view = new view(this);
   m_panel = new panel(this);
   SplitVertically(m_view,m_panel);
}
