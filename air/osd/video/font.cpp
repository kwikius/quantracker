/*
 Copyright (c) 2013 -2015 Andy Little 

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

/*
 N.B Only for easrly testing. This code is likely to change!
*/

#include "bitmap.hpp"

namespace {
struct fontchar_A_t : static_image<uint8_t,12,18,2>{
uint8_t const * get_data()const
{
   return data;
}
private:
  static constexpr uint8_t data[54] = 
{
255,255,255
,255,255,255
,255,170,255
,191,85,254
,111,85,249
,91,85,229
,91,105,229
,91,190,229
,91,190,229
,91,190,229
,91,170,229
,91,85,229
,91,85,229
,91,170,229
,91,190,229
,91,190,229
,91,190,229
,175,255,250
};
} fontchar_A;
struct fontchar_B_t : static_image<uint8_t,12,18,2>{
uint8_t const * get_data()const
{
   return data;
}
private:
  static constexpr uint8_t data[54] = 
{
255,255,255
,255,255,255
,175,170,255
,91,85,254
,91,85,249
,91,106,229
,91,190,229
,91,190,229
,91,106,229
,91,85,249
,91,85,249
,91,106,229
,91,190,229
,91,190,229
,91,106,229
,91,85,249
,91,85,254
,175,170,255
};
} fontchar_B;
struct fontchar_C_t : static_image<uint8_t,12,18,2>{
uint8_t const * get_data()const
{
   return data;
}
private:
  static constexpr uint8_t data[54] = 
{
255,255,255
,255,255,255
,255,170,255
,191,85,254
,111,85,249
,91,105,229
,91,190,229
,91,254,250
,91,254,255
,91,254,255
,91,254,255
,91,254,255
,91,254,250
,91,190,229
,91,105,229
,111,85,249
,191,85,254
,255,170,255
};
} fontchar_C;
struct fontchar_D_t : static_image<uint8_t,12,18,2>{
uint8_t const * get_data()const
{
   return data;
}
private:
  static constexpr uint8_t data[54] = 
{
255,255,255
,255,255,255
,175,170,255
,91,85,254
,91,85,249
,91,106,229
,91,190,229
,91,190,229
,91,190,229
,91,190,229
,91,190,229
,91,190,229
,91,190,229
,91,190,229
,91,106,229
,91,85,249
,91,85,254
,175,170,255
};
} fontchar_D;
struct fontchar_E_t : static_image<uint8_t,12,18,2>{
uint8_t const * get_data()const
{
   return data;
}
private:
  static constexpr uint8_t data[54] = 
{
255,255,255
,255,255,255
,175,170,250
,91,85,229
,91,85,229
,91,170,250
,91,254,255
,91,254,255
,91,170,255
,91,85,254
,91,85,254
,91,170,255
,91,254,255
,91,254,255
,91,170,250
,91,85,229
,91,85,229
,175,170,250
};
} fontchar_E;
struct fontchar_F_t : static_image<uint8_t,12,18,2>{
uint8_t const * get_data()const
{
   return data;
}
private:
  static constexpr uint8_t data[54] = 
{
255,255,255
,255,255,255
,175,170,250
,91,85,229
,91,85,229
,91,170,250
,91,254,255
,91,254,255
,91,170,255
,91,85,254
,91,85,254
,91,170,255
,91,254,255
,91,254,255
,91,254,255
,91,254,255
,91,254,255
,175,255,255
};
} fontchar_F;
struct fontchar_G_t : static_image<uint8_t,12,18,2>{
uint8_t const * get_data()const
{
   return data;
}
private:
  static constexpr uint8_t data[54] = 
{
255,255,255
,255,255,255
,191,170,255
,111,85,254
,91,85,249
,91,105,229
,91,190,229
,91,190,229
,91,254,250
,91,174,250
,91,90,229
,91,90,229
,91,174,229
,91,190,229
,91,170,229
,91,85,229
,111,85,229
,191,170,250
};
} fontchar_G;
struct fontchar_H_t : static_image<uint8_t,12,18,2>{
uint8_t const * get_data()const
{
   return data;
}
private:
  static constexpr uint8_t data[54] = 
{
255,255,255
,255,255,255
,175,255,250
,91,190,229
,91,190,229
,91,190,229
,91,190,229
,91,190,229
,91,170,229
,91,85,229
,91,85,229
,91,170,229
,91,190,229
,91,190,229
,91,190,229
,91,190,229
,91,190,229
,175,255,250
};
} fontchar_H;
struct fontchar_I_t : static_image<uint8_t,12,18,2>{
uint8_t const * get_data()const
{
   return data;
}
private:
  static constexpr uint8_t data[54] = 
{
255,255,255
,255,255,255
,191,170,254
,111,85,249
,111,85,249
,191,150,254
,255,150,255
,255,150,255
,255,150,255
,255,150,255
,255,150,255
,255,150,255
,255,150,255
,255,150,255
,191,150,254
,111,85,249
,111,85,249
,191,170,254
};
} fontchar_I;
struct fontchar_J_t : static_image<uint8_t,12,18,2>{
uint8_t const * get_data()const
{
   return data;
}
private:
  static constexpr uint8_t data[54] = 
{
255,255,255
,255,255,255
,255,170,250
,191,85,229
,191,85,229
,255,90,250
,255,91,254
,255,91,254
,255,91,254
,175,91,254
,91,90,254
,91,90,254
,91,90,254
,91,90,254
,91,90,254
,91,85,254
,111,149,255
,191,234,255
};
} fontchar_J;
struct fontchar_K_t : static_image<uint8_t,12,18,2>{
uint8_t const * get_data()const
{
   return data;
}
private:
  static constexpr uint8_t data[54] = 
{
255,255,255
,255,255,255
,175,255,255
,91,254,250
,91,190,229
,91,110,229
,91,90,249
,91,86,254
,91,149,255
,91,229,255
,91,249,255
,91,229,255
,91,149,255
,91,86,254
,91,90,249
,91,110,229
,91,190,229
,175,255,250
};
} fontchar_K;
struct fontchar_L_t : static_image<uint8_t,12,18,2>{
uint8_t const * get_data()const
{
   return data;
}
private:
  static constexpr uint8_t data[54] = 
{
255,255,255
,255,255,255
,175,255,255
,91,254,255
,91,254,255
,91,254,255
,91,254,255
,91,254,255
,91,254,255
,91,254,255
,91,254,255
,91,254,255
,91,254,255
,91,254,255
,91,170,250
,91,85,229
,91,85,229
,175,170,250
};
} fontchar_L;
struct fontchar_M_t : static_image<uint8_t,12,18,2>{
uint8_t const * get_data()const
{
   return data;
}
private:
  static constexpr uint8_t data[54] = 
{
255,255,255
,255,255,255
,175,255,250
,91,190,229
,91,105,229
,91,85,229
,91,85,229
,91,150,229
,91,150,229
,91,170,229
,91,190,229
,91,190,229
,91,190,229
,91,190,229
,91,190,229
,91,190,229
,91,190,229
,175,255,250
};
} fontchar_M;
struct fontchar_N_t : static_image<uint8_t,12,18,2>{
uint8_t const * get_data()const
{
   return data;
}
private:
  static constexpr uint8_t data[54] = 
{
255,255,255
,255,255,255
,175,255,250
,91,190,229
,91,190,229
,91,190,229
,91,185,229
,91,185,229
,91,165,229
,91,149,229
,91,86,229
,91,90,229
,91,110,229
,91,110,229
,91,190,229
,91,190,229
,91,190,229
,175,255,250
};
} fontchar_N;
struct fontchar_O_t : static_image<uint8_t,12,18,2>{
uint8_t const * get_data()const
{
   return data;
}
private:
  static constexpr uint8_t data[54] = 
{
255,255,255
,255,255,255
,255,170,255
,191,85,254
,111,85,249
,91,105,229
,91,190,229
,91,190,229
,91,190,229
,91,190,229
,91,190,229
,91,190,229
,91,190,229
,91,190,229
,91,105,229
,111,85,249
,191,85,254
,255,170,255
};
} fontchar_O;
struct fontchar_P_t : static_image<uint8_t,12,18,2>{
uint8_t const * get_data()const
{
   return data;
}
private:
  static constexpr uint8_t data[54] = 
{
255,255,255
,255,255,255
,175,170,255
,91,85,254
,91,85,249
,91,106,229
,91,190,229
,91,190,229
,91,106,229
,91,85,249
,91,85,254
,91,170,255
,91,254,255
,91,254,255
,91,254,255
,91,254,255
,91,254,255
,175,255,255
};
} fontchar_P;
struct fontchar_Q_t : static_image<uint8_t,12,18,2>{
uint8_t const * get_data()const
{
   return data;
}
private:
  static constexpr uint8_t data[54] = 
{
255,255,255
,255,255,255
,255,170,255
,191,85,254
,111,85,249
,91,105,229
,91,190,229
,91,190,229
,91,190,229
,91,190,229
,91,190,229
,91,170,229
,91,150,229
,91,86,229
,91,89,249
,111,85,229
,191,149,229
,255,234,250
};
} fontchar_Q;
struct fontchar_R_t : static_image<uint8_t,12,18,2>{
uint8_t const * get_data()const
{
   return data;
}
private:
  static constexpr uint8_t data[54] = 
{
255,255,255
,255,255,255
,175,170,255
,91,85,254
,91,85,249
,91,106,229
,91,190,229
,91,190,229
,91,106,229
,91,85,249
,91,149,254
,91,149,255
,91,86,254
,91,90,249
,91,110,229
,91,190,229
,91,190,229
,175,255,250
};
} fontchar_R;
struct fontchar_S_t : static_image<uint8_t,12,18,2>{
uint8_t const * get_data()const
{
   return data;
}
private:
  static constexpr uint8_t data[54] = 
{
255,255,255
,255,255,255
,255,170,254
,191,85,249
,111,85,229
,91,169,229
,91,254,250
,91,254,255
,91,169,255
,111,85,254
,191,85,249
,255,106,229
,255,191,229
,175,191,229
,91,106,229
,91,85,249
,111,85,254
,191,170,255
};
} fontchar_S;
struct fontchar_T_t : static_image<uint8_t,12,18,2>{
uint8_t const * get_data()const
{
   return data;
}
private:
  static constexpr uint8_t data[54] = 
{
255,255,255
,255,255,255
,175,170,250
,91,85,229
,91,85,229
,175,150,250
,255,150,255
,255,150,255
,255,150,255
,255,150,255
,255,150,255
,255,150,255
,255,150,255
,255,150,255
,255,150,255
,255,150,255
,255,150,255
,255,235,255
};
} fontchar_T;
struct fontchar_U_t : static_image<uint8_t,12,18,2>{
uint8_t const * get_data()const
{
   return data;
}
private:
  static constexpr uint8_t data[54] = 
{
255,255,255
,255,255,255
,175,255,250
,91,190,229
,91,190,229
,91,190,229
,91,190,229
,91,190,229
,91,190,229
,91,190,229
,91,190,229
,91,190,229
,91,190,229
,91,190,229
,91,105,229
,111,85,249
,191,85,254
,255,170,255
};
} fontchar_U;
struct fontchar_V_t : static_image<uint8_t,12,18,2>{
uint8_t const * get_data()const
{
   return data;
}
private:
  static constexpr uint8_t data[54] = 
{
255,255,255
,255,255,255
,175,255,250
,91,190,229
,91,190,229
,91,190,229
,91,190,229
,91,190,229
,91,190,229
,91,190,229
,91,190,229
,91,190,229
,91,105,229
,111,105,249
,111,85,249
,191,85,254
,255,150,255
,255,235,255
};
} fontchar_V;
struct fontchar_W_t : static_image<uint8_t,12,18,2>{
uint8_t const * get_data()const
{
   return data;
}
private:
  static constexpr uint8_t data[54] = 
{
255,255,255
,255,255,255
,175,255,250
,91,190,229
,91,190,229
,91,190,229
,91,190,229
,91,170,229
,91,150,229
,91,150,229
,91,150,229
,91,150,229
,91,150,229
,91,150,229
,91,85,229
,111,85,249
,111,105,249
,191,190,254
};
} fontchar_W;
struct fontchar_X_t : static_image<uint8_t,12,18,2>{
uint8_t const * get_data()const
{
   return data;
}
private:
  static constexpr uint8_t data[54] = 
{
255,255,255
,255,255,255
,175,255,250
,91,190,229
,91,190,229
,91,190,229
,91,190,229
,111,105,249
,191,85,254
,255,150,255
,255,150,255
,191,85,254
,111,105,249
,91,190,229
,91,190,229
,91,190,229
,91,190,229
,175,255,250
};
} fontchar_X;
struct fontchar_Y_t : static_image<uint8_t,12,18,2>{
uint8_t const * get_data()const
{
   return data;
}
private:
  static constexpr uint8_t data[54] = 
{
255,255,255
,255,255,255
,175,255,250
,91,190,229
,91,190,229
,91,190,229
,91,190,229
,91,190,229
,91,105,229
,111,85,249
,191,85,254
,255,150,255
,255,150,255
,255,150,255
,255,150,255
,255,150,255
,255,150,255
,255,235,255
};
} fontchar_Y;
struct fontchar_Z_t : static_image<uint8_t,12,18,2>{
uint8_t const * get_data()const
{
   return data;
}
private:
  static constexpr uint8_t data[54] = 
{
255,255,255
,255,255,255
,175,170,250
,91,85,229
,91,85,229
,175,170,229
,255,191,229
,255,111,229
,255,91,249
,255,86,254
,191,149,255
,111,229,255
,91,249,255
,91,254,255
,91,170,250
,91,85,229
,91,85,229
,175,170,250
};
} fontchar_Z;
struct fontchar_a_t : static_image<uint8_t,12,18,2>{
uint8_t const * get_data()const
{
   return data;
}
private:
  static constexpr uint8_t data[54] = 
{
255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,191,170,254
,111,85,249
,111,105,229
,175,190,229
,255,170,229
,175,85,229
,91,85,229
,91,169,229
,91,190,229
,91,169,229
,91,85,229
,111,85,229
,191,170,234
};
} fontchar_a;
struct fontchar_b_t : static_image<uint8_t,12,18,2>{
uint8_t const * get_data()const
{
   return data;
}
private:
  static constexpr uint8_t data[54] = 
{
255,255,255
,255,255,255
,255,255,255
,171,254,255
,91,254,255
,91,254,255
,91,254,255
,91,170,254
,91,85,249
,91,85,229
,91,106,229
,91,190,229
,91,190,229
,91,190,229
,91,106,229
,91,85,229
,91,85,249
,171,170,254
};
} fontchar_b;
struct fontchar_c_t : static_image<uint8_t,12,18,2>{
uint8_t const * get_data()const
{
   return data;
}
private:
  static constexpr uint8_t data[54] = 
{
255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,191,170,254
,111,85,249
,91,85,229
,91,170,229
,91,254,250
,91,254,255
,91,254,255
,91,254,250
,91,170,229
,91,85,229
,111,85,249
,191,170,254
};
} fontchar_c;
struct fontchar_d_t : static_image<uint8_t,12,18,2>{
uint8_t const * get_data()const
{
   return data;
}
private:
  static constexpr uint8_t data[54] = 
{
255,255,255
,255,255,255
,255,255,255
,255,191,234
,255,191,229
,255,191,229
,255,191,229
,191,170,229
,111,85,229
,91,85,229
,91,105,229
,91,190,229
,91,190,229
,91,190,229
,91,170,229
,91,85,229
,111,85,233
,191,170,250
};
} fontchar_d;
struct fontchar_e_t : static_image<uint8_t,12,18,2>{
uint8_t const * get_data()const
{
   return data;
}
private:
  static constexpr uint8_t data[54] = 
{
255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,191,170,254
,111,85,249
,91,85,229
,91,170,229
,91,170,229
,91,85,229
,91,85,229
,91,170,234
,91,170,250
,91,85,229
,111,85,249
,191,170,254
};
} fontchar_e;
struct fontchar_f_t : static_image<uint8_t,12,18,2>{
uint8_t const * get_data()const
{
   return data;
}
private:
  static constexpr uint8_t data[54] = 
{
255,255,255
,255,255,255
,255,255,255
,255,170,254
,191,85,249
,111,85,229
,111,169,229
,111,185,234
,107,169,255
,91,149,255
,91,149,255
,107,169,255
,111,249,255
,111,249,255
,111,249,255
,111,249,255
,111,249,255
,175,250,255
};
} fontchar_f;
struct fontchar_g_t : static_image<uint8_t,12,18,2>{
uint8_t const * get_data()const
{
   return data;
}
private:
  static constexpr uint8_t data[54] = 
{
255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,191,170,254
,111,85,249
,91,106,229
,91,190,229
,91,170,229
,91,105,229
,111,85,229
,171,170,229
,91,170,229
,91,85,229
,111,85,249
,191,170,254
};
} fontchar_g;
struct fontchar_h_t : static_image<uint8_t,12,18,2>{
uint8_t const * get_data()const
{
   return data;
}
private:
  static constexpr uint8_t data[54] = 
{
255,255,255
,255,255,255
,255,255,255
,171,254,255
,91,254,255
,91,254,255
,91,254,255
,91,170,254
,91,85,249
,91,85,229
,91,105,229
,91,190,229
,91,190,229
,91,190,229
,91,190,229
,91,190,229
,91,190,229
,171,190,234
};
} fontchar_h;
struct fontchar_i_t : static_image<uint8_t,12,18,2>{
uint8_t const * get_data()const
{
   return data;
}
private:
  static constexpr uint8_t data[54] = 
{
255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,235,255
,255,150,255
,255,150,255
,255,235,255
,255,170,255
,255,150,255
,255,150,255
,255,150,255
,255,150,255
,255,150,255
,255,150,255
,191,85,254
,191,85,254
,255,170,255
};
} fontchar_i;
struct fontchar_j_t : static_image<uint8_t,12,18,2>{
uint8_t const * get_data()const
{
   return data;
}
private:
  static constexpr uint8_t data[54] = 
{
255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,191,254
,255,111,249
,255,111,249
,255,191,254
,255,175,250
,255,111,249
,255,111,249
,255,111,249
,255,111,249
,191,110,249
,111,105,249
,111,85,249
,191,85,254
,255,170,255
};
} fontchar_j;
struct fontchar_k_t : static_image<uint8_t,12,18,2>{
uint8_t const * get_data()const
{
   return data;
}
private:
  static constexpr uint8_t data[54] = 
{
255,255,255
,255,255,255
,255,255,255
,171,254,255
,91,254,255
,91,254,255
,91,254,255
,91,190,254
,91,110,249
,91,90,249
,91,86,254
,91,149,255
,91,149,255
,91,85,254
,91,90,249
,91,110,229
,91,190,229
,171,254,250
};
} fontchar_k;
struct fontchar_l_t : static_image<uint8_t,12,18,2>{
uint8_t const * get_data()const
{
   return data;
}
private:
  static constexpr uint8_t data[54] = 
{
255,255,255
,255,255,255
,255,255,255
,255,250,255
,191,229,255
,191,229,255
,191,229,255
,191,229,255
,191,229,255
,191,229,255
,191,229,255
,191,229,255
,191,229,255
,191,229,255
,191,149,250
,191,85,249
,255,86,249
,255,171,250
};
} fontchar_l;
struct fontchar_m_t : static_image<uint8_t,12,18,2>{
uint8_t const * get_data()const
{
   return data;
}
private:
  static constexpr uint8_t data[54] = 
{
255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,191,190,254
,111,105,249
,91,85,229
,91,150,229
,91,150,229
,91,150,229
,91,170,229
,91,190,229
,91,190,229
,91,190,229
,171,190,234
};
} fontchar_m;
struct fontchar_n_t : static_image<uint8_t,12,18,2>{
uint8_t const * get_data()const
{
   return data;
}
private:
  static constexpr uint8_t data[54] = 
{
255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,171,254,255
,91,170,254
,91,85,249
,91,85,229
,91,170,229
,91,190,229
,91,190,229
,91,190,229
,91,190,229
,91,190,229
,91,190,229
,171,190,234
};
} fontchar_n;
struct fontchar_o_t : static_image<uint8_t,12,18,2>{
uint8_t const * get_data()const
{
   return data;
}
private:
  static constexpr uint8_t data[54] = 
{
255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,191,170,254
,111,85,249
,91,85,229
,91,170,229
,91,190,229
,91,190,229
,91,190,229
,91,170,229
,91,85,229
,111,85,249
,191,170,254
};
} fontchar_o;
struct fontchar_p_t : static_image<uint8_t,12,18,2>{
uint8_t const * get_data()const
{
   return data;
}
private:
  static constexpr uint8_t data[54] = 
{
255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,171,170,254
,91,85,249
,91,85,229
,91,170,229
,91,170,229
,91,85,229
,91,85,249
,91,170,254
,91,254,255
,91,254,255
,171,254,255
};
} fontchar_p;
struct fontchar_q_t : static_image<uint8_t,12,18,2>{
uint8_t const * get_data()const
{
   return data;
}
private:
  static constexpr uint8_t data[54] = 
{
255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,191,170,234
,111,85,229
,91,85,229
,91,170,229
,91,170,229
,91,85,229
,111,85,229
,191,170,229
,255,191,229
,255,191,229
,255,191,234
};
} fontchar_q;
struct fontchar_r_t : static_image<uint8_t,12,18,2>{
uint8_t const * get_data()const
{
   return data;
}
private:
  static constexpr uint8_t data[54] = 
{
255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,171,170,250
,91,85,233
,91,85,229
,91,170,229
,91,254,234
,91,254,255
,91,254,255
,91,254,255
,91,254,255
,91,254,255
,171,254,255
};
} fontchar_r;
struct fontchar_s_t : static_image<uint8_t,12,18,2>{
uint8_t const * get_data()const
{
   return data;
}
private:
  static constexpr uint8_t data[54] = 
{
255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,191,170,254
,111,85,249
,91,85,229
,91,170,229
,91,170,250
,91,85,249
,111,85,229
,175,170,229
,91,170,229
,91,85,229
,111,85,249
,191,170,254
};
} fontchar_s;
struct fontchar_t_t : static_image<uint8_t,12,18,2>{
uint8_t const * get_data()const
{
   return data;
}
private:
  static constexpr uint8_t data[54] = 
{
255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,191,234,255
,191,229,255
,171,165,234
,91,85,229
,91,85,229
,171,165,234
,191,229,255
,191,229,250
,191,165,249
,191,85,249
,255,86,249
,255,170,254
};
} fontchar_t;
struct fontchar_u_t : static_image<uint8_t,12,18,2>{
uint8_t const * get_data()const
{
   return data;
}
private:
  static constexpr uint8_t data[54] = 
{
255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,171,190,234
,91,190,229
,91,190,229
,91,190,229
,91,190,229
,91,190,229
,91,190,229
,91,170,229
,91,85,229
,111,85,249
,191,170,254
};
} fontchar_u;
struct fontchar_v_t : static_image<uint8_t,12,18,2>{
uint8_t const * get_data()const
{
   return data;
}
private:
  static constexpr uint8_t data[54] = 
{
255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,171,190,234
,91,190,229
,91,190,229
,91,190,229
,91,190,229
,91,190,229
,91,105,229
,111,85,249
,191,85,254
,255,150,255
,255,235,255
};
} fontchar_v;
struct fontchar_w_t : static_image<uint8_t,12,18,2>{
uint8_t const * get_data()const
{
   return data;
}
private:
  static constexpr uint8_t data[54] = 
{
255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,171,190,234
,91,190,229
,91,190,229
,91,190,229
,91,170,229
,91,150,229
,91,150,229
,91,85,229
,111,85,249
,111,105,249
,191,190,254
};
} fontchar_w;
struct fontchar_x_t : static_image<uint8_t,12,18,2>{
uint8_t const * get_data()const
{
   return data;
}
private:
  static constexpr uint8_t data[54] = 
{
255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,239,255,251
,155,255,230
,91,190,229
,91,105,229
,111,85,249
,191,85,254
,255,150,255
,191,85,254
,111,85,249
,91,105,229
,91,190,229
,175,255,250
};
} fontchar_x;
struct fontchar_y_t : static_image<uint8_t,12,18,2>{
uint8_t const * get_data()const
{
   return data;
}
private:
  static constexpr uint8_t data[54] = 
{
255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,171,190,234
,91,190,229
,91,190,229
,91,105,229
,111,85,249
,191,85,254
,255,150,255
,255,150,255
,255,150,255
,255,150,255
,255,170,255
};
} fontchar_y;
struct fontchar_z_t : static_image<uint8_t,12,18,2>{
uint8_t const * get_data()const
{
   return data;
}
private:
  static constexpr uint8_t data[54] = 
{
255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,171,170,234
,91,85,229
,91,85,229
,171,90,249
,255,86,254
,191,149,255
,111,229,255
,91,169,250
,91,85,229
,91,85,229
,171,170,234
};
} fontchar_z;
struct fontchar_1_t : static_image<uint8_t,12,18,2>{
uint8_t const * get_data()const
{
   return data;
}
private:
  static constexpr uint8_t data[54] = 
{
255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,235,255
,255,150,255
,191,149,255
,111,149,255
,111,149,255
,191,150,255
,255,150,255
,255,150,255
,191,150,254
,111,85,249
,111,85,249
,191,170,254
};
} fontchar_1;
struct fontchar_2_t : static_image<uint8_t,12,18,2>{
uint8_t const * get_data()const
{
   return data;
}
private:
  static constexpr uint8_t data[54] = 
{
255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,191,170,254
,111,85,249
,91,85,229
,91,170,229
,91,110,229
,175,91,249
,255,86,254
,191,149,255
,111,165,250
,91,85,229
,91,85,229
,175,170,250
};
} fontchar_2;
struct fontchar_3_t : static_image<uint8_t,12,18,2>{
uint8_t const * get_data()const
{
   return data;
}
private:
  static constexpr uint8_t data[54] = 
{
255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,175,170,254
,91,85,249
,91,85,229
,175,170,229
,255,170,229
,191,85,249
,191,85,249
,255,170,229
,175,170,229
,91,85,229
,91,85,249
,175,170,254
};
} fontchar_3;
struct fontchar_4_t : static_image<uint8_t,12,18,2>{
uint8_t const * get_data()const
{
   return data;
}
private:
  static constexpr uint8_t data[54] = 
{
255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,175,254
,255,91,249
,255,86,249
,191,101,249
,111,105,249
,91,106,249
,91,85,229
,91,85,229
,175,106,249
,255,111,249
,255,111,249
,255,191,254
};
} fontchar_4;
struct fontchar_5_t : static_image<uint8_t,12,18,2>{
uint8_t const * get_data()const
{
   return data;
}
private:
  static constexpr uint8_t data[54] = 
{
255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,175,170,250
,91,85,229
,91,85,229
,91,170,250
,91,85,249
,91,85,229
,175,170,229
,175,191,229
,91,170,229
,91,85,229
,107,85,249
,191,170,254
};
} fontchar_5;
struct fontchar_6_t : static_image<uint8_t,12,18,2>{
uint8_t const * get_data()const
{
   return data;
}
private:
  static constexpr uint8_t data[54] = 
{
255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,191,170,254
,111,85,249
,91,85,249
,91,170,254
,91,170,254
,91,85,249
,91,85,229
,91,170,229
,91,170,229
,91,85,229
,111,85,249
,191,170,254
};
} fontchar_6;
struct fontchar_7_t : static_image<uint8_t,12,18,2>{
uint8_t const * get_data()const
{
   return data;
}
private:
  static constexpr uint8_t data[54] = 
{
255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,175,170,250
,91,85,229
,91,85,229
,175,170,229
,255,111,229
,255,91,249
,255,86,254
,255,150,255
,255,150,255
,255,150,255
,255,150,255
,255,235,255
};
} fontchar_7;
struct fontchar_8_t : static_image<uint8_t,12,18,2>{
uint8_t const * get_data()const
{
   return data;
}
private:
  static constexpr uint8_t data[54] = 
{
255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,191,170,254
,111,85,249
,91,85,229
,91,170,229
,91,170,229
,111,85,249
,111,85,249
,91,170,229
,91,170,229
,91,85,229
,111,85,249
,191,170,254
};
} fontchar_8;
struct fontchar_9_t : static_image<uint8_t,12,18,2>{
uint8_t const * get_data()const
{
   return data;
}
private:
  static constexpr uint8_t data[54] = 
{
255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,191,170,254
,111,85,249
,91,85,229
,91,170,229
,91,170,229
,91,85,229
,111,85,229
,191,170,229
,175,170,229
,91,85,229
,91,85,249
,175,170,254
};
} fontchar_9;
struct fontchar_0_t : static_image<uint8_t,12,18,2>{
uint8_t const * get_data()const
{
   return data;
}
private:
  static constexpr uint8_t data[54] = 
{
255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,191,170,254
,111,85,249
,91,85,229
,91,170,229
,91,190,229
,91,190,229
,91,190,229
,91,190,229
,91,170,229
,91,85,229
,111,85,249
,191,170,254
};
} fontchar_0;
struct fontchar_not_t : static_image<uint8_t,12,18,2>{
uint8_t const * get_data()const
{
   return data;
}
private:
  static constexpr uint8_t data[54] = 
{
255,255,255
,255,235,255
,255,150,255
,255,150,255
,255,150,255
,255,150,255
,255,150,255
,255,150,255
,255,150,255
,255,150,255
,255,150,255
,255,150,255
,255,235,255
,255,235,255
,255,150,255
,255,150,255
,255,235,255
,255,255,255
};
} fontchar_not;
struct fontchar_dquot_t : static_image<uint8_t,12,18,2>{
uint8_t const * get_data()const
{
   return data;
}
private:
  static constexpr uint8_t data[54] = 
{
255,255,255
,191,255,254
,111,190,249
,91,105,229
,111,185,229
,191,249,230
,191,249,230
,111,190,249
,191,255,254
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
};
} fontchar_dquot;
struct fontchar_dollar_t : static_image<uint8_t,12,18,2>{
uint8_t const * get_data()const
{
   return data;
}
private:
  static constexpr uint8_t data[54] = 
{
255,255,255
,255,235,255
,255,150,255
,191,150,254
,111,85,249
,91,85,229
,91,150,229
,91,150,250
,91,85,254
,111,85,249
,175,150,229
,91,150,229
,91,85,229
,111,85,249
,191,150,254
,255,150,255
,255,235,255
,255,255,255
};
} fontchar_dollar;
struct fontchar_space_t : static_image<uint8_t,12,18,2>{
uint8_t const * get_data()const
{
   return data;
}
private:
  static constexpr uint8_t data[54] = 
{
255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
};
} fontchar_space;
struct fontchar_percent_t : static_image<uint8_t,12,18,2>{
uint8_t const * get_data()const
{
   return data;
}
private:
  static constexpr uint8_t data[54] = 
{
255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,175,255,250
,91,190,229
,91,110,229
,175,91,249
,255,86,254
,191,149,255
,111,229,250
,91,185,229
,91,190,229
,175,255,250
,255,255,255
};
} fontchar_percent;
struct fontchar_up_t : static_image<uint8_t,12,18,2>{
uint8_t const * get_data()const
{
   return data;
}
private:
  static constexpr uint8_t data[54] = 
{
255,255,255
,255,255,255
,255,255,255
,255,235,255
,255,150,255
,191,85,254
,111,105,249
,111,190,249
,175,255,250
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
};
} fontchar_up;
struct fontchar_and_t : static_image<uint8_t,12,18,2>{
uint8_t const * get_data()const
{
   return data;
}
private:
  static constexpr uint8_t data[54] = 
{
255,255,255
,255,255,255
,255,255,255
,255,234,255
,191,149,255
,111,105,254
,111,110,254
,111,110,254
,111,154,255
,191,229,255
,191,229,255
,111,154,250
,155,111,230
,155,191,249
,155,106,230
,111,149,155
,191,234,239
,255,255,255
};
} fontchar_and;
struct fontchar_star_t : static_image<uint8_t,12,18,2>{
uint8_t const * get_data()const
{
   return data;
}
private:
  static constexpr uint8_t data[54] = 
{
255,255,255
,255,255,255
,251,255,239
,230,255,155
,150,255,150
,91,190,229
,111,105,249
,171,85,234
,86,85,149
,171,85,234
,111,105,249
,91,190,229
,150,255,150
,230,255,155
,251,255,239
,255,255,255
,255,255,255
,255,255,255
};
} fontchar_star;
struct fontchar_lbrace_t : static_image<uint8_t,12,18,2>{
uint8_t const * get_data()const
{
   return data;
}
private:
  static constexpr uint8_t data[54] = 
{
255,255,255
,255,255,255
,255,255,255
,255,255,250
,255,191,229
,255,111,249
,255,91,254
,255,150,255
,255,150,255
,255,150,255
,255,150,255
,255,150,255
,255,150,255
,255,150,255
,255,91,254
,255,111,249
,255,191,229
,255,255,250
};
} fontchar_lbrace;
struct fontchar_rbrace_t : static_image<uint8_t,12,18,2>{
uint8_t const * get_data()const
{
   return data;
}
private:
  static constexpr uint8_t data[54] = 
{
255,255,255
,255,255,255
,255,255,255
,191,254,255
,111,249,255
,191,229,255
,255,150,255
,255,91,254
,255,91,254
,255,91,254
,255,91,254
,255,91,254
,255,91,254
,255,91,254
,255,150,255
,191,229,255
,111,249,255
,191,254,255
};
} fontchar_rbrace;
struct fontchar_underbar_t : static_image<uint8_t,12,18,2>{
uint8_t const * get_data()const
{
   return data;
}
private:
  static constexpr uint8_t data[54] = 
{
255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,171,170,234
,86,85,149
,86,85,149
,171,170,234
,255,255,255
};
} fontchar_underbar;
struct fontchar_minus_t : static_image<uint8_t,12,18,2>{
uint8_t const * get_data()const
{
   return data;
}
private:
  static constexpr uint8_t data[54] = 
{
255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,191,170,254
,111,85,249
,111,85,249
,191,170,254
,255,255,255
,255,255,255
,255,255,255
};
} fontchar_minus;
struct fontchar_plus_t : static_image<uint8_t,12,18,2>{
uint8_t const * get_data()const
{
   return data;
}
private:
  static constexpr uint8_t data[54] = 
{
255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,235,255
,255,150,255
,191,150,254
,111,85,249
,111,85,249
,191,150,254
,255,150,255
,255,235,255
,255,255,255
};
} fontchar_plus;
struct fontchar_equals_t : static_image<uint8_t,12,18,2>{
uint8_t const * get_data()const
{
   return data;
}
private:
  static constexpr uint8_t data[54] = 
{
255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,191,170,254
,111,85,249
,111,85,249
,191,170,254
,255,255,255
,255,255,255
,191,170,254
,111,85,249
,111,85,249
,191,170,254
,255,255,255
};
} fontchar_equals;
struct fontchar_lcurly_t : static_image<uint8_t,12,18,2>{
uint8_t const * get_data()const
{
   return data;
}
private:
  static constexpr uint8_t data[54] = 
{
255,255,255
,255,175,254
,255,91,249
,255,91,249
,255,86,249
,255,150,254
,255,150,255
,255,150,255
,191,229,255
,191,229,255
,255,150,255
,255,150,255
,255,150,254
,255,86,249
,255,91,249
,255,91,249
,255,175,254
,255,255,255
};
} fontchar_lcurly;
struct fontchar_rcurly_t : static_image<uint8_t,12,18,2>{
uint8_t const * get_data()const
{
   return data;
}
private:
  static constexpr uint8_t data[54] = 
{
255,255,255
,191,250,255
,111,229,255
,111,149,255
,111,149,255
,191,150,255
,255,150,255
,255,150,255
,255,91,254
,255,91,254
,255,150,255
,255,150,255
,191,150,255
,111,149,255
,111,149,255
,111,229,255
,191,250,255
,255,255,255
};
} fontchar_rcurly;
struct fontchar_lsqbacket_t : static_image<uint8_t,12,18,2>{
uint8_t const * get_data()const
{
   return data;
}
private:
  static constexpr uint8_t data[54] = 
{
255,255,255
,255,255,255
,255,171,254
,255,86,249
,255,86,249
,255,150,254
,255,150,255
,255,150,255
,255,150,255
,255,150,255
,255,150,255
,255,150,255
,255,150,254
,255,86,249
,255,86,249
,255,171,254
,255,255,255
,255,255,255
};
} fontchar_lsqbacket;
struct fontchar_rsqbracket_t : static_image<uint8_t,12,18,2>{
uint8_t const * get_data()const
{
   return data;
}
private:
  static constexpr uint8_t data[54] = 
{
255,255,255
,255,255,255
,191,234,255
,111,149,255
,111,149,255
,191,150,255
,255,150,255
,255,150,255
,255,150,255
,255,150,255
,255,150,255
,255,150,255
,191,150,255
,111,149,255
,111,149,255
,191,234,255
,255,255,255
,255,255,255
};
} fontchar_rsqbracket;
struct fontchar_colon_t : static_image<uint8_t,12,18,2>{
uint8_t const * get_data()const
{
   return data;
}
private:
  static constexpr uint8_t data[54] = 
{
255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,235,255
,255,150,255
,255,150,255
,255,235,255
,255,255,255
,255,255,255
,255,235,255
,255,150,255
,255,150,255
,255,235,255
,255,255,255
};
} fontchar_colon;
struct fontchar_semicolon_t : static_image<uint8_t,12,18,2>{
uint8_t const * get_data()const
{
   return data;
}
private:
  static constexpr uint8_t data[54] = 
{
255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,235,255
,255,150,255
,255,150,255
,255,235,255
,255,255,255
,255,255,255
,255,235,255
,255,150,255
,255,150,255
,191,229,255
,255,250,255
};
} fontchar_semicolon;
struct fontchar_squot_t : static_image<uint8_t,12,18,2>{
uint8_t const * get_data()const
{
   return data;
}
private:
  static constexpr uint8_t data[54] = 
{
255,255,255
,255,239,255
,255,155,255
,255,86,254
,255,91,254
,255,111,254
,255,111,254
,255,155,255
,255,239,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
};
} fontchar_squot;
struct fontchar_at_t : static_image<uint8_t,12,18,2>{
uint8_t const * get_data()const
{
   return data;
}
private:
  static constexpr uint8_t data[54] = 
{
255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,170,255
,191,85,254
,111,170,249
,155,186,230
,166,101,154
,102,90,154
,102,106,154
,102,90,154
,166,101,230
,230,186,249
,155,255,250
,111,170,230
,191,85,249
,255,170,254
};
} fontchar_at;
struct fontchar_hash_t : static_image<uint8_t,12,18,2>{
uint8_t const * get_data()const
{
   return data;
}
private:
  static constexpr uint8_t data[54] = 
{
255,255,255
,255,255,255
,191,190,254
,111,105,249
,111,105,249
,107,105,233
,86,85,149
,86,85,149
,107,105,233
,107,105,233
,86,85,149
,86,85,149
,107,105,233
,111,105,249
,111,105,249
,191,190,254
,255,255,255
,255,255,255
};
} fontchar_hash;
struct fontchar_tilde_t : static_image<uint8_t,12,18,2>{
uint8_t const * get_data()const
{
   return data;
}
private:
  static constexpr uint8_t data[54] = 
{
255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,250,239
,191,229,155
,111,149,150
,91,90,229
,150,111,249
,235,191,254
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
};
} fontchar_tilde;
struct fontchar_vbar_t : static_image<uint8_t,12,18,2>{
uint8_t const * get_data()const
{
   return data;
}
private:
  static constexpr uint8_t data[54] = 
{
255,255,255
,255,255,255
,255,235,255
,255,150,255
,255,150,255
,255,150,255
,255,150,255
,255,150,255
,255,150,255
,255,150,255
,255,150,255
,255,150,255
,255,150,255
,255,150,255
,255,150,255
,255,235,255
,255,255,255
,255,255,255
};
} fontchar_vbar;
struct fontchar_backslash_t : static_image<uint8_t,12,18,2>{
uint8_t const * get_data()const
{
   return data;
}
private:
  static constexpr uint8_t data[54] = 
{
255,255,255
,255,255,255
,255,255,255
,255,255,255
,235,255,255
,150,255,255
,86,254,255
,91,249,255
,111,229,255
,191,149,255
,255,86,254
,255,91,249
,255,111,229
,255,191,149
,255,255,150
,255,255,235
,255,255,255
,255,255,255
};
} fontchar_backslash;
struct fontchar_comma_t : static_image<uint8_t,12,18,2>{
uint8_t const * get_data()const
{
   return data;
}
private:
  static constexpr uint8_t data[54] = 
{
255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,235,255
,255,150,255
,255,150,255
,191,229,255
,111,249,255
,191,254,255
};
} fontchar_comma;
struct fontchar_lt_t : static_image<uint8_t,12,18,2>{
uint8_t const * get_data()const
{
   return data;
}
private:
  static constexpr uint8_t data[54] = 
{
255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,191,254
,255,111,249
,255,91,254
,255,150,255
,191,229,255
,111,249,255
,111,249,255
,191,229,255
,255,150,255
,255,91,254
,255,111,249
,255,191,254
};
} fontchar_lt;
struct fontchar_gt_t : static_image<uint8_t,12,18,2>{
uint8_t const * get_data()const
{
   return data;
}
private:
  static constexpr uint8_t data[54] = 
{
255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,175,255,255
,91,254,255
,111,249,255
,191,229,255
,255,150,255
,255,91,254
,255,91,254
,255,150,255
,191,229,255
,111,249,255
,91,254,255
,175,255,255
};
} fontchar_gt;
struct fontchar_dot_t : static_image<uint8_t,12,18,2>{
uint8_t const * get_data()const
{
   return data;
}
private:
  static constexpr uint8_t data[54] = 
{
255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,235,255
,255,150,255
,255,150,255
,255,235,255
};
} fontchar_dot;
struct fontchar_question_t : static_image<uint8_t,12,18,2>{
uint8_t const * get_data()const
{
   return data;
}
private:
  static constexpr uint8_t data[54] = 
{
255,255,255
,255,255,255
,191,170,254
,111,85,249
,91,85,229
,86,170,149
,150,255,150
,235,255,150
,255,191,149
,255,111,229
,255,91,249
,255,86,254
,255,150,255
,255,235,255
,255,235,255
,255,150,255
,255,150,255
,255,235,255
};
} fontchar_question;
struct fontchar_forward_slash_t : static_image<uint8_t,12,18,2>{
uint8_t const * get_data()const
{
   return data;
}
private:
  static constexpr uint8_t data[54] = 
{
255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,235
,255,255,150
,255,191,149
,255,111,229
,255,91,249
,255,86,254
,191,149,255
,111,229,255
,91,249,255
,86,254,255
,150,255,255
,235,255,255
,255,255,255
,255,255,255
};
} fontchar_forward_slash;
struct fontchar_bquot_t : static_image<uint8_t,12,18,2>{
uint8_t const * get_data()const
{
   return data;
}
private:
  static constexpr uint8_t data[54] = 
{
255,255,255
,175,255,255
,91,254,255
,111,249,255
,191,229,255
,255,150,255
,255,235,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
,255,255,255
};
} fontchar_bquot;
//------------------------------
constexpr uint8_t fontchar_A_t::data [54];
constexpr uint8_t fontchar_B_t::data [54];
constexpr uint8_t fontchar_C_t::data [54];
constexpr uint8_t fontchar_D_t::data [54];
constexpr uint8_t fontchar_E_t::data [54];
constexpr uint8_t fontchar_F_t::data [54];
constexpr uint8_t fontchar_G_t::data [54];
constexpr uint8_t fontchar_H_t::data [54];
constexpr uint8_t fontchar_I_t::data [54];
constexpr uint8_t fontchar_J_t::data [54];
constexpr uint8_t fontchar_K_t::data [54];
constexpr uint8_t fontchar_L_t::data [54];
constexpr uint8_t fontchar_M_t::data [54];
constexpr uint8_t fontchar_N_t::data [54];
constexpr uint8_t fontchar_O_t::data [54];
constexpr uint8_t fontchar_P_t::data [54];
constexpr uint8_t fontchar_Q_t::data [54];
constexpr uint8_t fontchar_R_t::data [54];
constexpr uint8_t fontchar_S_t::data [54];
constexpr uint8_t fontchar_T_t::data [54];
constexpr uint8_t fontchar_U_t::data [54];
constexpr uint8_t fontchar_V_t::data [54];
constexpr uint8_t fontchar_W_t::data [54];
constexpr uint8_t fontchar_X_t::data [54];
constexpr uint8_t fontchar_Y_t::data [54];
constexpr uint8_t fontchar_Z_t::data [54];
constexpr uint8_t fontchar_a_t::data [54];
constexpr uint8_t fontchar_b_t::data [54];
constexpr uint8_t fontchar_c_t::data [54];
constexpr uint8_t fontchar_d_t::data [54];
constexpr uint8_t fontchar_e_t::data [54];
constexpr uint8_t fontchar_f_t::data [54];
constexpr uint8_t fontchar_g_t::data [54];
constexpr uint8_t fontchar_h_t::data [54];
constexpr uint8_t fontchar_i_t::data [54];
constexpr uint8_t fontchar_j_t::data [54];
constexpr uint8_t fontchar_k_t::data [54];
constexpr uint8_t fontchar_l_t::data [54];
constexpr uint8_t fontchar_m_t::data [54];
constexpr uint8_t fontchar_n_t::data [54];
constexpr uint8_t fontchar_o_t::data [54];
constexpr uint8_t fontchar_p_t::data [54];
constexpr uint8_t fontchar_q_t::data [54];
constexpr uint8_t fontchar_r_t::data [54];
constexpr uint8_t fontchar_s_t::data [54];
constexpr uint8_t fontchar_t_t::data [54];
constexpr uint8_t fontchar_u_t::data [54];
constexpr uint8_t fontchar_v_t::data [54];
constexpr uint8_t fontchar_w_t::data [54];
constexpr uint8_t fontchar_x_t::data [54];
constexpr uint8_t fontchar_y_t::data [54];
constexpr uint8_t fontchar_z_t::data [54];
constexpr uint8_t fontchar_1_t::data [54];
constexpr uint8_t fontchar_2_t::data [54];
constexpr uint8_t fontchar_3_t::data [54];
constexpr uint8_t fontchar_4_t::data [54];
constexpr uint8_t fontchar_5_t::data [54];
constexpr uint8_t fontchar_6_t::data [54];
constexpr uint8_t fontchar_7_t::data [54];
constexpr uint8_t fontchar_8_t::data [54];
constexpr uint8_t fontchar_9_t::data [54];
constexpr uint8_t fontchar_0_t::data [54];
constexpr uint8_t fontchar_not_t::data [54];
constexpr uint8_t fontchar_dquot_t::data [54];
constexpr uint8_t fontchar_dollar_t::data [54];
constexpr uint8_t fontchar_space_t::data [54];
constexpr uint8_t fontchar_percent_t::data [54];
constexpr uint8_t fontchar_up_t::data [54];
constexpr uint8_t fontchar_and_t::data [54];
constexpr uint8_t fontchar_star_t::data [54];
constexpr uint8_t fontchar_lbrace_t::data [54];
constexpr uint8_t fontchar_rbrace_t::data [54];
constexpr uint8_t fontchar_underbar_t::data [54];
constexpr uint8_t fontchar_minus_t::data [54];
constexpr uint8_t fontchar_plus_t::data [54];
constexpr uint8_t fontchar_equals_t::data [54];
constexpr uint8_t fontchar_lcurly_t::data [54];
constexpr uint8_t fontchar_rcurly_t::data [54];
constexpr uint8_t fontchar_lsqbacket_t::data [54];
constexpr uint8_t fontchar_rsqbracket_t::data [54];
constexpr uint8_t fontchar_colon_t::data [54];
constexpr uint8_t fontchar_semicolon_t::data [54];
constexpr uint8_t fontchar_squot_t::data [54];
constexpr uint8_t fontchar_at_t::data [54];
constexpr uint8_t fontchar_hash_t::data [54];
constexpr uint8_t fontchar_tilde_t::data [54];
constexpr uint8_t fontchar_vbar_t::data [54];
constexpr uint8_t fontchar_backslash_t::data [54];
constexpr uint8_t fontchar_comma_t::data [54];
constexpr uint8_t fontchar_lt_t::data [54];
constexpr uint8_t fontchar_gt_t::data [54];
constexpr uint8_t fontchar_dot_t::data [54];
constexpr uint8_t fontchar_question_t::data [54];
constexpr uint8_t fontchar_forward_slash_t::data [54];
constexpr uint8_t fontchar_bquot_t::data [54];
//------------------------------
constexpr abc_bitmap<uint8_t> * lookup[127 - 32]= {
 &fontchar_space   //32
,&fontchar_not   //33
,&fontchar_dquot   //34
,&fontchar_hash   //35
,&fontchar_dollar   //36
,&fontchar_percent   //37
,&fontchar_and   //38
,&fontchar_squot   //39
,&fontchar_lbrace   //40
,&fontchar_rbrace   //41
,&fontchar_star   //42
,&fontchar_plus   //43
,&fontchar_comma   //44
,&fontchar_minus   //45
,&fontchar_dot   //46
,&fontchar_forward_slash   //47
,&fontchar_0   //48
,&fontchar_1   //49
,&fontchar_2   //50
,&fontchar_3   //51
,&fontchar_4   //52
,&fontchar_5   //53
,&fontchar_6   //54
,&fontchar_7   //55
,&fontchar_8   //56
,&fontchar_9   //57
,&fontchar_colon   //58
,&fontchar_semicolon   //59
,&fontchar_lt   //60
,&fontchar_equals   //61
,&fontchar_gt   //62
,&fontchar_question   //63
,&fontchar_at   //64
,&fontchar_A   //65
,&fontchar_B   //66
,&fontchar_C   //67
,&fontchar_D   //68
,&fontchar_E   //69
,&fontchar_F   //70
,&fontchar_G   //71
,&fontchar_H   //72
,&fontchar_I   //73
,&fontchar_J   //74
,&fontchar_K   //75
,&fontchar_L   //76
,&fontchar_M   //77
,&fontchar_N   //78
,&fontchar_O   //79
,&fontchar_P   //80
,&fontchar_Q   //81
,&fontchar_R   //82
,&fontchar_S   //83
,&fontchar_T   //84
,&fontchar_U   //85
,&fontchar_V   //86
,&fontchar_W   //87
,&fontchar_X   //88
,&fontchar_Y   //89
,&fontchar_Z   //90
,&fontchar_lsqbacket   //91
,&fontchar_backslash   //92
,&fontchar_rsqbracket   //93
,&fontchar_up   //94
,&fontchar_underbar   //95
,&fontchar_bquot   //96
,&fontchar_a   //97
,&fontchar_b   //98
,&fontchar_c   //99
,&fontchar_d   //100
,&fontchar_e   //101
,&fontchar_f   //102
,&fontchar_g   //103
,&fontchar_h   //104
,&fontchar_i   //105
,&fontchar_j   //106
,&fontchar_k   //107
,&fontchar_l   //108
,&fontchar_m   //109
,&fontchar_n   //110
,&fontchar_o   //111
,&fontchar_p   //112
,&fontchar_q   //113
,&fontchar_r   //114
,&fontchar_s   //115
,&fontchar_t   //116
,&fontchar_u   //117
,&fontchar_v   //118
,&fontchar_w   //119
,&fontchar_x   //120
,&fontchar_y   //121
,&fontchar_z   //122
,&fontchar_lcurly   //123
,&fontchar_vbar   //124
,&fontchar_rcurly   //125
,&fontchar_tilde   //126

};

}//namespace 
#include "font1.cpp"
