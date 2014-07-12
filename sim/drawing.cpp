#include "drawing.hpp"
#include "document.hpp"
#include "app.h"

drawing::vect drawing::get_size() const
{
  return wxGetApp().get_document()->get_map_size();
   
}
