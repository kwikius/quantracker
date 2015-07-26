#ifndef QUANTRACKER_AIR_VIDEO_OSD_STATE_HPP_INCLUDED
#define QUANTRACKER_AIR_VIDEO_OSD_STATE_HPP_INCLUDED

/*
 Copyright (c) 2013 -2015 Andy Little 

 With Grateful Acknowledgements to the prior work of:
   Sami Korhonen(Openpilot.org)
   taulabs ( taulabs.com) 
   brainFPV ( brainfpv.com)
   Thomas Oldbury (super-osd.com)

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

struct osd_state{
   enum state_t { external_video, internal_video, suspended};
   static void set(state_t state);
   // only useful in internal_video mode
   static bool have_external_video(){return m_have_external_video == true;}
   static state_t get() { return m_current_state;}
   static void set_have_external_video(){m_have_external_video = true;}
 private:
   static void suspend();
   static void switch_to_internal_video_mode();
   static void switch_to_external_video_mode();
   static state_t m_current_state;
   static volatile bool m_have_external_video; 
};

#endif

