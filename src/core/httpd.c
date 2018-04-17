#include "httpd.h"
#include "default.h"
#include "file.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <strings.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

void respond(int sockfd, const char *rootdir) {
    /* read bytes into buffer */
    char buff[BUFFER_SIZE];
    int re = read(sockfd, buff, BUFFER_SIZE - 1);
    if(re < 0) {
        perror("ERROR");
        exit(EXIT_FAILURE);
    }

    if(re == 0)
        fprintf(stderr, "WARNING: client disconneted\n");
    else {
        /* set string ending */
        buff[re] = 0;

        /* parse the request */
        char *method = strtok(buff, " "), *uri = strtok(NULL, " "),
             *protocal = strtok(NULL, " \t\r\n");

        /* simple GET method */
        if(strcmp(method, "GET"))
            fprintf(stderr, "WARNING: not a GET method\n");

        /* get the path of the file */
        int lr = strlen(rootdir), lu = strlen(uri);
        char path[lr + lu + 1];
        strcpy(path, rootdir);
        strcpy(path + lr, uri);
        path[lr + lu] = 0;

        /* try to obtain stat without opening the file */
        struct stat fs;
        int wr, lbuff;
        if(stat(path, &fs)) {
            /* output 404 error */
            sprintf(buff, "%s 404 Not Found\n", protocal);
            lbuff = strlen(buff);

            /* handle broken pipe */
            wr = write(sockfd, buff, lbuff);
            if(wr < lbuff - 1)
                fprintf(stderr, "WARNING: client terminated connection\n");
        } else {
            int fd = open(path, O_RDONLY);
#if defined __APPLE__
            sprintf(buff,
                "%s 200 OK\r\nContent-Type: %s\r\nContent-Length: %lld\r\n\r\n",
                protocal, file_MIME(uri), fs.st_size
            );
#elif defined __linux__
            sprintf(buff,
                "%s 200 OK\r\nContent-Type: %s\r\nContent-Length: %ld\r\n\r\n",
                protocal, file_MIME(uri), fs.st_size
            );
#else
#error "not supported"
#endif
            lbuff = strlen(buff);

            /* handle broken pipe */
            wr = write(sockfd, buff, lbuff);
            if(wr < lbuff - 1) {
                fprintf(stderr, "WARNING: client terminated connection\n");
                close(fd);
                return;
            }

            while((re = read(fd, buff, BUFFER_SIZE)) > 0) {
                /* handle broken pipe */
                wr = write(sockfd, buff, re);
                if(wr < re) {
                    fprintf(stderr, "WARNING: client terminated connection\n");
                    close(fd);
                    return;
                }
            }
            close(fd);
        }
    }
}
