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
 *	v0.1.1 - 07/02/2014 - Input Processing
 *
 *		Created a new function to process the user Input and run an executable
 *		file stored on the user's computer using the fork(), waitpid() & execv()
 *		functions. The program can currently execute executable files within 
 *		the current directory and it's subdirectories. See issues for bugs.
 *		
 *		- Thomas
 *
 *	v0.1.2 - 07/02/2014 - Further Input Processing
 *
 *		Changed execv() to execvp() to allow the SimpleShell to exectute commands
 *		from the PATH enviroment. Any programs stored locally can be ran with 
 *		" ./ ". Also added more commentary throughout the program to explain
 *		what it is doing.
 *
 *		- Thomas
 *
 *	v0.1.3 - 11/02/2014 - Fixed C99 Warnings
 *
 *		Added " #include <unistd.h> " to remove warnings from C99 when compiling.
 *
 *		- Thomas
 *
 *	v0.1.4 - 11/02/2014 - Invalid Input Filtering
 *
 *		Enclosed execvp() in an if statement so that when it returns "-1" indicating
 *		that it has failed due to invalid input being entered, the child process
 *		is killed allowing the parent to continue therefore stopping "Shellception".
 *
 *		- Thomas
 *
 *	v0.1.5 - 11/02/2014 - Passing Input to Commands
 *
 *		Set execvp() to take process the arguments stored in command.args. To do
 *		this, args[50][50] within the user_command struct had to be changed to
 *		args[50] as execvp() could not handle the data type. I am now using
 *		malloc within processInput() to assign each token it's space in memory.
 *		We also have to work out where and how to free any memory assigned by
 *		malloc in order avoid any problems with memory in out systems.
 *		<stdlib.h> was included to allow malloc to be used.
 *
 *		I also had to make another change within the processInput() function
 *		as the command must be stored in args[0] with the arguments stored in
 *		args[1] and onwards when it is passed into execp(). This has to be tidied
 *		up as input_command is still being used in some places when it could probably
 *		be completely removed.
 *
 *		The SimpleShell can now successfully carry out most built in unix commands
 *		and run executable files. It can also pass in any required parameters.
 *
 *		- Thomas		
 *
 ******************************************************************************/
#define VERSION "v0.1.5. Last Update 11/02/2014\n"

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

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
 * char args[50]		- The arguments of a command.
 *
 */
typedef struct
{
	char *input_command;
	char *args[50];
} user_command;

void freeMemory(user_command *command) {

	int i = 1;

	while(command->args[i] != NULL) {

		free(command->args[i]);

		i++;

	}

}

/* void processInput(user_command *command)
 *
 * #include <sys/types.h>
 * #include <sys/wait.h>
 * #include <unistd.h>
 * #include <signal.h>
 *
 * Description:
 *
 * Takes in the user's input and runs the relevant executable file or
 * built in command.
 *
 */
void processInput(user_command *command) {

	pid_t PID; //Process ID

	PID = fork();

	if(PID > 0) { //if parent process

		printf("Parent PID: %d\n", PID); //testing
		puts("Parent Waiting"); //testing
		wait(NULL); //wait for child to exit
		puts("Child Done"); //testing
		printf("Parent PID: %d\n", PID); //testing

	} else if(PID == 0) { //else must be child process

		printf("Child PID: %d\n", PID); //testing
		if(execvp(command->args[0], command->args) == -1) { //if execvp fails
			puts("Input not recognised");
			kill(getpid(), SIGKILL); //kill child process
		}

	} else { //fork failed

		puts("Something went horribly wrong :/"); //whoops :/

	}

	freeMemory(command);

} //end processInput

/* int getInput(user_command *command)
 * 
 * #include <stdio.h>
 * #include <string.h>
 * #include <stdlib.h>
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
 	char *tokenizer = " \t";
 	char *token;
 	command->input_command = strtok(input, tokenizer);
 	if(command->input_command == NULL)
 		return INPUT_CONTINUE;

 	//ensures command has been taken in. Shall be removed in future
 	printf("The command: '%s'\n", command->input_command);

 	//exit check
 	if(strcmp(command->input_command, "exit") == 0) 
 		return INPUT_EXIT;

 	int i = 1;
 	command->args[0] = malloc(50);
 	command->args[0] = command->input_command;

 	printf("The parameters: "); // Ensuring parameters. Shall be removed in future
 	while ( (token = strtok(NULL, tokenizer) ) != NULL) {

 		command->args[i] = malloc(50);

 		strcpy(command->args[i], token);
 		printf("'%s' ", command->args[i]); // Ensuring parameters. Shall be removed in future
 		i++;

 		if (i >= 50) {

 			printf("Error: Too many parameters\n");
 			return INPUT_CONTINUE;
 		}
 	}
 	printf("\n");

 	processInput(command); //user input all processed and stored, now carry it out.

 	return INPUT_RUN;
}

int main(){

	int return_val = -1;

	//user loop
	while (1) {
		//initializes the new command that the user wishes to execute
		user_command command = {
			NULL, {NULL}
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
