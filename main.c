
#include <stdio.h>

#define PROMPT "RedPizzaBaron> "

int shell(char* command);

int main(int argc, char** argv) {
	int i = 1;
	for(; i < argc; i++) {
		shell(argv[i]);
	}

	return 0;
}

// A TEST CHANGE

int shell(char* command) {
	printf("%s%s\n", PROMPT, command);
	return 0;
}