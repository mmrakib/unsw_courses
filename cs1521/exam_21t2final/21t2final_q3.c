// COMP1521 21T2 ... final exam, question 3

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void
cp (char *path_from, char *path_to)
{
	FILE *input = fopen(path_from, "r");
	FILE *output = fopen(path_to, "w");

	if (input == NULL || output == NULL) {
		fprintf(stderr, "Could not open file(s)\n");
		exit(1);
	}

	int byte;
	while ((byte = fgetc(input)) != EOF) {
		fputc(byte, output);
	}
}

