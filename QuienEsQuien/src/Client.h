#include <string>
#include <unistd.h>
#include <string.h>
#include <vector>
#include <memory>

#include "Socket.h"

/**
 *  Clase para el cliente
 */
class Client
{
public:
    enum Estado {
        TOCA_ESCRIBIR = 0,
        TOCA_RESPONDER = 1,
        TOCA_PASAR = 2,
        TOCA_ESPERAR = 3
    };

    Client(const char * s, const char * p, const char * n) :
        socket(s, p, 1), nick(n), inGame(true), state(Estado::TOCA_ESPERAR)
    {
        srand(time(0));
    };

    //Envía el mensaje de login al servidor
    void login();

    //Envía el mensaje de logout al servidor
    void logout();

    //Rutina principal para el Thread de E/S. Lee datos de STDIN (std::getline)
    //y los envía por red vía el Socket.
    void input_thread();

    //Rutina del thread de Red. Recibe datos de la red y los "renderiza" en STDOUT
    void net_thread();

    void sdl_thread();

    void closeClient() { close(socket.getSD()); }

private:
    void chooseFaces();
    void resolve(bool win);

    Socket socket;
    std::string nick;

    bool inGame;
    uint8_t state;
    int myFace;
    int otherFace;
};