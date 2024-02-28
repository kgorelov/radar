#pragma once

#include <unordered_set>
#include <SDL.h>
#include <SDL2_framerate.h>
#include "event.h"

class TSDLObject;

class TSDLApp: public TEventManager
{
public:
    TSDLApp();
    ~TSDLApp();
    void EventLoop();

public:
    // Event Manager Interface Implementation
    virtual void EventsSubscribe(TSDLObject *o);
    virtual void EventsUnsubscribe(TSDLObject *o);
    virtual void NotifyAllSubscribers(const TEvent& e);

private:
    SDL_Window *Window;
    SDL_Renderer *Renderer;
    FPSmanager FpsManager;
    std::unordered_set<TSDLObject*> EventSubscribers;
};
