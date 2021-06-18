#include <string>
#include <unistd.h>
#include <string.h>
#include <vector>
#include <memory>

#include "Socket.h"

/**
 *  Clase para el servidor de chat
 */
class Server
{
public:
    Server(const char * s, const char * p, const char * n);

    //Thread principal del servidor: recive mensajes en el socket y
    //lo distribuye a los clientes. Mantiene actualizada la lista de clientes
    void do_messages();

    void input_thread();

    void closeServer() { close(socket.getSD()); }

private:
    int waitForClient();
    void chooseFaces();

    bool inGame;
    bool tocaResponder;
    bool miTurno;
    int client_sd;
    std::string nick;
    std::vector<std::unique_ptr<Socket>> clients;
    Socket socket;

    int8_t myFace;
    int8_t otherFace;
};