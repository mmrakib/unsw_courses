#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
    FILE *fd = fopen(argv[1], "w+");

    char *e;
    int start = strtol(argv[2], &e, 10);
    int end = strtol(argv[3], &e, 10);

    for (int i = start; i <= end; i++) {
        fprintf(fd, "%d\n", i);
    }

    fclose(fd);

    return 0;
}