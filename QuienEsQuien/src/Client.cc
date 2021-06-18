#include "Client.h"
#include "Chat.h"

void Client::login()
{
    std::string msg;

    ChatMessage em(nick, msg);
    em.type = ChatMessage::NUEVAPARTIDA;

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

    ChatMessage out(nick, msgLogOut);
    out.type = ChatMessage::SALIR;

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
    int randID = rand() % ChatMessage::NUM_FACES;
    std::cout << "ID cara cliente: " << randID << '\n';
    myFace = randID;

    //Para que el servidor conozca nuestra cara internamente
    ChatMessage men(nick, "", randID); 
    men.type = ChatMessage::INICIO;

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
    do
    {
        if (tocaResponder) {
            std::cout << "Responde (SI/NO): ";

            // Leer stdin con std::getline
            std::string msg;
            std::getline(std::cin, msg);

            ChatMessage men(nick, msg);
            men.type = ChatMessage::RESPONDER;
            int returnCode = socket.send(men, socket.getSD());
            if (returnCode == -1)
            {
                std::cout << "Error: send\n";
                return;
            }
            tocaResponder = false;
        }
        else if (miTurno)
        {
            std::string msg;
            std::getline(std::cin, msg);
            ChatMessage men(nick, msg);

            if (msg == "SALIR")
            {
                logout();
                break;
            }
            else if(msg == "PASAR") {
                men.type = ChatMessage::PASAR;
                miTurno = false;
            }
            else if(msg == "RESOLVER") {
                std::cout << "ID de cara [0, 18): ";
                int8_t aux;
                std::cin >> aux;

                //Comprobamos si has acertado
                if (aux == otherFace) {
                    men.type = ChatMessage::FIN_PIERDES;
                    resolve(true);
                }
                else {
                    men.type = ChatMessage::FIN_GANAS;
                    resolve(false);
                }
                miTurno = false;           
            }
            else {
                men.type = ChatMessage::PREGUNTAR;
            }

            //Enviar el mensaje
            int returnCode = socket.send(men, socket.getSD());
            if (returnCode == -1)
            {
                std::cout << "Error: send\n";
                return;
            }
            std::cout << "El mensaje se ha mandado\n";
        }
    } while (inGame);

    std::cout << "Cliente sale de input_thread\n";
}

void Client::net_thread()
{
    do
    {
        while (inGame)
        {
            ChatMessage mensaje; //Recibir Mensajes y en función del tipo de mensaje
            
            int returnCode = socket.recv(mensaje, socket.getSD());
            if (returnCode == -1)
                continue;

            switch (mensaje.type)
            {
            case ChatMessage::SALIR: {
                inGame = false;
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
                resolve(true);
                break;
            }
            case ChatMessage::FIN_PIERDES: {
                resolve(false);
                break;
            }    
            default:
                break;
            }
        }
    } while (inGame);

    std::cout << "Cliente sale de net_thread\n";
}