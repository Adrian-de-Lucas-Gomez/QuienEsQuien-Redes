#include <thread>
#include "Server.h"

int main(int argc, char **argv)
{
    Server es(argv[1], argv[2], argv[3]);

    std::thread input_thread([&es](){ es.input_thread(); });

    es.do_messages();
    
    input_thread.join();

    std::cout << "Servidor cerrado\n";
    es.closeServer();

    return 0;
}