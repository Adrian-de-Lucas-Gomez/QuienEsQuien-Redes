#include <stdio.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define SCREEN_SIZE_X 800
#define SCREEN_SIZE_Y 600

//Temporal hasta que no haya makefile
//Para compilar: g++ -o prueba main.cc -lSDL2 -lSDL2_image

int main (int argc, char* argv[])
{
    //Variables
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;
	SDL_Texture* img = NULL;
	int w, h; // w y h de la imagen

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
    img = IMG_LoadTexture(renderer, "../media/prueba.jpg");
	SDL_QueryTexture(img, NULL, NULL, &w, &h);
	SDL_Rect rect;
    rect.x = 0; rect.y = 0;
    rect.w = w/2; rect.h = h/2; 

	//Bucle principal
	while (1) {

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
		SDL_RenderCopy(renderer, img, NULL, &rect);
		SDL_RenderPresent(renderer);
	}
	
	SDL_DestroyTexture(img);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

    std::cout << "babai\n";

    return 0;
}