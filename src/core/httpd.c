#include "httpd.h"
#include "default.h"
#include "file.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h>
#include <unistd.h>

void respond(int sockfd, const char *rootdir) {
    /* read bytes into buffer */
    char *buff = malloc(BUFFER_SIZE);
    int re = read(sockfd, buff, BUFFER_SIZE - 1);
    if(re < 0) {
        perror("ERROR");
        free(buff);
        exit(EXIT_FAILURE);
    }

    if(re == 0)
        fprintf(stderr, "WARNING: client disconneted\n");
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
        int lr = strlen(rootdir), lu = strlen(uri);
        char path[lr + lu + 1];
        strcpy(path, rootdir);
        strcpy(path + lr, uri);
        path[lr + lu] = 0;

        /* try to open the file */
        int fd = open(path, O_RDONLY);
        if(fd < 0)
            /* output 404 error */
            write(sockfd, "HTTP/1.0 404 Not Found\n", 23);
        else {
            sprintf(buff, "HTTP/1.0 200 OK\nContent-Type: %s\n\n",
                    file_MIME(uri));
            write(sockfd, buff, strlen(buff));
            while((re = read(fd, buff, BUFFER_SIZE)) > 0)
                write(sockfd, buff, re);
            close(fd);
        }
    }

    free(buff);
    exit(EXIT_SUCCESS);
}
