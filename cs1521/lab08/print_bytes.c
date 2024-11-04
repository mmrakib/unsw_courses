#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int main(int argc, char **argv) {
    FILE *fd = fopen(argv[1], "r+");

    int byte;
    int i = 0;
    while ((byte = fgetc(fd)) != EOF) {
        printf("byte %4ld: %3d 0x%02x", i, byte, byte);

        if (isprint(byte)) {
            printf(" '%c'", byte);
        } 

        printf("\n");
        i++;
    }

    fclose(fd);

    return 0;
}