#include <SDL2_gfxPrimitives.h>

#include "radar_beam.h"

TRadarBeam::TRadarBeam(TEventManager* evmgr, Sint16 x, Sint16 y, Sint16 radius, Sint16 angle, Uint32 ttl)
    : TGameObject(evmgr, ttl)
    , X(x)
    , Y(y)
    , Radius(radius)
    , Angle(360-angle)
    , Brightness(0xaa)
{
}

TRadarBeam::~TRadarBeam()
{
}

void TRadarBeam::ProcessEvent(const TEvent&)
{
}

void TRadarBeam::Render(SDL_Renderer *renderer)
{
    filledPieRGBA(renderer, X, Y, Radius, Angle-1, Angle+1, 0x00, Brightness, 0x00, 0x33);
    Brightness = Brightness * 0.90;
}
