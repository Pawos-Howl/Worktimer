#ifndef timerh
#define timerh

#include <SDL3/SDL_render.h>

#include <string>
#include <inttypes.h>
#include <vector>

struct timerentry {
    std::string name;
    uint32_t duration; // seconds
    bool isWorkTimer; // if false, its break time
};

class Timerer {
    private:
        // timers
        std::vector<timerentry> timerlist;
        bool LOCKTIMEVECTOR; // applayer, so you cant modify the times as it is running

        // internal calls
        void nextTimer();
        void betweenTimers(timerentry* priorEntry); // for stuffs like playing an audio cue at the end of a section

        uint8_t curTimer; // current timer location
        bool timerPaused;
        // in ms
        uint64_t timeleft; // time left when paused
        uint64_t timerEnd; // WHILE TIMER IS RUNNING, says the end of the timer (with relitivity to the epoch)

        // getting system time
        int64_t getSystimeMS(); // milliseconds
        int64_t getSystimeSec(); // seconds

        // renderer
        SDL_Renderer* renderer;
        SDL_Window* window; // for centering

        // delay between timers
        uint32_t btdelay; // ms
        int64_t btdelayStart; // 0 if not an active delay

    public:
        // constructor/destructor
        Timerer(SDL_Renderer* renderer, SDL_Window* windoww);
        ~Timerer();

        // timer controls
        void startTimer(); // int for status codes
        void stopTimer();
        void toggleState(); // quickly start/stop the timer
        void resetTimer();

        int64_t getRemainingMS(); // returns the time left on the active timer

        void drawTime(); // display time and label

        // data and such for the timerentries and whatnot
        std::vector<timerentry> getTimers();
        bool setTimers(std::vector<timerentry> entries); // returns false if there is currently a lock and does nothing, true otherwise
};
#endif
