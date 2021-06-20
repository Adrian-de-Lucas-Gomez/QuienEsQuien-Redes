#include <stdio.h>
#include <iostream>

#include "ButtonPermanent.h"

#define SCREEN_SIZE_X 800
#define SCREEN_SIZE_Y 600

//++++++++++VARIABLES++++++++++
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

//Imagen
SDL_Texture* imgFondo = NULL;
int w, h; // w y h de la imagen

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//Temporal hasta que no haya makefile
//Para compilar: g++ -o prueba mainprueba.cc -lSDL2 -lSDL2_image

int main (int argc, char* argv[])
{
    //Inicializar SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        fprintf(stderr, "SDL could not initialize\n");
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

	//Crear botones
    Button botonNormal(0, 0, 100, 100, renderer,
        "../media/cara0.png", "../media/cara1.png", "../media/cara2.png");
    ButtonPermanent botonPermanente(SCREEN_SIZE_X - 100, 0, 100, 100, renderer,
        "../media/cara3.png", "../media/cara4.png", "../media/cara5.png");

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

            botonNormal.handle_events(e);
            botonPermanente.handle_events(e);
		}

		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, imgFondo, NULL, &rect);
        botonNormal.show();
        botonPermanente.show();
		SDL_RenderPresent(renderer);
	}
	
    //Destruimos todo al cerrar
    SDL_DestroyTexture(imgFondo);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    std::cout << "babai\n";

    return 0;
}