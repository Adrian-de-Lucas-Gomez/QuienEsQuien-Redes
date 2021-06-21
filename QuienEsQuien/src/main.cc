#include <thread>
#include <climits>
#include "Server.h"
#include "Client.h"

void beServer(char **argv) {
    Server es(argv[1], argv[2], argv[3]);

    std::thread net_thread([&es](){ es.net_thread(); });
    std::thread input_thread([&es](){ es.input_thread(); });

    es.sdl_thread();

    net_thread.join();
    input_thread.join();

    std::cout << "Servidor cerrado\n";
    es.closeServer();
}

void beClient(char **argv) {
    Client ec(argv[1], argv[2], argv[3]);

    std::thread net_thread([&ec](){ ec.net_thread(); });
    std::thread input_thread([&ec](){ ec.input_thread(); });

    ec.login();
    ec.sdl_thread();

    net_thread.join();
    input_thread.join();

    std::cout << "Cliente cerrado\n";
    ec.closeClient();
}

int main(int argc, char **argv)
{
    if (argc < 3) {
        std::cout << "Argumentos: [direccion IP] [puerto] [usuario]\n";
        return 0;
    }

    std::cout << "Elige una opción:\n";
    std::cout << "0: Crear partida\n1: Unirse a partida\n";

    int aux;
    std::cin >> aux;
    if (aux == 0) { //Servidor
        std::cin.ignore();
        beServer(argv);
    }
    else if (aux == 1) { //Cliente
        std::cin.ignore();
        beClient(argv);
    }
    else {
        std::cout << "Opción inválida. Cerrando programa\n";
        return 0;
    }

    return 0;
}