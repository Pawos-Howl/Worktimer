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

class timerer {
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
    public:
        // constructor/destructor
        timerer(SDL_Renderer* renderer, SDL_Window* windoww);
        ~timerer();

        // timer controls
        void startTimer(); // int for status codes
        int64_t pauseTimer(); // returns time left on active timer
        void stopTimer();
        void resetTimer();

        void drawTime(); // display time and label

        // data and such for the timerentries and whatnot
        std::vector<timerentry> getTimers();
        bool setTimers(std::vector<timerentry> entries); // returns false if there is currently a lock and does nothing, true otherwise
};
#endif
