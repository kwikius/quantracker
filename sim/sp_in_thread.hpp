#ifndef QUANTRACKER_SIM_SP_IN_THREAD_HPP_INCLUDED
#define QUANTRACKER_SIM_SP_IN_THREAD_HPP_INCLUDED
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

#include <wx/thread.h>

struct main_frame;
struct sp_in_thread : wxThread
{

   sp_in_thread(main_frame* m);
   ~sp_in_thread();

   
   void * Entry();
   private:
   main_frame* m_main_frame;
};

#endif // QUANTRACKER_SIM_SP_IN_THREAD_HPP_INCLUDED
