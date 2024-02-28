#pragma once

#include <variant>
#include <SDL.h>
#include "radar_event.h"


using TEvent = std::variant<SDL_Event, TRadarEvent>;

class TSDLObject; // XXX TODO lazy hack, replace with a Subscriber class
class TEventManager
{
public:
    virtual void EventsSubscribe(TSDLObject*) = 0;
    virtual void EventsUnsubscribe(TSDLObject*) = 0;
    virtual void NotifyAllSubscribers(const TEvent& e) = 0;
};
