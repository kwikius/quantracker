#ifndef QUANTRACKER_AIR_FLAG_HPP_INCLUDED
#define QUANTRACKER_AIR_FLAG_HPP_INCLUDED
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


struct flag{

   flag() : m_flag{false}{}

   bool signalled()const { return m_flag;}

   void set() { m_flag = true;}
   void clear(){m_flag = false;}

private:
   bool m_flag;
};

#endif // QUANTRACKER_AIR_FLAG_HPP_INCLUDED
