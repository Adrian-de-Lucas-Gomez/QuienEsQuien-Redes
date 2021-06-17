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
    Server(const char * s, const char * p);

    //Thread principal del servidor: recive mensajes en el socket y
    //lo distribuye a los clientes. Mantiene actualizada la lista de clientes
    void do_messages();

private:
    std::vector<std::unique_ptr<Socket>> clients;
    Socket socket;
};