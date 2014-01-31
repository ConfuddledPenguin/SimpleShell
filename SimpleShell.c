/******************************************************************************
 * Synopsis: This is a simple unix like shell written in C, for the class CS210 
 *      at the University of Strathcylde. This was a group project to be 
 *      undertaken by four students.
 *
 * Compiling: This program should be compiled using the C99 standard. This is
 *      due to the inclusion of c++ style comments i.e: //comment
 *      The flag for this is: -std=c99
 *
 * Bugs:
 *      For past bugs please see Revision History
 *
 * Authors:
 *      Thomas Maxwell     - gvb12182
 *      Thomas Sinclair    - mlb12174
 *      Aidan O'Grady      - wlb12153
 *      Grant Toghill      - wsb12157
 *
 * Version:
 *      See VERSION below
 *
 *****************************************************************************/

/********************* R E V I S I O N   H I S T O R Y *************************
 * 
 * v0.0 - 28/01/2014 - File birth
 *
 *      Created initial file (SimpleShell.c) as well as read me files and a
 *      github repo.
 *
 ******************************************************************************/
#define VERSION "v0.0. Last Update 30/01/2014\n"

#include <stdio.h>
#include <string.h>
#include <windows.h>

typedef struct
{
	char *command;
} user_command;

int main(){

	//variable
	int run = 1;
	char *prompt = "> ";
	char input[512];
	char *p;

	//user loop
	while (run || (getchar() != EOF) ){

		user_command command = {
			NULL
		};

		//prompt user
 		printf("%s", prompt);
 		fgets(input, 512, stdin);

 		//Simple check
 		printf("The command: %s", input);

 		//Getting rid of the new line char, replacing with a terminating char
 		if ((p = strchr(input, '\n')) != NULL)
 			*p = '\0';

 		//TOKENISE HERE
 		char *tokenizer = " ";
 		char *token;
 		command.command = strtok(input, tokenizer);
 		printf("%s\n", command);
 		while ( (token = strtok(NULL, tokenizer) ) != NULL){

 			printf("%s\n", token);
 		}

 		//exit check
    	if(strcmp(command.command, "exit") == 0){ 
 			run = 0;
 		}
 	}

 return(0);
}