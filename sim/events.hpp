#ifndef QUANTRACKER_SIM_EVENTS_HPP_INCLUDED
#define QUANTRACKER_SIM_EVENTS_HPP_INCLUDED

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
