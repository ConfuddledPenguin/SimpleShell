/******************************************************************************
 * Synopsis: This is a simple unix like shell written in C, for the class CS210 
 *      at the University of Strathclyde. This was a group project to be 
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
 * v0.0.1 - 01/02/2014 - User input
 *
 *			Created a basic loop to prompt the user and taken input using 
 *		fgets(). This is then checked to ensure that the user has not
 *		just hit enter. Once we are sure we have a sensible input, it is
 *		tokenised and printed back out the console. The user can also exit the 
 *		program if he disires. 
 *
 *			The comment shall be stored in a struct, as this can be easily 
 *		accessed and passed, although this has not yet been fully implemented 
 *		yet.
 *
 ******************************************************************************/
#define VERSION "v0.0. Last Update 30/01/2014\n"

#include <stdio.h>
#include <string.h>

#define PROMPT "> "

typedef struct
{
	char *input_command;
	char args[50][50];
} user_command;

int getInput(user_command *command){
	char input[512];
	char *p;

	//prompt user
 	printf("%s", PROMPT);
 	fgets(input, 512, stdin);

 	//To ensure its been taken in. Shall be removed in future
 	printf("The input: %s", input);

 	//Checking user has not just hit enter
 	if (input[0] == '\n')
    	return 1;

 	//Getting rid of the new line char, replacing with a terminating char
 	if ((p = strchr(input, '\n')) != NULL)
 		*p = '\0';

 	//tokenising 
 	char *tokenizer = " ";
 	char *token;
 	command->input_command = strtok(input, tokenizer);
 	printf("The command: %s\n", command->input_command);

 	//exit check
    if(strcmp(command->input_command, "exit") == 0){ 
 		return 0;
 	}

 	int i = 0;
 	printf("The parameters: ");
 	while ( (token = strtok(NULL, tokenizer) ) != NULL){
 		//printf("%s\n", token);
 		// command.args[i] = token;
 		strcpy(command->args[i], token);
 		printf("%s ", command->args[i]);
 		i++;
 	}
 	printf("\n");

 	return 1;
}

int main(){
	//variable
	int run = 1;
	//user loop
	while (run){
		user_command command = {
			NULL
		};
		run = getInput(&command);
 	}

 	return(0);
}