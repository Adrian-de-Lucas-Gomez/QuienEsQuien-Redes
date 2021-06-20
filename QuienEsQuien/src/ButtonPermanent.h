#include "Button.h"

class ButtonPermanent : public Button
{
public:
    ButtonPermanent(int x, int y, int w, int h, int buttonID, SDL_Renderer* r, const char* imgNormal,
        const char* imgClickLeft, const char* imgClickRight = "") :
        Button(x, y, w, h, buttonID, r, imgNormal, imgClickLeft, imgClickRight) {}

    void handle_events(SDL_Event event) override {
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
                    if(mainTexture == textureClickLeft)
                        mainTexture = textureNormal;
                    else
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
                    if(mainTexture == textureClickRight)
                        mainTexture = textureNormal;
                    else
                        mainTexture = textureClickRight;
                }
            }
        }
    }
};