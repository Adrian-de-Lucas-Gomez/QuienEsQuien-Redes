#include "Server.h"
#include "Chat.h"

Server::Server(const char * s, const char * p) : socket(s, p, 0)
{
    std::cout << "Creado servidor en " << s << ":" << p << '\n';

    int aux = socket.accept();
    if (aux != 0) {
        std::cout << "El cliente no se pudo conectar al servidor\n";
        return;
    }
    std::cout << "El cliente se pudo conectar al servidor UWU\n";
}

void Server::do_messages()
{
    while (true)
    {
        //Recibir Mensajes y en función del tipo de mensaje
        ChatMessage mensaje;
        Socket *sock = new Socket(socket);
        //std::cout << "A\n";

        int returnCode = socket.recv(mensaje, sock);
        if(returnCode == -1)
            continue;

        //std::cout << "B\n";

        std::unique_ptr<Socket> cliente(sock);

        // - LOGIN: Añadir al vector clients
        // - LOGOUT: Eliminar del vector clients
        // - MESSAGE: Reenviar el mensaje a todos los clientes (menos el emisor)
        switch (mensaje.type)
        {
        case 0:
        {
            std::cout << "Recibido LOGIN del jugador " << mensaje.nick << "\n";
            clients.push_back(std::move(cliente));
            break;
        }
        case 1:
        {
            std::cout << "Recibido MESSAGE de " << mensaje.nick << "\n";
            socket.send(mensaje);
            // for (int i = 0; i < clients.size(); i++)
            // {
            //     Socket aux = *clients[i].get();

            //     if (!(aux == *cliente))
            //     { //Si no es la misma persona se lo reenvio
            //         socket.send(mensaje);
            //     }
            // }
            break;
        }
        case 2:
        {
            std::cout << "LOGOUT del jugador " << mensaje.nick << "\n";
            clients.erase(clients.begin());
            break;
        }
        default:
            break;
        }
    }
}