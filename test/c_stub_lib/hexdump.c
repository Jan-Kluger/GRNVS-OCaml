#include <stdio.h>

#include "hexdump.h"


char *
hexdump_str(const void *buffer, ssize_t len)
{
    static char str[65536];
    unsigned int i;
    int p = 0;

    for (i = 0; i < len; i++) {
        if ((i % 8 == 0) && (i != 0)) {
            p += sprintf(str + p, "  ");
        }
        if (i % 16 == 0) {
            if (i == 0) {
                p += sprintf(str + p, "0x%.4x  ", i);
            } else {
                str[p++] = '\n';
                p += sprintf(str + p, "0x%.4x  ", i);
            }
        }
        p += sprintf(str + p, "%.2x ", ((unsigned char *) buffer)[i]);

        if ((sizeof(str) - p) < 64)
            break;
    }

    return str;
}

void
hexdump(const void *buffer, ssize_t len) {
    char *str = hexdump_str(buffer, len);
    fprintf(stderr, "%s\n", str);
}
