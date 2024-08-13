#include "utils.hpp"

#include <sys/stat.h>
#include <iostream>
#include <stdint.h>
#include <SDL3_ttf/SDL_ttf.h>

bool f_exists(const std::string& name) {
    struct stat buffer;
    return (stat (name.c_str(), &buffer) == 0);
}

void uint32ToChars(uint32_t nyyaaa, char* chars) {
    sprintf(chars, "%u", nyyaaa);
}

SDL_FRect SDLRectToFRect(SDL_Rect* rect) {
    return {static_cast<float>(rect->x),
            static_cast<float>(rect->y),
            static_cast<float>(rect->w),
            static_cast<float>(rect->h)};
}

// error checking for SDL calls
void _checkSDLError(
    const int code,
    const char *const func,
    const char *const file,
    const int line) {
    if (code != 0)
    {
        fprintf(stderr, "\nSDL Error: %s\n...at %s:%d '%s'\n",
                        SDL_GetError(), file, line, func);
        fflush(stderr);
        exit(-1);
    }
}
void _checkTTFError(
    const int code,
    const char *const func,
    const char *const file,
    const int line) {
    if (code != 0)
    {
        fprintf(stderr, "\nSDL_TTF Error: %s\n...at %s:%d '%s'\n",
                        TTF_GetError(), file, line, func);
        fflush(stderr);
        exit(-1);
    }
}
