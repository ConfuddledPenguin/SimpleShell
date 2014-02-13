/******************************************************************************
 * Synopsis: This is a simple Unix like shell written in C, for the class CS210 
 *      at the University of Strathclyde, Glasgow. This was a group project to be 
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
 ******************************************************************************/

/********************* R E V I S I O N   H I S T O R Y *************************
 * 
 *	For detailed Version history please see "VERSIONHISTORY.txt"
 *
 *
 *
 * 	v0.0 - 28/01/2014 - File birth
 *
 * 		Created initial file (SimpleShell.c) as well as read me files and a
 * 		github repo.
 *
 * 	v0.1 - 01/02/2014 - Stage One
 *
 *		Created the user loop, which gets input from the user. The input is
 *		check to make sure it is not absolute gibberish and tokenized. The
 *		result is stored in a structure called user_command.
 *
 ******************************************************************************/
#define VERSION "v0.1. Last Update 04/02/2014\n"

#include <stdio.h>
#include <string.h>

#define PROMPT "> "
#define INPUT_EXIT 0
#define INPUT_CONTINUE 1
#define INPUT_RUN 2
#define INPUT_ERROR 3

/* This structure holds the command entered by the user. For example, the
 * command "cd Documents" would have input_command = "cd" and args to store
 * "Documents" in args[0].
 *
 * char *input_command	- The command that the user wishes to execute 
 * char args[50][50]	- The arguments of a command.
 *
 */
typedef struct
{
	char *input_command;
	char args[50][50];
} user_command;

/* int getInput(user_command *command)
 * 
 * #include <stdio.h>
 * #include <string.h>
 *
 * Description:
 *
 * Prompts the user for an input and tokenises it. This modifies the struct,
 * user_command, and fills it with appropriate values.
 *
 * Returns:
 *
 * int INPUT_EXIT		- If user wishes to exit the program
 * int INPUT_RUN		- If it was successful
 * int INPUT_ERROR		- If something has gone drastically wrong
 * int INPUT_CONTINUE	- If the entered command should not be processed.
 * 
 */
int getInput(user_command *command){
	
	char input[512];
	char *p;

	//prompt user
 	printf("%s", PROMPT);
 	if((fgets(input, 512, stdin)) == NULL){ //end of file check
 		printf("\n");
 		return INPUT_EXIT;
 	}

 	//Checking user has not just hit enter
 	if (input[0] == '\n')
 		return INPUT_CONTINUE;

 	//Getting rid of the new line char, replacing with a terminating char
 	if ((p = strchr(input, '\n')) != NULL)
 		*p = '\0';

 	//To ensure it's been taken in. Shall be removed in future
 	printf("The input: '%s'\n", input);

 	//tokenising 
 	char *tokenizer = " \t|><";
 	char *token;
 	command->input_command = strtok(input, tokenizer);
 	if(command->input_command == NULL)
 		return INPUT_CONTINUE;

 	//ensures command has been taken in. Shall be removed in future
 	printf("The command: '%s'\n", command->input_command);

 	//exit check
 	if(strcmp(command->input_command, "exit") == 0) 
 		return INPUT_EXIT;

 	int i = 0;

 	printf("The parameters: "); // Ensuring parameters. Shall be removed in future
 	while ( (token = strtok(NULL, tokenizer) ) != NULL) {

 		strcpy(command->args[i], token);
 		printf("'%s' ", command->args[i]); // Ensuring parameters. Shall be removed in future
 		i++;

 		if (i >= 50) {

 			printf("Error: To many parameters\n");
 			return INPUT_CONTINUE;
 		}
 	}
 	printf("\n");

 	return INPUT_RUN;
}

int main(){

	int return_val = -1;
	// int run = 1;

	//user loop
	while (1) {
		//initializes the new command that the user wishes to execute
		user_command command = {
			NULL
		};

		return_val = getInput(&command);
		if (return_val == INPUT_CONTINUE) {
			continue;
		} else if (return_val == INPUT_ERROR) {
			printf("I broke");
			continue;
		} else if (return_val == INPUT_EXIT) {
			break;
		}
 	};

 	return(0);
}
