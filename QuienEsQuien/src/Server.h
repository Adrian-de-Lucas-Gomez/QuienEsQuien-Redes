#include <string>
#include <unistd.h>
#include <string.h>
#include <vector>
#include <memory>

#include "Socket.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class ButtonPermanent;
class Button;

class Server
{
public:
    enum Estado {
        TOCA_DECIDIR = 0,
        TOCA_ESCRIBIR = 1,
        TOCA_RESPONDER = 2,
        TOCA_PASAR = 3,
        TOCA_ESPERAR = 4
    };

    Server(const char * s, const char * p, const char * n);

    //Thread principal del servidor: recive mensajes en el socket y
    //lo distribuye a los clientes. Mantiene actualizada la lista de clientes
    void net_thread();

    void input_thread();

    void sdl_thread();

    void closeServer() { close(socket.getSD()); }

private:
    int waitForClient();
    void chooseFaces();
    void resolve(int win);
    void sendMessage(uint8_t messageType, std::string message = "");

    int client_sd;
    std::string nick;
    std::vector<std::unique_ptr<Socket>> clients;
    Socket socket;

    bool inGame;
    uint8_t state;
    int myFace;
    int otherFace;

    //Variables SDL
    SDL_Renderer* renderer = NULL;
    Button* botonSalir;     bool salir = false;
    Button* botonSi;        bool si = false;
    Button* botonNo;        bool no = false;
    Button* botonPasar;     bool pasar = false;
    Button* botonResolver;  bool resolver = false;
    Button* botonPreguntar; bool preguntar = false;
    int caraSeleccionada;
};