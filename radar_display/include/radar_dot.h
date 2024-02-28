#pragma once

#include "object.h"


class TRadarDot: public TGameObject
{
public:
    TRadarDot(TEventManager* evmgr, Sint16 x, Sint16 y, Uint32 ttl);
    ~TRadarDot();
    virtual void ProcessEvent(const TEvent &ev);
    virtual void Render(SDL_Renderer *renderer);

private:
    Sint16 X, Y;
    Sint16 Brightness;
};
