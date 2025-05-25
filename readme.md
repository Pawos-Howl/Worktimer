## Notes

Please do not take any of this too seriously or use in production, this is honestly just how I am attempting to enforce better C++ habits and learning a whole bunch of new stuff. Consider this a passion project, because it really is. SDL3 is a thing I thought I should learn, so here is a project that uses it. As much as it looks like there are several people writing code, especially with the wildly different styles in header files, I wrote all of the headers I wrote myself, they just changed styles and I do not want to fix it. Also, the program is wildly inefficent, so sorry.

# Worktimer

This is a semicustomizable worktimer written in C++ using SDL3. This follows some form of the Pomodoro method for working, using 25 minute work sessions followed by a 5 minute break and then a long break.

## How to build

This uses a Makefile, so you just run "make" to build the executable. It will build into "bin" and needs the assets directory to work. It is required for the TTF that is used by default.

## Dependencies

This requires SDL3 and SDL3\_TTF. On Arch, since SDL3\_TTF is installed in a weird place by yay and by a normal build, I had to create a symlink from "/usr/lib/libSDL3\_ttf.so.0" to "/usr/local/lib/libSDL3\_ttf.so.0", since /usr/local/lib apparently is not on my PATH.

### TODO
Properly implement curved edges on buttons
Figure out what makes the project such a memory and CPU hog
Figure out how to speed up the project
