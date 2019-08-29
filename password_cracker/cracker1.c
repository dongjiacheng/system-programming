/**
 * Machine Problem: Password Cracker
 * CS 241 - Spring 2017
 */

#include "cracker1.h"
#include "format.h"
#include "utils.h"
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
/**
 * Splits a copy of 'str' by a 'delim'.
 * Then writes the number of tokens into 'numtokens'.
 * These strings are allocated on the heap, so call free_args(char **args) when
 * you are done with them.
 * Learn more at:
 * https://www.quora.com/How-do-you-write-a-C-program-to-split-a-string-by-a-delimiter
 */
queue *T;
int recoverSucess;
int failure;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t h = PTHREAD_MUTEX_INITIALIZER;

void string_destructor(void *elem) { free(elem); }

void *string_default_constructor(void) { return calloc(1, sizeof(char*)); }

void *string_copy_constructor(void *elem) {
  char* temp = (char*)elem;
  char* copy = strdup(temp);
  return copy;
}

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

void* threadcracker(void* ptr){
int* tid = (int*)ptr;
while(1)
{
char* temp = queue_pull(T);
if(*temp == 'Q')
{
  //puts("exit");
  free(temp);
	return NULL;
}
size_t number;
char** NewData = strsplit(temp, " ", &number);
free(temp);
pthread_mutex_lock(&h);
v1_print_thread_start(*tid, NewData[0]);
fflush(stdout);
pthread_mutex_unlock(&h);

if(number != 3)
  	{
  		perror("not gonna happened, input not enough");
  		exit(1);
  	}
  	int knowNumber = getPrefixLength(NewData[2]);
  	int PWlength = strlen(NewData[2]);
  	char* try = strdup(NewData[2]);//need free
 	for(int i = knowNumber; i < PWlength; i++)
 		try[i] = 'a';

 	int hashCount = 0;
 	int running = 1;
 	
  struct crypt_data cdata;
  double times = pow(26,(PWlength-knowNumber));
  cdata.initialized = 0;
  double time1 = getThreadCPUTime();
 	while(running && times)
 	{
  //threadStatusSet(try);
 	char* hashed;
	
	hashed = crypt_r(try, "xx", &cdata);
 	hashCount++;
 	if(strcmp(hashed,NewData[1]) == 0)
 	{
 		break;
 	}
 	else
 		 {
 		 running = incrementString(try);
 		 if(running == 0)
 		 	break;
 		 }
  	times--;
    }
  double time2 = getThreadCPUTime();
 	int rt;
 	if(running == 1 && times)
 		{
    rt = 0;
    pthread_mutex_lock(&m);
    recoverSucess++;
    pthread_mutex_unlock(&m);
    }
 	else
 		{
    rt = 1;
    pthread_mutex_lock(&h);
    failure++;
    pthread_mutex_unlock(&h);
    }
  pthread_mutex_lock(&h);
 	v1_print_thread_result(*tid, NewData[0], try,
                            hashCount, time2 - time1, rt);
  fflush(stdout);
  pthread_mutex_unlock(&h);
 	free_args(NewData);
 	NewData = NULL;
 	free(try);
 	try  = NULL;

}
}

int start(size_t thread_count) {
  // TODO your code here, make sure to use thread_count!
  // Remember to ONLY crack passwords in other threads
  
  size_t sizze = 0;
  char* buf = NULL;
  T = queue_create(-1, string_copy_constructor,
                    string_destructor);
  

  while(getline(&buf,&sizze,stdin) != -1)//need free
  {
  	char* temp = strdup(buf);//need free
  	
  	if(temp[strlen(temp) - 1] == '\n')
  		temp[strlen(temp) - 1] = '\0';
  	queue_push(T,temp);

    free(buf);
    buf = NULL;
  	//need free
  	free(temp);//freed
  	temp = NULL;
  }
  if(buf)
    free(buf);

  char Q = 'Q';
  pthread_t th[thread_count];
  int** thh = malloc(thread_count*sizeof(int*));
  
  for(size_t i = 0; i < thread_count; i++)
  {
  	th[i] = i+1;
  	queue_push(T,(void*)&Q);
  	thh[i] = malloc(sizeof(int));
  	*thh[i] = th[i];
    
  	pthread_create(&th[i], 0, threadcracker, (void*)thh[i]);
  }
  void *R;
  for(size_t i = 0; i < thread_count; i++)
  {
  pthread_join(th[i],&R);
  //puts("join");
  }
  queue_destroy(T);
  for(size_t i = 0; i < thread_count; i++)
  {
   // puts("free");
  	free(thh[i]);
  }
  free(thh);
  pthread_mutex_lock(&h);
  v1_print_summary(recoverSucess, failure);
  fflush(stdout);
  pthread_mutex_unlock(&h);
  pthread_mutex_destroy(&m);
  pthread_mutex_destroy(&h);
  return 0;
}
