#include "graphics_elements/button.hpp"

#include "graphics_elements/utils.hpp"
#include "utils.hpp"

#include "textrender.hpp"

#include <SDL3/SDL_render.h>

#define buttoncolor {200,200,200,255}
#define buttonoutline {255,255,255,255}
#define hovercolor {175,175,175,255}
#define pressedcolor {128,128,128,255}
#define buttonTextColor {0,0,0,255}
#define textspacing 4 // pixels to add on any side of the text as a buffer for the button
#define bordersize 2 // the size of the button border in pixels
#define buttontextsize 20


GraphicsButton::GraphicsButton(char* text, bool pressedEffect, bool hoverEffect, bool roundedCorners, SDL_FRect place, SDL_Renderer* renderer) {
    // render the buttons
    button = renderTexture(text, roundedCorners, buttoncolor, buttonoutline, renderer);
    if ( hoverEffect ) {
        hover = renderTexture(text, roundedCorners, hovercolor, buttonoutline, renderer);
    }
    if ( pressedEffect ) {
        pressed = renderTexture(text, roundedCorners, pressedcolor, buttonoutline, renderer);
    }

    placement = place;
}
GraphicsButton::~GraphicsButton() {
    SDL_DestroyTexture(button);
    if ( hover != nullptr ) {
        SDL_DestroyTexture(hover);
    }
    if ( pressed != nullptr ) {
        SDL_DestroyTexture(pressed);
    }
}

void GraphicsButton::setCallback(std::function<void()> new_callback) {
    callback = new_callback;
}

void GraphicsButton::render(SDL_Renderer* renderer) {
    // handle state things

    // std::cout << "button texture ptr" << button << std::endl;
    if ( state & GRAPHICS_BUTTON_PRESSED ) SDL_RenderTexture(renderer, pressed, NULL, &placement);
    else if ( state & GRAPHICS_BUTTON_HOVER ) SDL_RenderTexture(renderer, hover, NULL, &placement);
    else SDL_RenderTexture(renderer, button, NULL, &placement);

    // handle state swaps
    oldstate = state;
    state = 0x00;
}

// place isclicked, isovered, isPointAbove
void GraphicsButton::isClicked() {
    if ( oldstate & GRAPHICS_BUTTON_PRESSED || state & GRAPHICS_BUTTON_PRESSED) {
        state |= GRAPHICS_BUTTON_PRESSED;
        return;
    }
    // first click
    state |= GRAPHICS_BUTTON_PRESSED;

    // run callback
    if (callback!=NULL) {
        callback();
    }
}
void GraphicsButton::isHovered() {
    if ( state & GRAPHICS_BUTTON_HOVER ) {
        state |= GRAPHICS_BUTTON_HOVER;
        return;
    }
    // first hover
    state |= GRAPHICS_BUTTON_HOVER;
}
bool GraphicsButton::isPointAbove(int x, int y) {
    return (
        x >= placement.x && x <= placement.x + placement.w &&
        y >= placement.y && y <= placement.y + placement.h
    );
}


SDL_Texture* GraphicsButton::renderTexture(char* text, bool roundedCorners, SDL_Color color, SDL_Color bordercolor, SDL_Renderer* renderer) {
    // rounded corners are not implemented yet...
    if ( roundedCorners ) printv("Rounded corners are not implemented, ignoring instruction...");

    RenderInfo* renderinfo = getTextureFromChars(text, buttontextsize, buttonTextColor, renderer);
    // currenty rounded corners are not supported, so ignore it, but when it is, it will look sick
    SDL_FRect box;
    box.h = renderinfo->height + textspacing*2 + bordersize*2;
    box.w = renderinfo->width + textspacing*2 + bordersize*2;
    box.x = 0;
    box.y = 0;

    // ensure the renderer exists, then set it as the renderer object
    if ( renderer == nullptr ) {
        printv("Cannot use the provided renderer... aborting...");
        return NULL;
    }

    SDL_Texture* button = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, box.w,box.h);

    SDL_SetRenderTarget(renderer, button);
    SDL_SetRenderDrawColor(renderer, bordercolor.r,bordercolor.g,bordercolor.b,bordercolor.a);
    SDL_RenderFillRect(renderer,&box);

    box.h = renderinfo->height + textspacing*2;
    box.w = renderinfo->width + textspacing*2;
    box.x = bordersize;
    box.y = bordersize;

    SDL_SetRenderDrawColor(renderer, color.r,color.g,color.b,color.a);
    SDL_RenderFillRect(renderer,&box);

    box.h = renderinfo->height;
    box.w = renderinfo->width;
    box.x = textspacing + bordersize;
    box.y = textspacing + bordersize;

    SDL_RenderTexture(renderer,renderinfo->texture,NULL,&box);

    // clean up
    SDL_SetRenderDrawColor(renderer,0,0,0,255);
    SDL_SetRenderTarget(renderer,NULL);

    return button;
}
