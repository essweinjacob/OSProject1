/*===============================================================================
# Author: Jacob Esswein
# Course: CMPSCI 4760-01 - Operating Systems
# File Name:
# Data Started: Febuary 2 2020
# Date Finished: Febuary 2020
# Purpose: The goal of this project is to become familiar with hoare
	   enviornment while praciticing system calls such as  perror getopts.
	
	   Specifically, we wil traverse directories using breadth-first order
	   as well as using command line arguments for options such as following:
		symbolic links, printing information of the file type, print
	   	permissions, etc.
	 
 	   We will also use perror for useful error messages.
===============================================================================*/
#include <stdio.h>	// Standard C libary
#include <stdlib.h>	
#include <string.h>	// Libary used for string operations (i.e strcat())
#include <stdbool.h> 	// So that we can use boolean operations
#include <getopt.h>	// Was running intp an error with 'optarg' not working without this library for some reason

int main(int argc, char* argv[]){
	// Set up default operation variables
	bool followSymLink = false;	// By default, this should not follow symbolic links
	int indentSpaces = 4;		// Default, indent by space
	char optList[11];		// Create a blank string of characters that will store the given options from user. Max that SHOULD be given is 11, based on the # of options that are available to the user. As our loop below cycles through the options, it will add the options to his array.
		
	// Use getopts to setup receiving command line arguments
	/* getopts prototype is (int argc, char * const argv[], const char *optstring)
	 aka getopts(number of command line 'strings', the 'strings', the options to
	 look for and what options to accept
	*/
	// Use while loop to cycle through comman line arguments/options 
	int option;
	while((option = getopt(argc, argv, "hI:hLtpiugsdl")) != -1){
		// Use switch case as multiple options. More efficient then if->else statements
		switch(option){
			// h - Print a help message and exit.
			case 'h':
				printf("This program is used to traverse a specified directory using breadth-first order.\n");
				printf("-h: Prints out a help message, displaying what each option does, and exits program.\n");
				printf("-In: Changes indentation for each 'level' of the directory. By default set at 4.\n");
				printf("-L: Will change setting to follow symbolic links. By default this program wont.\n");
				printf("-t: Prints information of file type.\n");
				printf("-p: Prints permission bits as rwxrwxrwx.\n");
				printf("-i: Prints the number of links to file in inode table.\n");
				printf("-u: Prints the UID associated with the file.\n");
				printf("-g: Prints the GID asscoiated witht he file.\n");
				printf("-s: Prints the size of the file.\n");
				printf("-d: Show the name of last modification.\n");
				printf("-l: Will be used to print information on the file as if the options 'tpiugs' are all specified.\n");
				// Once all option descriptions have been displayed, exit program.
				return EXIT_SUCCESS;
			// In - indented by n spaces.
			case 'I':
				indentSpaces = atoi(optarg);	// Change indentation spaces given by the number, atoi will grab optarg and change it from a string/char and turn it into a int
				break;
			// L - Follow symbolic links, if any. Default will be to not follow any symbolic links.
			case 'L':
				followSymLink = true;
				break;
			// t - Print information on file type.
			case 't':
				strcat(optList, "t");
				break;
			// p - Print permission bits as rwxrwxrwx.
			case 'p':
				strcat(optList, "p");
				break;
			// i - Print the number of links to file in inode table.
			case 'i':
				strcat(optList, "i");
				break;
			// u - Print the UID associated with the file.
			case 'u':
				strcat(optList, "u");
				break;
			// g - Print the GID associated with the file.
			case 'g':
				strcat(optList, "g");
				break;
			// s - Print the size of file in bytes. If the size is larger than 1K, indicate the size in KB with a suffix K; if the size is larger than
			// 1M, indicate the size in MB with a suffix M; if the size is larger than 1G, indicate the size in GB with a suffix G.
			case 's':
				strcat(optList, "s");
				break;
			// d - Show the name of last modification.
			case 'd':
				strcat(optList, "d");
				break;
			// l - This option will be used to print formation on the file as if the options tpiugs are all specified
			case 'l':
				strcat(optList, "tpiugs");
				break;
			// Default case
			default:
				break;
		}
	}
	
	// Search directory
}
