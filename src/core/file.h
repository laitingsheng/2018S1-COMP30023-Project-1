/*
 * Responsible for parsing the filename to get an appropriate MIME type
 * Create by Tingsheng Lai (tingshengl, 781319)
 */

#ifndef _FILE_H_
#define _FILE_H_

/*
 * Return a file MIME with the given filename
 * Parameters:
 *     filename: const char *, the given filename
 * Return:
 *     corresponding MIME type
 */
const char *file_MIME(const char *filename);

#endif
