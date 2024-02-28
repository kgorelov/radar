#include <iostream>
#include <algorithm>
#include <sstream>
#include <cmath>
#include <SDL2_gfxPrimitives.h>

#include "radar.h"
#include "radar_beam.h"
#include "radar_dot.h"
#include "util.h"

const Uint32 DotTTL = 5000;
const Uint32 BeamTTL = 1500;

TRadar::TRadar(TEventManager* evmgr, Sint16 width, Sint16 height)
    : TSDLObject(evmgr)
    , Width(width)
    , Height(height)
    , Radius(std::min(static_cast<Sint16>(Width/2), Height))
    , Brightness(0xbb)
{
    GetEvMgr()->EventsSubscribe(this);
}

TRadar::~TRadar()
{
    GetEvMgr()->EventsUnsubscribe(this);
}

std::pair<double, double> TRadar::ToPolar(unsigned x, unsigned y) {
    double x2 = std::pow((double)Width/2 - x, 2);
    double y2 = std::pow((double)Height - y, 2);
    double distance = sqrt(x2 + y2);
    double angle = 180*(asin(((double)Height - y)/distance))/M_PI;
    if (x < (unsigned)Width/2) {
        angle = 180 - angle;
    }
    return {angle, distance};
}

std::pair<Sint16, Sint16> TRadar::ToDecard(double angle, double distance) {
    double arad = angle * M_PI / 180;
    Sint16 x = (Width/2) + distance * cos(arad);
    Sint16 y = Height - distance * sin(arad);
    return {x, y};
}

void TRadar::ProcessSDLEvent(const SDL_Event &e) {
    if (e.type == SDL_MOUSEMOTION) {
        // auto [angle, distance] = ToPolar(e.motion.x, e.motion.y);
    } else if (e.type == SDL_MOUSEBUTTONDOWN) {
        auto [angle, distance] = ToPolar(e.button.x, e.button.y);
        GetEvMgr()->NotifyAllSubscribers(TRadarEvent{(unsigned)angle, (unsigned)distance});
    }
}

void TRadar::ProcessRadarEvent(const TRadarEvent &e) {
    ObjList.push_back(std::make_unique<TRadarBeam>(
        GetEvMgr(), Width/2, Height, Radius, e.Angle, BeamTTL));
    if (e.Distance > 0) {
        auto [x, y] = ToDecard(e.Angle, e.Distance);
        ObjList.push_back(std::make_unique<TRadarDot>(
          GetEvMgr(), x, y, DotTTL));
    }
}

void TRadar::ProcessEvent(const TEvent& e) {
    std::visit(overloaded{
            [](auto arg) { std::cout << arg << ' '; },
            [&](const SDL_Event& arg) { ProcessSDLEvent(arg);},
            [&](const TRadarEvent& arg) { ProcessRadarEvent(arg);}
        }, e);
}

void TRadar::RenderGrid(SDL_Renderer *renderer)
{
    Sint16 X = Width/2;
    Sint16 Y = Height;
    for (int r = 80; r <= Radius; r += 80) {
        arcRGBA(renderer, X, Y, r, 180, 360, 0x00, Brightness, 0x00, 0xFF);
        std::stringstream ss;
        ss << r;
        stringRGBA(renderer, X + 8, Y - r + 8, ss.str().c_str(), 0x00, Brightness, 0x00, 0xFF);
    }

    for (int a = 0; a <= 180; a += 15) {
        double arad = a * M_PI / 180;
        Sint16 x1 = (Radius+5) * cos(arad);
        Sint16 y1 = (Radius+5) * sin(arad);
        lineRGBA(renderer, X, Y, X + x1, Y - y1, 0x00, Brightness, 0x00, 0xFF);

        Sint16 r2 = Radius + 5 + a/6;
        x1 = r2 * cos(arad);
        y1 = r2 * sin(arad);
        stringRGBA(renderer, X + x1, Y - y1, std::to_string(a).c_str(), 0x00, Brightness, 0x00, 0xFF);
    }
}

void TRadar::RenderObjects(SDL_Renderer *renderer)
{
    auto oit = ObjList.begin();
    while (oit != ObjList.end()) {
        if ((*oit)->IsExpired()) {
            oit = ObjList.erase(oit);
            continue;
        }
        (*oit)->Render(renderer);
        ++oit;
    }
}

void TRadar::Render(SDL_Renderer *renderer)
{
    RenderObjects(renderer);
    RenderGrid(renderer);
}
