#ifndef textrender
#define textrender

#include <SDL3/SDL_pixels.h>

#include <SDL3_ttf/SDL_ttf.h>

#include <string>

struct RenderInfo {
    SDL_Texture* texture;
    int width;
    int height;
};
struct centeredInfo {
    int x, y; // SDL uses int, so I will too
};

// big struct call
RenderInfo *getTextureFromChars(char* text, TTF_Font* font, SDL_Color color, SDL_Renderer* renderer);

// other helper calls
RenderInfo *getTextureFromChars(char* text, char* paf, int size, SDL_Color color, SDL_Renderer* renderer);
RenderInfo *getTextureFromChars(char* text, int size, SDL_Renderer* renderer);
RenderInfo *getTextureFromChars(char* text, SDL_Renderer* renderer);

// render
void RenderTTFTexture(RenderInfo* renderinfo, SDL_Renderer* renderer, int locx, int locy);
void RenderTTFTexture(RenderInfo* renderinfo, SDL_Renderer* renderer, centeredInfo* centering);
void RenderTTFTexture(RenderInfo* renderinfo, SDL_Renderer* renderer);

// get the cords to render on screen
int centerTextureY(RenderInfo* renderinfo, SDL_Window* window);
int centerTextureX(RenderInfo* renderinfo, SDL_Window* window);
centeredInfo *centerTexture(RenderInfo* renderinfo, SDL_Window* window);

void DestroyRenderInfo(RenderInfo* renderinfo);
void DestroyCenteredInfo(centeredInfo* center);

#endif

