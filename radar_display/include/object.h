#pragma once
#include <SDL.h>
#include "event.h"

class TSDLObject
{
public:
    TSDLObject(TEventManager* evmgr)
        : EvMgr(evmgr)
    {
    }

    virtual void ProcessEvent(const TEvent &ev) = 0;
    virtual void Render(SDL_Renderer *renderer) = 0;

    TEventManager* GetEvMgr() {
        return EvMgr;
    }

private:
    TEventManager *EvMgr;
};


class TExpiringObject
{
public:
    TExpiringObject(Uint32 ttl)
        : CreationTime(SDL_GetTicks())
        , Ttl(ttl)
    {
    }

    bool IsExpired() {
        return GetAge() > Ttl;
    }

    virtual Uint32 GetAge() {
        return SDL_GetTicks() - CreationTime;
    }

private:
    Uint32 CreationTime;
    Uint32 Ttl;
};


class TGameObject: public TSDLObject, public TExpiringObject
{
public:
    TGameObject(TEventManager* evmgr, Uint32 ttl)
        : TSDLObject(evmgr)
        , TExpiringObject(ttl)
    {
    }
};
