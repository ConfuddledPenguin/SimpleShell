/******************************************************************************
 * Synopsis: This is a simple unix like shell written in C, for the class CS210 
 *      at the University of Strathcylde. This was a group project to be 
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
 *****************************************************************************/

/********************* R E V I S I O N   H I S T O R Y *************************
 * 
 * v0.0 - 28/01/2014 - File birth
 *
 *      Created initial file (SimpleShell.c) as well as read me files and a
 *      github repo.
 *
 ******************************************************************************/
#define VERSION "v0.0. Last Update 30/01/2014\n"

#include <stdio.h>
#include <string.h>

int main(){

	int run = 1;
	char command[512];
	char *p;

	while (run){

 		printf("> ");
 		fgets(command, 512, stdin);
 		printf("%s", command);

 		if ((p = strchr(command, '\n')) != NULL)
    		p = '\0';
 		run = 0;
 	}

 return(0);
}