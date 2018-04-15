#ifndef _MACRO_H_
#define _MACRO_H_

#define PORT 80
#define PATH "/home/www"

/*
 * for struct alignment, only allow path with a maximum length of 2046, as in
 * the httpd.c
 */
#define PATH_MAX_LENGTH 2046

#define SERVER_STRING "Simple HTTP Server - Project @ University of Melbourne"

#define BUFFER_SIZE 4096

#endif
