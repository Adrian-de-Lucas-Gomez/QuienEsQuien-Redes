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
    SDL_Texture* imgCara = NULL;
    SDL_Texture* imgTexto = NULL;

    //Inicializar SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "SDL could not initialize\n");
        return;
    }

    //Crear la ventana
    std::string windowName = "¿Quién es quién? : " + nick;
    window = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_SIZE_X, SCREEN_SIZE_Y, SDL_WINDOW_SHOWN);
    if (!window) {
        fprintf(stderr, "Error creating window.\n");
        return;
    }
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    //+++++++++++++++++++++++++Recursos++++++++++++++++++++++++++++++++

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

    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

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

            for (int i = 0; i < botonCaras.capacity(); ++i)
            {
                if(botonCaras[i]->handle_leftClick(e)) {
                    if(caraSeleccionada == botonCaras[i]->getID())
                        caraSeleccionada = -1;
                    else if(caraSeleccionada == -1)
                        caraSeleccionada = botonCaras[i]->getID();
                    else {
                        botonCaras[caraSeleccionada]->setNormal();
                        caraSeleccionada = botonCaras[i]->getID();
                    }
                }
                botonCaras[i]->handle_rightClick(e);
            }
        }

        //Renderizado
		SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, imgFondo, NULL, &rectFondo);
		SDL_RenderCopy(renderer, imgCara, NULL, &rectCara);

        if (state == Estado::TOCA_DECIDIR) {
            botonPreguntar->show();
            botonResolver->show();
            botonSalir->show();
        }
        else if (state == Estado::TOCA_ESCRIBIR) {
            SDL_RenderCopy(renderer, imgTexto, NULL, &rectTexto);
        }
        else if (state == Estado::TOCA_RESPONDER) {
            SDL_RenderCopy(renderer, imgTexto, NULL, &rectTexto);
            botonSi->show();
            botonNo->show();
            botonSalir->show();
        }
        else if (state == Estado::TOCA_PASAR) {
            botonPasar->show();
            botonSalir->show();
        }

        //Las caras se muestran siempre
        for(int i = 0; i < botonCaras.capacity(); ++i){
            botonCaras[i]->show();
        }

		SDL_RenderPresent(renderer);
	}
	
    //Destruimos todo al acabar
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
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    std::cout << "babai\n";
}