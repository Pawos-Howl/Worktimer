#ifndef timerrender
#define timerrender

#include "textrender.hpp"

void timeToChars(int64_t time, const char* finstr);
void renderTime(int64_t time, char* label, SDL_Renderer* renderer, SDL_Window* windows);

#endif
