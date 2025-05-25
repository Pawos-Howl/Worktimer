#ifndef graphicsbutton
#define graphicsbutton

#include "graphics_elements/interactable.hpp"
#include "graphics_elements/utils.hpp"

#include <SDL3/SDL_render.h>

#include <functional>

// button state
typedef int8_t buttonstate;
#define GRAPHICS_BUTTON_PRESSED 0x01
#define GRAPHICS_BUTTON_HOVER   0x02

class GraphicsButton : InteractableObject {
public:
    GraphicsButton(char* text, bool pressedEffect, bool hoverEffect, bool roundedCorners, SDL_FRect place, SDL_Renderer* renderer);
    ~GraphicsButton() override;
    // void setCallback(void (*new_callback)());
    void setCallback(std::function<void()> new_callback); // alr, this is weird, lemme explain
    /* The function has to return void, but it can be passed one value as wanted by the dev
     * The optional arg MUST be set to NULL or nullptr if it is NOT used, otherwise it will be treated as used
     */


    void render(SDL_Renderer* renderer) override;

    // methods for status pushes
    void isClicked() override;
    void isHovered() override;

    bool isPointAbove(int x, int y) override;

private:
    SDL_Texture* button;
    SDL_Texture* hover; // nullptr if no effect
    SDL_Texture* pressed; // ^
    SDL_FRect placement;

    // void(*callback)(); // C implementation, here if needed later
    std::function<void()> callback;

    SDL_Texture* renderTexture(char* text, bool roundedCorners, SDL_Color color, SDL_Color bordercolor, SDL_Renderer* renderer);

    // button states
    buttonstate oldstate;
    buttonstate state;
};

#endif
