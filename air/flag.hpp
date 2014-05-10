#ifndef QUANTRACKER_AIR_FLAG_HPP_INCLUDED
#define QUANTRACKER_AIR_FLAG_HPP_INCLUDED


struct flag{

   flag() : m_flag{false}{}

   bool signalled()const { return m_flag;}

   void set() { m_flag = true;}
   void clear(){m_flag = false;}

private:
   bool m_flag;
};

#endif // QUANTRACKER_AIR_FLAG_HPP_INCLUDED
