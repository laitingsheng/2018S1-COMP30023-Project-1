#include "server.h"
#include "httpd.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <unistd.h>

typedef struct {
    const char * path;
    int clifd;
} serve_thread_args_t;

static void *serve_thread(void *arg) {
    serve_thread_args_t *args = arg;
    respond(args->clifd, args->path);
    close(args->clifd);
    free(arg);
    return NULL;
}

void serve(unsigned int port, const char *path) {
    /* create TCP socket */
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if(listenfd < 0) {
		perror("socket");
        exit(EXIT_FAILURE);
    }

    /* initialise listening socket */
    struct sockaddr_in serv_addr;
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);

    /* bind the host address */
    if(bind(listenfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    /* start listening */
    listen(listenfd, 5);
    struct sockaddr_in cli_addr;
    socklen_t clilen = sizeof(cli_addr);
    int clifd;
    pthread_t pid;
    serve_thread_args_t *args;
    while(true) {
        /* accept client connection */
        clifd = accept(listenfd, (struct sockaddr *)&cli_addr, &clilen);
        if(clifd < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        args = malloc(sizeof(serve_thread_args_t));
        args->path = path;
        args->clifd = clifd;

        if(pthread_create(&pid, NULL, serve_thread, args))
            perror("pthread_create");
        pthread_detach(pid);
    }

    close(listenfd);
}
