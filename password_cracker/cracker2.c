/**
 * Machine Problem: Password Cracker
 * CS 241 - Spring 2017
 */

#include "cracker2.h"
#include "utils.h"
#include "format.h"
#include "string.h"
#include "stdlib.h"
#include <stdio.h>
#include <unistd.h>
#include <crypt.h>
#include <math.h>
#include <sys/types.h>
#include <pthread.h>
#include "queue.h"
#include "thread_status.h"
#include "pthread.h"
#include <signal.h>

queue *T;
int found;
int finish;
int thread_num;
//int hashCount;
long total_hash;
char* password;

char** NewData;
pthread_mutex_t globle = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t local = PTHREAD_MUTEX_INITIALIZER;

pthread_barrier_t starts;
pthread_barrier_t end;
pthread_barrier_t InThread;





char **strsplit(const char *str, const char *delim, size_t *numtokens) {
  // copy the original string so that we don't overwrite parts of it
  // (don't do this if you don't need to keep the old line,
  // as this is less efficient)
  char *s = strdup(str);
  // these three variables are part of a very common idiom to
  // implement a dynamically-growing array
  size_t tokens_alloc = 1;
  size_t tokens_used = 0;
  char **tokens = calloc(tokens_alloc, sizeof(char *));
  char *token, *strtok_ctx;
  for (token = strtok_r(s, delim, &strtok_ctx); token != NULL;
       token = strtok_r(NULL, delim, &strtok_ctx)) {
    // check if we need to allocate more space for tokens
    if (tokens_used == tokens_alloc) {
      tokens_alloc *= 2;
      tokens = realloc(tokens, tokens_alloc * sizeof(char *));
    }
    tokens[tokens_used++] = strdup(token);
  }
  // cleanup
  if (tokens_used == 0) {
    free(tokens);
    tokens = NULL;
  } else {
    tokens = realloc(tokens, tokens_used * sizeof(char *));
  }
  *numtokens = tokens_used;
  free(s);
  // Adding a null terminator
  tokens = realloc(tokens, sizeof(char *) * (tokens_used + 1));
  tokens[tokens_used] = NULL;
  return tokens;
}
//free return value of args
void free_args(char **args) {
  char **ptr = args;
  while (*ptr) {
    free(*ptr);
    ptr++;
  }
  free(args);
}

void* threadworker(void* ptr){
	int* tid = (int*)ptr;
	

  while(1)
	{
  pthread_barrier_wait(&starts);
  
  if(finish == 1)
  {
    pthread_barrier_wait(&end);
    
    return NULL;
  }

    
	int knowNumber = getPrefixLength(NewData[2]);
  	int PWlength = strlen(NewData[2]);
  	char* try = strdup(NewData[2]);//need free
 	long start_index;
 	long count;
 	getSubrange((PWlength - knowNumber), thread_num, *tid,
                 &start_index, &count);

 	setStringPosition(try, start_index);
 	for(int i = 0 ; i < knowNumber; i ++)
 	{
 		try[i] = NewData[2][i];
 	}
   pthread_mutex_lock(&local);
   v2_print_thread_start(*tid, NewData[0], start_index, try);
   pthread_mutex_unlock(&local);
  int result = 1;
  int hashCount = 0;
  int running = 1;
  
  struct crypt_data cdata;
  cdata.initialized = 0;
    

  while(running && count)
  {

    pthread_mutex_lock(&local);
    if(found == 1)
    {
      pthread_mutex_unlock(&local);
      break;
    }
    pthread_mutex_unlock(&local);
  
  char* hashed;
  
  hashed = crypt_r(try, "xx", &cdata);
  hashCount++;
  if(strcmp(hashed,NewData[1]) == 0)
  {
    pthread_mutex_lock(&local);
    found = 1;
    password = strdup(try);
    pthread_mutex_unlock(&local);
    result = 0;
    pthread_mutex_lock(&local);
    v2_print_thread_result(*tid, hashCount, result);
    pthread_mutex_unlock(&local);
    
    pthread_barrier_wait(&InThread);

    break;
  }
  else
     {
     running = incrementString(try);
     if(running == 0)
     {
      break;     
     }
     }
    count--;

  }

   pthread_mutex_lock(&globle);
   total_hash += hashCount;
   pthread_mutex_unlock(&globle);


   free(try);
   try = NULL;

   pthread_mutex_lock(&local);
  
  pthread_mutex_unlock(&local);
  if(result != 0)
  {
  pthread_barrier_wait(&InThread);
  pthread_mutex_lock(&local);

  if(found == 0)
    result = 2;
  v2_print_thread_result(*tid, hashCount, result);
  pthread_mutex_unlock(&local);
  }

  pthread_barrier_wait(&end);
  }
 	//try is the one need to crack

 	return NULL;
}

int start(size_t thread_count) {
  // TODO your code here, make sure to use thread_count!
  // Remember to ONLY crack passwords in other threads
  thread_num = thread_count;
  
  size_t sizze = 0;
  char* buf = NULL;
  
  pthread_t th[thread_count];
  int** thh = malloc(thread_count*sizeof(int*));
  for(size_t i = 0; i < thread_count; i++)
  	{
  	th[i] = i+1;
  	thh[i] = malloc(sizeof(int));
  	*thh[i] = th[i];
  	}

  pthread_barrier_init(&starts,
         NULL, thread_count+1);
  pthread_barrier_init(&end,
         NULL, thread_count+1);
  pthread_barrier_init(&InThread,
         NULL, thread_count);


  for(size_t i = 0; i < thread_count; i++)
    {
    pthread_create(&th[i], 0, threadworker, (void*)thh[i]);
    }
  
  char* Needcrack;
  while(getline(&buf,&sizze,stdin) != -1)//need free
  {
    size_t number;
  	Needcrack = strdup(buf);//need free
  	free(buf);
  	buf = NULL;
  	if(Needcrack[strlen(Needcrack) - 1] == '\n')
  		Needcrack[strlen(Needcrack) - 1] = '\0';
    

    NewData = strsplit(Needcrack, " ", &number);
    pthread_mutex_lock(&local);
    v2_print_start_user(NewData[0]);
    pthread_mutex_unlock(&local);
    pthread_barrier_wait(&starts);
  	
  	double start_time = getTime();
  	double start_cpu_time = getCPUTime();
 
    pthread_barrier_wait(&end);
    double elapsed = getTime() - start_time;
    double total_cpu_time = getCPUTime() - start_cpu_time;
    
    pthread_mutex_lock(&local);
    int R;
    if(found == 0)
      R = 1;
    else
      R = 0;

  	v2_print_summary(NewData[0], password, total_hash ,elapsed, total_cpu_time, R);
  	pthread_mutex_unlock(&local);
    pthread_mutex_lock(&globle);
    found = 0;
    free(password);
    password = NULL;
    free_args(NewData);
    NewData = NULL;
    total_hash = 0;
    pthread_mutex_unlock(&globle);
    //need free
  	free(Needcrack);//freed
  	Needcrack = NULL;
  }
  if(buf!=NULL)
    free(buf);


  finish = 1;
  pthread_barrier_wait(&starts);
  pthread_barrier_wait(&end);

    
  
  void* R;
  
  for(size_t i = 0; i < thread_count; i++)
    {
    pthread_join(th[i],&R);
    }
   
  
  for(size_t i = 0; i < thread_count; i++)
  	{
    
  	free(thh[i]);
  	}
  	free(thh);
    pthread_barrier_destroy(&starts);
    pthread_barrier_destroy(&end);
    pthread_barrier_destroy(&InThread);
    
  return 0;
}




/*
char* ptr = strdup("hhhhhhhh");
  char* ptrr = strdup(ptr);
  long start;
  long times;
  getSubrange(5, 4, 2,
                 &start, &times);
  printf("%s\n",ptr);
  setStringPosition(ptr, start);
  for(int i = 0 ; i < 3; i ++)
 	{
 		ptr[i] = ptrr[i];
 	}
  printf("%s\n", ptr);
*/
