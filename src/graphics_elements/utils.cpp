#include "graphics_elements/utils.hpp"

Uint32 SDL_ColortoUint32(SDL_Color color) {
    return (color.r << 24) + // these should bitshift
        (color.g << 16) +
        (color.b << 8) +
        color.a;
}
