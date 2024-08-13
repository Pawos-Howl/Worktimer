// define macro tells SDL to use callbacks.....
#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_pixels.h>

#include <SDL3_ttf/SDL_ttf.h>

#include <iostream>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

#include "utils.hpp"
#include "textrender.hpp"
#include "timer/timer.hpp"

// allow for defining the FPS cap on SDL, by setting a delay up. mostly to not tax the system too much
#define FPS 30
#define FPSDELAY 1000/FPS // normal delay for just any FPS thing

struct AppState {
    SDL_Window* p_window = nullptr;
    SDL_Renderer* p_renderer = nullptr;

    uint32_t fps = 0;
    struct {
        uint32_t lastTicks;
        uint32_t nFrames;
        const char* fpschars;
        std::string fpsstr;
    } fpsData;
    TTF_Font* fpsfont = nullptr;
    RenderInfo* fpsrenderinfo = nullptr;

    timerer* timer;
};


int SDL_AppInit(void **appstate, int argc, char **argv) {
    std::cerr << "SDL_AppInit" << std::endl;

    *appstate = new AppState;
    AppState* state = static_cast<AppState*>(*appstate);

    // init SDL3
    const int initFlags = SDL_INIT_VIDEO | SDL_INIT_EVENTS;
    checkSDLError(SDL_InitSubSystem(initFlags));

    const int windowFlags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_VULKAN;
    state->p_window = SDL_CreateWindow(
        "Worktimer",
        960, 540,
        windowFlags);
    checkSDLError(state->p_window == NULL);

    state->p_renderer = SDL_CreateRenderer(state->p_window, NULL);
    checkSDLError(state->p_renderer == NULL);
    // fps textures
    TTF_Init();
    state->fpsfont = TTF_OpenFont("assets/arial.ttf", 12);
    checkSDLError(state->fpsfont == NULL);

    state->fpsrenderinfo = getTextureFromChars((char*)"FPS ???", state->fpsfont, {255, 0, 0, 255}, state->p_renderer);

    state->timer = new timerer(state->p_renderer, state->p_window);
    state->timer->startTimer();

    return 0;
}

int SDL_AppIterate(void *appstate) {
    AppState* state = static_cast<AppState*>(appstate);
    uint32_t starttick = SDL_GetTicks();

    SDL_RenderClear(state->p_renderer); // clear the render

    state->timer->drawTime();

    // render fps overlay
    RenderTTFTexture(state->fpsrenderinfo, state->p_renderer, 5, 5);
    SDL_RenderPresent(state->p_renderer);

    // update fps
    state->fpsData.nFrames++;
    const uint32_t ticksSinceLast = SDL_GetTicks() - state->fpsData.lastTicks;
    if (ticksSinceLast > 1000.0) {
        state->fps = state->fpsData.nFrames;
        state->fpsData.lastTicks = SDL_GetTicks();
        state->fpsData.nFrames = 0;

        // fprintf(stderr, "FPS %" PRIu32 "\n", state->fps);
        // fflush(stderr);

        // render the FPS
        char str[16];
        memset(str, 0x00, sizeof(char)*16);
        sprintf(str, "FPS %" PRIu32, state->fps);
        state->fpsData.fpsstr = str;
        state->fpsData.fpschars = str;

        state->fpsrenderinfo = getTextureFromChars((char*)state->fpsData.fpschars, state->fpsfont, {255, 0, 0, 255}, state->p_renderer);
    }

    // end of all iteration, so wait out the remaining fps time
    uint32_t FrameTime = SDL_GetTicks() - starttick; // how long the frame has taken
    int32_t waittime = FPSDELAY - FrameTime;
    // std::cout << waittime << std::endl; // here because sometimes delay is fine, other times it is just broken
    if(waittime >= 1000 || waittime < 0) waittime = 0;
    SDL_Delay(FPSDELAY - FrameTime);

    return 0;
}

int SDL_AppEvent(void *appstate, const SDL_Event *event) {
    if(event->type == SDL_EVENT_QUIT) {return 1;}

    return 0;
}

void SDL_AppQuit(void *appstate) {
    AppState* state = static_cast<AppState*>(appstate);

    SDL_DestroyRenderer(state->p_renderer);
    SDL_DestroyWindow(state->p_window);
    delete state->timer;

    DestroyRenderInfo(state->fpsrenderinfo);
    TTF_CloseFont(state->fpsfont);

    delete state;
    return;
}
