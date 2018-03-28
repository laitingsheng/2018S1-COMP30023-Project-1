#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "macros.h"

int main(int argc, char *argv[]) {
    unsigned int port = PORT;
    char *path = PATH;

    if(argc > 1)
        port = atol(argv[1]);

    if(argc > 2)
        path = argv[2];

    return 0;
}
