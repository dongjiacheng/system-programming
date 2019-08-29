/**
 * Lab: Utilities Unleashed
 * CS 241 - Spring 2017
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "format.h"
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	if(argc == 1)
	print_time_usage();
//	printf("size of %lu\n",sizeof(argv));
	char* excuteName = argv[1];
	char* arguments[argc];
	for(int i = 0; i < (argc-1); i++)
		arguments[i] = argv[i+1];
	arguments[argc-1] = NULL;
	pid_t id = fork();
	if(id == -1)
	{
		print_fork_failed();
		exit(1);
	}
	 
	struct timespec start,end;
    clock_gettime(CLOCK_MONOTONIC,&start);
	if(id!=0)
	{
//		int status;

//		waitpid(id,&status,0);//parent
		wait(NULL);
		puts("??");

      		clock_gettime(CLOCK_MONOTONIC,&end);
	}
	else	
	{
	execvp(excuteName,arguments);
	print_exec_failed();
	return 0;
	}
//	wait(NULL);
//	clock_gettime(CLOCK_MONOTONIC,&end);
	double duration = (double)(end.tv_sec-start.tv_sec)+((double)(end.tv_nsec - start.tv_nsec) / 1000000000);
	display_results(argv,duration);
	return 0;
}
