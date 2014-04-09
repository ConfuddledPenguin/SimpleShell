
#define PROMPT "> "
#define INPUT_CONTINUE 0
#define INPUT_RUN 1
#define INPUT_ERROR 2

#define LENGTH(x) (sizeof(x)/sizeof(x[0]))  //number of elements in array

void process_input(); //Forward declaration to be used in invoke_previous()
int alias_exists(char * target);
int tokenise(char *input);

typedef struct{
	char * alias;
	char * aliased_command;
} Alias;