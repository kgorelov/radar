#include <iostream>
#include <sstream>
#include <stdexcept>
#include <SDL2_gfxPrimitives.h>

#include "sdl_app.h"
#include "radar.h"
#include "radar_reader.h"

#define SCREEN_WIDTH    1920
#define SCREEN_HEIGHT   1080


TSDLApp::TSDLApp() {
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::stringstream err;
        err << "SDL intitialization error: "
            << SDL_GetError();
        throw std::runtime_error(err.str());
    }
#if defined linux && SDL_VERSION_ATLEAST(2, 0, 8)
    // Disable compositor bypass
    if(!SDL_SetHint(SDL_HINT_VIDEO_X11_NET_WM_BYPASS_COMPOSITOR, "0"))
    {
        throw std::runtime_error("SDL can not disable compositor bypass!");
    }
#endif

    // Create SDL window
    Window = SDL_CreateWindow(
        "Radar",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN);

    if (!Window) {
        std::stringstream err;
        err << "Error creating SDL window: "
            << SDL_GetError();
        throw std::runtime_error(err.str());
    }

    // Create renderer
    Renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED);
    if (!Renderer) {
        std::stringstream err;
        err << "Error creating renderer: "
            << SDL_GetError();
        throw std::runtime_error(err.str());
    }

    SDL_initFramerate(&FpsManager);
    SDL_setFramerate(&FpsManager, 60);
}


TSDLApp::~TSDLApp() {
    // Destroy renderer
    SDL_DestroyRenderer(Renderer);

    // Destroy window
    SDL_DestroyWindow(Window);

    // Quit SDL
    SDL_Quit();
}


void TSDLApp::EventLoop() {
    TRadar radar(this, SCREEN_WIDTH, SCREEN_HEIGHT);
    TRadarReader radarReader;

    bool quit = false;
    while (!quit) {
        // HandleEvents
        SDL_Event e;

        while(SDL_PollEvent(&e))
        {
            // User requests quit
            if (e.type == SDL_QUIT ||
                (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_q)) {
                quit = true;
                break;
            }

            // Notify all subscribers
            NotifyAllSubscribers(TEvent{e});
        }

        // Read radar signal (from stdin)
        TRadarEvent radarEvent;
        if (radarReader.ReadEvent(radarEvent)) {
            NotifyAllSubscribers(radarEvent);
        }

        // Clear
        SDL_SetRenderDrawColor(Renderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(Renderer);

        // Render the main object
        radar.Render(Renderer);

        // Update screen
        SDL_RenderPresent(Renderer);

        // Delay
        SDL_framerateDelay(&FpsManager);
    }
}

void TSDLApp::EventsSubscribe(TSDLObject *o)
{
    EventSubscribers.insert(o);
}

void TSDLApp::EventsUnsubscribe(TSDLObject *o)
{
    EventSubscribers.erase(o);
}

void TSDLApp::NotifyAllSubscribers(const TEvent& e) {
    for (auto subscriber: EventSubscribers) {
        subscriber->ProcessEvent(e);
    }
}
