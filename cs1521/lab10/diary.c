#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 4096

int main(int argc, char **argv) {
    char *home = getenv("HOME");

    char path[MAX_LEN];
    snprintf(path, MAX_LEN, "%s/.diary", home);

    FILE *stream = fopen(path, "a+");

    for (int i = 1; i < argc; i++) {
        fputs(argv[i], stream);
        fputc(' ', stream);
    }
    fputc('\n', stream);

    fclose(stream);

    return 0;
}