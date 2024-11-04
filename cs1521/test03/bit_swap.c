// swap pairs of bits of a 64-bit value, using bitwise operators

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>

// return value with pairs of bits swapped
uint64_t bit_swap(uint64_t value) {
    uint64_t swapped = 0;

    for (int i = 0; i < 64; i += 2) {
        uint64_t mask = 1;
        uint64_t bit1 = value & (mask << i);

        mask = 1;
        uint64_t bit2 = value & (mask << (i + 1));

        swapped |= (bit2 >> 1);
        swapped |= (bit1 << 1);
    }

    return swapped;
}
