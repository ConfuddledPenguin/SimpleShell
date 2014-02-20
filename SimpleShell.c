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
 *		The structure from stage one was replaced with an array.
 *
 *	v0.3 - 14/02/2014 - Stage Three
 *
 *		The directory thats the shell starts in has been updated to the users
 *		home directory.
 *
 *		Also added the command, pwd - print working directory. This prints the
 *		users current directory.
 *
 *	v0.4 - 16/02/2014 - Stage Four
 *
 *		Getting and restoring of the PATH
 *
 *		Added the getpath and setpath commands to allow the user to interface
 *		with the PATH.
 *
 *	v0.5 - 16/02/2014 - Stage Five
 *
 *		Added the function change_directory() to allow the user to
 *		change directory within the simple shell through the use of the command
 *		cd.
 * 
 *
 *	v0.5.1 - 20/02/2014 - Moved tokenising
 *
 *		Move the tokenising code into its own method to facilitate the execution
 *		of commands stored in history.
 *		^ Tom
 *
 *	v0.5.2 - 20/02/2014 - Improved Error Messages
 *		
 *		Added perror() to change_directory() and print_working_directory() to
 *		print out the relevant errno message when the user enters incorrect
 *		information
 *		^ Thomas
 *
 ******************************************************************************/
#define VERSION "v0.5.2. Last Update 20/02/2014\n"

//To allow kill() to compile in linux without error
#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE
#endif

//to allow setenv() to compile in linux without error
#ifndef _BSD_SOURCE
#define _BSD_SOURCE
#endif


#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <errno.h> 


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

/* char *getPath()
 *
 * #include <string.h>
 * #include <stdlib.h>
 *
 * Description:
 *
 * Gets the current PATH of the system
 *
 * Returns:
 *
 * char *PATH	- the current PATH of the system 
 */
char *getPath(){

	return getenv("PATH");
}

/* char *setPath()
 *
 * #include <string.h>
 * #include <stdlib.h>
 *
 * Description:
 *
 * Sets the current PATH of the system
 *
 */
void setPath(char *path){

	setenv("PATH", path, 1);
}

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
 * #include <errno.h>
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
			perror(command[0]);
			kill(getpid(), SIGKILL); //kill child process
		}

	} else { //fork failed

		puts("Something went horribly wrong :/"); //whoops :/

	}
} //end run_external_cmd()

/* void set_home_dir()
 *
 * #include <stdlib.h>
 * 
 * Description:
 *
 * Set the current directory as the home directory
 *
 */
void set_home_dir() {

	chdir(getenv("HOME"));
} //end set_home_dir()

/* void print_working_dir()
 *
 * #include <unistd.h>
 *
 * Description:
 ** int INPUT_EXIT		- If user wishes to exit th* int INPUT_EXIT		- If user wishes to exit the program* int INPUT_EXIT		- If user wishes to exit the programe program
 * Print the current directory.
 *
 */
void print_working_dir() {

	char current_dir[100];

	puts(getcwd(current_dir, 100));
} //end print_working_dir()

/* void change_directory()
 *
 * #include <errno.h>
 *
 * Description:
 *
 * Navigate to a directory specified by the user.
 * If no parameters are input after the command then the
 * function sets the home directory as the current directory.
 * If the parameters contains a "/" at the begining it will
 * navigate from the root of the filesystem (absolute 
 * path) otherwise it will treat it as a relative path.
 *
 */
void change_directory() {

	if(command[1] == NULL) { //no parameters, navigate to HOME

		set_home_dir();

	} else { //absolute path

		if(chdir(command[1]) == -1)
			perror(command[1]);

	}

} //end change_directory()

/* void getpath()
 *
 * #include <stdlib.h>
 *
 * Description:
 *
 * Prints out the contents of the system's PATH
 * enviroment
 *
 */
void getpath() {

	puts(getenv("PATH"));

} //end getpath()

/* void setpath()
 *
 * #include <stdlib.h>
 *
 * Description:
 *
 * Adds new directories specified by the user into the system's
 * PATH enviroment.
 *
 */
void setpath() {

	char newpath[1000];

	sprintf(newpath, "%s:%s/", getenv("PATH"), command[1]);

	if(command[1] != NULL)
		if(setenv("PATH", newpath, 1) == -1)
			puts("PATH update failed");

} //end setpath()

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

		print_working_dir();

	} else if(strcmp(command[0], "cd") == 0) {

		change_directory();

	} else if(strcmp(command[0], "getpath") == 0) {

		getpath();

	} else if(strcmp(command[0], "setpath") == 0) {

		setpath();

	} else {

		run_external_cmd();

	}

	free_memory();

} //end process_input()

/*	int tokenise(char *input)
 *
 * #include <stdio.h>
 * #include <string.h>
 * #include <stdlib.h>
 *
 * Description:
 *
 * Tokenises the passed in string and places it in the global command array
 *
 * Returns:
 *
 * int INPUT_EXIT		- If user wishes to exit the program
 * int INPUT_RUN		- If it was successful
 * int INPUT_ERROR		- If something has gone drastically wrong
 * int INPUT_CONTINUE	- If the entered command should not be processed.
 * 
 */
int tokenise(char *input){

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

 	return INPUT_RUN;
} //  End tokenise(char *input)

/* int getInput()
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
	
	char input[513];
	char *p;

	// Prompt user
 	printf("%s", PROMPT);

 	// Get input
 	if((fgets(input, 513, stdin)) == NULL){ //end of file check
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
 	int return_val;
 	if( ( return_val = tokenise(input)  ) != INPUT_RUN ){

 		return return_val;
 	}

 	process_input(); //user input all processed and stored, now carry it out.
 	return INPUT_RUN;
} // End of getInput()

int main() {

	char *path = getPath();
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
			setPath(path);
			break;
		}

 	};
 	return(0);
}
