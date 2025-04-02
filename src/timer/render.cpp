#include "timer/render.hpp"

#include "textrender.hpp"

#include <string>

inline void addPaddingZeros(std::string &str, int8_t &num) {
    if ( num < 10 && str.length() != 0 ) {
        // add padded zero
        str.append("0");
    }
}

void timeToChars(int64_t time, char* finstr) {
    int16_t ms = time % 1000; // extract the MS
    time = time / 1000; // remove the ms

    int8_t sec = time % 60; // get the seconds
    time = time / 60; // remove the seconds

    int8_t mins = time % 60; // get the minutes
    time = time / 60; // remove minutes

    int8_t hr = time; // the last of it should be hours

    // char str[20];
    // memset(str, 0x00, sizeof(char)*20);
    // sprintf(str, "%d:%d:%d.%d", hr, mins, sec, ms);
    std::string str; // will fall out of scope
    if ( hr != 0 ) {
        str.append(std::to_string(hr)+":");
    }
    if ( mins != 0 ) {
        addPaddingZeros(str,mins);
        str.append(std::to_string(mins)+":");
    }
    // use seconds always
    addPaddingZeros(str,sec);
    str.append(std::to_string(sec)+".");
    if ( ms < 100 ) {
        // add padded zero
        str.append("0");
        if ( ms < 10 ) {
            // another padded zero
            str.append("0");
        }
    }
    str.append(std::to_string(ms));
    strcpy(finstr,str.c_str());
}
void renderTime(int64_t time, char* label, SDL_Renderer* renderer, SDL_Window* window) {
    // time
    char* str = (char *)malloc(21); // size of string, plus one the null byte
    timeToChars(time, str);

    RenderInfo* trender = getTextureFromChars(str, 50, renderer);
    centeredInfo *tcenter = centerTexture(trender, window);
    // shift down
    tcenter->y += 15;

    RenderTTFTexture(trender, renderer, tcenter);
    DestroyRenderInfo(trender);
    DestroyCenteredInfo(tcenter);

    free(str); // clean up the malloc

    // label
    RenderInfo* lrender = getTextureFromChars(label, 30, renderer);
    centeredInfo *lcenter = centerTexture(lrender, window);
    // shift it up
    lcenter->y -= 25;

    RenderTTFTexture(lrender, renderer, lcenter);
    DestroyRenderInfo(lrender);
    DestroyCenteredInfo(lcenter);
}
