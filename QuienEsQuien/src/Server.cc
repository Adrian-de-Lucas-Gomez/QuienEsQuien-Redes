#include "Server.h"
#include "Message.h"

Server::Server(const char * s, const char * p, const char * n) :
    socket(s, p, 0), nick(n), inGame(false), state(Estado::TOCA_ESPERAR)
{
    srand(time(0));
    std::cout << "Creado servidor en " << s << ":" << p << '\n';

    if (waitForClient() == -1) {
        std::cout << "El cliente no se pudo conectar al servidor\n";
        return;
    }

    std::cout << "Se ha conectado el cliente\n";
    inGame = true;

    chooseFaces();
}

int Server::waitForClient()
{
    client_sd = socket.accept();

    while (true)
    {
        GameMessage login;
        Socket *sock = new Socket(socket);

        int returnCode = socket.recv(login, sock, client_sd);
        if (returnCode == -1)
            continue;

        std::unique_ptr<Socket> cliente(sock);

        if (login.type == GameMessage::NUEVAPARTIDA)
        {
            clients.push_back(std::move(cliente));
            break;
        }
    }
    return client_sd;
}

void Server::chooseFaces()
{
    //Cara servidor
    int randID = rand() % GameMessage::NUM_FACES;
    std::cout << "ID cara server: " << randID << '\n';
    myFace = randID;

    //Para que el cliente conozca nuestra cara internamente
    GameMessage men(nick, "", randID); 
    men.type = GameMessage::INICIO;

    int returnCode = socket.send(men, client_sd);
    if (returnCode == -1)
    {
        std::cout << "Error: face message send\n";
        return;
    }
}

void Server::resolve(int win)
{
    std::cout << "\nSE ACABÓ LA PARTIDA\n";
    if (win)
        std::cout << "¡Enhorabuena! Has ganado" << "\n";
    else
        std::cout << "¡Oh no! Has perdido" << "\n";
}

//Mensjes: Resolver, preguntar, respuesta, pasar turno
//mandar cara(la que tiene el oponente), fin de partida.
void Server::do_messages()  
{
    do
    {
        while (inGame)
        {
            GameMessage mensaje; //Recibir Mensajes y en función del tipo de mensaje
            
            int returnCode = socket.recv(mensaje, client_sd);
            if (returnCode == -1)
                continue;

            switch (mensaje.type)
            {
            case GameMessage::SALIR: {
                inGame = false;
                std::cout << "\nJugador " << mensaje.nick << " ha abandonado la partida\n";
                clients.erase(clients.begin());
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
                state = Estado::TOCA_ESCRIBIR;
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

void Server::input_thread() {
    do {
        switch(state) {
            case Estado::TOCA_ESCRIBIR: {
                std::cout << "\nTE TOCA JUGAR " << nick << "\n";

                std::string msg;
                std::getline(std::cin, msg);
                GameMessage men(nick, msg);

                if (msg == "SALIR") { inGame = false; men.type = GameMessage::SALIR; }
                
                else if(msg == "RESOLVER") {
                    std::cout << "ID de cara [0, 18): ";
                    int aux;
                    std::cin >> aux;

                    //Comprobamos si has acertado
                    if (aux == otherFace) {
                        men.type = GameMessage::FIN_PIERDES;
                        resolve(true);
                    }  
                    else {
                        men.type = GameMessage::FIN_GANAS;
                        resolve(false);
                    }
                }
                else {
                    men.type = GameMessage::PREGUNTAR;
                }
                //Enviar el mensaje
                int returnCode = socket.send(men, client_sd);
                if (returnCode == -1)
                {
                    std::cout << "Error: send in TOCA_ESCRIBIR\n";
                    return;
                }
                state = Estado::TOCA_ESPERAR;
                break;
            }
            case Estado::TOCA_RESPONDER: {
                std::cout << "\nResponde (SI/NO): ";

                // Leer stdin con std::getline
                std::string msg;
                std::getline(std::cin, msg);
                GameMessage men(nick, msg);

                if (msg == "SALIR") { inGame = false; men.type = GameMessage::SALIR; }
                else { men.type = GameMessage::RESPONDER; }
                
                int returnCode = socket.send(men, client_sd);
                if (returnCode == -1)
                {
                    std::cout << "Error: send in TOCA_RESPONDER\n";
                    return;
                }
                state = Estado::TOCA_ESPERAR;
                break;
            }
            case Estado::TOCA_PASAR: {
                std::cout << "\nPulsa ENTER para pasar de turno";

                // Leer stdin con std::getline
                std::string msg;
                std::getline(std::cin, msg);
                GameMessage men(nick, msg);

                if (msg == "SALIR") { inGame = false; men.type = GameMessage::SALIR; }
                else { men.type = GameMessage::PASAR; }

                int returnCode = socket.send(men, client_sd);
                if (returnCode == -1)
                {
                    std::cout << "Error: send in TOCA_PASAR\n";
                    return;
                }
                state = Estado::TOCA_ESPERAR;
                break;
            }
            default:
                break;
        }
    } while(inGame);
}