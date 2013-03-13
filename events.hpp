#ifndef QUANTRACKER_EVENT_HPP_INCLUDED
#define QUANTRACKER_EVENT_HPP_INCLUDED

struct event{

   event() : m_flag{false}{}

   bool signalled()const { return m_flag;}

   void set() { m_flag = true;}
   void clear(){m_flag = false;}

private:
   bool m_flag;
};

extern event ms20_event;
extern event ms1_event;
extern event azimuth_pwm_calc_event;

#endif // QUANTRACKER_EVENT_HPP_INCLUDED
