// Extract the 3 parts of a float using bit operations only

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

#include "floats.h"

// Boolean definitions
#define TRUE 1
#define FALSE 0

// separate out the 3 components of a float
float_components_t float_bits(uint32_t f) {
    float_components_t fc;

    // if (f & 2147483648 == 0) {
    //     fc.sign = 0; // Positive
    // } else {
    //     fc.sign = 1; // Negative
    // }

    fc.sign = (f & 2147483648) ? 1 : 0;
    fc.exponent = (f & 2139095040) >> 23;
    fc.fraction = (f & 8388607);

    return fc;
}

// given the 3 components of a float
// return 1 if it is NaN, 0 otherwise
int is_nan(float_components_t f) {
    // uint32_t ff = 0;
    // ff += (f.sign >> 31);
    // ff += (f.exponent );
    // ff += f.fraction;
    // if (ff != ff) {
    //     return TRUE;
    // } else {
    //     return FALSE;
    // }

    if (f.exponent == 255 && f.fraction != 0) {
        return TRUE;
    } else {
        return FALSE;
    }
}

// given the 3 components of a float
// return 1 if it is inf, 0 otherwise
int is_positive_infinity(float_components_t f) {
    if (f.sign == 0 && f.exponent == 255 && f.fraction == 0) {
        return TRUE;
    } else {
        return FALSE;
    }
}

// given the 3 components of a float
// return 1 if it is -inf, 0 otherwise
int is_negative_infinity(float_components_t f) {
    if (f.sign == 1 && f.exponent == 255 && f.fraction == 0) {
        return TRUE;
    } else {
        return FALSE;
    }
}

// given the 3 components of a float
// return 1 if it is 0 or -0, 0 otherwise
int is_zero(float_components_t f) {
    if (f.exponent == 0 && f.fraction == 0) {
        return TRUE;
    } else {
        return FALSE;
    }
}
