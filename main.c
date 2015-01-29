
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define PROMPT "RedPizzaBaron"
#define INTER 0
#define BATCH 1
#define BUFFSIZE 1024
#define STRLEN 1024
#define MAX_ARGS 32

int shell(FILE* src, int mode);
int spawnComm(char* comm);
int dummyProc(char* comm);
int shell(char* command);
int parseCommand(char* com);

int main(int argc, char** argv) {
	if(argc == 1) { // No arguments implies interactive mode
		shell(stdin, INTER);
	} else if(argc == 2) { // Batch mode
		/* Open batch file */
		FILE* src = fopen(argv[1], "r");
		if(src == NULL) {
			perror("Could not open batch file");
			return -1;
		}

		shell(src, BATCH);
	} else {
		fprintf(stderr, "Program expects at most one argument (the batch file)\n");
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
		buff[strlen(buff) - 1] = '\0'; // Strip newline

		if(feof(src)) { // Exit when end of file reached
			printf("\n");
			break;
		}

		/* Parse input */
		int numCom = 0;
		char* currCom = NULL;
		for(currCom = strtok(buff, ";"); currCom != NULL; currCom = strtok(NULL, ";"), numCom++) {
			if(strncmp(currCom, "quit", 5) == 0) { // If user requested to quit, finish running commands on this line, but don't take in more input
				breakLoop = 1;
			}

			spawnComm(currCom);
		}

		/* Wait for all procs to finish */
		for(; numCom > 0; numCom--) {
			wait(NULL);
		}
	}

	return 0;
}

/**
* Spawns a new process for executing a process
*/
int spawnComm(char* comm) {
	pid_t proc = fork();

	if(proc < 0) {
		perror("Error creating process");
		return -1;
	} else if(proc == 0) { // Child process {
		exit(dummyProc(comm));
	}

	return 0;
}

/**
* Dummy function for processing a command.  Replace with AJ's function.
**/
int dummyProc(char* comm) {
	printf("%s\n", comm);

	return 0;
}

/**
 * Parses the command into proper format
 * and then runs the command
 * @argument com, the command to parse and run
 * @returns 0 if successful, -1 if error
 **/
int parseCommand(char* com){
	char* args[MAX_ARGS];
	int i;

	// set null terminator properly
	for(i = 0; i < sizeof(com); i++){
		if(com[i] == '\n' || com[i] == '\0'){
			com[i] = '\0';
			break;
		}
	}

	// get first token
	args[0] = strtok(com, " ");
	for(i = 1; i < MAX_ARGS;i++){
		args[i] = strtok(NULL, " ");
		if(args[i] == NULL)
			break;
		
	}

	// execute command
	execvp(args[0], args);

	return 0;
}
