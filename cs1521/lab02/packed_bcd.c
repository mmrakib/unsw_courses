#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

#define N_BCD_DIGITS 8

uint32_t packed_bcd(uint32_t packed_bcd);

int main(int argc, char *argv[]) {

    for (int arg = 1; arg < argc; arg++) {
        long l = strtol(argv[arg], NULL, 0);
        assert(l >= 0 && l <= UINT32_MAX);
        uint32_t packed_bcd_value = l;

        printf("%lu\n", (unsigned long)packed_bcd(packed_bcd_value));
    }

    return 0;
}

int power(int b, int x) {
    int value = 1;

    for (int i = 0; i < x; i++) {
        value = value * b;
    }

    return value;
} 

// given a packed BCD encoded value between 0 .. 99999999
// return the corresponding integer
uint32_t packed_bcd(uint32_t packed_bcd_value) {
    uint32_t decimal = 0;

    for (int i = 0; i < 8; i++) {
        uint32_t mask = 0xF;
        mask = mask << (i * 4);
        uint32_t nibble = packed_bcd_value & mask;
        nibble = nibble >> (i * 4);
        decimal += nibble * power(10, i);
    }

    return decimal;
}
