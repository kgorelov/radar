#include <SDL2_gfxPrimitives.h>
#include "radar_dot.h"

TRadarDot::TRadarDot(TEventManager* evmgr, Sint16 x, Sint16 y, Uint32 ttl)
    : TGameObject(evmgr, ttl)
    , X(x)
    , Y(y)
    , Brightness(0xee)
{
}

TRadarDot::~TRadarDot() {
}

void TRadarDot::ProcessEvent(const TEvent&) {
}

void TRadarDot::Render(SDL_Renderer *renderer) {
    filledCircleRGBA(renderer, X, Y, 15, 0x00, 0xFF, 0x00, Brightness/3);
    filledCircleRGBA(renderer, X, Y, 10, 0x00, 0xFF, 0x00, Brightness/2);
    filledCircleRGBA(renderer, X, Y, 3, 0x00, 0xFF, 0x00, Brightness);
    Brightness = Brightness * 0.99;
}
