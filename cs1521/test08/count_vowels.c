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
    FILE *fp = fopen(argv[1], "r+");
    int count = 0;

    char c;
    while ((c = fgetc(fp)) != EOF) {
        if (is_vowel(c)) {
            count++;
        }
    }

    printf("%d\n", count);

    fclose(fp);

    return 0;
}