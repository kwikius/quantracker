

struct abc_bitmap<uint8_t>* get_font_char( char c)
{
  int ch = c;
  if ( (ch > 31) && ( ch < 126)){
     return lookup[(ch - 32)];
  }
  else{
      return nullptr;
  }
}