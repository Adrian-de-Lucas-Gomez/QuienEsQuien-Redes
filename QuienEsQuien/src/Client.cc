#include "Client.h"
#include "Chat.h"

void Client::login()
{
    std::string msg;

    ChatMessage em(nick, msg);
    em.type = ChatMessage::LOGIN;

    int returnCode = socket.send(em, socket.getSD());
    if (returnCode == -1) {
        std::cout << "Error: login send\n";
        return;
    }
}

void Client::logout()
{
    std::string msgLogOut;

    ChatMessage out(nick, msgLogOut);
    out.type = ChatMessage::LOGOUT;

    int returnCode = socket.send(out, socket.getSD());
    if (returnCode == -1) {
        std::cout << "Error: logout send\n";
        return;
    }
}

void Client::input_thread()
{
    do
    {
        while (miTurno)
        {
            // Leer stdin con std::getline
            // Enviar al servidor usando socket
            std::string msg;
            std::getline(std::cin, msg);

            ChatMessage men(nick, msg);

            if (msg == "LOGOUT")
            {
                //Indica que queremos salirnos y se manda LogOut
                desconectado = true;
                logout();
            }
            else
            {
                //Si no se manda como mensaje normal
                men.type = ChatMessage::MESSAGE;

                int returnCode = socket.send(men, socket.getSD());
                if (returnCode == -1)
                {
                    std::cout << "Error: message send\n";
                    return;
                }

                miTurno = false; //TODO
            }
            std::cout << "El mensaje se ha mandado\n";
        }
    } while (!desconectado);
}

void Client::net_thread()
{
    do
    {
        while (!miTurno)
        {
            ChatMessage recibido;

            //Recibir Mensajes de red
            int returnCode = socket.recv(recibido, socket.getSD());
            if (returnCode == -1)
                continue;

            if (recibido.type == ChatMessage::MESSAGE)
            {
                miTurno = true; //TODO

                //Mostrar en pantalla el mensaje de la forma "nick: mensaje"
                std::cout << recibido.nick << " dijo: " << recibido.message << "\n";
            }
        }
    } while (!desconectado);
}