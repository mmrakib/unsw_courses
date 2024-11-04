#include <stdio.h>
#include <stdint.h>

uint32_t hash(uint32_t old_hash_value, uint8_t byte, size_t index);

int main(int argc, char *argv[]) {
    FILE *fp = fopen(argv[1], "r+");
    uint32_t h = 0;

    int i = 0;
    char c;
    while ((c = fgetc(fp)) != EOF) {
        h = hash(h, c, i);
        i++;
    }

    printf("%d\n", h);

    fclose(fp);

    return 0;
}

// DO NOT CHANGE THIS FUNCTION

uint32_t hash(uint32_t old_hash_value, uint8_t byte, size_t index) {
    return old_hash_value + (byte << (index % 17));
}
