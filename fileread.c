#include <stdio.h>
#include <stdlib.h>

int main(void) {
	char *filename = "src/pizza/instructions.txt";
	FILE *fp = fopen(filename, "r");

	if (fp == NULL) {
		printf("Error: Could not open file %s", filename);
		return 1;
	}

	const unsigned MAX_LENGTH = 256;
	char buffer[MAX_LENGTH];

	while (fgets(buffer, MAX_LENGTH, fp))
		printf("%s", buffer);

	fclose(fp);

	return 0;
}

