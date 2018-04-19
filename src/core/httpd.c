/*
 * Responsible for responding to the client
 * Create by Tingsheng Lai (tingshengl, 781319)
 */

#include "httpd.h"
#include "default.h"
#include "file.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <strings.h>

#include <fcntl.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <unistd.h>

static const char * const HTTP_404 = "HTTP/1.0 404 Not Found\r\n\
Content-Type: text/html\r\n\r\n\
<html><h1>File Not Found<h1></html>";
static const int HTTP_404L = 86;

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
        char *method = strtok(buff, " "), *uri = strtok(NULL, " ");

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
            /* output 404 error and handle broken pipe */
            wr = write(sockfd, HTTP_404, HTTP_404L);
            if(wr < HTTP_404L)
                fprintf(stderr, "WARNING: client terminated connection\n");
        } else {
            int fd = open(path, O_RDONLY);

/*
 * since stat.st_size is defined as "long long" on macOS High Sierra but "long"
 * on Ubuntu Xenial (16.04)
 */
#if defined __APPLE__
            sprintf(buff,
                "HTTP/1.0 200 OK\r\nContent-Type: %s\r\nContent-Length: %lld\r\n\r\n",
                file_MIME(uri), fs.st_size
            );
#elif defined __linux__
            sprintf(buff,
                "HTTP/1.0 200 OK\r\nContent-Type: %s\r\nContent-Length: %ld\r\n\r\n",
                file_MIME(uri), fs.st_size
            );
#else
#error "not supported"
#endif

            lbuff = strlen(buff);

            /* handle broken pipe */
            wr = write(sockfd, buff, lbuff);
            if(wr < lbuff) {
                fprintf(stderr, "WARNING: client terminated connection\n");
                close(fd);
                return;
            }

            do {
                re = sendfile(sockfd, fd, NULL, BUFFER_SIZE);
            } while(re > 0);
            if(re == -1)
                perror("sendfile");
            close(fd);
        }
    }
}
