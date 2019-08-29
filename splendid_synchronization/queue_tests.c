/**
 * Splendid Synchronization Lab 
 * CS 241 - Spring 2017
 */
#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "queue.h"


queue* Q;
int data[110];

void* calc(void *ptr) {

	int* data = (int*)ptr;
	for(int i = 0; i < 10; i ++)
    {
    //	printf("datainput%d\n", data[i]);
//	fflush(stdout);  
  }
	for(int i = 0; i < 10; i++)
		queue_push(Q,(data+i));
	return NULL;
}

void* calc1(void *ptr) {

	
	
		for(int i = 0; i < 15; i ++)
    {
    	int* t = queue_pull(Q);
  //  	printf("pullout%d\n", *t);
//	fflush(stdout);    
	free(t);
    }
	return NULL;
}

int main(int argc, char **argv) {
	Q = queue_create(500, int_copy_constructor,
                    int_destructor);
	for(int i = 0; i< 110; i++)
		data[i] = i;
	for(int i = 0; i < 100; i ++)
    {
    	printf("%d\n", data[i]);
    }
    puts("real");
    pthread_t ids[12];
    for(int i = 0; i < 11; i++)  
        pthread_create(&ids[i], NULL, calc, (void *) &data[i*10] );
    sleep(2);
    pthread_create(&ids[11], NULL, calc1, (void *) &data[10] );

   void* result;
    for(int i = 0; i < 12; i ++)
    	pthread_join(ids[i],&result);

    for(int i = 0; i < 10; i ++)
    {
    	int* t = queue_pull(Q);
   	printf("pullout%d\n", *t);
	fflush(stdout);    
	free(t);
    }

    queue_destroy(Q);
  printf("Please write tests cases\n");
  return 0;
}
