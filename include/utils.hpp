#ifndef utils
#define utils

#include <string>
#include <bits/stdint-uintn.h>
#include <SDL3/SDL_rect.h>
#include <iostream>

extern bool verbose; // verbosity mode

bool f_exists(const std::string& name);

void uint32ToChars(uint32_t num, char* chars);

SDL_FRect SDLRectToFRect(SDL_Rect* rect);

#define checkSDLError(val) _checkSDLError( (val), #val, __FILE__, __LINE__ )
void _checkSDLError(
    const int code,
    const char *const func,
    const char *const file,
    const int line);

#define checkTTFError(val) _checkTTFError( (val), #val, __FILE__, __LINE__ )
void _checkTTFError(
    const int code,
    const char *const func,
    const char *const file,
    const int line);

// template and header definition
// this just returns zero since apparently &arr == arr[0]... maybe fix that... later
template <typename T> inline static int sizeOfArray(T* arr) {
    return sizeof(&arr) / sizeof(arr[0]); //divide bytes in array by the size of the first element in bytes to get number of items
}

inline void printv(const char* str) {
    if (verbose) std::cout << str << std::endl;
}

#endif
