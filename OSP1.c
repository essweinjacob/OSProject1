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

void main(int argc, char* argv[]){
	// Use getopts to setup receiving command line arguments
	/* getopts prototype is (int argc, char * const argv[], const char *optstring)
	 aka getopts(number of command line 'strings', the 'strings', the options to
	 look for and what options to accept
	*/
	// Use while loop to cycle through comman line arguments/options 
	while((int option = getopt(argc, argv, "hI:hLtpiugsdl")) != -1){
		// Use switch case as multiple options. More efficient then if->else statements
		switch(option){
			// h - Print a help message and exit.
			case 'h':
			// In - indented by n spaces.
			case 'I':
			// L - Follow symbolic links, if any. Default will be to not follow any symbolic links.
			case 'L':
			// t - Print information on file type.
			case 't':
			// p - Print permission bits as rwxrwxrwx.
			case 'p':
			// i - Print the number of links to file in inode table.
			case 'i':
			// u - Print the UID associated with the file.
			case 'u':
			// g - Print the GID associated with the file.
			case 'g':
			// s - Print the size of file in bytes. If the size is larger than 1K, indicate the size in KB with a suffix K; if the size is larger than
			// 1M, indicate the size in MB with a suffix M; if the size is larger than 1G, indicate the size in GB with a suffix G.
			case 's':
			// d - Show the name of last modification.
			case 'd':
			// l - This option will be used to print formation on the file as if the options tpiugs are all specified
			case 'l':
			// Default case
			default:
		}
	}
	
	// Search directory
}
