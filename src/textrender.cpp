#include "textrender.hpp"

#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>

#include <SDL3_ttf/SDL_ttf.h>

#include "utils.hpp"

struct DEFAULTS {
    // ONLY if the file is there
    std::string fontPaf = "assets/arial.ttf";
    int size = 12;
    SDL_Color color = {255,255,255,255};

};

DEFAULTS defs;

RenderInfo *getTextureFromChars(char* text, TTF_Font* font, SDL_Color color, SDL_Renderer* renderer) {
    RenderInfo *rinfo = new RenderInfo;

    SDL_Surface* tmpsurface = TTF_RenderUTF8_Solid(font, text, color);
    checkTTFError(tmpsurface == NULL);

    rinfo->width = tmpsurface->w;
    rinfo->height = tmpsurface->h;

    rinfo->texture = SDL_CreateTextureFromSurface(renderer, tmpsurface);
    SDL_DestroySurface(tmpsurface);

    return rinfo;
}

inline TTF_Font *loadFont(char* paf, int size) {
    if(!f_exists(paf)){return NULL;}
    TTF_Font *tmpfont = TTF_OpenFont(paf, size);
    checkTTFError(tmpfont == NULL);
    return tmpfont;
}

RenderInfo *getTextureFromChars(char* text, char* paf, int size, SDL_Color color, SDL_Renderer* renderer) {
    TTF_Font *tmpfont = loadFont(paf, size);
    checkTTFError(tmpfont == NULL);

    RenderInfo *info = getTextureFromChars(text, tmpfont, color, renderer);

    TTF_CloseFont(tmpfont);

    return info;
}
RenderInfo *getTextureFromChars(char* text, int size, SDL_Renderer* renderer){
    TTF_Font *tmpfont = loadFont((char*)defs.fontPaf.data(), size);
    checkTTFError(tmpfont == NULL);

    RenderInfo *info = getTextureFromChars(text, tmpfont, defs.color, renderer);

    TTF_CloseFont(tmpfont);

    return info;
}
RenderInfo *getTextureFromChars(char* text, SDL_Renderer* renderer) {
    TTF_Font *tmpfont = loadFont((char*)defs.fontPaf.data(), defs.size);
    checkTTFError(tmpfont == NULL);

    RenderInfo *info = getTextureFromChars(text, tmpfont, defs.color, renderer);

    TTF_CloseFont(tmpfont);

    return info;
}

void RenderTTFTexture(RenderInfo* renderinfo, SDL_Renderer* renderer, int locx, int locy) {
    // do some weird logic to get the SDL_FRect
    SDL_Rect tmprect = {locx, locy, renderinfo->width, renderinfo->height};
    SDL_FRect tmpfrect = SDLRectToFRect(&tmprect);

    SDL_RenderTexture(renderer, renderinfo->texture, NULL, &tmpfrect);
}
void RenderTTFTexture(RenderInfo* renderinfo, SDL_Renderer* renderer, centeredInfo* centering) {
    RenderTTFTexture(renderinfo, renderer, centering->x, centering->y);
}
void RenderTTFTexture(RenderInfo* renderinfo, SDL_Renderer* renderer) {
    RenderTTFTexture(renderinfo, renderer, 0, 0);
}

int centerTextureX(RenderInfo* renderinfo, SDL_Window* window) {
    int w, h;
    checkSDLError(SDL_GetWindowSizeInPixels(window, &w, &h));
    return (w-renderinfo->width)/2;
}
int centerTextureY(RenderInfo* renderinfo, SDL_Window* window) {
    int w, h;
    checkSDLError(SDL_GetWindowSizeInPixels(window, &w, &h));
    return (h-renderinfo->height)/2;
}
centeredInfo *centerTexture(RenderInfo* renderinfo, SDL_Window* window) {
    centeredInfo *center = new centeredInfo;
    int w, h;
    checkSDLError(SDL_GetWindowSizeInPixels(window, &w, &h));
    int rw = renderinfo->width;
    int rh = renderinfo->height;
    center->x = (w-rw)/2;
    center->y = (h-rh)/2;
    return center;
}


void DestroyRenderInfo(RenderInfo* renderinfo) {
    SDL_DestroyTexture(renderinfo->texture);
    delete renderinfo;
}
void DestroyCenteredInfo(centeredInfo* center) {
    delete center;
}
