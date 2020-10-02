#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>


//Constants
#define SIZE 50
#define LENGTH 256


//Global Variables
//Define defaults if no argument entered 
int max_total_ChildProcess = 4; 
int max_Concurrent_Children = 2; 
int time_seconds = 100;

//Shared memory
struct shared_memory {
	int id;
	int turn; 
	int flags[20]; 
	int index; 
	char data[SIZE][LENGTH];

};



int main(int argc, char* argv[]) {
	
	//Check that an argument and/or infile is given 
	if(argc <= 1) { 
		printf("Error: Too few arguments\n"); 
		return 1;
	}	
	
	//If an argument is given, parse command line options and execute the following instructions
	int opt; 
	
	while((opt = getopt(argc, argv, "hn:s:t:")) !=-1) { 
		switch(opt){
			
			//help 
			case 'h':
				printf("DESC:\n");
				printf("	- Read a list of palindromes from a file into shared memory and fork off processes.\n");
				printf("USAGE:\n");
				printf("	master -h\n");
				printf("	master [-n x] [-s x] [-t time] infile\n");
				printf("	-h 	help\n");
				printf("	-n x	Indicate the maximum total of child processes master will ever create (Default 4)\n");
				printf("	-s x	Indicate the number of children allowed to exist in the system at the same time (Default 2)\n");
				printf("	-t time The time in seconds after which the process will terminate, even if it has not finished (Default 100)\n");
				printf("	infile	Input file containing strings to be tested.\n");
				return EXIT_SUCCESS;  
			
			//Define maximum total of child processes
			case 'n':
				max_total_ChildProcess = atoi(optarg);
				//Check that max total child processes is between 1-20
				if (max_total_ChildProcess == 0) { 
					printf("ERROR: Child processes must be between 1-20\n");
					return EXIT_FAILURE;  
				}
				break;  
				
			//Define number of children
			case 's':
				max_Concurrent_Children = atoi(optarg); 
				//Check that max concurrent children is greater than 0 
				if(max_Concurrent_Children <= 0) { 
					printf("ERROR: Concurrent children processes must be greater than 0\n");
				return EXIT_FAILURE;
				}
				break;		

			//Define time when process will terminate
			case 't':
				time_seconds = atoi(optarg); 
				//Check that time is greater than 0
				if(time_seconds <=0) { 
					printf("ERROR: Time in seconds must be greater than 0\n");
				return EXIT_FAILURE;
				}
				break;

			//All other cases
			default: 
				printf("ERROR: Invalid parameter\n"); 
				return EXIT_FAILURE; 
		
		}
	}

	//Open file. Give error if unreadable 
	FILE *fp = fopen(argv[optind], "r"); //read from file 
	//File can't be opened 
	if (!fp) { 
		printf("ERROR: Could not open file\n");
		return EXIT_FAILURE; 
	}
	
	//File is opened
	//Read from file one string per line, store in array
	
	char line[LENGTH];
	//read line from stream and store it into string pointed by 'line'. Stops when either n characters are read or EOF 
	while(fgets(line, sizeof(line), fp)){
		//Terminating symbol at end of string is NULL character
		line[strlen(line) -1] = '\0';
		strcpy(
		
	}
	

		

	printf("Max total child process is %d\n", max_total_ChildProcess); 
	printf("Max concurrent children is %d\n", max_Concurrent_Children); 
	printf("Time in seconds is %d\n", time_seconds);	

	return 0;

}
