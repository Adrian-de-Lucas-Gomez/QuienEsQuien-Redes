#ifndef __Button_H__
#define __Button_H__

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
    SDL_Renderer* renderer = NULL;
    int id;

public:
    Button(int x, int y, int w, int h, int buttonID, SDL_Renderer* r, const char* imgNormal,
        const char* imgClickLeft, const char* imgClickRight = "");

    ~Button();

    virtual bool handle_events(SDL_Event event);
    void show() { SDL_RenderCopy(renderer, mainTexture, NULL, &box); }
    int getID() { return id; }
};

#endif