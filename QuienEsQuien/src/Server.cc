#include "Server.h"
#include "Chat.h"

Server::Server(const char * s, const char * p, const char * n) :
    socket(s, p, 0), nick(n), miTurno(true), inGame(false), tocaResponder(false)
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
        ChatMessage login;
        Socket *sock = new Socket(socket);

        int returnCode = socket.recv(login, sock, client_sd);
        if (returnCode == -1)
            continue;

        std::unique_ptr<Socket> cliente(sock);

        if (login.type == ChatMessage::NUEVAPARTIDA)
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
    int randID = rand() % ChatMessage::NUM_FACES;
    std::cout << "ID cara server: " << randID << '\n';
    myFace = randID;

    //Para que el cliente conozca nuestra cara internamente
    ChatMessage men(nick, "", randID); 
    men.type = ChatMessage::INICIO;

    int returnCode = socket.send(men, client_sd);
    if (returnCode == -1)
    {
        std::cout << "Error: face message send\n";
        return;
    }
}

//Mensjes: Resolver, preguntar, respuesta, pasar turno
//mandar cara(la que tiene el oponente), fin de partida.
void Server::do_messages()  
{
    do
    {
        while (inGame)
        {
            ChatMessage mensaje; //Recibir Mensajes y en función del tipo de mensaje
            
            int returnCode = socket.recv(mensaje, client_sd);
            if (returnCode == -1)
                continue;

            switch (mensaje.type)
            {
            case ChatMessage::SALIR: {
                inGame = false;
                std::cout << "\nJugador " << mensaje.nick << " ha abandonado la partida\n";
                clients.erase(clients.begin());
                break;
            }
            case ChatMessage::PREGUNTAR: {
                std::cout << "\nPREGUNTA de " << mensaje.nick << ": \n";
                std::cout << "¿" << mensaje.message << "?\n";
                tocaResponder = true;
                break;
            }
            case ChatMessage::RESPONDER: {
                std::cout << "\nRESPUESTA de " << mensaje.nick << ": \n";
                std::cout << mensaje.message << "\n";
                break;
            }
            case ChatMessage::PASAR: {
                std::cout << "\nTE TOCA JUGAR " << nick << "\n";
                miTurno = true;
                break;
            }
            case ChatMessage::INICIO: {
                otherFace = mensaje.idFace;
                break;
            }
            case ChatMessage::FIN_GANAS: {
                std::cout << "\nSE ACABÓ LA PARTIDA\n";
                std::cout << "¡Enhorabuena! Has ganado" << "\n";
                break;
            }
            case ChatMessage::FIN_PIERDES: {
                std::cout << "\nSE ACABÓ LA PARTIDA\n";
                std::cout << "¡Oh no! Has perdido" << "\n";
                break;
            }    
            default:
                break;
            }
        }
    } while (inGame);

    std::cout << "Servidor salio de do_messages\n";
}

void Server::input_thread()
{
    do
    {
        if (tocaResponder) {
            std::cout << "Responde (SI/NO): ";

            // Leer stdin con std::getline
            std::string msg;
            std::getline(std::cin, msg);

            ChatMessage men(nick, msg);
            men.type = ChatMessage::RESPONDER;
            int returnCode = socket.send(men, client_sd);
            if (returnCode == -1)
            {
                std::cout << "Error: send\n";
                return;
            }

            tocaResponder = false;
        }
        else {
            std::string msg;
            std::getline(std::cin, msg); //TODO esto se para aqui: cambiar a miTurno

            ChatMessage men(nick, msg); 

            if (msg == "SALIR")
            {
                inGame = false;
                men.type = ChatMessage::SALIR;
                int returnCode = socket.send(men, client_sd);
                if (returnCode == -1)
                {
                    std::cout << "Error: send\n";
                    return;
                }
                break;
            }
            else if (miTurno)
            {
                if(msg == "PASAR") {
                    men.type = ChatMessage::PASAR;
                    miTurno = false;
                }
                else if(msg == "RESOLVER") {
                    std::cout << "ID de cara [0, 18): ";
                    int8_t aux;
                    std::cin >> aux;

                    //Comprobamos si has acertado
                    if (aux == otherFace)
                        men.type = ChatMessage::FIN_PIERDES;
                    else
                        men.type = ChatMessage::FIN_GANAS;
                }
                else {
                    men.type = ChatMessage::PREGUNTAR;
                }

                //Enviar el mensaje
                int returnCode = socket.send(men, client_sd);
                if (returnCode == -1)
                {
                    std::cout << "Error: send\n";
                    return;
                }
            }
            std::cout << "El mensaje se ha mandado\n";
        }

    } while (inGame);

    std::cout << "Servidor sale de input_thread\n";
}