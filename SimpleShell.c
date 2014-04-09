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
 *		Added functions to allow the user to call the function history to get
 *		history for the last 20 commands or less. THe program can now store the
 *		last 20 commands entered by the user and be able to recall them to use
 *		again with the commands ! or !!.
 *	
 *	v0.7 - 06/03/2014 - Stage Seven
 *	
 *		Added the functions openHistory() and saveHistory() to allow the user's
 *		history file to be read and saved after each use of the SimpleShell so
 *		the history of the commands they have used is kept over multiple uses.
 *
 *	v0.8 - 27/03/2014 - Stage Eight
 *	
 *		Added the functions alias(), add_alias(), alias_exists(), unalias()
 * 		print_alias() to add the ability for the user to add alias during their
 *		use of the shell. Changes to tokenise() where needed to check if their
 *		input is an alias before the line is fully tokenised. It is also
 *		possible for the user to remove alias that they had previously created.
 *
 *	v1.0 - 09/04/2014 - Stage Nine
 *	
 *		Added functions loadAlias and saveAlias that bth allow the storage of  
 * 		aliases in a .aliases file in the users home directory so that
 *		that with each use of the shell any aliases that esist when it is closed
 *		is kept stored for the next use of the program.
 *
 *	v1.1 - 09/04/2014 - New Macros
 *
 *		Changed SET_HOME_DIR(), GET_PATH(), SET_PATH_STRING() functions into 
 *		seperate macros for each function. Each funtion was onle one line of
 *		code so having it as a marco makes it a bit more maintainable.
 *
 *	v1.2 - 09/04/2014 - Header File
 *
 *		New header file called SimpleShell.h has been created to store the
 *		macros used in the program, the contants, forward declerations as
 *		well as the struct used for aliases created for the shell.
 *
 ******************************************************************************/

#define VERSION "Simple Shell v1.1.0. Last Update 09/04/2014\n"
#define AUTHORS "Created by: Thomas Maxwell, Thomas Sinclair, Grant Toghill" \
				" & Aidan O'Grady\n"
#define COPYRIGHT "Copyright 2014.\n"


//To allow kill() to compile in linux without error
#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE
#endif

//to allow setenv() to compile in linux without error
#ifndef _BSD_SOURCE
#define _BSD_SOURCE
#endif

//Include required files
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

//Include SimpleShell header file
#include "SimpleShell.h"


/* char *history[20]
 * 
 * Description:
 *
 * An array of 20 strings which store the previous 20 inputs from the user.
 *
 */
char history[20][512];

/* int count_history, count_alias
 *
 * Description:
 *
 * Stores the next available position in the history and aliases arrays.
 *
 */
int count_history, count_alias; 

/* Alias aliases[10]
 *
 * Description:
 *
 * Stores 10 aliases.
 *
 */
Alias aliases[10];

/* char *command[50], path
 *
 * Description command[50]:
 *
 * An array of strings that stores a command and it's parameters.
 * 	-	command[0] stores the command.
 * 	-	command[1-X] stores the subsequent parameters.
 *
<<<<<<< HEAD
 *
 * Description path:
 *
 * Stores the orginal path, from before the shell is started
 * This is stored here due to the fact that passing it would become so very
 * messy
 *
=======
 */
char *command[50];

/* char *path
 *
 * Description:
 *
 * Stores the orginal path, from before the shell is started
 * 
>>>>>>> final_commentary
 */
char *command[50], *path;

/* void update_history(char input[512])
 *
 * Description:
 * 
 * Adds the latest input into the command array. In the event of a fully array,
 * the contents of the array is shifted left (history[1] is now history[0]), 
 * so the first element is removed.
 *
 */
void update_history(char input[512]){

	if(count_history < LENGTH(history)){

 		strcpy(history[count_history], input);
 		count_history++;

 	} else{

 		for(int i=1; i<LENGTH(history); i++) //Array contents shifted to left.
 			strcpy(history[i-1], history[i]);

 		strcpy(history[LENGTH(history)-1], input);

 	}
} //end updateHistory()

/* void loadHistory()
 *
 * Description:
 *
 * Opens the file .hist_list if it exists and if it exists then take the data
 * from the file and puts it into the history array until there is no more
 * data left to take from the file.
 *
 */
void loadHistory(){

	FILE *fp;
	char c[513], *p;
 	int h = 0;

 	/* If .hist_list exists, read it and add to history.
 	   If .hist_list doesn't exist, create it and skip
 	   adding to history as the file is empty therefore
 	   there is nothing to add. 
 	 */
	if((fp = fopen(".hist_list", "r")) == NULL){ //find or create file
		puts("Creating new history file in home directory");
	} else { //file exists, read it.

		while (1) {
			if((fgets(c, 513, fp)) == NULL){ //end of file check
				break;
			}

			// Getting rid of the new line char, replacing with a terminating 
			// char
			if ((p = strchr(c, '\n')) != NULL)
			*p = '\0';

			update_history(c);

			h++;
	 	}

	 	fclose(fp);

	}

} //end loadHistory()

/* void saveHistory()
 *
 * Description:
 *
 * Opens the file .hist_list or creates the file if none exists. It takes the
 * contents of the history array and puts each bit of data on a new line in 
 * order from the start of the array. When there is nothing left to be copied 
 * from the array then the file if closed.
 *
 */
void saveHistory(){

	FILE *fp;

	fp = fopen(".hist_list", "w+");

 	int g = 0;
	while(strcmp(history[g], "") != 0){ //Ensures empty history isn't printed.
		fprintf(fp, "%s\n", history[g]);
		g++;
	}
 	

 	fclose(fp);
} //end saveHistory()

/* void loadAlias()
 *
 * Description:
 *
 * Opens the file .aliases if it exists and if it exists then take the data
 * from the file and processes it to make a new alias until there is no more
 * data left to take from the file.
 *
 */
void loadAlias(){

	FILE *fp;
	char c[513];
	char *p;
 	int h = 0;

 	/* If .aliases exists, read it and add to list of aliases.
 	   If .aliases doesn't exist, create it and skip
 	   adding to history as the file is empty therefore
 	   there is nothing to add. 
 	 */
	if((fp = fopen(".aliases", "r")) == NULL){ //find or create file
		puts("Creating new alias file in home directory");
	} 
	else { //file exists, read it.

		while (1) {
			char *temp = malloc(sizeof(temp));
			strcpy(temp, "alias");
			if((fgets(c, 513, fp)) == NULL){ //end of file check
				break;
			}

			// Getting rid of the new line char, replacing with a terminating 
			// char
			if ((p = strchr(c, '\n')) != NULL)
			*p = '\0';

			strcat(temp, " ");
			strcat(temp, c);

			int return_val;
			if( ( return_val = tokenise(temp)  ) == INPUT_RUN ){
				process_input();
 			}

			h++;
	 	}

	 	fclose(fp);

	}

} //end loadAlias()

/* void saveAlias()
 *
 * Description:
 *
 * Opens the file .aliases or creates the file if none exists. It will take all the
 * contents of the alias stucts if any exist and print them out into the file with
 * each line containing the alias name and the command that it maps to seperated by
 * a space. Once all the alias are printed to the file, the file is closed.
 *
 */
void saveAlias(){

	FILE *fp;

	fp = fopen(".aliases", "w+");

	int i = 0;
	while(i < count_alias && count_alias != 0){
		fprintf(fp, "%s %s\n", aliases[i].alias, aliases[i].aliased_command);
		i++;
	}

 	fclose(fp);
} //end saveAlias()

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
 * int INPUT_RUN		- If it was successful
 * int INPUT_ERROR		- If something has gone drastically wrong
 * int INPUT_CONTINUE	- If the entered command should not be processed.
 * 
 */
int tokenise(char *input){

 	char *tokenizer = " \t|<>";
 	char *token;
 	char *inputCopy = malloc(sizeof(input));
 	strcpy(inputCopy, input);
 	command[0] = malloc(sizeof(command[0]));
 	command[0] = strtok(input, tokenizer);
 	if(command[0] == NULL)
 		return INPUT_CONTINUE;


 	/* If the input includes an alias invoke, then it must be processed. */
 	int index = alias_exists(command[0]);
 	if(index >= 0){
 		char *temp = malloc(sizeof(temp));
 		strcpy(temp, aliases[index].aliased_command); //The aliased command
 		while ( (token = strtok(NULL, tokenizer) ) != NULL) {
 			/* The rest of the input is added to the temp string.
 			 * This means that "alias x ls" will turn "x Desktop" to 
 			 * "ls Desktop. */
 			strcat(temp, " ");
 			strcat(temp, token);
 		}
 		return tokenise(temp); //Recursively call tokenise with the new command.
 	}

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
} //end tokenise(char *input)


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

		SET_HOME_DIR();

	} else { //absolute path

		if(chdir(command[1]) == -1)
			perror(command[1]);

	}

} //end change_directory()


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

} //end print_history()

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
} //end invoke_previous(int index)

/* void invoke_history()
 *
 * Description:
 *
 * Determines whether or not the history invocation is of the previous command
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
		char temp[LENGTH(position)]; //A new string the same size as position.


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

} //end invoke_history()

/* void print_alias
 *
 * Description:
 *
 * Prints out all the current existing aliases one by one, assuming that they
 * exist, otherwise, a message will be printed telling the user.
 *
 */
void print_alias(){
	int i = 0;
	if(count_alias == 0)
		puts("There are no aliases recognized.");
	while(i < count_alias){
		printf("%s: %s\n", aliases[i].alias, aliases[i].aliased_command);
		i++;
	}
} //end print_alias()

/* int alias_exists(char * target )
 *
 * Description:
 *
 * This function takes the string passed in, and looks through all existing 
 * aliases with the target as the name of the alias.
 *
 * Returns:
 *
 * i:	The index where the target is an alias.
 * -1:	The alias has not been found.
 *
 */
int alias_exists(char * target){
	for(int i = 0; i < count_alias; i++){
		if(strcmp(aliases[i].alias, target) == 0)
			return i;
	}
	return -1;
} //end alias_exists(char * target)

/* void add_alias()
 *
 * This function adds a new alias to the list of aliases in the program.
 * It ensures that an alias creation is valid (that there is both an alias and
 * aliased command, the alias array has space available and that an alias is not
 * being given to an existing alias.
 *
 * If the alias already exists, then it is overwritten with the new one.
 *
 */
void add_alias(){

	//Ensures there is an aliased command and that and the user is not aliasing
	//the same command, ie "alias a a" which shouldn't be allowed.
	if(command[2] == NULL || strcmp(command[1], command[2]) == 0){
		puts("Invalid alias");
		return;
	}
	
	int index = alias_exists(command[2]);
	//The user has attempted to give an alias an alias.
	if(index >= 0)
		puts("You cannot give an alias to an existing alias.");

	//Creating an alias.
	else{
		index = alias_exists(command[1]); 
		//Overwriting an existing one.
		if(index >= 0){
			puts("An alias with this name already exists, overwriting.");
			aliases[index].alias = command[1];
			int i = 2;
			//Every command token is concatenated to the aliased command. 
			strcpy(aliases[index].aliased_command, "");
			while(command[i] != NULL){
				strcat(aliases[index].aliased_command, command[i]);
				strcat(aliases[index].aliased_command, " ");
				i++;
			}
		}
		//Creating a brand new alias.
		else{
			if(count_alias >= LENGTH(aliases)){
				puts("List of aliases is full");
				return;
			}		
			aliases[count_alias].alias = command[1];
			int i = 2;
			strcpy(aliases[count_alias].aliased_command, "");
			//Every command token is concatenated to the aliased command. 
			while(command[i] != NULL){
				strcat(aliases[count_alias].aliased_command, command[i]);
				strcat(aliases[count_alias].aliased_command, " ");
				i++;
			}
			count_alias++; //Incrementing to reflect position of next alias.
		}
	}
} //end add_Alias()

/* void alias()
 *
 * Description:
 *
 * This knows that command[0] is "alias". The function determines whether or not
 * the user wishes to create an alias, or see all aliases based on whether or
 * not the next token is null or not.
 *
 */
void alias(){

	if(command[1] == NULL)
		print_alias();

	else
		add_alias();

} //end alias()

/* void un_alias()
 *
 * Description:
 *
 * Removing an alias. Ensuring that the alias exists in the first place, the
 * program shifts items in the aliases aaray left so that the alias to be
 * removed is overwritten.
 *
 */
void unalias(){

	if(command[1] == NULL)
		puts("No alias selected");
	else{
		int index = alias_exists(command[1]);
		if(index >= 0){
			for(int i=index+1; i<count_alias; i++){
				//The aliases after the removed one are shifted left by one.
	 			strcpy(aliases[i-1].alias, aliases[i].alias);
				strcpy(aliases[i-1].aliased_command,aliases[i].aliased_command);
 			}
			count_alias--;
		}
		else
			puts("Alias does not exist.");
	}
} //end unalias()

/* void exiting()
 *
 * Description:
 *
 * Exits the program
 */
void exiting(){

	SET_PATH_STRING(path);
	printf("\nExiting the shell . . .\n\n");
	printf("PATH returned to: %s \n\n", GET_PATH());
	saveHistory();
	saveAlias();
	exit(0);
} //end exiting()

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

		puts(GET_PATH());

	} else if(strcmp(command[0], "setpath") == 0) {

		setPath();

	} else if(strcmp(command[0], "history") == 0){

		print_history();

	} else if(command[0][0] == '!'){

		invoke_history();

	} else if(strcmp(command[0], "alias") == 0){

		alias();

	} else if(strcmp(command[0], "unalias") == 0){

		unalias();
		
	} else if (strcmp(command[0], "exit") == 0){

		exiting();
		
	} else {

		run_external_cmd();

	}

} //end process_input()

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
 * int INPUT_RUN		- If it was successful
 * int INPUT_ERROR		- If something has gone drastically wrong
 * int INPUT_CONTINUE	- If the entered command should not be processed.
 * 
 */
int getInput(){
	
	char inputCopy[513];
	char input[513];
	char *p;

	// Prompt user
 	printf("%s", PROMPT);

 	// Get input
 	if((fgets(input, 513, stdin)) == NULL){ //end of file check
 		printf("\n");
 		exiting();
 	}

 	//This may clear the input remove the issue of there being to many
 	//in the input
 	fflush(stdin);

 	// Checking user has not just hit enter
 	if (input[0] == '\n')
 		return INPUT_CONTINUE;

 	// Getting rid of the new line char, replacing with a terminating char
 	if ((p = strchr(input, '\n')) != NULL)
 		*p = '\0';

 	//Get copy of the string before tokenising
 	strcpy(inputCopy, input);

 	int return_val;
 	if( ( return_val = tokenise(input)  ) != INPUT_RUN ){

 		return return_val;
 	}

 	//Add the input to the history if possible
 	if(inputCopy[0] != '!')
 		update_history(inputCopy);

 	process_input(); //user input all processed and stored, now carry it out.

 	return INPUT_RUN;

} //end getInput()

/* int loadHistoryAlias()
 * 
 * #include <string.h>
 * #include <stdlib.h>
 *
 * Description:
 *
 * Intialises the history and alias storage and loads the files from the disk
 * 
 */
void loadHistoryAlias(){

	
	count_history = 0;
	count_alias = 0;

	for(int i=0; i<LENGTH(history); i++)
		strcpy(history[i], "");

	for(int i = 0; i < LENGTH(aliases); i++){
		aliases[i].alias = malloc(sizeof(aliases[i].alias));
		aliases[i].aliased_command = malloc(sizeof(aliases[i].aliased_command));
	}

	loadHistory();
	loadAlias();
} //end loadHistoryAlias()

int main() {

	printf(VERSION);
	printf(AUTHORS);
	printf(COPYRIGHT);

	path = GET_PATH();
	SET_HOME_DIR();

	loadHistoryAlias();

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
		} 
 	}; // Close Shell Loop

 	return(0);
}
