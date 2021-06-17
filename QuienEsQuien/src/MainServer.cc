#include "Server.h"

int main(int argc, char **argv)
{
    Server es(argv[1], argv[2]);

    es.do_messages();

    es.closeServer();

    return 0;
}