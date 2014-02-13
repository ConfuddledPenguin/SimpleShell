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
 *	v0.1.6 - 12/02/2014 - Memory Freeing
 *
 *		Added a function to free memory from malloc after each command has been
 *		executed.		
 *
 *		- Thomas
 *
 *	v0.1.7 - 13/02/2014 - Remove user_command
 *
 *		The user_command has been found to be redundant and unneccesary. The 
 *		variable char *args[50] has been used to replace the struct. The command
 *		will be in args[0] with the parameters proceeding it. The entire program
 *		has been gutted to reflect these changes, with function headers which
 *		take in a command as a parameter being altered and such.
 *
 *		- Aidan
 *
 *	v0.1.8 - 13/02/2014 - Fixing Aidan's Errors
 *
 *		Discovered that Aidan's changes had a few bugs which I went through and
 *		Fixed. There were problems running commands as the command was stored in
 *		args[0] then malloc was being used to assign the memory. The line assigning
 *		memory had simply been overlooked when making changes. Also added a function
 *		to reset args to NULL everytime the program loops round.
 *
 *		- Thomas
 *
 *
 ******************************************************************************/
#define VERSION "v0.1.8. Last Update 13/02/2014\n"

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

/* char *args[50]
 *
 * Description:
 *
 * An array of strings that stores a command and it's parameters.
 * 	-	args[0] stores the command.
 * 	-	args[1-X] stores the subsequent parameters.
 *
 */
char *args[50]; 


/* void freeMemory(user_command *command)
 *
 * Description:
 *
 * This function frees the memory allocated to the args array.
 *
 */

void freeMemory() {

	int i = 0;

	while(args[i] != NULL) {
		free(args[i]);
		i++;
	}

} //end freeMemory

/* void reset_args()
 *
 * Description:
 *
 * Resets the contents of args to NULL
 *
 */

void reset_args() {

	int i = 0;
	while(args[i] != NULL) {
		args[i] = NULL;
		i++;
	}

} //end reset_args

/* void processInput(char *args[50])
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
void processInput() {

	pid_t PID; //Process ID

	PID = fork();

	if(PID > 0) { //if parent process

		printf("Parent PID: %d\n", PID); //testing
		puts("Parent Waiting"); //testing
		wait(NULL); //wait for child to exit
		puts("Child Done"); //testing
		printf("Parent PID: %d\n", PID); //testing

	} else if(PID == 0){ //else must be child process

		printf("Child PID: %d\n", PID); //testing
		if(execvp(args[0], args) == -1) { //if execvp fails
			puts("Input not recognised");
			kill(getpid(), SIGKILL); //kill child process
		}

	} else { //fork failed

		puts("Something went horribly wrong :/"); //whoops :/

	}

	// freeMemory();

} //end processInput

/* int getInput(char *args[50])
 * 
 * #include <stdio.h>
 * #include <string.h>
 * #include <stdlib.h>
 *
 * Description:
 *
 * Prompts the user for an input and tokenises it. This modifies the args
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
 	args[0] = malloc(50);
 	args[0] = strtok(input, tokenizer);
 	if(args[0] == NULL)
 		return INPUT_CONTINUE;

 	//ensures command has been taken in. Shall be removed in future
 	printf("The command: '%s'\n", args[0]);

 	//exit check
 	if(strcmp(args[0], "exit") == 0) 
 		return INPUT_EXIT;

 	int i = 1;
 	printf("The parameters: "); // Ensuring parameters. Shall be removed in future
 	while ( (token = strtok(NULL, tokenizer) ) != NULL) {

 		args[i] = malloc(50);

 		strcpy(args[i], token);
 		printf("'%s' ", args[i]); // Ensuring parameters. Shall be removed in future
 		i++;

 		if (i >= 50) {

 			printf("Error: Too many parameters\n");
 			return INPUT_CONTINUE;
 		}
 	}

 	printf("\n");

 	processInput(); //user input all processed and stored, now carry it out.
 	return INPUT_RUN;
}

int main(){

	int return_val = -1;
	//user loop
	while (1) {

		reset_args(); //empty args

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
