// COMP1521 21T2 ... final exam, question 1

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BITS 8

void
and (bool x[BITS], bool y[BITS], bool result[BITS])
{
	for (int i = 0; i < BITS; i++) {
		if (x[i] == false || y[i] == false) {
			result[i] = false;
		} else {
			result[i] = true;
		}
	}
}

void
or (bool x[BITS], bool y[BITS], bool result[BITS])
{
	for (int i = 0; i < BITS; i++) {
		if (x[i] == true || y[i] == true) {
			result[i] = true;
		} else {
			result[i] = false;
		}
	}
}

void
xor (bool x[BITS], bool y[BITS], bool result[BITS])
{
	for (int i = 0; i < BITS; i++) {
		if (x[i] == y[i]) {
			result[i] = false;
		} else {
			result[i] = true;
		}
	}
}

void
not (bool x[BITS], bool result[BITS])
{
	for (int i = 0; i < BITS; i++) {
		if (x[i] == 1) {
			result[i] = false;
		} else {
			result[i] = true;
		}
	}
}
