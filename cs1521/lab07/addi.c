// generate the opcode for an addi instruction

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

#include "addi.h"

// return the MIPS opcode for addi $t,$s, i
uint32_t addi(int t, int s, int i) {
    int32_t opcode = 0;

    int op = 8;
    opcode += (op << 26);
    opcode += (s << 21);
    opcode += (t << 16);
    opcode += i;

    if (i < 0) opcode |= 0x10000;

    return opcode;
}
