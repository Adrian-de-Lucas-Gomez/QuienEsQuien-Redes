#include <string.h>

#include "Serializable.h"
#include "Socket.h"

Socket::Socket(const char *address, const char *port, const int type) : sd(-1)
{
    //Construir un socket de tipo AF_INET y SOCK_STREAM usando getaddrinfo.
    //Con el resultado inicializar los miembros sd, sa y sa_len de la clase
    struct addrinfo infoaddres;
    struct addrinfo *sockaddr;

    memset((void*) &infoaddres, 0, sizeof(struct addrinfo));    //Reservamos memoria

    infoaddres.ai_family = AF_INET; //Mira para IPv4
    infoaddres.ai_socktype = SOCK_STREAM; //TCP

    int info = getaddrinfo(address, port, &infoaddres, &sockaddr);

    if(info != 0)   //Ha habido un error y acaba el programa
    { 
        std::cout << "Error: Servicio desconocido\n";
        return;
    }

    if(type == 0) //Si esta en modo servidor abre las conexiones
    {
        sd = socket(sockaddr->ai_family, sockaddr->ai_socktype, 0);
        if (sd == -1)
        {
            std::cerr << "Error: socket\n";
            return;
        }

        int returnCode = bind(sd, sockaddr->ai_addr, sockaddr->ai_addrlen);
        if (returnCode == -1)
        {
            std::cerr << "Error: bind\n";
            return;
        }

        returnCode = listen(sd, 16);
        if (returnCode == -1)
        {
            std::cerr << "Error: listen\n";
            return;
        }
    }
    else  //Si esta en en modo cliente se trata de conectar
    {
        sd = socket(sockaddr->ai_family, sockaddr->ai_socktype, 0);
        if (sd == -1)
        {
            std::cout << "Error: socket\n";
            return;
        }

        int returnCode = connect(sd, sockaddr->ai_addr, sockaddr->ai_addrlen);
        if (returnCode == -1)
        {
            std::cout << "Error: connect\n";
            return;
        }
    }
    
    //freeaddrinfo(sockaddr);

    //Guardamos la información de la dirección
    sa = *sockaddr->ai_addr;
    sa_len = sockaddr->ai_addrlen;
}

Socket::Socket(struct sockaddr * _sa, socklen_t _sa_len):sd(-1), sa(*_sa), sa_len(_sa_len)
{
    sd = socket(AF_INET, SOCK_STREAM, 0);
}

int Socket::recv(Serializable &obj, Socket * &sock, int sdOther)
{
    // struct sockaddr sa;
    // socklen_t sa_len = sizeof(struct sockaddr);

    char buffer[MAX_MESSAGE_SIZE];

    ssize_t bytes = ::recv(sdOther, buffer, MAX_MESSAGE_SIZE, 0);

    if ( bytes <= 0 )
    {
        return -1;
    }

    if ( sock != 0 )
    {
        sock = new Socket(&sa, sa_len);
    }

    obj.from_bin(buffer);

    return 0;
}

int Socket::send(Serializable& obj, int sdOther) //, const Socket& sock
{
    //Serializar el objeto
    obj.to_bin();   //Llamamos al metodo de serializacion del objeto a mandar
    //Enviar el objeto binario a sock usando el socket sd
    int envioMensaje = ::send(sdOther, obj.data(), obj.size(), 0);

    if(envioMensaje < 0 )
        std::cerr << "Error: send " << strerror(errno) << "\n";

	return envioMensaje;
}

int Socket::accept()
{
    char host[NI_MAXHOST];
	char serv[NI_MAXSERV];
    struct sockaddr cliente;
	socklen_t clienteLen = sizeof(struct sockaddr);

	int client_sd = ::accept(sd, &cliente, &clienteLen);
    if (client_sd == -1) {
        std::cerr << "Error: accept " << strerror(errno) << "\n";
        return -1;
    }

    getnameinfo(&cliente, clienteLen, host, NI_MAXHOST, serv, NI_MAXSERV,
            NI_NUMERICHOST | NI_NUMERICSERV);

	std::cout << "Conexión desde " << host << " " << serv << std::endl;

    return client_sd;
}

bool operator== (const Socket &s1, const Socket &s2)
{
    //Comparar los campos sin_family, sin_addr.s_addr y sin_port
    //de la estructura sockaddr_in de los Sockets s1 y s2
    //Retornar false si alguno difiere

    struct sockaddr_in* addrIn_Socket1 = (struct sockaddr_in*)&(s1.sa);
    struct sockaddr_in* addrIn_Socket2 = (struct sockaddr_in*)&(s2.sa);

    //Miramos si los valores son iguales para ver si los sockets son iguales;
    return(addrIn_Socket1->sin_family == addrIn_Socket2->sin_family && addrIn_Socket1->sin_port == addrIn_Socket2->sin_port &&
    addrIn_Socket1->sin_addr.s_addr == addrIn_Socket2->sin_addr.s_addr);
};

std::ostream& operator<<(std::ostream& os, const Socket& s)
{
    char host[NI_MAXHOST];
    char serv[NI_MAXSERV];

    getnameinfo((struct sockaddr *) &(s.sa), s.sa_len, host, NI_MAXHOST, serv,
                NI_MAXSERV, NI_NUMERICHOST);

    os << host << ":" << serv;

    return os;
};