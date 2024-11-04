#include <stdio.h>
#include <ctype.h>

int main(void) {
	int c = getchar();

	while (c != EOF) {
		if (c >= 65 && c <= 90) {
			putchar(c + 32);
		} else {
			putchar(c);
		}
		c = getchar();
	}

	return 0;
}