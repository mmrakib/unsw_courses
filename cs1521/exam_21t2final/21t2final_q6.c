// COMP1521 21T2 ... final exam, question 6

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

void
convert_hexdump_to_bytes (FILE *hexdump_input, FILE *byte_output)
{
	int address;
	while (fscanf(hexdump_input, "%8x", &address) == 1) {
		fseek(byte_output, address, SEEK_SET);

		fgetc(hexdump_input);
		fgetc(hexdump_input);

		for (int i = 0; i < 16; i++) {
			int byte;
			fscanf(hexdump_input, "%2x", &byte);
			fputc(byte, byte_output);
			fgetc(hexdump_input);
			if (i == 7 || i == 15) fgetc(hexdump_input);		
		}

		for (int i = 0; i < 19; i++) fgetc(hexdump_input);
	}
}
