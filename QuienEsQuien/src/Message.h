#include <string>
#include <unistd.h>
#include <string.h>
#include <vector>
#include <memory>

#include "Serializable.h"
#include "Socket.h"

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

/**
 *  Mensaje del protocolo de la aplicación de Chat
 *
 *  +-------------------+
 *  | Tipo: uint8_t     | 0 (NUEVAPARTIDA), 1 (SALIR), 2 (PREGUNTAR), 3(RESPONDER), 4(PASAR), 5 (INICIO), 6 (FIN_GANAS), 7 (FIN_PIERDES)
 *  +-------------------+
 *  | IDface: int       | Número que indica una cara en el juego 
 *  +-------------------+
 *  | Nick: char[8]     | Nick incluido el char terminación de cadena '\0'
 *  +-------------------+
 *  |                   |
 *  | Mensaje: char[80] | Mensaje incluido el char terminación de cadena '\0'
 *  |                   |
 *  +-------------------+
 *
 */
class GameMessage: public Serializable
{
public:
    static const size_t MESSAGE_SIZE = sizeof(char) * 88 + sizeof(uint8_t) + sizeof(int);
    static const uint8_t NUM_FACES = 18;

    enum MessageType
    {
        NUEVAPARTIDA = 0, //Se une un jugador
        SALIR  = 1, //Cerrar la conexión (volver al menú)
        PREGUNTAR = 2,  //Manda mensaje para preguntar sobre el personaje enemigo
        RESPONDER = 3,  //SI o NO
        PASAR = 4,      //Acabar el turno
        INICIO = 5,     //Manda cara propia
        FIN_GANAS = 6,  //Se manda al jugador que ha ganado
        FIN_PIERDES = 7 //Se manda al jugador que ha perdido
    };

    GameMessage(){};
    GameMessage(const std::string& n, const std::string& m = "", const int& face = -1) :
        nick(n), message(m), idFace(face) {};

    void to_bin();
    int from_bin(char * bobj);

    uint8_t type = -1;
    int idFace = -1;
    std::string nick;
    std::string message;
};