#include "Client.h"
#include "Message.h"
#include "ButtonPermanent.h"

#define SCREEN_SIZE_X 800
#define SCREEN_SIZE_Y 600

void Client::login()
{
    std::string msg;

    GameMessage em(nick, msg);
    em.type = GameMessage::NUEVAPARTIDA;

    int returnCode = socket.send(em, socket.getSD());
    if (returnCode == -1) {
        std::cout << "Error: login send\n";
        return;
    }
    chooseFaces();
}

void Client::logout()
{
    std::string msgLogOut;

    GameMessage out(nick, msgLogOut);
    out.type = GameMessage::SALIR;

    int returnCode = socket.send(out, socket.getSD());
    if (returnCode == -1) {
        std::cout << "Error: logout send\n";
        return;
    }

    inGame = false;
}

void Client::chooseFaces()
{
    //Cara cliente
    int randID = rand() % GameMessage::NUM_FACES;
    std::cout << "ID cara cliente: " << randID << '\n';
    myFace = randID;

    //Para que el servidor conozca nuestra cara internamente
    GameMessage men(nick, "", randID); 
    men.type = GameMessage::INICIO;

    int returnCode = socket.send(men, socket.getSD());
    if (returnCode == -1)
    {
        std::cout << "Error: face message send\n";
        return;
    }
}

void Client::resolve(bool win)
{
    std::cout << "\nSE ACABÓ LA PARTIDA\n";
    if (win)
        std::cout << "¡Enhorabuena! Has ganado" << "\n";
    else
        std::cout << "¡Oh no! Has perdido" << "\n";
}

void Client::sendMessage(uint8_t messageType, std::string message) {
    GameMessage men(nick, message);
    men.type = messageType;

    int returnCode = socket.send(men, socket.getSD());
    if (returnCode == -1)
    {
        std::cout << "Error: sendMessage\n";
        return;
    }
}

void Client::input_thread()
{
    do {
        if (salir) {
            inGame = false;
            sendMessage(GameMessage::SALIR);
            state = Estado::TOCA_ESPERAR; //TODO
        }

        switch(state) {
            case Estado::TOCA_DECIDIR: {
                if (preguntar) {
                    state = Estado::TOCA_ESCRIBIR;
                }
                if (resolver) {
                    if (caraSeleccionada == otherFace) {
                        sendMessage(GameMessage::FIN_PIERDES);
                        resolve(true);
                    }
                    else {
                        sendMessage(GameMessage::FIN_GANAS);
                        resolve(false);
                    }
                    state = Estado::TOCA_ESPERAR;
                }
                break;
            }
            case Estado::TOCA_ESCRIBIR: {
                std::cout << "\n\nESCRIBE TU PREGUNTA " << nick << "\n";

                std::string msg;
                std::getline(std::cin, msg);
                sendMessage(GameMessage::PREGUNTAR, msg);

                state = Estado::TOCA_ESPERAR;
                break;
            }
            case Estado::TOCA_RESPONDER: {
                if (si) {
                    sendMessage(GameMessage::RESPONDER, "SI");
                    state = Estado::TOCA_ESPERAR;
                }
                if (no)
                {
                    sendMessage(GameMessage::RESPONDER, "NO");
                    state = Estado::TOCA_ESPERAR;
                }
                break;
            }
            case Estado::TOCA_PASAR: {
                if (pasar) {
                    sendMessage(GameMessage::PASAR);
                    state = Estado::TOCA_ESPERAR;
                }
                break;
            }
            default:
                break;
        }
    } while(inGame);
}

void Client::net_thread()
{
    do {
        while (inGame)
        {
            GameMessage mensaje; //Recibir Mensajes y en función del tipo de mensaje
            
            int returnCode = socket.recv(mensaje, socket.getSD());
            if (returnCode == -1)
                continue;

            switch (mensaje.type)
            {
            case GameMessage::SALIR: {
                inGame = false;
                break;
            }
            case GameMessage::PREGUNTAR: {
                std::cout << "\nPREGUNTA de " << mensaje.nick << ": \n";
                std::cout << "¿" << mensaje.message << "?\n";
                state = Estado::TOCA_RESPONDER;
                break;
            }
            case GameMessage::RESPONDER: {
                std::cout << "\nRESPUESTA de " << mensaje.nick << ": \n";
                std::cout << mensaje.message << "\n";
                state = Estado::TOCA_PASAR;
                break;
            }
            case GameMessage::PASAR: {
                state = Estado::TOCA_ESCRIBIR;
                break;
            }
            case GameMessage::INICIO: {
                otherFace = mensaje.idFace;
                break;
            }
            case GameMessage::FIN_GANAS: {
                state = Estado::TOCA_ESPERAR;
                resolve(true);
                break;
            }
            case GameMessage::FIN_PIERDES: {
                state = Estado::TOCA_ESPERAR;
                resolve(false);
                break;
            }    
            default:
                break;
            }
        }
    } while (inGame);
}

void Client::sdl_thread()
{
    SDL_Window* window = NULL;
    SDL_Texture* imgFondo = NULL;
    int w, h; // w y h de la imagen

    //Inicializar SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "SDL could not initialize\n");
        return;
    }

    //Crear la ventana
    window = SDL_CreateWindow("¿Quién es quién?", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_SIZE_X, SCREEN_SIZE_Y, SDL_WINDOW_SHOWN);
    if (!window) {
        fprintf(stderr, "Error creating window.\n");
        return;
    }
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    //Crear imagen
    imgFondo = IMG_LoadTexture(renderer, "../media/prueba.jpg");
	SDL_QueryTexture(imgFondo, NULL, NULL, &w, &h);
	SDL_Rect rect;
    rect.x = 0; rect.y = 0;
    rect.w = w/2; rect.h = h/2; 

	//Crear botones
    botonSalir = new Button(20, SCREEN_SIZE_Y - 150, 20, 20, -1, renderer,
        "../media/botonSalir.png", "../media/botonSalir2.png");
    botonSi = new Button(SCREEN_SIZE_X - 240, SCREEN_SIZE_Y - 120, 50, 50, -1, renderer,
        "../media/botonSi.png", "../media/botonSi2.png");
    botonNo = new Button(SCREEN_SIZE_X - 120, SCREEN_SIZE_Y - 120, 50, 50, -1, renderer,
        "../media/botonNo.png", "../media/botonNo2.png");
    botonPasar = new Button(SCREEN_SIZE_X - 150, SCREEN_SIZE_Y - 120, 50, 50, -1, renderer,
        "../media/botonPasar.png", "../media/botonPasar2.png");
    botonResolver = new Button(SCREEN_SIZE_X - 150, SCREEN_SIZE_Y - 120, 50, 50, -1, renderer,
        "../media/botonResolver.png", "../media/botonResolver2.png");
    botonPreguntar = new Button(SCREEN_SIZE_X - 150, SCREEN_SIZE_Y - 120, 50, 50, -1, renderer,
        "../media/botonPregunta.png", "../media/botonPregunta2.png");
        
    // ButtonPermanent botonPermanente(SCREEN_SIZE_X - 200, 0, 100, 100, -1, renderer,
    //     "../media/cara3.png", "../media/cara4.png", "../media/cara5.png");

	//Bucle principal
	while (inGame) {

		SDL_Event e;
		if (SDL_PollEvent(&e)) {
            if(state != Estado::TOCA_ESCRIBIR) {
                if (e.type == SDL_QUIT) salir = true;
                else if (e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_ESCAPE) salir = true;

                salir = botonSalir->handle_events(e);
            }

            if (state == Estado::TOCA_DECIDIR) {
                preguntar = botonPreguntar->handle_events(e);
                resolver = botonResolver->handle_events(e) && caraSeleccionada != -1;
            }
            else if (state == Estado::TOCA_RESPONDER) {
                si = botonSi->handle_events(e);
                no = botonNo->handle_events(e);
            }
            else if(state == Estado::TOCA_PASAR) {
                pasar = botonPasar->handle_events(e);
            }
        }

        //Renderizado
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, imgFondo, NULL, &rect);

        if (state == Estado::TOCA_DECIDIR) {
            botonPreguntar->show();
            botonResolver->show();
            botonSalir->show();
        }
        else if (state == Estado::TOCA_RESPONDER) {
            botonSi->show();
            botonNo->show();
            botonSalir->show();
        }
        else if (state == Estado::TOCA_PASAR) {
            botonPasar->show();
            botonSalir->show();
        }

		SDL_RenderPresent(renderer);
	}
	
    //Destruimos todo al cerrar
    delete botonNo;
    delete botonSi;
    delete botonPasar;
    delete botonResolver;
    delete botonSalir;
    delete botonPreguntar;

    SDL_DestroyTexture(imgFondo);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    std::cout << "babai\n";
}