#include "Client.h"
#include "Chat.h"

void Client::login()
{
    std::string msg;

    ChatMessage em(nick, msg);
    em.type = ChatMessage::LOGIN;

    socket.send(em, socket);
}

void Client::logout()
{
    std::string msgLogOut;

    ChatMessage out(nick, msgLogOut);
    out.type = ChatMessage::LOGOUT;

    socket.send(out, socket);
}

void Client::input_thread()
{
    while (!desconectado)
    {
        // Leer stdin con std::getline
        // Enviar al servidor usando socket
        std::string msg;
        std::getline(std::cin,msg);

        ChatMessage men(nick, msg);

        if(msg == "LogOut"){
            //Indica que queremos salirnos y se manda LogOut
            men.type = ChatMessage::LOGOUT;
            desconectado = true;
        }
        else{
            //Si no se manda como mensaje normal
            men.type = ChatMessage::MESSAGE;
        }
        
        socket.send(men, socket);
        std::cout << "El mensaje se ha mandado\n";
    }
}

void Client::net_thread()
{
    while(!desconectado)
    {
        ChatMessage recibido;
        //Recibir Mensajes de red
        socket.recv(recibido);

        if(recibido.nick != nick){   //SI es mi mensaje se ignora
            //Mostrar en pantalla el mensaje de la forma "nick: mensaje"
            std::cout << recibido.nick << " dijo: " << recibido.message << "\n";
        }
    }
}