/****************************************************************************************************
* Filename: main.c
*
* Description: This file parses Linux commands either in the form of an interactive shell or by
* 				reading a file that contains a list of commands to execute (batch). In the case of 2 
*				commands being input seperated by ';', these commands will be executed simultaniously. 
*
* Authors: Andrew (AJ) Burns, William Rosenberger
* Class: CSE 325
* Instructor: Zheng
* Assignment: Lab Project #2
* Assigned: January 28, 2015
* Due: Feburuary 11, 2015
*****************************************************************************************************/


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
int spawnComm(char* comm, int mode);
int parseCommand(char* com, int mode);
char* stripWhiteSpace(char* string);

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

			currCom = stripWhiteSpace(currCom); // strip whitespace from commands

			if(strncmp(currCom, "quit", 5) == 0) { // If user requested to quit, finish running commands on this line, but don't take in more input
				breakLoop = 1;
			}

			if(strncmp(currCom, "quit", 5) != 0){
				spawnComm(currCom, mode);
			}
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
int spawnComm(char* comm, int mode) {
	pid_t proc = fork();

	if(proc < 0) {
		perror("Error creating process");
		return -1;
	} else if(proc == 0) { // Child process 
		exit(parseCommand(comm, mode));
	}

	return 0;
}

/**
 * Parses the command into proper format
 * and then runs the command
 * @param com, the command to parse and run
 * @returns 0 if successful, -1 if error
 *
 * @author Andrew Burns
 **/
int parseCommand(char* com, int mode){
	char* args[MAX_ARGS];
	int i;

	// print command in batch mode
	if(mode == BATCH){
			printf("%s\n", com);
	}

	// get first token
	args[0] = strtok(com, " ");
	for(i = 1; i < MAX_ARGS;i++){
		args[i] = strtok(NULL, " ");
		if(args[i] == NULL)
			break;
		
	}


	// execute command
	if(execvp(args[0], args) == -1){
		perror("Error executing command(s)");
		return -1;
	}

	return 0;
}

/**
 * strips whitespace from begining and end of string
 * @param string: the string to strip whitespace from
 * @returns: the string with stripped whitespace
 *
 * @author Will and Andrew
 **/
char* stripWhiteSpace(char* string) {
	for(; *string == ' '; string++); // Strip leading whitespace

	char* startWhite = string + strlen(string) - 1;
	for(; *startWhite == ' '; startWhite--); // Find first whitespace on the end of the string
	*(startWhite + 1) = '\0';

	return string;
}