#include "httpd.h"
#include "../macros/default.h"
#include "../macros/MIME.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void respond(int sockfd, const char *rootdir) {
    /* read bytes into buffer */
    char *buff = malloc(BUFFER_SIZE);
    int re = read(sockfd, buff, BUFFER_SIZE - 1);
    if(re < 0) {
        perror("ERROR: cannot read from socket");
        free(buff);
        exit(EXIT_FAILURE);
    }

    if(re == 0)
        fprintf(stderr, "WARNING: client disconneted");
    else {
        /* set string ending */
        buff[re] = 0;

        /* parse the request */
        char *method = strtok(buff, " "), *uri = strtok(NULL, " ");

        /* simple GET method */
        if(strcmp(method, "GET")) {
            fprintf(stderr, "WARNING: not a GET method\n");

            /* send the index.html instead */
            uri = "/index.html";
        }

        /* get the path of the file */
        int lr = strlen(rootdir);
        strcpy(buff, rootdir);
        strcpy(buff + lr, uri);
        path[lr + strlen(uri)] = 0;

        /* try to open the file */
        struct FILE *f = fopen(buff, "r");
        if(!f) {
            perror("ERROR:, cannot open file %d\n", buff);

            /* output 404 error */
            write(sockfd, "HTTP/1.0 404 Not Found\n", 23);
            free(buff);
            exit(EXIT_FAILURE);
        }

        write(sockfd, "HTTP/1.0 200 OK", 19);
        while((re = fread(buff, 1, BUFFER_SIZE, f)) != EOF)
            write(sockfd, buff, re);
    }

    free(buff);
    exit(EXIT_SUCCESS);
}
