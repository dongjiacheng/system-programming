/**
 * Teaching Threads
 * CS 241 - Spring 2017
 */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "reduce.h"
#include "reducers.h"

// Task struct that gets passed into pthread_create
typedef struct {
  int *output;         // Pointer to where the output should be written
  int *list;           // Pointer to the begining of the list.
  size_t list_len;     // Length of the list.
  reducer reduce_func; // Reduce function for reduce()
  int base_case;       // Base case for the reduce function
} task;

/* You should create a start routine for your threads. */


void *busy(void *ptr) {
	task *currentTask = (task*) ptr;
	currentTask->output = malloc(sizeof(int));
	*currentTask->output = reduce(currentTask->list,currentTask->list_len, currentTask->reduce_func, currentTask->base_case);

	return NULL;
}

int par_reduce(int *list, size_t list_len, reducer reduce_func, int base_case,
               size_t num_threads) {
  size_t NumTh = 1;
if(num_threads > list_len)
 NumTh = list_len/2;
else
	NumTh = num_threads;
  pthread_t th[NumTh];
  void *result;

  task **info = calloc(NumTh , sizeof(task*));



  int remain = list_len%NumTh;

  int howMany = list_len/NumTh;


  /* Your implementation goes here */
  for(size_t i = 0; i < NumTh; i++)
  {
  info[i] = malloc(sizeof(task)) ;
  info[i]->list = (list+i*howMany);
  if(i == (NumTh-1))
  	info[i]->list_len = remain + howMany;
  else	
  	info[i]->list_len = howMany;
  info[i]->base_case = base_case;
  info[i]->reduce_func = reduce_func;
  pthread_create(&th[i], 0, busy, (void*)info[i]);
  
  }
  int *LList = malloc(NumTh*sizeof(int));
  for(size_t i = 0; i < NumTh; i++)
  {
  pthread_join(th[i],&result);
  LList[i] = *(info[i]->output);
  free(info[i]->output);
  free(info[i]);
  }
  
  int tI = reduce(LList,NumTh,reduce_func,base_case);

  free (info);
  free (LList);
  return tI;
}
