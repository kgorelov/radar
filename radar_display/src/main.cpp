#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <math.h>
#include <unistd.h>

#include <SDL.h>
#include <SDL2_framerate.h>
#include <SDL2_gfxPrimitives.h>

#include "sdl_app.h"


#define SCREEN_WIDTH    1920
#define SCREEN_HEIGHT   1080


int main()
{
    TSDLApp app;
    app.EventLoop();
    return 0;
}
