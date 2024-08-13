#ifndef utils
#define utils

#include <string>
#include <bits/stdint-uintn.h>
#include <SDL3/SDL_rect.h>

bool f_exists(const std::string& name);

void uint32ToChars(uint32_t nyyaaa, char* chars);

SDL_FRect SDLRectToFRect(SDL_Rect* rect);

#define checkSDLError(val) _checkSDLError( (val), #val, __FILE__, __LINE__ )
void _checkSDLError(
    const int code,
    const char *const func,
    const char *const file,
    const int line);

#define checkTTFError(val) _checkTTFError( (val), #val, __FILE__, __LINE__ )
void _checkTTFError(
    const int code,
    const char *const func,
    const char *const file,
    const int line);

#endif
