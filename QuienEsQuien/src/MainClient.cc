#include <thread>
#include "Client.h"

int main(int argc, char **argv)
{
    Client ec(argv[1], argv[2], argv[3]);

    std::thread net_thread([&ec](){ ec.net_thread(); });

    ec.login();

    ec.input_thread();

    //Para evitar errores nos aseguramos que se una la thread
    net_thread.join();

    std::cout << "Cliente cerrado\n";
    ec.closeClient();

    return 0;
}