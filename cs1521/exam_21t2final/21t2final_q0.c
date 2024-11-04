// COMP1521 21T2 ... final exam, question 0

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int
count_leading_zeroes (uint32_t x)
{
	int count = 0;

	for (int i = 0; i < 32; i++) {
		if (x & 0x80000000) break;
		x = x << 1;
		count++;
	}

	return count;
}
