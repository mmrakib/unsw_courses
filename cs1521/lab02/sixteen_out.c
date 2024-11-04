// Convert a 16-bit signed integer to a string of binary digits

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define N_BITS 16

char *sixteen_out(int16_t value);

int main(int argc, char *argv[]) {

    for (int arg = 1; arg < argc; arg++) {
        long l = strtol(argv[arg], NULL, 0);
        assert(l >= INT16_MIN && l <= INT16_MAX);
        int16_t value = l;

        char *bits = sixteen_out(value);
        printf("%s\n", bits);

        free(bits);
    }

    return 0;
}

// given a signed 16 bit integer
// return a null-terminated string of 16 binary digits ('1' and '0')
// storage for string is allocated using malloc
// char *sixteen_out(int16_t value) {
//     char *bits = (char*) malloc(17*sizeof(char));
//     int16_t n = value;

//     bits[16] = '\0';
//     int i = 15;
//     while (n > 0) {
//         if (n % 2 == 0) {
//             bits[i] = '0';
//         } else {
//             bits[i] = '1';
//         }

//         n = n / 2;
//         i--;
//     }

//     return bits;
// }

// char *strrev(char *str)
// {
//       char *p1, *p2;

//       if (! str || ! *str)
//             return str;
//       for (p1 = str, p2 = str + strlen(str) - 1; p2 > p1; ++p1, --p2)
//       {
//             *p1 ^= *p2;
//             *p2 ^= *p1;
//             *p1 ^= *p2;
//       }
//       return str;
// }

// char *sixteen_out(int16_t value) {
//     int16_t n = value, i, result;
//     char *bits = (char *) malloc(17*sizeof(char));

//     bits[16] = '\0';
//     for (i = 15; i >= 0; i--) {
//         result = n >> i;

//         if (result & 1) {
//             bits[i] = '1';
//         } else {
//             bits[i] = '0';
//         }
//     }

//     strrev(bits);
//     return bits;
// }

char *sixteen_out(int16_t value) {
    char *bits = malloc(17);

    for (int i = 0; i < 16; i++) {
        uint16_t mask = 0x8000;
        uint16_t bit = value & (mask >> i);
        //bit = bit >> (15 - i);
        if (bit) bits[i] = '1';
        else     bits[i] = '0';
    }
    bits[16] = '\0';

    return bits;
}