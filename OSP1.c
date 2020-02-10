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
#include <dirent.h>	// For directories
#include <sys/stat.h>	// File information
#include <errno.h>
#include <grp.h>	// For GID
#include <pwd.h>	// FPR UID
#include <time.h>

int enQueue(char queue[20][80], int *rear, char data[80]);
int deQueue(char queue[20][80], int *front, int *rear);
char *formatDate(char *emptyDate, size_t size, time_t val);
void breadthFirst(char *dir, char dirQueue[20][80],  int *front, int *rear,  int depth, char *options);

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
				fprintf(stderr, "%s: Invalid option. Please use '-h' for reference of available options.\n", argv[0]);
				return EXIT_FAILURE;
		}
	}
	
	// Set up for searches, we will want the project to run on queues(FIFO) as they are similar to breadth first order
	// Variables for queue
	char *parentDir, *targetDir, currentDir[2]=".";
	int front = -1;		// -1 as there are not elements in array initally
	int rear = -1;	
	char dirQueue[20][80];	// Array of directories
	// If no directory is given
	if(argv[optind] == NULL){
		char defDir[4096];		// Default directory is the directory that the file is currently in
		getcwd(defDir, sizeof(defDir));
		parentDir = defDir;
		targetDir = currentDir;
		enQueue(dirQueue, &rear, parentDir);
	        breadthFirst(dirQueue[rear], dirQueue, &front, &rear,  0, optList);
	}
	// If user gives a directory
	else{
		enQueue(dirQueue, &rear, argv[optind]);		// Last element in the command arguments will be the directory so we add it to the queue
		breadthFirst(dirQueue[rear], dirQueue, &front, &rear, 0, optList);
	}

	return 0;
}

/*============================================================================================================================
 * enQueue()
 * This function will put elements/strings/directories into a queue
 * ========================================================================================================================*/
int enQueue(char queue[20][80],  int *rear, char data[80]){
	if(*rear == 20-1)	// If the queue is full
		return(-1);
	else{
		*rear = *rear + 1;	// Change the rear location
		strcpy(queue[*rear], data);	// Copy data into queue
		return(1);
	}
}

/*============================================================================================================================
 * deQueue()
 * This function will remove items from the queue
 * ==========================================================================================================================*/
int deQueue(char queue[20][80], int *front, int *rear){
	if(*front == *rear){		// If the the queue is empty
		return(-1);
	}
	else{
		(*front)++;
		return(1);
	}
}
/*============================================================================================================================
formatDate
This function will take in the st_mtime and format the date
============================================================================================================================*/
char *formatDate(char *emptyDate, size_t size, time_t val){
	strftime(emptyDate, size, "%b %d, %Y", localtime(&val));
	return emptyDate;
}
/*============================================================================================================================
 * breadthFirst()
 * This function will search directories recursively in breadth first order
 *===========================================================================================================================*/
void breadthFirst(char *dir, char dirQueue[20][80],  int *front, int *rear,  int depth, char *optList){
	DIR *dp;	// Directory stream type
	struct dirent *entry;	// Directory traversing
	struct stat fileInfo;	// File information
	
	int spaces = depth * 4;	// Spacing for the identation depth
	if((dp = opendir(dir)) == NULL){
		fprintf(stderr, "%*sERROR: %s\n", spaces, "", strerror(errno));
		return;
	}

	chdir(dir);		// Change directories
	char cwd[4096];
	getcwd(cwd, sizeof(cwd));
	printf("%*sNow scanning: %s\n",spaces, "",  dir);
	while((entry = readdir(dp)) != NULL){
		stat(entry->d_name, &fileInfo);
		// Ignore certain files
		if(strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0 || strcmp(".git", entry->d_name) == 0){
			continue;
		}
		// If the item is a file
		if(S_ISREG(fileInfo.st_mode)){
			// Stuff here
		// If the item is a directory
		}else if(S_ISDIR(fileInfo.st_mode)){
			enQueue(dirQueue, rear, entry->d_name);		// Add it to queue
		}
		printf("%*s%s   ", spaces, "", entry->d_name);
		// Go through options for files required things
		char permissionBits[11] = "";	// Permission bits
		struct passwd *uid;		// UID
		struct group *gid;		// GID
		long long int fileSize = (long long)fileInfo.st_size;
		char *byteSuffix = "";
		char date[20];
		char *fileType = "";
		int i;
		for(i = 0; i < strlen(optList); i++){
			switch(optList[i]){
				// Permissions on file
				case 'p':
					// Check what type of item it is and append correct bit to permissionBits
					if(S_ISREG(fileInfo.st_mode))
						strcat(permissionBits, "-");
					else if(S_ISDIR(fileInfo.st_mode))
						strcat(permissionBits, "d");
					else if(S_ISLNK(fileInfo.st_mode))
						strcat(permissionBits, "l");
					// Use teranry statements for easy to read code and use few lines/if statments
					// Check bits and append bits for user group, and other, read write and execute permissions
					(fileInfo.st_mode & S_IRUSR) ? strcat(permissionBits, "r") : strcat(permissionBits, "-");	
					(fileInfo.st_mode & S_IWUSR) ? strcat(permissionBits, "w") : strcat(permissionBits, "-");
					(fileInfo.st_mode & S_IXUSR) ? strcat(permissionBits, "x") : strcat(permissionBits, "-");

					(fileInfo.st_mode & S_IRGRP) ? strcat(permissionBits, "r") : strcat(permissionBits, "-");
					(fileInfo.st_mode & S_IWGRP) ? strcat(permissionBits, "w") : strcat(permissionBits, "-");
					(fileInfo.st_mode & S_IXGRP) ? strcat(permissionBits, "x") : strcat(permissionBits, "-");

					(fileInfo.st_mode & S_IROTH) ? strcat(permissionBits, "r") : strcat(permissionBits, "-");
					(fileInfo.st_mode & S_IWOTH) ? strcat(permissionBits, "w") : strcat(permissionBits, "-");
					(fileInfo.st_mode & S_IXOTH) ? strcat(permissionBits, "x") : strcat(permissionBits, "-");
					
					printf("%s   ",permissionBits);
					break;
				case 'u':
					uid = getpwuid(fileInfo.st_uid);
					if(uid != NULL)
						printf("%s   ", uid->pw_name);
					else
						printf("%s   ", fileInfo.st_uid);
					break;
				case 'g':
					gid = getgrgid(fileInfo.st_gid);
					if(gid != NULL)
                                                printf("%s   ", gid->gr_name);
                                        else
                                                printf("%s   ", fileInfo.st_gid);

					break;
				case 's':
					if(fileSize >= 1000000000){
						fileSize = (long long)(fileSize/1000000000);
						byteSuffix = "G";
					}else if(fileSize >= 1000000){
						fileSize = (long long)(fileSize/1000000);
						byteSuffix = "M";
					}else if(fileSize >= 1000){
						fileSize = (long long)(fileSize/1000);
						byteSuffix = "K";
					}else
						byteSuffix = "B";
					printf("%d%s   ", fileSize, byteSuffix);
					break;
				case 'd':
					printf("%s   ",formatDate(date, sizeof(date), fileInfo.st_mtime));
					break;
				case 'i':
					printf("%d   ",(unsigned int)fileInfo.st_nlink);
					break;
				case 't':
					if(S_ISREG(fileInfo.st_mode))
						fileType = "file";
					else if(S_ISDIR(fileInfo.st_mode))
						fileType = "directory";
					else if(S_ISLNK(fileInfo.st_mode))
						fileType = "symlink";
					printf("%s   ", fileType);
					break;
				case 'l':
					// For -t
					if(S_ISREG(fileInfo.st_mode))
                                                fileType = "file";
                                        else if(S_ISDIR(fileInfo.st_mode))
                                                fileType = "directory";
                                        else if(S_ISLNK(fileInfo.st_mode))
                                                fileType = "symlink";
                                        // For -p
                                       	if(S_ISREG(fileInfo.st_mode))
						strcat(permissionBits, "-");
					else if(S_ISDIR(fileInfo.st_mode))
						strcat(permissionBits, "d");
					else if(S_ISLNK(fileInfo.st_mode))
						strcat(permissionBits, "l");

					(fileInfo.st_mode & S_IRUSR) ? strcat(permissionBits, "r") : strcat(permissionBits, "-");
                                        (fileInfo.st_mode & S_IWUSR) ? strcat(permissionBits, "w") : strcat(permissionBits, "-");
                                        (fileInfo.st_mode & S_IXUSR) ? strcat(permissionBits, "x") : strcat(permissionBits, "-");

                                        (fileInfo.st_mode & S_IRGRP) ? strcat(permissionBits, "r") : strcat(permissionBits, "-");
                                        (fileInfo.st_mode & S_IWGRP) ? strcat(permissionBits, "w") : strcat(permissionBits, "-");
                                        (fileInfo.st_mode & S_IXGRP) ? strcat(permissionBits, "x") : strcat(permissionBits, "-");

                                        (fileInfo.st_mode & S_IROTH) ? strcat(permissionBits, "r") : strcat(permissionBits, "-");
                                        (fileInfo.st_mode & S_IWOTH) ? strcat(permissionBits, "w") : strcat(permissionBits, "-");
                                        (fileInfo.st_mode & S_IXOTH) ? strcat(permissionBits, "x") : strcat(permissionBits, "-");
					
					// For -u
					char *uidStr;
					uid = getpwuid(fileInfo.st_uid);
                                        if(uid != NULL)
                                                uidStr = uid->pw_name;
                                        else
                                                uidStr = fileInfo.st_uid;
					
					// For -g
					char *gidStr;
					gid = getgrgid(fileInfo.st_gid);
                                        if(gid != NULL)
                                                gidStr = gid->gr_name;
                                        else
                                                gidStr = fileInfo.st_gid;
					
					// For -s
					if(fileSize >= 1000000000){
                                                fileSize = (long long)(fileSize/1000000000);
                                                byteSuffix = "G";
                                        }else if(fileSize >= 1000000){
                                                fileSize = (long long)(fileSize/1000000);
                                                byteSuffix = "M";
                                        }else if(fileSize >= 1000){
                                                fileSize = (long long)(fileSize/1000);
                                                byteSuffix = "K";
                                        }else
                                                byteSuffix = "B";
	
					printf("%s   %s   %d   %s   %s   %d%s\n", fileType, permissionBits, (unsigned int)fileInfo.st_nlink, uidStr, gidStr, fileSize, byteSuffix);
					break;
				case 'L':
					enQueue(dirQueue, rear, entry->d_name);
					break;
			}
		}
		printf("\n");
	}
	deQueue(dirQueue, front, rear);
	if(*front != *rear){		// Continue until queue is 'clear'
		breadthFirst(dirQueue[*rear], dirQueue, front, rear, depth + 1 , optList);
	}
	closedir(dp);	// Clear directory stream
}
