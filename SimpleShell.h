// Constants
#define PROMPT "> "

#define INPUT_CONTINUE 0
#define INPUT_RUN 1

// Array sizes
#define SIZE_OF_ALIASES 10
#define SIZE_OF_HISTORY 20
#define SIZE_OF_COMMANDS 50


// Macro's
#define LENGTH(x) (sizeof(x)/sizeof(x[0]))  //number of elements in array
#define SET_HOME_DIR() chdir(getenv("HOME")) //Set current dir as home dir
#define GET_PATH() getenv("PATH") //Gets current PATH of system. 
#define SET_PATH_STRING(path) setenv("PATH", path, 1) //Sets current PATH

/* The following macro is used to remove any extra characters from stdin if it
 * overflows preventing the user from entering more characters than the
 * character limit. Source: http://goo.gl/IO3ojg
 */
#define FLUSH_STDIN(x) {if(x[strlen(x)-1]!='\n'){do fgets(Junk,16,stdin); \
						while(Junk[strlen(Junk)-1]!='\n');} \
						else x[strlen(x)-1]='\0';}

// Forward declarations
void process_input(); 
int alias_exists(char * target);
int tokenise(char *input);


// Alias struct
typedef struct{
	char * alias;
	char * aliased_command;
} Alias;
