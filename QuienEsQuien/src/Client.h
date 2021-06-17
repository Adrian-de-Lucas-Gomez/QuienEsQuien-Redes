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
    bool desconectado=false;

    /**
     * @param s dirección del servidor
     * @param p puerto del servidor
     * @param n nick del usuario
     */
    Client(const char * s, const char * p, const char * n) : socket(s, p, 1), nick(n) {};

    //Envía el mensaje de login al servidor
    void login();

    //Envía el mensaje de logout al servidor
    void logout();

    //Rutina principal para el Thread de E/S. Lee datos de STDIN (std::getline)
    //y los envía por red vía el Socket.
    void input_thread();

    //Rutina del thread de Red. Recibe datos de la red y los "renderiza" en STDOUT
    void net_thread();

    void closeClient() { close(socket.getSD()); }

private:
    Socket socket;
    std::string nick;
};