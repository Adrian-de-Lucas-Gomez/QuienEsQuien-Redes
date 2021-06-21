#include <stdio.h>
#include <iostream>
#include <vector>

#include "ButtonPermanent.h"

#define SCREEN_SIZE_X 800
#define SCREEN_SIZE_Y 600

//++++++++++VARIABLES++++++++++
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

//Imagen
SDL_Texture* imgFondo = NULL;
SDL_Texture* imgCara = NULL;
SDL_Texture* imgTexto = NULL;
SDL_Texture* imgVictoria = NULL;
SDL_Texture* imgPerder = NULL;

Button* botonSalir;     bool salir = false;
Button* botonSi;        bool si = false;
Button* botonNo;        bool no = false;
Button* botonPasar;     bool pasar = false;
Button* botonResolver;  bool resolver = false;
Button* botonPreguntar; bool preguntar = false;

std::vector<ButtonPermanent*> botonCaras;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//Temporal hasta que no haya makefile
//Para compilar: g++ -o prueba mainprueba.cc -lSDL2 -lSDL2_image

int main (int argc, char* argv[])
{
    SDL_Window* window = NULL;
    SDL_Texture* imgFondo = NULL;
    int w, h; // w y h de la imagen

    //Inicializar SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "SDL could not initialize\n");
        return -1;
    }

    //Crear la ventana
    window = SDL_CreateWindow("¿Quién es quién?", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_SIZE_X, SCREEN_SIZE_Y, SDL_WINDOW_SHOWN);
    if (!window) {
        fprintf(stderr, "Error creating window.\n");
        return -1;
    }
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    //Crear imagenes
    imgFondo = IMG_LoadTexture(renderer, "../media/fondo.png");
	SDL_Rect rectFondo;
    rectFondo.x = 0; rectFondo.y = 0;
    rectFondo.w = SCREEN_SIZE_X; rectFondo.h = SCREEN_SIZE_Y;

    imgCara = IMG_LoadTexture(renderer, "../media/cara8.png");
	SDL_Rect rectCara;
    rectCara.x = SCREEN_SIZE_X/2 - 130/2; rectCara.y = SCREEN_SIZE_Y - 170;
    rectCara.w = 130; rectCara.h = 130; 

    imgTexto = IMG_LoadTexture(renderer, "../media/prueba.jpg");
	SDL_Rect rectTexto;
    rectTexto.x = 30; rectTexto.y = 450;
    rectTexto.w = 300; rectTexto.h = 70; 

    imgVictoria = IMG_LoadTexture(renderer, "../media/prueba.jpg");
    imgPerder = IMG_LoadTexture(renderer, "../media/prueba.jpg");
	SDL_Rect rectFin;
    rectFin.x = 100; rectFin.y = 200;
    rectFin.w = 600; rectFin.h = 200; 

	//Crear botones
    botonSalir = new Button(30, SCREEN_SIZE_Y - 70, 40, 40, -1, renderer,
        "../media/botonSalir.png", "../media/botonSalir2.png");
    botonResolver = new Button(505, SCREEN_SIZE_Y - 150, 160, 80, -1, renderer,
        "../media/botonResolver.png", "../media/botonResolver2.png");
    botonPreguntar = new Button(135, SCREEN_SIZE_Y - 150, 160, 80, -1, renderer,
        "../media/botonPregunta.png", "../media/botonPregunta2.png");
    botonSi = new Button(510, SCREEN_SIZE_Y - 150, 80, 80, -1, renderer,
        "../media/botonSi.png", "../media/botonSi2.png");
    botonNo = new Button(635, SCREEN_SIZE_Y - 150, 80, 80, -1, renderer,
        "../media/botonNo.png", "../media/botonNo2.png");
    botonPasar = new Button(505, SCREEN_SIZE_Y - 150, 160, 80, -1, renderer,
        "../media/botonPasar.png", "../media/botonPasar2.png");
        
    //Crear botones caras
    botonCaras.reserve(18);
    for (int i = 0; i < botonCaras.capacity(); ++i) {
        std::string a = "../media/cara" + std::to_string(i) + ".png";
        std::string b = "../media/cara" + std::to_string(i) + "I.png";
        std::string c = "../media/cara" + std::to_string(i) + "D.png";

        int x = 22 + 117*(i%6); int y = 22 + 117*(i/6);

        ButtonPermanent* aux = new ButtonPermanent(x + 10*(i%6), y + 10*(i/6), 117, 117, i, renderer,
            a.c_str(), b.c_str(), c.c_str());
        botonCaras.push_back(aux);
    }

	//Bucle principal
	while (true) {

		SDL_Event e;
		if (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                break;
            else if (e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_ESCAPE)
                break;

            // for (int i = 0; i < botonCaras.capacity(); ++i) {
            //     botonCaras[i]->handle_events(e);
            // }
        }

        //Renderizado
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, imgFondo, NULL, &rectFondo);
		//SDL_RenderCopy(renderer, imgCara, NULL, &rectCara);
		//SDL_RenderCopy(renderer, imgTexto, NULL, &rectTexto);

		SDL_RenderCopy(renderer, imgVictoria, NULL, &rectFin);

        //botonPreguntar->show();
        //botonResolver->show();
        //botonSi->show();
        //botonNo->show();
        //botonSalir->show();
        //botonPasar->show();

        //Las caras se muestran siempre
        // for(int i = 0; i < botonCaras.capacity(); ++i){
        //     botonCaras[i]->show();
        // }

		SDL_RenderPresent(renderer);
	}
	
    //Destruimos todo al cerrar
    delete botonNo;
    delete botonSi;
    delete botonPasar;
    delete botonResolver;
    delete botonSalir;
    delete botonPreguntar;

    for (int i = 0; i < botonCaras.capacity(); ++i) {
        delete botonCaras[i];
        botonCaras[i] = nullptr;
    }
    botonCaras.clear();

    SDL_DestroyTexture(imgFondo);
    SDL_DestroyTexture(imgCara);
    SDL_DestroyTexture(imgTexto);
    SDL_DestroyTexture(imgVictoria);
    SDL_DestroyTexture(imgPerder);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    std::cout << "babai\n";

    return 0;
}