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

class Client
{
public:
    enum Estado {
        TOCA_DECIDIR = 0,
        TOCA_ESCRIBIR = 1,
        TOCA_RESPONDER = 2,
        TOCA_PASAR = 3,
        TOCA_ESPERAR = 4,
        TOCA_CELEBRAR = 5,
        TOCA_LLORAR = 6
    };

    Client(const char * s, const char * p, const char * n) :
        socket(s, p, 1), nick(n), inGame(true), state(Estado::TOCA_ESPERAR),
        caraSeleccionada(-1)
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
    void sendMessage(uint8_t messageType, std::string message = ""); 

    Socket socket;
    std::string nick;

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

    std::vector<ButtonPermanent*> botonCaras;

    int caraSeleccionada;
};