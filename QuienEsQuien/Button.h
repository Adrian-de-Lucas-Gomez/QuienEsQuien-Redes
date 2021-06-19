#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

enum ButtonState = {
    CLIP_MOUSEOVER = 0,
    CLIP_MOUSEOUT = 1,
    CLIP_MOUSEDOWN = 2,
    CLIP_MOUSEUP = 3
}

class Button
{
private:
    SDL_Rect box;
    SDL_Rect* clip;
    SDL_Surface* buttonSheet;

public:
    Button(int x, int y, int w, int h, SDL_Surface* b) : buttonSheet(b) {
        box.x = x; box.y = y;
        box.w = w; box.h = h;

        //Set the default sprite
        clip = &clips[CLIP_MOUSEOUT];
    }

    void handle_events(SDL_Event event) {
        int x = 0, y = 0; //Offsets del raton

        if (event.type == SDL_MOUSEMOTION) //Movimiento del raton
        {
            x = event.motion.x;
            y = event.motion.y;

            //Si el raton esta sobre el boton
            if ((x > box.x) && (x < box.x + box.w) && (y > box.y) && (y < box.y + box.h))
            {
                clip = &clips[CLIP_MOUSEOVER];
            }
            else
            {
                clip = &clips[CLIP_MOUSEOUT];
            }
        }
        if (event.type == SDL_MOUSEBUTTONDOWN) //Boton pulsado
        {
            if (event.button.button == SDL_BUTTON_LEFT) //Izquierdo
            {
                x = event.button.x;
                y = event.button.y;

                //Si el raton esta sobre el boton
                if ((x > box.x) && (x < box.x + box.w) && (y > box.y) && (y < box.y + box.h))
                {
                    clip = &clips[CLIP_MOUSEDOWN];
                }
            }
        }
        if (event.type == SDL_MOUSEBUTTONUP) //Boton dejado de pulsar
        {
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                x = event.button.x;
                y = event.button.y;

                //Si el raton esta sobre el boton
                if ((x > box.x) && (x < box.x + box.w) && (y > box.y) && (y < box.y + box.h))
                {
                    clip = &clips[CLIP_MOUSEUP];
                }
            }
        }
    }

    void show() {
        apply_surface(box.x, box.y, buttonSheet, screen, clip);
    }
};
