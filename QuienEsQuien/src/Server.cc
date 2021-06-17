#include "Server.h"
#include "Chat.h"

void Server::do_messages()
{
    while (true)
    {
        /*
         * NOTA: los clientes están definidos con "smart pointers", es necesario
         * crear un unique_ptr con el objeto socket recibido y usar std::move
         * para añadirlo al vector
         */

        //Recibir Mensajes en y en función del tipo de mensaje
        ChatMessage mensaje;
        Socket *sock= &socket;

        int aux = socket.recv(mensaje, sock);

        if(aux != -1)
        {
            std::unique_ptr<Socket> cliente(sock);

            // - LOGIN: Añadir al vector clients
            // - LOGOUT: Eliminar del vector clients
            // - MESSAGE: Reenviar el mensaje a todos los clientes (menos el emisor)
            switch (mensaje.type)
            {
                case 0:
                {
                    std::cout << "Recibido LOGGIN de " << mensaje.nick << "\n";
                    clients.push_back(std::move(cliente));
                    break;
                }
                case 1:
                {
                    std::cout << "Recibido MESSAGE de " << mensaje.nick << "\n";
                    for (int i = 0; i < clients.size(); i++)
                    {
                        Socket aux = *clients[i].get();

                        if (!(aux == *cliente))
                        { //Si no es la misma persona se lo reenvio
                            socket.send(mensaje, aux);
                        }
                    }
                    break;
                }
                case 2:
                {
                    std::cout << "LOGOUT de " << mensaje.nick << "\n";

                    bool desconexionRealizada = false;
                    int i = 0;

                    while (!desconexionRealizada && i < clients.size())
                    {
                        Socket aux = *clients[i].get();

                        if (aux == *sock)
                        { //Si no es la misma persona se lo reenvio
                            socket.send(mensaje, aux);
                            clients.erase(clients.begin() + i);
                            desconexionRealizada = true;
                        }

                        i++;
                    }
                    break;
                }
                default:
                    break;
            }
        }
    }
}