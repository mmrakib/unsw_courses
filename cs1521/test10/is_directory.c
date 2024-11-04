#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h> // stat()
#include <unistd.h> // POSIX API, useful constants/fucntions
#include <dirent.h> // opendir(), readdir() and closedir()

int main(int argc, char **argv) {
    char *pathname = argv[1];

    DIR *d = opendir(pathname);

    if (d == NULL) {
        printf("0\n");
    } else {
        closedir(d);
        printf("1\n");
    }

    return 0;
}