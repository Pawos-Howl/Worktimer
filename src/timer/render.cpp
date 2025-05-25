#include "timer/render.hpp"

#include "textrender.hpp"

#include <string>

// buffered time
int64_t bufftime;
RenderInfo* bufftrender;
centeredInfo *bufftcenter;

// buffered label
char* bufflabel;
RenderInfo* bufflrender;
centeredInfo *bufflcenter;

// clearing cache
inline void clearTimeRenderCache() {
    if ( bufftrender != NULL ) DestroyRenderInfo(bufftrender);
    if ( bufftcenter != NULL ) DestroyCenteredInfo(bufftcenter);
}
inline void clearLabelRenderCache() {
    if ( bufflrender != NULL ) DestroyRenderInfo(bufflrender);
    if ( bufflcenter != NULL ) DestroyCenteredInfo(bufflcenter);
}
void clearTimerRenderCache() {
    // time and the label fall out of scope
    clearTimeRenderCache();
    clearLabelRenderCache();
}

void timeToChars(int64_t time, char* buff, int buffsize) {
    int16_t ms = time % 1000; // extract the MS
    time = time / 1000; // remove the ms

    int8_t sec = time % 60; // get the seconds
    time = time / 60; // remove the seconds

    int8_t min = time % 60; // get the minutes
    time = time / 60; // remove minutes

    int64_t hr = time; // the last of it should be hours

    // snprintf(buff, 32, "%02d:%02d:%02d.%03d", hr, min, sec, ms);
    if ( hr == 0 ){
        if ( min == 0 ) {
            snprintf(buff, buffsize, "%d.%03d", sec, ms);
        } else {
            snprintf(buff, buffsize, "%d:%02d.%03d", min, sec, ms);
        }
    } else {
        snprintf(buff, buffsize, "%ld:%02d:%02d.%03d", hr, min, sec, ms);
    }
}
void renderTime(int64_t time, char* label, SDL_Renderer* renderer, SDL_Window* window) {
    // time
    // check if time is buffered
    if ( time == bufftime ) {
        RenderTTFTexture(bufftrender, renderer, bufftcenter);
    }
    else {
        // destroy buffered textures
        clearTimeRenderCache();

        char* str = (char *)malloc(32); // size of string, plus one the null byte
        timeToChars(time, str, 32);

        RenderInfo* trender = getTextureFromChars(str, 50, renderer);
        centeredInfo *tcenter = centerTexture(trender, window);
        free(str); // clean up the malloc
        // shift down
        tcenter->y += 15;

        RenderTTFTexture(trender, renderer, tcenter);

        // map the ptrs to the buffer
        bufftrender = trender;
        bufftcenter = tcenter;
    }

    // label
    // check if label is buffered
    if ( label == bufflabel ) { // comparing the ptr addresses
        RenderTTFTexture(bufflrender, renderer, bufflcenter);
    }
    else {
        // destroy buffered textures
        clearLabelRenderCache();

        RenderInfo* lrender = getTextureFromChars(label, 30, renderer);
        centeredInfo *lcenter = centerTexture(lrender, window);
        // shift it up
        lcenter->y -= 25;

        RenderTTFTexture(lrender, renderer, lcenter);

        // map the ptrs to the buffer
        bufflrender = lrender;
        bufflcenter = lcenter;
    }
}
