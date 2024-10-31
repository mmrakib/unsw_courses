// exam_q5.c
//
// This program was written by z5555555
// on INSERT-DATE-HERE


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_SIZE 1024 

// Truth definitions
#define TRUE 1
#define FALSE 0

// Compares last few characters with suffix
int ends_with(char *input, char *word_ending) {
    if (strcmp(input + strlen(input) - strlen(word_ending), word_ending) == 0) {
        return TRUE;
    } else {
        return FALSE;
    }
}

int main(int argc, char *argv[]) {
    char input[MAX_LINE_SIZE];

    while (fgets(input, MAX_LINE_SIZE, stdin) != NULL) {
        for (int i = 0; i < (argc - 1); i++) {
            // Strips newline since strcmp() counts newlines (took me ages for me to figure this out *facepalm*)
            strtok(input, "\n");
            
            if (ends_with(input, argv[i + 1])) {
                printf("%s\n", input);
            }
        }
    }

    return 0;
}
