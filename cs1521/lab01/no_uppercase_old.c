#include <stdio.h>
#include <ctype.h>

#define MAX_LINE 256
#define TRUE 1

int main(void) {
	while (TRUE) {
		char line[MAX_LINE];

		char c;
		int line_length = 0;

		while ((c = getchar()) != '\n' && line_length < MAX_LINE - 1) {
			line[line_length] = c;
			line_length++;
		}
		line[line_length] = '\0';

		for (int i = 0; i < line_length; i++) {
			if (line[i] >= 65 && line[i] <= 90) {
				// 32 is the difference in ASCII code between lowercase and uppercase letters
				putchar(line[i] + 32); 
			} else {
				putchar(line[i]);
			}
		}
		putchar('\n');
	}

	return 0;
}