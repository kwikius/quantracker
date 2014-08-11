#ifndef QUANTRACKER_SIM_EVENTS_HPP_INCLUDED
#define QUANTRACKER_SIM_EVENTS_HPP_INCLUDED
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

#include <wx/event.h>

BEGIN_DECLARE_EVENT_TYPES()
DECLARE_EVENT_TYPE(wxEvent_AircraftPositionChanged, -1)
DECLARE_EVENT_TYPE(wxEvent_RemoteDistanceChanged, -1)
DECLARE_EVENT_TYPE(wxEvent_RemoteBearingChanged, -1)
DECLARE_EVENT_TYPE(wxEvent_RemoteElevationChanged, -1)
END_DECLARE_EVENT_TYPES()

struct QuanTrackerSimEvent : wxEvent
{
   QuanTrackerSimEvent(wxEventType EventType) : wxEvent(wxID_ANY, EventType){}
   wxEvent* Clone() const ;
};

#endif // QUANTRACKER_SIM_EVENTS_HPP_INCLUDED
