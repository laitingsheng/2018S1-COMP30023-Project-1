#include "server.h"
#include "httpd.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>

#ifdef _PTHREAD
#include <pthread.h>
#endif

void serve(unsigned int port, const char *path) {
    /* create TCP socket */
    int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(sockfd < 0) {
		perror("ERROR");
        exit(EXIT_FAILURE);
    }

    /* initialise socket structure */
    struct sockaddr_in serv_addr;
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);

    /* bind the host address */
    if(bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR");
        exit(EXIT_FAILURE);
    }

    /* starting listening */
    listen(sockfd, 5);
    struct sockaddr_in cli_addr;
    int newsockfd, clilen = sizeof(cli_addr), pid;
    while(true) {
#ifdef _PTHREAD
        printf("\n");
#else
        /* accept client connection */
        newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
        if(newsockfd < 0) {
            perror("ERROR");
            exit(EXIT_FAILURE);
        }

        /* fork process for multiple connections */
        pid = fork();
        if(pid < 0) {
            perror("ERROR");
            exit(EXIT_FAILURE);
        }
        if(pid)
            close(newsockfd);
        else {
            close(sockfd);
            respond(newsockfd, path);
            _exit(EXIT_SUCCESS);
        }
#endif
    }
}
