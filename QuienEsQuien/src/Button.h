#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Button
{
protected:
    SDL_Texture* mainTexture = NULL;
    SDL_Texture* textureNormal = NULL;
    SDL_Texture* textureClickLeft = NULL;
    SDL_Texture* textureClickRight = NULL;
    SDL_Rect box;
    SDL_Rect* clip;
    SDL_Renderer* renderer = NULL;
    int id;

public:
    Button(int x, int y, int w, int h, int buttonID, SDL_Renderer* r, const char* imgNormal,
        const char* imgClickLeft, const char* imgClickRight = "") : id(buttonID), renderer(r)
    {
        box.x = x; box.y = y;
        box.w = w; box.h = h;

        textureNormal = IMG_LoadTexture(renderer, imgNormal);
        SDL_QueryTexture(textureNormal, NULL, NULL, &box.w, &box.h);
        textureClickLeft = IMG_LoadTexture(renderer, imgClickLeft);
        SDL_QueryTexture(textureClickLeft, NULL, NULL, &box.w, &box.h);

        if (imgClickRight != "") {
            textureClickRight = IMG_LoadTexture(renderer, imgClickRight);
            SDL_QueryTexture(textureClickRight, NULL, NULL, &box.w, &box.h);
        }

        mainTexture = textureNormal;
    }

    ~Button() {
        renderer = NULL;
        mainTexture = NULL;
        SDL_DestroyTexture(textureNormal);
        SDL_DestroyTexture(textureClickLeft);
        SDL_DestroyTexture(textureClickRight);
    }

    virtual void handle_events(SDL_Event event) {
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
    }

    void show() {
        SDL_RenderCopy(renderer, mainTexture, NULL, &box);
    }

    int getID() {
        return id;
    }
};