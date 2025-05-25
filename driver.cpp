// define macro tells SDL to use callbacks
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
#include <string.h>
#include <vector>
#include <algorithm>

#include "utils.hpp"
#include "textrender.hpp"
#include "timer/timer.hpp"
#include "graphics_elements/button.hpp"

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

    Timerer* timer;
    std::vector<GraphicsButton> timerButtons;
    std::vector<char*> timerButtonText;
    std::vector<GraphicsButton> hiddenTimerButtons; // So I avoid deleting them
};


SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv) {
    std::cerr << "SDL_AppInit" << std::endl;

    *appstate = new AppState;
    AppState* state = static_cast<AppState*>(*appstate);

    // go through input args
    if (argc > 1) {
        for (int i = 1; i < argc; i++) {
            // switch case can't do chars
            char *arg = argv[i];
            if ( arg[0] == '-' ) {
                // param
                if ( sizeOfArray(arg) == 1 ) { [[unlikely]]
                    // malformed arg
                    std::cerr << "argument malformed (no space between args and '-')" << std::endl;
                    return SDL_APP_FAILURE;
                }
                if ( arg[1] == '-' ) {
                    // full name param
                    if ( sizeOfArray(arg) == 2 ) { [[unlikely]]
                        // malformed arg
                        std::cerr << "argument malformed (no space between args and \"--\")" << std::endl;
                        return SDL_APP_FAILURE;
                    }

                    // real arg checks
                    if ( strcmp(arg, "--verbose") == 0 ) {
                        // state->verbose = true;
                        verbose = true;
                        printv("verbose enabled.");
                    }
                    // not found
                    else {
                        std::cerr << "\"" << arg << "\" is not a valid arg (full name argument)" << std::endl;
                        return SDL_APP_FAILURE;
                    }
                } else if ( strcmp(arg, "-v" ) == 0 ) {
                    // state->verbose = true;
                    verbose = true;
                    printv("verbose enabled.");
                }
                // not found
                else {
                    std::cerr << "\"" << arg << "\" is not a valid arg (abreviated argument)" << std::endl;
                    return SDL_APP_FAILURE;
                }
            } else {
                // not implemented yet, exit
                std::cerr << "\"" << arg << "\" is not a valid arg (whole words not implemented)" << std::endl;
                return SDL_APP_FAILURE;
            }
        }
    }
    // verbose = state->verbose; // set the utils.hpp verbose to state verbosity

    // init SDL3
    const int initFlags = SDL_INIT_EVENTS | SDL_INIT_VIDEO;
    checkSDLError(!SDL_InitSubSystem(initFlags));

    // const int windowFlags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_VULKAN;
    const int windowFlags = SDL_WINDOW_VULKAN;
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

    state->timer = new Timerer(state->p_renderer, state->p_window);
    state->timer->startTimer();

    // the button for controlling timer functions
    state->timerButtonText.push_back((char*) "Start/Stop");
    state->timerButtons.push_back(*new GraphicsButton(state->timerButtonText.at(0), true, true, true, {430,360,80,30},state->p_renderer)); // note, the NEW is important, or else the texture is not the right one
    state->timerButtons.at(0).setCallback(std::bind(&Timerer::toggleState,state->timer));

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate) {
    AppState* state = static_cast<AppState*>(appstate);
    uint32_t starttick = SDL_GetTicks();

    SDL_SetRenderDrawColor(state->p_renderer, 0,0,0,0);
    SDL_RenderClear(state->p_renderer); // clear the render

    // check for hover position and if the object is clicked
    float x, y;
    if (SDL_GetMouseState(&x,&y) & SDL_BUTTON_LEFT){
        // left click handling
        std::for_each(state->timerButtons.begin(), state->timerButtons.end(), [x, y](GraphicsButton& button) {
            if (button.isPointAbove(x, y)) {
                button.isClicked();
            }
        });
    }
    else {
        // no clicks
        std::for_each(state->timerButtons.begin(), state->timerButtons.end(), [x, y](GraphicsButton& button) {
            if (button.isPointAbove(x, y)) {
                button.isHovered();
            }
        });
    }


    state->timer->drawTime();
    // draw buttons
    std::for_each(state->timerButtons.begin(), state->timerButtons.end(), [state](GraphicsButton& button) {
        button.render(state->p_renderer);
    });

    // render fps overlay
    RenderTTFTexture(state->fpsrenderinfo, state->p_renderer, 5, 5);
    SDL_RenderPresent(state->p_renderer);
    // std::cout << "fps texture ptr" << state->fpsrenderinfo->texture << std::endl;

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
        // sprintf(str, "FPS %" PRIu32, state->fps);
        sprintf(str, "FPS %" PRIu32, state->fps);
        state->fpsData.fpsstr = str;
        state->fpsData.fpschars = str;

        RenderInfo *oldrenderinfo = state->fpsrenderinfo;
        state->fpsrenderinfo = getTextureFromChars((char*)state->fpsData.fpschars, state->fpsfont, {255, 0, 0, 255}, state->p_renderer);
        DestroyRenderInfo(oldrenderinfo);
    }

    // end of all iteration, so wait out the remaining fps time
    uint32_t FrameTime = SDL_GetTicks() - starttick; // how long the frame has taken
    int32_t waittime = FPSDELAY - FrameTime;
    // std::cout << waittime << std::endl; // here because sometimes delay is fine, other times it is just broken
    if(waittime >= 1000 || waittime < 0) waittime = 0;
    if ( verbose ) {
        char str[64];
        sprintf(str, "DEBUG: WAITTIME LEFT %d WAITTIME FULL %d", waittime, FPSDELAY);
        // printf("DEBUG: WAITTIME LEFT %d WAITTIME FULL %d", waittime, FPSDELAY);
        std::cout << str << std::endl;
    }
    SDL_Delay(FPSDELAY - FrameTime);

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
    // AppState* state = static_cast<AppState*>(appstate);
    switch (event->type) {
        // remeber break or it will fall through switch case
        case SDL_EVENT_QUIT: return SDL_APP_SUCCESS;
        /*case SDL_EVENT_MOUSE_MOTION:
            break; // write code
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            break; // write code
        case SDL_EVENT_MOUSE_BUTTON_UP:
            break; // write code
        */
    }


    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
    AppState* state = static_cast<AppState*>(appstate);

    SDL_DestroyRenderer(state->p_renderer);
    SDL_DestroyWindow(state->p_window);
    delete state->timer;

    DestroyRenderInfo(state->fpsrenderinfo);
    TTF_CloseFont(state->fpsfont);

    delete state;

    // final remarks
    if(result == SDL_APP_FAILURE) {
        std::cout << "damn. guess I fluffed up." << std::endl;
    }

    return;
}
