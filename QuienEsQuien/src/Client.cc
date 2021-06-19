#include "Client.h"
#include "Message.h"

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

void Client::input_thread()
{
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
                int returnCode = socket.send(men, socket.getSD());
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
                
                int returnCode = socket.send(men, socket.getSD());
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

                int returnCode = socket.send(men, socket.getSD());
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