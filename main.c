
#include <stdio.h>
#include <string.h>

#define PROMPT "RedPizzaBaron"
#define INTER 0
#define BATCH 1
#define BUFFSIZE 1024

int shell(FILE* src, int mode);
int dummyExec(char* comm);

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
*
* @return 0 for success, -1 for failure.
*
* @author William Rosenberger
**/
int shell(FILE* src, int mode) {
	int breakLoop = 0;
	char buff[BUFFSIZE];

	while(breakLoop == 0) {
		if(mode == INTER) // Only prompt if in interactive mode
			printf("%s> ", PROMPT);

		/* Get user input */
		fgets(buff, BUFFSIZE, src);
		if(feof(src)) // If user requested to quit, exit
			break;

		/* Parse input */
		char* currCom = NULL;
		for(currCom = strtok(buff, ";"); currCom != NULL; currCom = strtok(NULL, ";")) {
			if(strncmp(currCom, "quit\n", 5) == 0) { // If user requested to quit, exit
				breakLoop = 1;
				break;
			}

			/* Quit with error value if any of the commands error */
			int retVal = dummyExec(currCom);
			if(retVal != 0) {
				return retVal;
			}
		}
	}

	return 0;
}

/**
* Dummy function for executing an individual command. Replace with AJ's function.
*/
int dummyExec(char* comm) {
	printf("%s\n", comm);

	return 0;
}