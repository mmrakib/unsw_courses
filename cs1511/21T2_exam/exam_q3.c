// exam_q3.c
//
// This program was written by z5361151
// on INSERT-DATE-HERE

#include <stdio.h>

#define MAX_INPUTS 99

int main(void) {
    int input;
    int input_count = 0;
    int input_list[MAX_INPUTS];

    while (scanf("%d", &input) == 1) {
        if (input == 0) {
            break;
        } else {
            input_list[input_count] = input;
            input_count++;
        }
    }

    // Print even indices
    for (int i = 0; i < input_count; i += 2) {
        printf("%d ", input_list[i]);
    }

    // Print odd indices
    for (int i = 1; i < input_count; i += 2) {
        printf("%d ", input_list[i]);
    }

    printf("\n");

    return 0;
}
