#include <stdio.h>

// read two integers and print all the integers which have their bottom 2 bits set.

#define BOTTOM_2 0x3

int main(void) {
    int x, y;

    scanf("%d", &x);
    scanf("%d", &y);

    for (int i = x + 1; i < y; i++) {
        if ((i & BOTTOM_2) == BOTTOM_2) {
            printf("%d\n", i);
        }
    }

    return 0;
}
