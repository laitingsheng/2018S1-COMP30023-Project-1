#include "server.h"
#include "httpd.h"

#include <stdio.h>
#include <stdbool.h>

#include <netdb.h>
#include <netinet/in.h>

void serve(unsigned int port, const char *path) {
    /* create TCP socket */
    int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(sockfd < 0) {
		perror("ERROR: cannot open socket");
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
        perror("ERROR: cannot bind");
        exit(EXIT_FAILURE);
    }

    /* starting listening */
    listen(sockfd, 5);
    struct sockaddr_in cli_addr;
    int newsockfd, clilen = sizeof(cli_addr), pid;
    while(true) {
        /* accept client connection */
        newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
        if(newsockfd < 0) {
            perror("ERROR: cannot accept");
            exit(EXIT_FAILURE);
        }

        /* fork process for multiple connections */
        pid = fork();
        if(pid < 0) {
            perror("Error: cannot fork");
            exit(EXIT_FAILURE);
        }
        if(pid)
            close(newsockfd);
        else {
            close(sockfd);
            respond(newsockfd, path);
            exit(EXIT_SUCCESS);
        }
    }
}
