/*
 * Responsible for responding to the client
 * Create by Tingsheng Lai (tingshengl, 781319)
 */

#ifndef _HTTPD_H_
#define _HTTPD_H_

/*
 * Respond to the client socket with the given root directory of web service
 * Parameters:
 *     sockfd: int, client socket fild descriptor
 *     rootdir: const char *, root directory of web service
 * Returns:
 *     None
 */
void respond(int sockfd, const char *rootdir);

#endif
