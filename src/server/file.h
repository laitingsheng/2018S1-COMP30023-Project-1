#ifndef _FILE_H_
#define _FILE_H_

char *path_concat(char *dest, char *src_segs[], int seg_num);
char *parse_file_type(char *filename);
char *generate_MIME_response(char *ext, char *response);

#endif
