/*
 *	The following program outputs the number of tokens input to
 *	the program then prints out each token one by one to test
 *	SimpleShell's ability to execute other programs and pass
 *	data to them.
 */

#include <stdio.h>

int main(int argc, char *argv[]) {

	printf("\nHello, %d token(s) input:\n", argc);

	for(int i = 0; i < argc; i++) {

		puts(argv[i]);

	}

	puts("[Program Complete]\n");	

	return(0);

}
