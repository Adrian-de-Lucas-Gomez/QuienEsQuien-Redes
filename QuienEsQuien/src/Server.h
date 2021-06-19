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
    enum Estado {
        TOCA_ESCRIBIR = 0,
        TOCA_RESPONDER = 1,
        TOCA_PASAR = 2,
        TOCA_ESPERAR = 3
    };

    Server(const char * s, const char * p, const char * n);

    //Thread principal del servidor: recive mensajes en el socket y
    //lo distribuye a los clientes. Mantiene actualizada la lista de clientes
    void do_messages();

    void input_thread();

    void closeServer() { close(socket.getSD()); }

private:
    int waitForClient();
    void chooseFaces();
    void resolve(int win);

    int client_sd;
    std::string nick;
    std::vector<std::unique_ptr<Socket>> clients;
    Socket socket;

    bool inGame;
    uint8_t state;
    int myFace;
    int otherFace;
};