#include "file.h"

#include <stdio.h>

/* incomplete list of known file types */
static const struct {
    const char *ext, *MIME_type;
} MIME_types[] = {
    {"css", "text/css"},
    {"es", "application/ecmascript"},
    {"gif", "image/gif"},
    {"htm", "text/html"},
    {"html", "text/html"},
    {"ico", "image/x-icon"},
    {"jpeg", "image/jpeg"},
    {"jpg", "image/jpeg"},
    {"js", "application/javascript"},
    {"json", "application/json"},
    {"pdf", "application/pdf"},
    {"png", "image/png"}
};
static const int NUM_DEFINED_MIME = 12;
static const char *UNKNOWN = "application/octet-stream";

const char *file_MIME(const char *filename) {
    /* allow for filename such as file.name.ext */
    const char *last_dot = NULL, *curr = filename;
    while(*curr) {
        if(*curr == '.')
            last_dot = curr;
        ++curr;
    }

    /* avoid case such as file. */
    if(last_dot + 1 == curr)
        last_dot = NULL;

    const char *MIME_type = NULL;
    if(last_dot) {
        const char *ext = last_dot + 1;

        /* binary search */
        int lo = 0, hi = NUM_DEFINED_MIME, mid, re;
        while(lo < hi) {
            mid = (lo + hi) / 2;

            if((re = strcmp(ext, MIME_types[mid].ext)) == 0)
                return MIME_types[mid].MIME_type;

            if(re < 0)
                hi = mid;
            else
                lo = mid + 1;
        }
    }

    return UNKNOWN;
}
