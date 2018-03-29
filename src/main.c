#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "macros.h"
#include "server/server.h"

int main(int argc, char *argv[]) {
    int port = PORT;
    char *path = PATH;

    if(argc == 2 && !strcmp(argv[1], "-h")) {
        printf("Usage: server [port [default: %d]] [path [default: %s]]\n",
            PORT, PATH
        );
        return 0;
    }

    if(argc > 1)
        port = atoi(argv[1]);

    if(argc > 2)
        path = argv[2];

    serve(port, path);

    return 0;
}
