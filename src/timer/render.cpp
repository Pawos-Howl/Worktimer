#include "timer/render.hpp"

#include "textrender.hpp"

void timeToChars(int64_t time, char* finstr) {
    int16_t ms = time % 1000; // extract the MS
    time = time / 1000; // remove the ms

    int8_t sec = time % 60; // get the seconds
    time = time / 60; // remove the seconds

    int8_t mins = time % 60; // get the minutes
    time = time / 60; // remove minutes

    int8_t hr = time; // the last of it should be hours

    char str[20];
    memset(str, 0x00, sizeof(char)*20);
    sprintf(str, "%d:%d:%d.%d", hr, mins, sec, ms);
    strcpy(finstr,str);
}
void renderTime(int64_t time, char* label, SDL_Renderer* renderer, SDL_Window* window) {
    // time
    char* str = (char *)malloc(21); // size of string, plus one the null byte
    timeToChars(time, str);

    RenderInfo* trender = getTextureFromChars(str, 50, renderer);
    centeredInfo *tcenter = centerTexture(trender, window);
    RenderTTFTexture(trender, renderer, tcenter);
    DestroyRenderInfo(trender);
    DestroyCenteredInfo(tcenter);

    free(str); // clean up the malloc

    // label
    RenderInfo* lrender = getTextureFromChars(label, 30, renderer);
    centeredInfo *lcenter = centerTexture(lrender, window);
    // shift it up
    lcenter->y -= 40;

    RenderTTFTexture(lrender, renderer, lcenter);
    DestroyRenderInfo(lrender);
    DestroyCenteredInfo(lcenter);
}
