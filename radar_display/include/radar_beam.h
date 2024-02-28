#pragma once

#include "object.h"


class TRadarBeam: public TGameObject
{
public:
    TRadarBeam(TEventManager* evmgr, Sint16 x, Sint16 y, Sint16 radius, Sint16 angle, Uint32 ttl);
    ~TRadarBeam();
    virtual void ProcessEvent(const TEvent &ev);
    virtual void Render(SDL_Renderer *renderer);

private:
    Sint16 X, Y, Radius, Angle;
    Sint16 Brightness;
};

