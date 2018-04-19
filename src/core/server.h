/*
 * Responsible for monitoring the incoming connection
 * Create by Tingsheng Lai (tingshengl, 781319)
 */

#ifndef _SERVER_H_
#define _SERVER_H_

/*
 * Serve the given port with given root directory
 * Parameters:
 *     port: unsigned int, port number to be binded
 *     path: the path to the root directory of the web service
 * Returns:
 *     None
 */
void serve(unsigned int port, const char *path);

#endif
