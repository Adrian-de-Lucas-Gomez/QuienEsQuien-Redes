#include "ButtonPermanent.h"

bool ButtonPermanent::handle_leftClick(SDL_Event event) {
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
                if (mainTexture == textureClickLeft)
                    mainTexture = textureNormal;
                else
                    mainTexture = textureClickLeft;

                return true;
            }
        }
    }
    return false;
}

void ButtonPermanent::setNormal() {
    mainTexture = textureNormal;
}

bool ButtonPermanent::handle_rightClick(SDL_Event event) {
    int x = 0, y = 0; //Offsets del raton

    if (event.type == SDL_MOUSEBUTTONDOWN) //Boton pulsado
    {
        if (event.button.button == SDL_BUTTON_RIGHT) //Derecho
        {
            x = event.button.x;
            y = event.button.y;

            //Si el raton esta sobre el boton
            if ((x > box.x) && (x < box.x + box.w) && (y > box.y) && (y < box.y + box.h))
            {
                if(mainTexture == textureClickRight)
                    mainTexture = textureNormal;
                else
                    mainTexture = textureClickRight;
                    
                return true;
            }
        }
    }
    return false;
}