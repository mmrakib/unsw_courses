#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// print a specified byte from a file
//
// first command line argument specifies a file
// second command line argument specifies a byte location
//
// output is a single line containing only the byte's value
// printed as a unsigned decimal integer (0..255)
// if the location does not exist in the file
// a single line is printed saying: error

int get_file_length(FILE *fp) {
    int file_length;

    fseek(fp, 0, SEEK_END);
    file_length = ftell(fp);
    rewind(fp);

    return file_length;
}

int main(int argc, char *argv[]) {
    char *pathname = argv[1];
    int offset = atoi(argv[2]);

    FILE *fp = fopen(pathname, "r");
    int fp_length = get_file_length(fp);

    if (offset >= 0) {
        if (fseek(fp, offset, SEEK_SET) != 0 || offset > fp_length) {
            printf("error\n");
            exit(1);
        }
    } else {
        if (fseek(fp, offset, SEEK_END) != 0) {
            printf("error\n");
            exit(1);
        }
    }

    int byte = fgetc(fp);
    
    if (byte >= 0 && byte <= 255) {
        printf("%d\n", byte);
    } else {
        printf("error\n", byte);
        exit(1);
    }

    fclose(fp);

    return 0;
}
