#include "Button.h"

Button::Button(int x, int y, int w, int h, int buttonID, SDL_Renderer* r, const char* imgNormal,
    const char* imgClickLeft, const char* imgClickRight) : id(buttonID), renderer(r)
{
    box.x = x; box.y = y;
    box.w = w; box.h = h;

    textureNormal = IMG_LoadTexture(renderer, imgNormal);
    textureClickLeft = IMG_LoadTexture(renderer, imgClickLeft);

    if (imgClickRight != "") {
        textureClickRight = IMG_LoadTexture(renderer, imgClickRight);
    }

    mainTexture = textureNormal;
}

Button::~Button() {
    renderer = NULL;
    mainTexture = NULL;
    SDL_DestroyTexture(textureNormal);
    SDL_DestroyTexture(textureClickLeft);
    SDL_DestroyTexture(textureClickRight);
}

bool Button::handle_events(SDL_Event event) {
        int x = 0, y = 0; //Offsets del raton

        if (event.type == SDL_MOUSEBUTTONDOWN) //Boton pulsado
        {
            if (event.button.button == SDL_BUTTON_LEFT) //Izquierdo
            {
                x = event.button.x;
                y = event.button.y;

                //Si el raton esta sobre el boton
                if ((x > box.x) && (x < box.x + box.w) && (y > box.y) && (y < box.y + box.h))
                {
                    mainTexture = textureClickLeft;
                    return true;
                }
            }
            else if (textureClickRight != NULL && event.button.button == SDL_BUTTON_RIGHT) //Derecho
            {
                x = event.button.x;
                y = event.button.y;

                //Si el raton esta sobre el boton
                if ((x > box.x) && (x < box.x + box.w) && (y > box.y) && (y < box.y + box.h))
                {
                    mainTexture = textureClickRight;
                    return true;
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
                    mainTexture = textureNormal;
                }
            }
            else if (textureClickRight != NULL && event.button.button == SDL_BUTTON_RIGHT) //Derecho
            {
                x = event.button.x;
                y = event.button.y;

                //Si el raton esta sobre el boton
                if ((x > box.x) && (x < box.x + box.w) && (y > box.y) && (y < box.y + box.h))
                {
                    mainTexture = textureNormal;
                }
            }
        }
        return false;
    }