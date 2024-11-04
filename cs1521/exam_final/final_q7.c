#include <stdio.h>

// copy file specified as command line argument 1 to
// file specified as command line argument 2
// convert UTF8 to ASCII by replacing multibyte UTF8 characters with '?'

#define MASK_FIRST_1 0x80
#define MASK_FIRST_3 0xE0
#define MASK_FIRST_4 0xF0
#define MASK_FIRST_5 0xF8

int main(int argc, char *argv[]) {
    char *input_path = argv[1];
    char *output_path = argv[2];

    FILE *input_file = fopen(input_path, "r");
    FILE *output_file = fopen(output_path, "w");

    int byte;
    while ((byte = fgetc(input_file)) != EOF) {
        if ((byte & MASK_FIRST_1) == 0) {
            fputc(byte, output_file);
        } else {
            fputc('?', output_file);

            if ((byte & MASK_FIRST_3) == 0xC0) {
				// 2 byte encoding (unicode)
				fgetc(input_file);
			} else if ((byte & MASK_FIRST_4) == 0xE0) {
				// 3 byte encoding (unicode)
				fgetc(input_file);
				fgetc(input_file);
			} else if ((byte & MASK_FIRST_5) == 0xF0) {
				// 4 byte encoding (unicode)
				fgetc(input_file);
				fgetc(input_file);
				fgetc(input_file);
			}
        }
    }

    fclose(input_file);
    fclose(output_file);

    return 0;
}
