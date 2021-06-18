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
 *  | Tipo: uint8_t     | 0 (login), 1 (mensaje), 2 (logout)
 *  +-------------------+
 *  | Nick: char[8]     | Nick incluido el char terminación de cadena '\0'
 *  +-------------------+
 *  |                   |
 *  | Mensaje: char[80] | Mensaje incluido el char terminación de cadena '\0'
 *  |                   |
 *  +-------------------+
 *
 */
class ChatMessage: public Serializable
{
public:
    static const size_t MESSAGE_SIZE = sizeof(char) * 88 + sizeof(uint8_t);

    enum MessageType
    {
        LOGIN   = 0,
        MESSAGE = 1,
        LOGOUT  = 2,

        //Mensajes exclusivos del QuienEsQuien
        PREGUNTAR = 3,  //Manda mensaje para preguntar sobre el personaje enemigo
        RESOLVER = 4,   //Mandar ID de la cara que crea oportuna
        RESPONDER = 5,  //SI o NO
        PASAR = 6,      //Acabar el turno
        INICIO = 7,     //Manda cara propia
        FIN = 8,        //Se manda al acertar o fallar al resolver
    };

    ChatMessage(){};

    ChatMessage(const std::string& n, const std::string& m):nick(n),message(m){};

    void to_bin();

    int from_bin(char * bobj);

    uint8_t type= -1;

    std::string nick;
    std::string message;
};