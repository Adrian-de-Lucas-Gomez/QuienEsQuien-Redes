#ifndef __ButtonPermanent_H__
#define __ButtonPermanent_H__

#include "Button.h"

class ButtonPermanent : public Button
{
public:
    ButtonPermanent(int x, int y, int w, int h, int buttonID, SDL_Renderer* r, const char* imgNormal,
        const char* imgClickLeft, const char* imgClickRight = "") :
        Button(x, y, w, h, buttonID, r, imgNormal, imgClickLeft, imgClickRight) {}

    virtual bool handle_events(SDL_Event event) override;
};

#endif