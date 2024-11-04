#include <stdlib.h>
#include <stdint.h>

// given 2 uint32_t values
// return the number of bits which are set (1) in both values

int final_q5(uint32_t value1, uint32_t value2) {
    int count = 0;

    for (int i = 0; i < 32; i++) {
        uint32_t mask = 1;
        uint32_t bit1 = value1 & (mask << i);
        uint32_t bit2 = value2 & (mask << i);
        bit1 >>= i;
        bit2 >>= i;

        if (bit1 == 1 && bit2 == 1) count++;
    }

    return count;
}
