#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <byteswap.h>

int main(int argc, char **argv) {
    FILE *fd = fopen(argv[1], "wb+");

    char *e;
    uint16_t start = strtol(argv[2], &e, 10);
    uint16_t end = strtol(argv[3], &e, 10);

    for (uint16_t i = start; i <= end; i++) {
        fputc(((i & 65280) >> 8), fd); // (i & 1111111100000000) >> 8
        fputc((i & 255), fd); // i & 0000000011111111
    }

    fclose(fd);

    return 0;
}