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
#include <ctype.h>
#include <string.h>
#include <assert.h>

extern char **environ;

char *replace_str(char *str,const char *orig, const char *rep)
{
  char* buffer = malloc((strlen(str)+strlen(orig)+strlen(rep))*sizeof(char));//need free
  char *p;

  if(!(p = strstr(str, orig)))  // Is 'orig' even in 'str'?
    return str;

  strncpy(buffer, str, p-str); // Copy characters from 'str' start to 'orig' st$
  buffer[p-str] = '\0';

  sprintf(buffer+(p-str), "%s%s", rep, p+strlen(orig));
  printf("%s\n",buffer);
  return buffer;
}


char** stringChanger(char** var)
{
if(!getenv(var[0]))
{
	strcpy(var[1],"");
	return var; 
}


char* tptr;
while((tptr = strchr(var[1],'%')))
{
	int z = 1;
	while((!isspace(tptr[z]) && !ispunct(tptr[z])))
	{
		if(tptr[z] == '\0')
		{
	
		break;
		}
		z++;
	}
	char* dest = malloc(256*sizeof(char));
	strncpy(dest,tptr+1,z-1);
	dest[z-1] = '\0';
	printf("%s\n",dest);	
	char*temp;
	temp = getenv(dest);
	if(temp == NULL)
	{
	print_env_usage();
	return NULL;
	}
	char* dest2 = malloc(256*sizeof(char));
	dest2[0] = '%';
	dest2[1] = '\0';
	strcat(dest2,dest);
	printf("%s\n",dest2);
	char* newvar = replace_str(var[1], dest2, temp);
	free(var[1]);
	var[1] = newvar;
	printf("%s\n",newvar);
	free(dest);
	free(dest2);
}
	return var;
}



char** tokener(char* arg)
{
char* temp = strdup(arg);
char** arrayO = calloc(2,sizeof(char*));
int Capacity = 10;
int size = 0;
char* start;
char* token;
while((start = strsep(&temp,"="))!= NULL)
{
	char* emp = "";
	if((size+1)>Capacity)
	{
		Capacity *= 2;
		arrayO = realloc(arrayO,Capacity*sizeof(char*));
	}
	if(!strcmp(start,emp))
	{
	arrayO[size] = "";
	size++;
	continue;
	}	
	token = strdup(start);
	arrayO[size]  = token;
	size++;
	
}

for(int i = 0 ; i < size; i++)
	printf("%s\n",arrayO[i]);

free(temp);
temp = NULL;

if(size!=2)
{
print_env_usage();

for(int i = 0; i < size; i ++)
	free(arrayO[i]);

return NULL;
}
return arrayO;
}



int main(int argc, char *argv[]) 
{

if(argc == 1)
{
	//printing
	int i = 1;
  	char *s = *environ;

  	for (; s; i++) 
  	{
    printf("%s\n", s);
    s = *(environ+i);
  	}

	return 0;
}

if(argc <= 2)
{
	
	print_env_usage();
	exit(1);
}


//token argv[1]
char* temp = strdup(argv[1]);
char** arrayOfCommand = calloc(10,sizeof(char*));//need free
int Capacity = 10;
int size = 0;
char* start;
char* token;
while((start = strsep(&temp,","))!= NULL)
{
	char* emp = "";
	if((size+1)>Capacity)
	{
		Capacity *= 2;
		arrayOfCommand = realloc(arrayOfCommand,Capacity*sizeof(char*));
	}
	if(!strcmp(start,emp))
	{
	arrayOfCommand[size] = "";
	size++;
	continue;
	}	
	token = strdup(start);//need free
	arrayOfCommand[size]  = token;
	size++;
	//arryoOfCommand need to be free
}
for(int i = 0 ; i < size; i++)
	printf("%s\n",arrayOfCommand[i]);
free(temp);
temp = NULL;

//pacl command namme



char* command[argc-1];
for(int i = 0; i < (argc-2); i++)
		command[i] = argv[i+2];
command[argc-2] = NULL;

pid_t child = fork();
printf("%d\n",(int)child);
if(child == -1)
{
		print_fork_failed();
		for(int i = 0; i < size; i++)
	{
			free(arrayOfCommand[i]);
	}

		free(arrayOfCommand);
		exit(1);

}

puts("beforeif");
if(child!=0)
{
	int ssiz;
	sleep(5);
	waitpid(child,&ssiz,0);//parent
}

else
{
	//need to set for Env
	puts("thisline??");
	
	for(int i = 0; i < size; i++)
	{
	printf("%d\n",size);
	char** top1 = tokener(arrayOfCommand[i]);
	puts("aftertoken");
	   printf("var:%s\n value:%s\n",top1[0],top1[1]);

	if(top1 != NULL)
	{
	puts("beforetop");
	top1 = stringChanger(top1);
	if(top1 == NULL)
		break;
	printf("var:%s\n value:%s\n",top1[0],top1[1]);
	int erro = setenv(top1[0],top1[1],1);
	if(erro != 0)
		print_environment_change_failed();
	free(top1[0]);
	if(top1[1])
	free(top1[1]);
	free(top1);
	}
	free(arrayOfCommand[i]);
	}
	puts("'''''''''''''''''''''''''''''''''''''''''''");
	//free
	//for(int i = 0; i < size; i++)
	//{
	//	free(arrayOfCommandp[i]);
	//}

	free(arrayOfCommand);
	puts("excutefile");
	execvp(command[0],command);

	print_exec_failed();

	exit(1);

	//child
}

for(int i = 0; i < size; i++)
	{
		free(arrayOfCommand[i]);
	}

	free(arrayOfCommand);






















}
