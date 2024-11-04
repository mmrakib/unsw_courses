#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

int is_vowel(char c) {
    if (c == 'a' || c == 'A' ||
        c == 'e' || c == 'E' ||
        c == 'i' || c == 'I' ||
        c == 'o' || c == 'O' ||
        c == 'u' || c == 'U') {
        return TRUE;
    } else {
        return FALSE;
    }
}

int main(int argc, char **argv) {
    FILE *infp = fopen(argv[1], "r+");
    FILE *outfp = fopen(argv[2], "w+");

    char c;
    while ((c = fgetc(infp)) != EOF) {
        if (!is_vowel(c)) {
            fputc(c, outfp);
        }
    }

    fclose(infp);
    fclose(outfp);

    return 0;
}