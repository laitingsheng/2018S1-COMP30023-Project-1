#ifndef _MIME_H_
#define _MIME_H_

typedef struct MIME {
    const char *file_ext;
    const char *file_type;
} MIME_t;

const MIME_t CSS = {"css", "text/css"};
const MIME_t HTML = {"html", "text/html"};
const MIME_t JS = {"js", "text/js"};
const MIME_t JPEG = {"jpg", "image/jpeg"};

#endif
