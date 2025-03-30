#include "timer/timer.hpp"
#include "utils.hpp"
#include "timer/render.hpp"

#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_time.h>

#include <iostream>

void timerer::nextTimer() {
    // if during a timerdelay, don't
    if(btdelayStart != 0) return;

    // check if it is at the end of the timerlist
    if(curTimer+1 >= timerlist.size()){
        // end timer
        std::cout << "Timers are all done :3" << std::endl;
        resetTimer();
        return;
    }
    timerentry preventry = timerlist.at(curTimer);
    betweenTimers(&preventry);
    curTimer++;
    timerentry nxtEntry = timerlist.at(curTimer);

    // restart the timing stuffs
    int64_t curtime = getSystimeMS();

    // init the delay
    btdelayStart = curtime;

    timerEnd = curtime + (nxtEntry.duration * 1000);
}

void timerer::betweenTimers(timerentry* priorEntry) {
    std::cout << "Finished entry: " + priorEntry->name << std::endl;
}

int64_t timerer::getSystimeMS() {
    SDL_Time systime; // basically a signed int64
    checkSDLError(!SDL_GetCurrentTime(&systime)); // epoch
    return (uint64_t)systime/1000000; // to ms (div 10^6)
}
int64_t timerer::getSystimeSec() {
    return getSystimeMS()/1000;
}
// public
// constructor/deconstructor
timerer::timerer(SDL_Renderer* render, SDL_Window* windoww) {
    renderer = render;
    window = windoww;

    // timer stuffs
    curTimer = 0;
    timerPaused = true;
    timeleft = 0;
    timerEnd = 0;

    // between timer stuffs
    btdelay = 1000; // ms of delay
    btdelayStart = 0;

    // timers data
    LOCKTIMEVECTOR = true; // just for now, since the code doesn't support modifications yet

    // times are done by hand
    // the first item is to fix a bug where it keeps skipping the first item
    timerlist.push_back({"bugfix",0,true});

    timerlist.push_back({"work1",1500,true});
    timerlist.push_back({"break1",300,false});
    timerlist.push_back({"work2",1500,true});
    timerlist.push_back({"break2",300,false});
    timerlist.push_back({"work3",1500,true});
    timerlist.push_back({"larger break :3",1800,false});
}
timerer::~timerer() {
    // nothing needed for now
}

// this call somehow skips the first item... WHY???
void timerer::startTimer() {
    // check if it is a start or a restart
    if(!timerPaused) {
        // start
        if(timerlist.size() == 0) return; // can't have no entries
        curTimer = 0;
        LOCKTIMEVECTOR = true;

        // start the timing stuffs
        uint32_t furtime = (timerlist.at(0).duration * 1000); // to ms
        int64_t curtime = getSystimeMS();

        // setup the delay to start off with
        btdelayStart = curtime;

        timerEnd = curtime + furtime;
    }
    // restart
    int64_t systime = getSystimeMS();
    timerEnd = systime + timeleft;
    timerPaused = false;
}
int64_t timerer::pauseTimer() {
    // soft stop, can be restarted
    int64_t systime = getSystimeMS();
    timeleft = timerEnd - systime;
    timerPaused = true;
    return timeleft;
}
void timerer::stopTimer() {
    // soft stop, can be restarted
    int64_t systime = getSystimeMS();
    timeleft = timerEnd - systime;
    timerPaused = true;
}
void timerer::resetTimer() {
    // hard stop
    // make sure timer is off first
    stopTimer();

    timeleft = 0;
    timerPaused = false;
    timerEnd = 0;

    LOCKTIMEVECTOR = false;
}

// render
void timerer::drawTime() {
    if(!LOCKTIMEVECTOR) {return;} // this just assumes that the timer is running
    int64_t systime  = getSystimeMS();

    bool betweenTimers = false;
    // check if it is between timers, if it is, then the whole thing is weird
    if(btdelayStart + btdelay > systime) betweenTimers = true;
    // if the delay is set, but out, then fix it
    if(btdelay != 0) btdelay = 0;

    // check if it is out of time, if it is then iterate timers
    if(systime >= timerEnd && timerEnd != 0 && !betweenTimers) nextTimer();
    int64_t difftime = timerEnd - systime;
    renderTime(difftime, (char*)timerlist.at(curTimer).name.data(), renderer, window);
}

std::vector<timerentry> timerer::getTimers() {
    return timerlist;
}
bool timerer::setTimers(std::vector<timerentry> entries) {
    if(LOCKTIMEVECTOR) return false;
    timerlist = entries;
    return true;
}
