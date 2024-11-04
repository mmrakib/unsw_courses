// COMP1521 21T2 ... final exam, question 5

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define MASK_FIRST_1 0x80
#define MASK_FIRST_3 0xE0
#define MASK_FIRST_4 0xF0
#define MASK_FIRST_5 0xF8

void
print_utf8_count (FILE *file)
{
	unsigned long amount_1_byte = 0;
	unsigned long amount_2_byte = 0;
	unsigned long amount_3_byte = 0;
	unsigned long amount_4_byte = 0;

	int byte;
	while ((byte = fgetc(file)) != EOF) {
		if ((byte & MASK_FIRST_1) == 0) {
			// 1 byte encoding (ASCII)
			amount_1_byte++;
		} else {
			if ((byte & MASK_FIRST_3) == 0xC0) {
				// 2 byte encoding (unicode)
				amount_2_byte++;
				//fgetc(file);
			} else if ((byte & MASK_FIRST_4) == 0xE0) {
				// 3 byte encoding (unicode)
				amount_3_byte++;
				//fgetc(file);
				//fgetc(file);
			} else if ((byte & MASK_FIRST_5) == 0xF0) {
				// 4 byte encoding (unicode)
				amount_4_byte++;
				//fgetc(file);
				//fgetc(file);
				//fgetc(file);
			}
		}
	}

	printf("1-byte UTF-8 characters: %lu\n", amount_1_byte);
	printf("2-byte UTF-8 characters: %lu\n", amount_2_byte);
	printf("3-byte UTF-8 characters: %lu\n", amount_3_byte);
	printf("4-byte UTF-8 characters: %lu\n", amount_4_byte);
}
