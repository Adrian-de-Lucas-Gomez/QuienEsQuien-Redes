#include <stdio.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Button.h"

#define SCREEN_SIZE_X 800
#define SCREEN_SIZE_Y 600

//++++++++++VARIABLES++++++++++
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

//Imagen
SDL_Texture* imgFondo = NULL;
int w, h; // w y h de la imagen

//Boton
SDL_Surface* buttonSheet = NULL;
SDL_Rect clips[4];

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

SDL_Surface* load_image(std::string filename)
{
    SDL_Surface* loadedImage = NULL;
    SDL_Surface* optimizedImage = NULL;

    //Cargar imagen
    loadedImage = IMG_Load(filename.c_str());

    if (loadedImage != NULL)
    {
        optimizedImage = SDL_DisplayFormat(loadedImage);
        SDL_FreeSurface(loadedImage);

        if (optimizedImage != NULL)
        {
            SDL_SetColorKey(optimizedImage, SDL_SRCCOLORKEY, SDL_MapRGB(optimizedImage->format, 0, 0xFF, 0xFF));
        }
    }
    return optimizedImage;
}

void apply_surface(int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL)
{
    SDL_Rect offset;
    offset.x = x;
    offset.y = y;

    SDL_BlitSurface(source, clip, destination, &offset);
}

bool load_files()
{
    //Imagen fondo
    imgFondo = load_image("../media/prueba.jpg");
    if (imgFondo == NULL)
        return false;

    //Spritesheet del boton
    buttonSheet = load_image("../media/button.png");
    if (buttonSheet == NULL)
        return false;

    return true;
}

void close()
{
    SDL_DestroyTexture(imgFondo);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

void set_clips()
{
    //Spritesheet del boton
    clips[ButtonState::CLIP_MOUSEOVER].x = 0;
    clips[ButtonState::CLIP_MOUSEOVER].y = 0;
    clips[ButtonState::CLIP_MOUSEOVER].w = 320;
    clips[ButtonState::CLIP_MOUSEOVER].h = 240;

    clips[ButtonState::CLIP_MOUSEOUT].x = 320;
    clips[ButtonState::CLIP_MOUSEOUT].y = 0;
    clips[ButtonState::CLIP_MOUSEOUT].w = 320;
    clips[ButtonState::CLIP_MOUSEOUT].h = 240;

    clips[ButtonState::CLIP_MOUSEDOWN].x = 0;
    clips[ButtonState::CLIP_MOUSEDOWN].y = 240;
    clips[ButtonState::CLIP_MOUSEDOWN].w = 320;
    clips[ButtonState::CLIP_MOUSEDOWN].h = 240;

    clips[ButtonState::CLIP_MOUSEUP].x = 320;
    clips[ButtonState::CLIP_MOUSEUP].y = 240;
    clips[ButtonState::CLIP_MOUSEUP].w = 320;
    clips[ButtonState::CLIP_MOUSEUP].h = 240;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//Temporal hasta que no haya makefile
//Para compilar: g++ -o prueba main.cc -lSDL2 -lSDL2_image

int main (int argc, char* argv[])
{
    //Inicializar SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        fprintf(stderr, "SDL could not initialize\n");
        return -1;
    }

    //Cargar recursos
    if (!load_files())
    {
        fprintf(stderr, "Error loading files\n");
        return -1;
    }

    //Crear la ventana
    window = SDL_CreateWindow("¿Quién es quién?", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_SIZE_X, SCREEN_SIZE_Y, SDL_WINDOW_SHOWN);
    if (!window)
    {
        fprintf(stderr, "Error creating window.\n");
        return -1;
    }
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    //Crear imagen
    imgFondo = IMG_LoadTexture(renderer, "../media/prueba.jpg");
	SDL_QueryTexture(imgFondo, NULL, NULL, &w, &h);
	SDL_Rect rect;
    rect.x = 0; rect.y = 0;
    rect.w = w/2; rect.h = h/2; 

	//Crear boton
    Button myButton(170, 120, 320, 240);

	//Bucle principal
	while (true) {

		SDL_Event e;
		if (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				break;
            }
			else if (e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_ESCAPE) {
				break;
            }
		} 
		
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, imgFondo, NULL, &rect);
		SDL_RenderPresent(renderer);
	}
	
    //Destruimos todo al cerrar
    close();
    std::cout << "babai\n";

    return 0;
}