#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// print the locations of a specified byte sequence in a file
// the first command line argument is a filname
// other command line arguments are integers specifying a byte sequence
// all positions the byte sequence occurs in the file are printed

#define TRUE 1
#define FALSE 0

int find_sequence(FILE *fp, int bytes[], int bytes_len) {
    long curr_pos = ftell(fp);

    int byte = 0;
    for (int i = 0; i < bytes_len; i++) {
        byte = fgetc(fp);
        if (bytes[i] != byte) {
            fseek(fp, curr_pos, SEEK_SET); // Reset
            return FALSE;
        }
    }

    fseek(fp, curr_pos, SEEK_SET); // Reset
    return TRUE;
}

int main(int argc, char *argv[]) {
    char *pathname = argv[1];
    int bytes[argc - 2];

    for (int i = 2; i < argc; i++) {
        bytes[i - 2] = atoi(argv[i]);
    }

    FILE *fp = fopen(pathname, "r");

    do {
        int match = find_sequence(fp, bytes, argc - 2);

        if (match) {
            long curr_pos = ftell(fp);
            printf("Sequence found at byte position: %lu\n", curr_pos);

            fclose(fp);
            return 0;
        }
    } while (fgetc(fp) != EOF);

    printf("Sequence not found\n");

    fclose(fp);
    return 0;
}
