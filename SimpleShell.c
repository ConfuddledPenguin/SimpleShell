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
 *		Also added the command, pwd - This prints the
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
 *	v0.6 - 06/03/2014 - Stage Six
 *		
 *		GRANT WILL DO THIS BIT
 *
 ******************************************************************************/

#define VERSION "Simple Shell v0.6.0. Last Update 06/03/2014\n"

//To allow kill() to compile in linux without error
#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE
#endif

//to allow setenv() to compile in linux without error
#ifndef _BSD_SOURCE
#define _BSD_SOURCE
#endif

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <errno.h> 
#include <sys/stat.h>


#define PROMPT "> "
#define INPUT_EXIT 0
#define INPUT_CONTINUE 1
#define INPUT_RUN 2
#define INPUT_ERROR 3


#define SIZE(x) (sizeof(x)/sizeof(x[0])) //number of elements in array


void process_input(); //Forward declaration to be used in invoke_previous()

/* char *history[20]
 * 
 * Description:
 *
 * An array of 20 strings which store the previous 20 inputs from the user.
 *
 */
char history[20][512];

/* int count_history
 *
 * Description:
 *
 * Stores the next available position in the history array.
 *
 */
int count_history;

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

 	char *tokenizer = " \t|<>";
 	char *token;

 	//free_memory();

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
 *
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

/* char *setPathString()
 *
 * #include <string.h>
 * #include <stdlib.h>
 *
 * Description:
 *
 * Sets the current PATH of the system
 *
 */
void setPathString(char *path){
	setenv("PATH", path, 1);
}

/* void setPath()
 *
 * #include <stdlib.h>
 *
 * Description:
 *
 * Adds new directories specified by the user into the system's
 * PATH enviroment.
 *
 */
void setPath() {

	struct stat sb;

	if(command[1] == NULL || command[2] != NULL){

		printf("ERROR: Wrong Usage - setpath <PATH>\n");
		return;
	}

	if (stat(command[1], &sb) == 0 && S_ISDIR(sb.st_mode)){
		if(setenv("PATH", command[1], 1) == -1)
			puts("PATH update failed");
	}else {
		printf("ERROR: <%s> is not a directory\n", command[1]);
	}

} //end setpath()

/* void print_history()
 * 
 * Description:
 *
 * Prints the previous 20 inputs from the user.
 *
 */
void print_history(){
	int i = 0;
	while(i < count_history){ //Ensures empty history isn't printed.
		printf("%i. %s\n", i+1, history[i]);
		i++;
	}

}

/* void invoke_previous(int index)
 *
 * Description:
 *
 * Invokes a previous command from the history.
 *
 */
void invoke_previous(int index){

	char temp[513];

	if(index >= 0 && index <= 19){ //Checks if index is between 0 and 19.
		if(strcmp(history[index], "") == 0){ //Checks if there is history
			
			puts("No command in history.");

		} else{
			
			strcpy(temp, history[index]); //Copies history[index] to temp
			int index = 1;
			
			while(command[index] != NULL) {
				
				sprintf(temp, "%s ", temp); //Tidiness
				strcat(temp, command[index]); //Adds commands after !! to temp
				index++;

			}
			
			puts(temp);
			if(tokenise(temp) == INPUT_RUN)
				process_input();
		}
	}
	else{
		puts("Invalid history invocation."); //The number put in is invalid.
	}
}

/* void invoke_history 
 *
 * Description:
 *
 * Determines whether or not the histoy invocation is of the previous command
 * or another command in history. The function also checks whether or not the
 * command is valid.
 *
 */
void invoke_history(){

	if(strcmp(command[0], "!!") == 0){
		invoke_previous(count_history-1);
	}
	else{

		char *position = strtok(command[0], "!"); //Copies command[0] w/o '!'
		char temp[SIZE(position)]; //A new string the same size as position.


		int index = atoi(position); /* Retrieves an int from position, if there
		is no int, then it is 0. */
		sprintf(temp, "%i", index); // Converts index back into its own string.

		/* If the two strings match, then the history invocation is valid. 
		 * This means that an input of "!1a" is not valid, while "!1" is.
		 */
		if(strcmp(temp, position) == 0)
			invoke_previous(index-1);
		else
			puts("Invalid history invocation.");
	}

}

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
		if(command[1] != NULL)
			puts("Too many arguments input");
		else
			print_working_dir();

	} else if(strcmp(command[0], "cd") == 0) {
		if(command[2] != NULL)
			puts("Too many arguments input");
		else
			change_directory();

	} else if(strcmp(command[0], "getpath") == 0) {

		puts(getPath());

	} else if(strcmp(command[0], "setpath") == 0) {

		setPath();

	} else if(strcmp(command[0], "history") == 0){

		print_history();

	} else if(command[0][0] == '!'){

		invoke_history();

	}
	else {

		run_external_cmd();

	}

} //end process_input()

/* void update(history)
 *
 * Description:
 * 
 * Adds the latest input into the command array. In the event of a fully array,
 * the contents of the array is shifted left (history[1] is now history[0]), 
 * so the first element is removed.
 *
 */
void update_history(char input[512]){
	if(count_history < SIZE(history)){

 		strcpy(history[count_history], input);
 		count_history++;

 	} else{

 		for(int i=1; i<SIZE(history); i++){ //Array contents shifted to left.
 			strcpy(history[i-1], history[i]);
 		}

 		strcpy(history[SIZE(history)-1], input);

 	}
}

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
 * 1

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

 	fflush(stdin);

 	// Checking user has not just hit enter
 	if (input[0] == '\n')
 		return INPUT_CONTINUE;

 	// Getting rid of the new line char, replacing with a terminating char
 	if ((p = strchr(input, '\n')) != NULL)
 		*p = '\0';

 	if(input[0] != '!')
 		update_history(input);

 	int return_val;
 	if( ( return_val = tokenise(input)  ) != INPUT_RUN ){

 		return return_val;
 	}

 	

 	process_input(); //user input all processed and stored, now carry it out.

 	return INPUT_RUN;

} // End of getInput()

int main() {

	printf(VERSION);
	printf("Created by: Thomas Maxwell, Thomas Sinclair, Grant Toghill" 
		" & Aidan O'Grady\n");
	printf("Copyright 2014.\n");

	char *path = getPath();
	set_home_dir();

	int return_val = -1;

	// Initializing the count_history.
	count_history = 0;

	for(int i=0; i<SIZE(history); i++)
		strcpy(history[i], "");

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
			printf("\nExiting the shell . . .\n\n");
			printf("PATH returned to: %s \n\n", getPath());
			break;
		} // User if
 	}; // Close Shell Loop
 	return(0);
}
