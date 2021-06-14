#include <stdio.h>
#include <iostream>
#include <SDL2/SDL.h>

#define SCREEN_SIZE_X 800
#define SCREEN_SIZE_Y 600

int main (int argc, char* argv[])
{
    // ----- Initialize SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        fprintf(stderr, "SDL could not initialize\n");
        return 1;
    }

    // ----- Create window
    SDL_Window* window = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_SIZE_X, SCREEN_SIZE_Y, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (!window)
    {
        fprintf(stderr, "Error creating window.\n");
        return 2;
    }

    char a;
    std::cin >> a;

    std::cout << "Hola\n";

    return 0;
}