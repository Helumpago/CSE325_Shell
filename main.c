
#include <stdio.h>

#define PROMPT "RedPizzaBaron> "
#define INTER 0
#define BATCH 1

int shell(FILE* src, int mode);

int main(int argc, char** argv) {
	if(argc == 1) { // No arguments implies interactive mode
		shell(stdin, INTER);
	} else if(argc > 1) { // Batch mode
		/* Open batch file */
		FILE* src = fopen(argv[1], "r");
		if(src == NULL) {
			perror("Could not open batch file");
			return -1;
		}

		shell(src, BATCH);
	}

	return 0;
}

/**
* Takes in input from the specified file and parses each command
* @param FILE* src File from which commands should be taken
* @param int mode Terminal mode. 0 means interactive, 1 means batch
**/
int shell(FILE* src, int mode) {
	printf("Doing stuff\n");

	return 0;
}