/**
 * Machine Problem: Shell
 * CS 241 - Fall 2016
 */
#include "format.h"
#include "shell.h"
#include "vector.h"
#include "string.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>


static pid_t little_child;
//static pid_t childBK;
static vector* processeS;
//volatile __sig_atomic_t pleaseStop;

typedef struct process {
  char *command;
  char *status;
  pid_t pid;
} process;



void *process_copy_constructor(void *elem) { 
	process* newone = calloc(1,sizeof(process));
	process* oldone = (process*)elem; 
	char* newCm;
	char* newSt;


	if (oldone->command != NULL)
	{
		newCm = strdup(oldone->command);
	}
	else
		newCm = NULL;
	if (oldone->status != NULL)
	{
	    newSt = strdup(oldone->status);
	}
	else
		newSt = NULL;
	newone->command = newCm;
	newone->status = newSt;
	newone->pid = oldone->pid;
	return newone;
}

void process_destructor(void *elem) {
  if(elem == NULL)
  	return;
  process* oldone = (process*)elem; 

	if (oldone->command != NULL)
		{
			free(oldone->command);
		}
	if (oldone->status != NULL)
	{
		free(oldone->status);
	}
	free(oldone);
  return;
}

void *process_default_constructor(void) { return calloc(1,sizeof(process)); }

vector *process_vector_create() {
return vector_create(process_copy_constructor, process_destructor,
                       process_default_constructor);
}

void changeDir(char* dEst)
{
char* temp = NULL;
if(*dEst != '/')
{
temp = get_full_path(dEst);
}
int valid;
if(temp == NULL)
valid = chdir(dEst);
else
valid = chdir(dEst);
if(valid != 0)
{
	if(temp == NULL)
	{
	print_no_directory(dEst);
	fflush(stdout);
	}
	else
	{
	print_no_directory(temp);
	fflush(stdout);
	}
}
free(temp);
return;
}

void cleanup(int sig)
{
    pid_t p;
    int status;

    while ((p=waitpid(-1, &status, WNOHANG)) > 0 )
    {
    	
      for(size_t i = 0; i < vector_size(processeS);i++)
		{
			process* theOne = vector_get(processeS,i);
			if(theOne->pid == p)
			{
				
				vector_erase(processeS,i);
			
			}
			
		} 

    }
}


void ExtCommand(char* commd,char** argV,size_t number, int BK,vector* processeS)
{	
	signal(SIGCHLD, cleanup);
	pid_t child = fork();
	if(child!=0 && BK == 0)
		little_child = child;
	if(child == -1)
	{
		print_fork_failed();
		fflush(stdout);
		exit(0);
	}

	if(child!=0)
	{
		print_command_executed(child);
		fflush(stdout);
		process newchild;
		newchild.command = commd;
		newchild.status = "STATUS_RUNNING";
		newchild.pid = child;
		vector_push_back(processeS,&newchild);
		int status;

		if(BK == 0)
		{
		pid_t L = waitpid(child,&status,0);
		 for(size_t i = 0; i < vector_size(processeS);i++)
		{
			process* theOne = vector_get(processeS,i);
			if(theOne->pid == L)
			{
				vector_erase(processeS,i);
			}
			
		} /* Handle the death of pid p */
		
		}
		else
		{
			waitpid(child, &status, WNOHANG);
		}
		
	}
	else	
	{
		//free
	vector_destroy(processeS);

	execvp(commd,argV);
	print_exec_failed(commd);
	exit(0);
	}
}


int biuldInCommand(char* commd,char** argV,size_t number, vector* processeS)
{
if(strcmp(commd,"cd") == 0)
{
	if(number != 1)
	{
		print_invalid_command(commd);
		fflush(stdout);
	}
	else
		changeDir(argV[1]);
		return 1;
}

if(strcmp(commd,"ps") == 0)
{
	if(number != 0)
		{
		print_invalid_command(commd);
		fflush(stdout);
		}
	else
		{
		for(size_t i = 0; i < vector_size(processeS);i++)
		{
			process* theOne = vector_get(processeS,i);
			if(strcmp("STATUS_RUNNING",theOne->status) == 0)
			{
				print_process_info(theOne->status,theOne->pid,theOne->command);
				fflush(stdout);
			}
		}
		}

		return 1;
}

if(strcmp(commd,"kill") == 0)
{	
	
	if(number != 1)
		{
		print_invalid_command(commd);
		fflush(stdout);
		}
	else
	{
		int valid;
		int id = atoi(argV[1]);
		valid = kill(id,SIGTERM);
	if(valid != 0)
		{
		print_no_process_found(id);
		fflush(stdout);
		}
	else
	{	
		process* theOne;
		int id = atoi(argV[1]);
		int loc = 0;
		//char* temp;
		for(size_t i = 0; i < vector_size(processeS);i++)
		{	
			theOne = (process*)vector_get(processeS,i);
			if(id == theOne->pid)
			{
				loc = i;
				//temp = theOne->command;
				break;
			}
		}
		print_killed_process(id, theOne->command);
		fflush(stdout);
		vector_erase(processeS, loc);
	}
	}

	return 1;
}

if(strcmp(commd,"stop") == 0)
{
	if(number != 1)
		{
		print_invalid_command(commd);
		fflush(stdout);
		}
	else
	{
		process* theOne;
		int valid;
		int id = atoi(argV[1]);
		int loc;
		valid = kill(id,SIGSTOP);
	if(valid != 0)
		{
		print_no_process_found(id);
		fflush(stdout);
		}
	else
	{	
		
		
		for(size_t i = 0; i < vector_size(processeS);i++)
		{
			theOne = (process*)vector_get(processeS,i);
			if(id == theOne->pid)
			{
				loc = i;
				strcpy(theOne->status,"STATUS_STOPPED");
				//temp = theOne->command;
				break;
			}
		}
		
		print_stopped_process(id, theOne->command);
		fflush(stdout);
		//strcpy(theOne->status,"STATUS_STOPPED");
	}
	}

	return 1;
}

if(strcmp(commd,"cont") == 0)
{
	if(number != 1)
		{
		print_invalid_command(commd);
		fflush(stdout);
		}
	else
	{
		process* theOne;
		int valid;
		int id = atoi(argV[1]);
		valid = kill(id,SIGCONT);
	if(valid != 0)
		{
		print_no_process_found(id);
		fflush(stdout);
		}
	else
	{	
		int id = atoi(argV[1]);
		int loc;
		//char* temp;
		for(size_t i = 0; i < vector_size(processeS);i++)
		{
			theOne = (process*)vector_get(processeS,i);	
			if(id == theOne->pid)
			{
				loc = i;
				//temp = theOne->command;
				break;
			}
		}
		fflush(stdout);
		strcpy(theOne->status,"STATUS_RUNNING");
	}
	}

	return 1;
}


return 0;
//exit 
}

void cleanOther(vector* processeS) {
  //int status;
  while(vector_size(processeS)>1)
  {
  	process* needKill = vector_get(processeS,1);
  	//pid_t ptid = getpid();
  	kill(needKill->pid,SIGTERM);
  	//vector_erase(processeS,1);
  }
}

void cltC(int signal){
if(little_child != 0)
kill(little_child,SIGTERM);

}

int shell(int argc, char *argv[]) {
  // TODO: This is the entry point for your shell.
  if(argc!=1 && argc !=3)
  	{
  		print_usage();
  		fflush(stdout);
  		return 0;
  	}

  	//storing process
  	processeS = process_vector_create();//need free
  	
   signal(SIGINT, cltC);//where should i put

  FILE* file_I;
  if(argc == 3)
  {
  if((strcmp(argv[1],"-f") != 0))
  	{
  		print_usage();
  		fflush(stdout);
  		return 0;
  	}//user using eror
  //vector * Commands = char_vector_create();
  char* file_name = argv[2];
  file_I = fopen(file_name,"r");
	if(file_I == NULL)
  {
  	print_script_file_error();
  	fflush(stdout);
  	exit(0);
  }  




	char *file_buf = NULL;
  size_t n;
  ssize_t read = 0;
  size_t lines = 1;
  pid_t curr = getpid();
  
  process P;
  char* Pcm = "parent";
  char* Pst = "STATUS_RUNNING";
  P.command = Pcm;
  P.status = Pst;
  P.pid = curr;
  vector_push_back(processeS,&P);
  
 //likelt truoble//Need to free
  //print_prompt(, curr);
  
  

  while ((read = getline(&file_buf, &n, file_I)) != -1)
  	{

  	char* buf = NULL;
  	buf = getcwd(0,0);
  	print_prompt(buf,curr);
  	fflush(stdout);
  	free(buf);
   	buf = NULL;


    size_t len = strlen(file_buf);
    char temp[1000];
    strcpy(temp,file_buf);
    if(temp[len-1] == '\n' && len == 1)
    {
        print_command(" ");
        continue;
    }
    if(temp[len-1] == '\n')
    temp[len-1] = '\0';
	
	print_command(temp);
	
	size_t number = 0;
	char ** cmdptr = strsplit(temp, " ", &number);//need Free!
	
	process* theOne = vector_get(processeS,0);
	//change process command status, assume parent at 0
	char* cld = strdup(cmdptr[0]);
	if(theOne->command != NULL)
	free(theOne->command);
	theOne->command = cld;

	//exit function
	if(strcmp(cmdptr[0],"exit") == 0)
	{
	if(number != 1)
		{
		print_invalid_command(cmdptr[0]);
		fflush(stdout);
		}
	else
	{
	free_args(cmdptr);
	free(file_buf);
	cleanOther(processeS);
	fclose(file_I);
	vector_destroy(processeS);
	processeS = NULL;
	exit(0);
	}

	}

	int internal = biuldInCommand(cmdptr[0],cmdptr,number - 1,processeS);
	
	//external Command
	if(internal == 0)
	{
		char* Las = cmdptr[number-1];
		if(Las[strlen(Las)-1] == '&')
		{
			if(strlen(Las) == 1)
		{
			free(cmdptr[number-1]);
			cmdptr[number-1] = NULL;
			number--;
		}
		else
		Las[strlen(Las)-1] = '\0';	
		ExtCommand(cmdptr[0],cmdptr,number - 1,1,processeS);
		}
		else	
		ExtCommand(cmdptr[0],cmdptr,number - 1,0,processeS);
	}

	free_args(cmdptr);
	curr = getpid();
    lines++;
   	}
 	cleanOther(processeS);
 	//processeS = NULL;
 	free(file_buf);
 	fclose(file_I);
 	vector_destroy(processeS);

 	exit(0);
  }
  else
  {
  	file_I = stdin;
  	//file_name = "shell.c";  
  char *file_buf = NULL;
  size_t n;
  ssize_t read = 0;
  size_t lines = 1;
  pid_t curr = getpid();
  
  process P;
  char* Pcm = "parent";
  char* Pst = "STATUS_RUNNING";
  P.command = Pcm;
  P.status = Pst;
  P.pid = curr;
  vector_push_back(processeS,&P);
  
  char* buf = NULL;
  buf = getcwd(0,0);//likelt truoble//Need to free
  //print_prompt(, curr);
  
  print_prompt(buf,curr);
  fflush(stdout);
   free(buf);
   buf = NULL;

  while ((read = getline(&file_buf, &n, file_I)) != -1)
  	{
    size_t len = strlen(file_buf);
    char temp[1000];
    strcpy(temp,file_buf);
    if(temp[len-1] == '\n' && len == 1)
    	{
    	curr = getpid();
		buf = getcwd(0,0);//trouble??
		print_prompt(buf,curr);
		fflush(stdout);
		free(buf);
   		buf = NULL;
    	continue;
    	}
   if(temp[len-1] == '\n')
    temp[len-1] = '\0';
	
	size_t number = 0;
	char ** cmdptr = strsplit(temp, " ", &number);//need Free!
	
	process* theOne = vector_get(processeS,0);
	//change process command status, assume parent at 0
	char* cld = strdup(cmdptr[0]);
	if(theOne->command != NULL)
	free(theOne->command);
	theOne->command = cld;

	//exit function
	if(strcmp(cmdptr[0],"exit") == 0)
	{
	if(number != 1)
		{
		print_invalid_command(cmdptr[0]);
		fflush(stdout);
		}
	else
	{
	free_args(cmdptr);
	free(file_buf);
	cleanOther(processeS);
	fclose(file_I);
	vector_destroy(processeS);
	//processeS = NULL;
	exit(0);
	}

	}

	int internal = biuldInCommand(cmdptr[0],cmdptr,number - 1,processeS);
	
	//external Command
	if(internal == 0)
	{
		char* Las = cmdptr[number-1];
		if(Las[strlen(Las)-1] == '&')
		{
		if(strlen(Las) == 1)
		{
			free(cmdptr[number-1]);
			cmdptr[number-1] = NULL;
			number--;
		}
		else	
		Las[strlen(Las)-1] = '\0';
		ExtCommand(cmdptr[0],cmdptr,number - 1,1,processeS);
		}
		else	
		ExtCommand(cmdptr[0],cmdptr,number - 1,0,processeS);
	}

	free_args(cmdptr);
	curr = getpid();
	buf = getcwd(0,0);//trouble??
	print_prompt(buf,curr);
	fflush(stdout);
	free(buf);
   	buf = NULL;
    lines++;
   	}
 	cleanOther(processeS);
 	//processeS = NULL;
 	free(file_buf);
 	fclose(file_I);
  //	kill(0,)
   
    vector_destroy(processeS);


  exit(0);
	}
}


