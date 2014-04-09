
// Constants
#define PROMPT "> "
#define INPUT_CONTINUE 0
#define INPUT_RUN 1
#define INPUT_ERROR 2


// Maco's
#define LENGTH(x) (sizeof(x)/sizeof(x[0]))  //number of elements in array
#define SET_HOME_DIR() chdir(getenv("HOME")) //Set current dir as home dir
#define GET_PATH() getenv("PATH") //Gets current PATH of system. 
#define SET_PATH_STRING(path) setenv("PATH", path, 1) //Sets current PATH


// Forword declarations
void process_input(); //Forward declaration to be used in invoke_previous()
int alias_exists(char * target);
int tokenise(char *input);


// Alias struct
typedef struct{
	char * alias;
	char * aliased_command;
} Alias;