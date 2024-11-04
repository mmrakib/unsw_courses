#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
    FILE *fd = fopen(argv[1], "wb+");
    
    int c;
    char *e;

    for (int i = 2; i < argc; i++) {
        c = strtol(argv[i], &e, 10);
        fputc(c, fd);
    }

    fclose(fd);

    return 0;
}