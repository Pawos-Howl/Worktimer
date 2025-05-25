/*
 * Basic definition for a graphical object such as a button
 * This allows for later code to better have a set of code that allow for better interactions between the object and IO.
 */

#ifndef interactable
#define interactable

#include <SDL3/SDL_render.h>

// silence the warnings of unused params
#if defined(__GCC__) || defined(__clang__) || defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

/*Microsoft uses the following in MSVC
 * #pragma warning(push)
 * #pragma warning(disable: <warning-number>)
 * // code
 * #pragma warning(pop)
 *
 */

struct InteractableObject
{
public:
    // default deconstructor
    virtual ~InteractableObject() {};

    // render the object to the window
    virtual void render(SDL_Renderer* renderer) {}

    /* basic cursor and IO position based interactions
     * One thing to note about isClicked and isHovered is they have priority. isClicked will be triggered by isHovered will not be. When the click is removed, isHovered will be run.
     * isClicked or isHovered will be called before render always
     */
    // isClicked is called for the top object visually that has a clicked mouse pointer on it
    virtual void isClicked() {}
    // isHovered is called for the top object visually that has a mouse pointer on it
    virtual void isHovered() {}

    /* isPointAbove checks if the labeled X and Y are above the interactable object
     * used to determine isClicked or isHovered actions and calls.
     */
    virtual bool isPointAbove(int x, int y) {return false;}
};

// allow the warnings back
#ifdef __GCC__
#pragma GCC diagnostic pop
#endif

#endif
