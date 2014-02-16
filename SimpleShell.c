 /******************************************************************************
 * Synopsis: This is a simple Unix like shell written in C, for the class CS210 
 *      at the University of Strathclyde, Glasgow. This was a group project to 
 *		be undertaken by four students.
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
 * 	v0.0 - 28/01/2014 - File birth
 *
 * 		Created initial file (SimpleShell.c) as well as read me files and a
 * 		github repo.
 *
 * 	v0.1 - 04/02/2014 - Stage One
 *
 *		Created the user loop, which gets input from the user. The input is
 *		check to make sure it is not absolute gibberish and tokenized. The
 *		result is stored in a structure called user_command.
 *
 *	v0.2 - 14/02/2014 - Stage Two
 *
 *		Added forking and executing of external system commands. This is 
 *		achieved using execvp().
 *
 *		The struct from stage one was replaced with an array.
 *
 *	v0.3 - 14/02/2014 - Ping Working Directory
 *
 *		Used the getcwd() function to get the current working directory and
 *		then print it out.
 * 
 ******************************************************************************/

#define VERSION "v0.3. Last Update 14/02/2014\n"

//To allow kill() to compile in linux without error
#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE
#endif


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

/* char *command[50]
 *
 * Description:
 *
 * An array of strings that stores a command and it's parameters.
 * 	-	command[0] stores the command.
 * 	-	command[1-X] stores the subsequent parameters.
 *
 */
char *command[50]; 


/* void free_memory(user_command *command)
 *
 * Description:
 *
 * This function frees the memory allocated to the command array.
 *
 */

void free_memory() {

	int i = 1;

	while(command[i] != NULL) {
		free(command[i]);
		i++;
	}

} //end free_memory()

/* void reset_command()
 *
 * Description:
 *
 * Resets the contents of command to NULL
 *
 */

void reset_command() {

	int i = 0;
	while(command[i] != NULL) {
		command[i] = NULL;
		i++;
	}

} //end reset_command()

/* void run_external_cmd(char *command[50])
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
void run_external_cmd() {

	pid_t PID; //Process ID

	PID = fork();

	if(PID > 0) { //if parent process

		wait(NULL); //wait for child to exit

	} else if(PID == 0){ //else must be child process

		if(execvp(command[0], command) == -1) { //if execvp fails
			puts("Input not recognised");
			kill(getpid(), SIGKILL); //kill child process
		}

	} else { //fork failed

		puts("Something went horribly wrong :/"); //whoops :/

	}

} //end run_external_cmd()

/* void set_home_dir()
 * 
 * Description:
 *
 * Set the current directory as the home directory
 *
 */
void set_home_dir() {

	chdir(getenv("HOME"));

} //end set_home_dir()

/* void ping_working_dir()
 *
 * Description:
 *
 * Print the current directory.
 *
 */
void ping_working_dir() {

	char current_dir[100];

	puts(getcwd(current_dir, 100));

} //end ping_working_dir()

/* void process_input()
 *
 * Description:
 *
 * Checks for built in commands. If none available
 * then run system command.
 *
 */
void process_input() {

	if(strcmp(command[0], "pwd") == 0) {

		ping_working_dir();

	} else {

		run_external_cmd();

	}

	free_memory();

} //end process_input()

/* int getInput(char *command[50])
 * 
 * #include <stdio.h>
 * #include <string.h>
 * #include <stdlib.h>
 *
 * Description:
 *
 * Prompts the user for an input and tokenises it. This modifies the command
 * array, and fills it with appropriate values.
 *
 * Returns:
 *
 * int INPUT_EXIT		- If user wishes to exit the program
 * int INPUT_RUN		- If it was successful
 * int INPUT_ERROR		- If something has gone drastically wrong
 * int INPUT_CONTINUE	- If the entered command should not be processed.
 * 
 */
int getInput(){
	
	char input[512];
	char *p;

	// Prompt user
 	printf("%s", PROMPT);

 	// Get input
 	if((fgets(input, 512, stdin)) == NULL){ //end of file check
 		printf("\n");
 		return INPUT_EXIT;
 	}

 	// Checking user has not just hit enter
 	if (input[0] == '\n')
 		return INPUT_CONTINUE;

 	// Getting rid of the new line char, replacing with a terminating char
 	if ((p = strchr(input, '\n')) != NULL)
 		*p = '\0';

 	// Tokenising 

 	char *tokenizer = " \t";
 	char *token;

 	command[0] = malloc(sizeof(command[0]));
 	command[0] = strtok(input, tokenizer);
 	if(command[0] == NULL)
 		return INPUT_CONTINUE;

 	/* Exit check
 	 *
 	 * To check if user wishes to exit the shell before continuing with 
 	 * tokenising
 	 */
 	if(strcmp(command[0], "exit") == 0) 
 		return INPUT_EXIT;

 	int i = 1;
 	while ( (token = strtok(NULL, tokenizer) ) != NULL) {

 		command[i] = malloc(sizeof(command[i]));
 		strcpy(command[i], token);
 		i++;

 		// Spec says no more than 50 parameters will be entered
 		if (i >= 50) {

 			printf("Error: Too many parameters\n");
 			return INPUT_CONTINUE;
 		}
 	}

 	process_input(); //user input all processed and stored, now carry it out.
 	return INPUT_RUN;
} // End of getInput()

int main() {

	set_home_dir();

	int return_val = -1;

	// User loop
	while (1) {

		reset_command(); // Empty command

		return_val = getInput();
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
