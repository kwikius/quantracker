#ifndef QUANTRACKER_SIM_SP_IN_THREAD_HPP_INCLUDED
#define QUANTRACKER_SIM_SP_IN_THREAD_HPP_INCLUDED

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
