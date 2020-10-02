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
int user_max_Concurrent_Children = 2; 
int time_seconds = 100;

//Shared memory
struct shared_memory {
	int t_processes;
	int turn; 
	int flags[20]; 
	int index; 
	char data[SIZE][LENGTH];
	int process_group;
};

struct shared_memory* shared_m; //Pointer to shared_memory struct (shared_m)

//Never have more than 20 processes in the system at any time even if program is invoked with n>20
#define MAX_CONCURRENT_CHILDREN 20;
int current_ConcurrentProcesses = 0; 
int shared_m_SegId; 

//Main function
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
	//Read from file, store  one string per line
	
	char line[LENGTH];
	//read line from stream and store it into string pointed by 'line'. Stops when either n characters are read or EOF 
	while(fgets(line, sizeof(line), fp)){
		//Terminating symbol at end of string is NULL character
		line[strlen(line) -1] = '\0';
		strcpy(shared_m->data[i], line); //Copy current line to shared_m data
		i++; //Increment i by 1
	}
	
	//Each child will test the string at the index assigned to it. The total child proceses are the amount of string lines. if i is greater, then max_total_ChildProcess is i else it will be default value.
	if(i < max_total_ChildProcess){ 
		max_total_ChildProcesses = i;
	}
	//user set concurrent children must be less than total child processes
	if(max_total_ChildProcess < user_max_Concurrent_Children) { 
		user_max_Concurrent_Children = max_total_ChildProcess;
	}
	//set total processes in shared memory
	shared_m->t_processes = max_total_ChildProcesses;
	//amount of times expected to spawn
	int spawn_count = max_total_ChildProcesses;
	//keep track of processes
	int track_processes
	while(spawn_count >= 0) { 
		//check that current amount of processes are less than user set value AND check that processes made so far are less than the max total children allowed to have 
		if(current_ConcurrrentProcesses < user_max_Concurrent_Children) && (track_processes < max_total_ChildProcess) { 
			current_ConcurrentProcesses += 1;
			if(fork() ==-1) { 
				printf("ERROR: failed to fork");
				
			}
			else if(fork() == 0) { 
				if(track_processes == 1){
					shared_m->process_group = getpid();
					//0 means the process ID of the calling process is used. setpgid joins process group with the session of the calling process
					setpgid(0, shared_m->process_group);
					char cpid[256];
					//set id and send to palin
					sprintf(cpid, "%d", track_processes + 1);
					execl("./palin", "palin", cpid, (char*) NULL);
					
				}	
			}
		}		
		spawn_count--;
	}	

	releaseMemory(); 
	return 0; 


}

//Signal to kill processes
void signalHandler(int signal) { 
	//Ask program to terminate
	killpg(shared_m->process_group, SIGTERM);
	releaseMemory();
	return EXIT_SUCCESS;
}

//Release shared memory 
void releaseMemory() { 
	//detaches shared memory segment
	shmdt(shared_m); 
	shmctl(shared_m_SegId, IPC_RMID, NULL);	
}
