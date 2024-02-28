#pragma once

#include <memory>
#include <list>
#include "object.h"

class TRadar: public TSDLObject
{
public:
    TRadar(TEventManager* evmgr, Sint16 width, Sint16 height);
    ~TRadar();
    virtual void ProcessEvent(const TEvent &ev);
    virtual void Render(SDL_Renderer *renderer);

private:
    void ProcessSDLEvent(const SDL_Event &e);
    void ProcessRadarEvent(const TRadarEvent &e);

    void RenderGrid(SDL_Renderer *renderer);
    void RenderObjects(SDL_Renderer *renderer);
    std::pair<double, double> ToPolar(unsigned x, unsigned y);
    std::pair<Sint16, Sint16> ToDecard(double angle, double distance);

private:
    Sint16 Width;
    Sint16 Height;
    Sint16 Radius;
    unsigned char Brightness;
    std::list<std::unique_ptr<TGameObject>> ObjList;
};
