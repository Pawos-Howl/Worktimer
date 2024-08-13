# Worktimer

This is a semicustomizable worktimer written in C++ using SDL3. This follows some form of the Pomodoro method for working, using 25 minute work sessions followed by a 5 minute break and then a long break.

## How to build

This uses a Makefile, so you just run "make" to build the executable. It will build into "bin" and needs the assets directory to work. It is required for the TTF that is used by default.

## Dependencies

This requires SDL3 and SDL3\_TTF. On Arch, since SDL3\_TTF is installed in a weird place by yay and by a normal build, I had to create a symlink from "/usr/lib/libSDL3\_ttf.so.0" to "/usr/local/lib/libSDL3\_ttf.so.0", since /usr/local/lib apparently is not on my PATH.
