#include "Chat.h"

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

void ChatMessage::to_bin()
{
    alloc_data(MESSAGE_SIZE);

    memset(_data, 0, MESSAGE_SIZE);

    //Serializar los campos type, nick y message en el buffer _data
    char *aux= _data;   //Guardamos el puntero
    
    //Escribimos primero TYPE
        memcpy(aux, &type, sizeof(uint8_t));
        //Movemos el puntero de escritura
        aux += sizeof(uint8_t);

    //Escribimos el nombre del jugador
        memcpy(aux, nick.c_str(), sizeof(char) * 8);  //Nombre de 8 caracteres
        //Movemos el puntero de escritura
        aux += sizeof(char) * 8;

    //Escribimos el mensaje
        memcpy(aux, message.c_str(), sizeof(char)*80 );    //Mensaje de 80 caracteres
        //Movemos el puntero de escritura
        aux += sizeof(char)*80;

}

int ChatMessage::from_bin(char * bobj)
{
    alloc_data(MESSAGE_SIZE);

    memcpy(static_cast<void *>(_data), bobj, MESSAGE_SIZE);

    //Reconstruir la clase usando el buffer _data
    char *aux= _data;   //Guardamos el puntero
    
    //Leemos primero TYPE
        memcpy(&type, aux, sizeof(uint8_t));
        //Movemos el puntero de escritura
        aux += sizeof(uint8_t);

    //Leemos el nombre del jugador
        nick = aux;
        aux += sizeof(char) * 8;

    //Leemos el mensaje
        message = aux;
        aux += sizeof(char)*80;

    return 0;
}