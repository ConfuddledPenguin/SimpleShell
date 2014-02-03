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
 ******************************************************************************/

/********************* R E V I S I O N   H I S T O R Y *************************
 * 
 * 	v0.0 - 28/01/2014 - File birth
 *
 *      Created initial file (SimpleShell.c) as well as read me files and a
 *      github repo.
 *
 * 	v0.0.1 - 01/02/2014 - User input
 *
 *		Created a basic loop to prompt the user and taken input using 
 *		fgets(). This is then checked to ensure that the user has not
 *		just hit enter. Once we are sure we have a sensible input, it is
 *		tokenised and printed back out the console. The user can also exit the 
 *		program if he desires. 
 *
 *		The command shall be stored in a struct, as this can be easily 
 *		accessed and passed, although this has not yet been fully implemented 
 *		yet.
 *
 * 	v0.0.2 - 01/02/2014 - Reorganized 
 * 
 *		Mover the prompting of the user; getting input and tokenizing it to 
 *		a new function: getInput().
 *
 *	v0.0.3 - 02/02/2014 - Loop improvement
 *
 *		Changed while loop to a do while loop. Removes the need for an if check
 *		in main for when an INPUT_EXIT is returned. While check now replaces it. 
 *
 ******************************************************************************/
#define VERSION "v0.0.2. Last Update 30/01/2014\n"

#include <stdio.h>
#include <string.h>

#define PROMPT "> "
#define INPUT_EXIT 0
#define INPUT_CONTINUE 1
#define INPUT_RUN 2
#define INPUT_ERROR 4

/* This structure holds the command entered by the user.
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
 	fgets(input, 512, stdin);

 	//Checking user has not just hit enter
 	if (input[0] == '\n')
    	return INPUT_CONTINUE;

 	//Getting rid of the new line char, replacing with a terminating char
 	if ((p = strchr(input, '\n')) != NULL)
 		*p = '\0';

 	//To ensure its been taken in. Shall be removed in future
 	printf("The input: '%s'\n", input);

 	//tokenising 
 	char *tokenizer = " ";
 	char *token;
 	command->input_command = strtok(input, tokenizer);
 	printf("The command: '%s'\n", command->input_command);

 	//exit check
    if(strcmp(command->input_command, "exit") == 0){ 
 		return INPUT_EXIT;
 	}

 	int i = 0;
 	printf("The parameters: ");
 	while ( (token = strtok(NULL, tokenizer) ) != NULL){
 		//printf("%s\n", token);
 		// command.args[i] = token;
 		strcpy(command->args[i], token);
 		printf("'%s' ", command->args[i]);
 		i++;
 	}
 	printf("\n");

 	return INPUT_RUN;
}

int main(){

	int return_val = -1;

	//user loop
	do{
		user_command command = {
			NULL
		};

		return_val = getInput(&command);
		if (return_val == INPUT_CONTINUE) {
			continue;
		} else if (return_val == INPUT_ERROR) {
			printf("I broke");
			continue;
		}
 	} while(return_val != INPUT_EXIT);

 	return(0);
}
