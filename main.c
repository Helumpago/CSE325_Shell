
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#define PROMPT "RedPizzaBaron> "
#define STRLEN 1024
#define MAX_ARGS 32

int shell(char* command);
int parseCommand(char* com);

int main(int argc, char** argv) {
	char tmpstr[STRLEN];
	printf(PROMPT);
	while(fgets(tmpstr, STRLEN, stdin)){
		parseCommand(tmpstr);
		printf(PROMPT);
	}

	return 0;
}

int shell(char* command) {
	printf("%s%s\n", PROMPT, command);
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
