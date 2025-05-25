#ifndef graphics_utils
#define graphics_utils

#include <SDL3/SDL_pixels.h>

struct LocationAndSize {
    int x;
    int y;
    int w; // of the top left corner iirc
    int h;
};

Uint32 SDL_ColortoUint32(SDL_Color color);

#endif
