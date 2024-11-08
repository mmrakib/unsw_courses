
#include <stdio.h>
#include <stdlib.h>

long long rabbits(int month);

int main(int argc, char *argv[]) {
	if (argc != 2) {
		fprintf(stderr, "usage: %s <month>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	int months = 0;
	char c;
	if (sscanf(argv[1], "%d %c", &months, &c) != 1 || months < 0) {
		fprintf(stderr, "error: invalid number of months\n");
		exit(EXIT_FAILURE);
	}

	long long numRabbits = rabbits(months);
	printf("Number of rabbits after %d month(s): %lld\n",
	       months, numRabbits);

	return 0;
}

// !!! DO NOT MODIFY THE CODE ABOVE !!!
////////////////////////////////////////////////////////////////////////
// Your task

// f(0) = 2;
// f(1) = 2;
// f(2) = 4;
// f(3) = 6;
// f(4) = 8;
// f(5) = 16;

// Returns the number of rabbits after the given number of months
long long rabbits(int months) {
	if (months == 0 || months == 1) return 2;
	return rabbits(months - 1) + rabbits(months - 2);
}

