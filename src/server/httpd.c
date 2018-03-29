#include "httpd.h"
#include "macros.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void respond(int sockfd, char *rootdir) {
    /* read bytes into buffer */
    char *buff = malloc(BUFFER_SIZE + 1);
    int re = read(sockfd, buff, BUFFER_SIZE);
    if(re < 0)
        perror("ERROR: cannot read from socket");
    else if(re == 0)
        fprintf(stderr, "WARNING: client disconneted");
    else {
        /* set string ending */
        buff[re] = 0;

        char *method = strtok(buff, " \t\r\n"), *uri = strtok(NULL, " \t"),
             *protocal = strtok(NULL, " \t\r\n");
    }

    free(buff);
}
