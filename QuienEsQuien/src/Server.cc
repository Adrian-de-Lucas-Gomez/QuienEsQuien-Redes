#include "Server.h"
#include "Chat.h"

Server::Server(const char * s, const char * p, const char * n) :
    socket(s, p, 0), nick(n), miTurno(true)
{
    std::cout << "Creado servidor en " << s << ":" << p << '\n';

    if (waitForClient() == -1) {
        std::cout << "El cliente no se pudo conectar al servidor\n";
        return;
    }
    std::cout << "Se ha conectado el cliente\n";
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

        if (login.type == ChatMessage::LOGIN)
        {
            std::cout << "Recibido LOGIN del jugador " << login.nick << "\n";
            clients.push_back(std::move(cliente));
            break;
        }
    }
    return client_sd;
}

//Mensjes: Resolver, preguntar, respuesta, pasar turno
//mandar cara(la que tiene el oponente), fin de partida.
void Server::do_messages()  
{
    do
    {
        while (!miTurno)
        {
            //Recibir Mensajes y en función del tipo de mensaje
            ChatMessage mensaje;
            
            int returnCode = socket.recv(mensaje, client_sd);
            if (returnCode == -1)
                continue;

            // - LOGOUT: Eliminar del vector clients
            // - MESSAGE: Reenviar el mensaje a todos los clientes (menos el emisor)
            switch (mensaje.type)
            {
            case 1:
            {
                std::cout << "Recibido MESSAGE de " << mensaje.nick << "\n";
                std::cout << mensaje.nick << " dijo: " << mensaje.message << "\n";
                miTurno = true;
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
    } while (true);
}

void Server::input_thread()
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
            { //TODO
                //Indica que queremos salirnos y se manda LogOut
                //desconectado = true;
                //logout();
            }
            else
            {
                
                //Si no se manda como mensaje normal
                men.type = ChatMessage::MESSAGE;

                int returnCode = socket.send(men, client_sd);
                if (returnCode == -1)
                {
                    std::cout << "Error: message send\n";
                    return;
                }
                miTurno = false; //TODO
            }
            std::cout << "El mensaje se ha mandado\n";
        }

    } while (true);
}