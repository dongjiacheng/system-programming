/**
 * Parallel Make
 * CS 241 - Spring 2017
 */


#include "format.h"
#include "graph.h"
#include "parmake.h"
#include "parser.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include "queue.h"

#define SUCCESS 8
#define FAILED 9


// Treat this as main
int cycleDetected;
int queueSize;
queue* task_queue;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;

void string_destructor(void *elem) { free(elem); }

void *string_default_constructor(void) { return calloc(1, sizeof(char*)); }

void *string_copy_constructor(void *elem) {
  char* temp = (char*)elem;
  char* copy = strdup(temp);
  return copy;
}


int deleteRelate(char* targetCurrent,graph* dependency_graph)				
{
	char* tt = strdup(targetCurrent);
	if( strcmp("", targetCurrent) == 0)
		{
		free(tt);
		return 1;
		}
	vector* antineighbors = graph_antineighbors(dependency_graph, targetCurrent);
	graph_remove_vertex(dependency_graph, targetCurrent);
	for(size_t i = 0; i < vector_size(antineighbors); i++)
	{
		char* Delete_Name = (char*)vector_get(antineighbors, i);
		int A = deleteRelate(Delete_Name,dependency_graph);
		if(A)
		{
		print_cycle_failure(tt);
		}
		//free(Delete_Name);
	}
	vector_destroy(antineighbors);
	free(tt);
	return 0;
}


void topologicalsort(char* vertex, graph* dependency_graph){

//3 is grey 4 is black

rule_t* current = 	(rule_t *)graph_get_vertex_value(dependency_graph, vertex);
if(current->state == 3)
	return;
vector* neighbors =  graph_neighbors(dependency_graph, vertex);
current->state = 3;

if(vector_size(neighbors) == 0)
{
	queue_push(task_queue,vertex);
	vector_destroy(neighbors);
	return;
}

for(size_t i = 0; i < vector_size(neighbors);i++)
{
		char* targetCurrent = (char*)vector_get(neighbors, i);
		rule_t* currentT = 	(rule_t *)graph_get_vertex_value(dependency_graph, targetCurrent);
		if(currentT->state != 3)
		{
			topologicalsort(targetCurrent, dependency_graph);	
		}


}

queue_push(task_queue,vertex);
vector_destroy(neighbors);
return;
//current->state = 4;
}

void* make(void* depen)
{
	graph* dependency_graph = (graph*)depen;
	int unfinish = 1;
	while(unfinish)
	{


	char* target_Name = queue_pull(task_queue);

	if(strcmp("DONE",target_Name) == 0)
	{
		queue_push(task_queue,"DONE");
		unfinish = 0;
		free(target_Name);
		return NULL;
	}

	if(strcmp("",target_Name) == 0)
	{
	//	rule_t* current = 	(rule_t *)graph_get_vertex_value(dependency_graph, target_Name);
		vector* neighbors =  graph_neighbors(dependency_graph, target_Name);
		int notfinish = 0;
		for(size_t i = 0; i < vector_size(neighbors); i ++)
		{
		char* target = (char*)vector_get(neighbors, i);
		rule_t* currentT = 	(rule_t *)graph_get_vertex_value(dependency_graph, target);
		pthread_mutex_lock(&m);
		int STA = currentT->state;
		pthread_mutex_unlock(&m);
		if(STA != SUCCESS && STA != FAILED)
		{
			notfinish = 1;
			break;
		}
		
		}

		if(notfinish)
		{
			queue_push(task_queue,target_Name);
			vector_destroy(neighbors);
			free(target_Name);
			continue;
		}

		else
		{
		queue_push(task_queue,"DONE");
		unfinish = 0;
		vector_destroy(neighbors);
		free(target_Name);
		return NULL;
		}
	}
	rule_t* current = 	(rule_t *)graph_get_vertex_value(dependency_graph, target_Name);
	vector* neighbors =  graph_neighbors(dependency_graph, target_Name);
			int failure = 0;
			int goahead = 0;

	for(size_t i = 0; i < vector_size(neighbors); i ++)
	{
		char* target = (char*)vector_get(neighbors, i);
		rule_t* currentT = 	(rule_t *)graph_get_vertex_value(dependency_graph, target);
		pthread_mutex_lock(&m);
		int STA = currentT->state;
		pthread_mutex_unlock(&m);
		if(STA != SUCCESS)
		{
			if(STA == FAILED)
			{
				pthread_mutex_lock(&m);
				current->state = FAILED;
				pthread_mutex_unlock(&m);
				failure = 1;

				break;
			}
			else
			{
				goahead = 1;
				break;
			}
		}

		if(failure)
			break;
	}

	if(failure)
	{
		vector_destroy(neighbors);
		free(target_Name);
		continue;
	}

	if(goahead)
	{
		queue_push(task_queue,target_Name);
		vector_destroy(neighbors);
		free(target_Name);
		continue;
	}

	//if(current->state == SUCCESS)
	//{
	//	vector_destroy(neighbors);
	//	continue;
	//}
	
	if(access(target_Name , F_OK) == 0)
	{
		int doo = 0;
		struct stat fileStat;
		stat(target_Name,&fileStat);
		time_t ruleTime = fileStat.st_mtime;
		for(size_t i = 0; i < vector_size(neighbors); i++)
			{
			char* target = (char*)vector_get(neighbors, i);
		if(access(target , F_OK) != 0)
		{
			doo = 1;
			//free(target);
			break;
		}

		struct stat fileStatT;
		stat(target,&fileStatT);
		time_t compareTime = fileStatT.st_mtime;
		if(difftime(compareTime,ruleTime) >= 0)
		{
			doo = 1;
			//free(target);
			break;
		}
		//free(target);
			}
		if(doo)
		{
			for(size_t i = 0; i < vector_size(current->commands);i++)
			{
			char* commands = (char*)vector_get(current->commands, i);
			if(system(commands) == -1)
			{
				//rule_destroy(current);
				//free(commands);
				pthread_mutex_lock(&m);
				current->state = FAILED;
				pthread_mutex_unlock(&m);
				failure = 1;

				break;
				//vector_destroy(neighbors);
				//return 1;
			}
			//free(commands);
			}
			if(failure)
			{
				vector_destroy(neighbors);
				free(target_Name);
				continue;
			}
			//rule_destroy(current);
			//vector_destroy(neighbors);
			pthread_mutex_lock(&m);
			current->state = SUCCESS;
			pthread_mutex_unlock(&m);
			//return 0;
		}
		else
		{
			//vector_destroy(neighbors);
			pthread_mutex_lock(&m);
			current->state = SUCCESS;
			pthread_mutex_unlock(&m);
			//return 0;
		}
		
		}
		else
		{
		//	rule_t* current = 	(rule_t *)graph_get_vertex_value(dependency_graph, target_Name);
			for(size_t i = 0; i < vector_size(current->commands);i++)
			{
			char* commands = (char*)vector_get(current->commands, i);
			if(system(commands)  != 0)
			{
				//free(commands);
				//rule_destroy(current);
				pthread_mutex_lock(&m);
				current->state = FAILED;
				pthread_mutex_unlock(&m);
				failure = 1;

				break;
				//vector_destroy(neighbors);
				//return 1;
			}
			//free(commands);
			}
			if(failure)
			{
				vector_destroy(neighbors);
				free(target_Name);
				continue;
			}
			//rule_destroy(current);
			//vector_destroy(neighbors);
			pthread_mutex_lock(&m);
			current->state = SUCCESS;
			pthread_mutex_unlock(&m);
			//return 0;
		}
	vector_destroy(neighbors);
	free(target_Name);
	//current->state = SUCCESS;
	//return 0;


	}
	return NULL;
}






int cycledetec(char* vertex,graph* dependency_graph)
{
	//0 is white 1 is grey 2 is black
   // mark v visited
   // set color of v to gray
  //  for each successor v' of v {
//	if v' not yet visited {
//	    DFS(v')
//	}
  //  }
//    set color of v to black
	int cycle = 0;
	rule_t* current = 	(rule_t *)graph_get_vertex_value(dependency_graph, vertex);
	current->state = 1;
	//graph_set_vertex_value(dependency_graph, vertex, (void*)current);
	vector* neighbors =  graph_neighbors(dependency_graph, vertex);
	for(size_t i = 0; i < vector_size(neighbors);i++)
		{
			char* targetCurrent = (char*)vector_get(neighbors, i);
			rule_t* currentT = 	(rule_t *)graph_get_vertex_value(dependency_graph, targetCurrent);
			if(currentT->state == 1)
			{
				deleteRelate(targetCurrent,dependency_graph);
				//free(targetCurrent);
				//rule_destroy(current);
				//rule_destroy(currentT);
				vector_destroy(neighbors);
				return 1;
				//cycle
			}

			if(currentT->state == 0)
				cycle = cycledetec(targetCurrent,dependency_graph);
			if(cycle == 1)
			{
				//free(targetCurrent);
				//rule_destroy(current);
				//rule_destroy(currentT);
				vector_destroy(neighbors);
				return 1;
			}
		//free(targetCurrent);
		//rule_destroy(currentT);
		}

	//current = 	(rule_t *)graph_get_vertex_value(dependency_graph, vertex);
	current->state = 2;
	//graph_set_vertex_value(dependency_graph, vertex,current);
	//rule_destroy(current);
	vector_destroy(neighbors);
	return cycle;
}



int parmake(int argc, char **argv) {
  // good luck!
  int option = 0;
  
  extern char *optarg;
  extern int optind;
  //int err;
  int fflag=0;
  char* fname = NULL;
  //char* target;
  int targetNumber = 0;
  int jnumber = 1;
  //char** targets;
 

	while ((option = getopt(argc, argv,"f:j:")) != -1)
	{
		switch(option){
		case 'f' :
			fflag = 1;
			fname = optarg;
			break;
		case 'j':
			//jflag = 1;
			jnumber = atoi(optarg);
			break;
		case '?':
			//err = 1;
			break;
		}
	}
	
	if(optind + 1 > argc)
	{
		targetNumber = 0;
		//target = NULL;
	}
	else
	{
		targetNumber = argc - optind;
	}
	if(fflag == 1)
	{
		if(access(fname, F_OK) != 0)
			fflag = 0;
	}
	char* makefile = NULL;
	if(fflag == 0)
	{
		if(access("makefile", F_OK) == 0)
              makefile = "makefile";
        else if (access("Makefile", F_OK) == 0)
        	  makefile = "Makefile";
        if(makefile == NULL) {
               return 1;
           }

	}
	else
		makefile = fname;
	
	//puts("283");
	//char*  Targets [argc - optind];
	char** Targets = malloc((targetNumber+1)*sizeof(char*));

	for(int i = 0;i < targetNumber; i++)
	{
		Targets[i] = argv[optind+i];
		//vector_push_back(targets, argv[optind]);
	}
	
	Targets[targetNumber] = NULL;
	//puts("293");

	graph* dependency_graph;

	if(targetNumber == 0)
		dependency_graph = parser_parse_makefile(makefile, NULL);
	else	
		dependency_graph = parser_parse_makefile(makefile, Targets);

	//vector *target_names = graph_vertices(dependency_graph);

	//puts("304");
	vector *dependencies = graph_neighbors(dependency_graph, "");
	//puts("306");
	for(size_t i = 0; i < vector_size(dependencies); i++)
		{
			char* target_Name = (char*)vector_get(dependencies, i);
			if(graph_contains_vertex(dependency_graph, target_Name))
				cycledetec(target_Name,dependency_graph);
			//free(target_Name);
		}
	//puts("314");


	//graph* dependency_graph = (graph*)depen;

	task_queue = queue_create(-1, string_copy_constructor,
                    string_destructor);
	topologicalsort("",dependency_graph);
	//for(int i = 0; i < jnumber; i++)
	//queue_push(task_queue,"");
	//queue_push("");


	pthread_t th[jnumber];

	for(int i = 0; i < jnumber; i ++)
	pthread_create(&th[i], 0, make, (void*)dependency_graph);

	void* result;

	for(int i = 0; i < jnumber; i++)
  	pthread_join(th[i],&result);
  	//puts("331");
  	free(Targets);
	
	graph_destroy(dependency_graph);

  	vector_destroy(dependencies);

	queue_destroy(task_queue);	

  	pthread_mutex_destroy(&m);
  return 0;
}

/*
void* doDepent(void* depen)
{
	graph* dependency_graph = (graph*)depen;

	vector *dependencies = graph_neighbors(dependency_graph, "");



	for(size_t i = 0; i < vector_size(dependencies); i ++)
	{
		char* target_Name = (char*)vector_get(dependencies, i);
		//rule_t* current = 	(rule_t *)graph_get_vertex_value(dependency_graph, target_Name);
		
		make(target_Name,dependency_graph);
		//free(target_Name);
	}
	vector_destroy(dependencies);
	//free(dependencies);
	return NULL;
}


int make(char* target_Name, graph* dependency_graph)
{

	vector* neighbors =  graph_neighbors(dependency_graph, target_Name);
	rule_t* current = 	(rule_t *)graph_get_vertex_value(dependency_graph, target_Name);

	int Parentfailed = 0;

	for(size_t i = 0; i < vector_size(neighbors); i++)
	{
		char* target = (char*)vector_get(neighbors, i);
		rule_t* currentT = 	(rule_t *)graph_get_vertex_value(dependency_graph, target);
		int result = 0;
		if(currentT->state != 9)
			result = make(target,dependency_graph);
		if(result)
		{
			//free(target);
			Parentfailed = 1;
			//vector_destroy(neighbors);
			//return 1;
		}
		//free(target);
	}

	if(Parentfailed)
		{
			//free(target);
			//Parentfailed = 1;
			current->state = 9;
			vector_destroy(neighbors);
			return 1;
		}

	if(current->state == 9)
	{
		vector_destroy(neighbors);
		return 0;
	}

	if(access(target_Name , F_OK) == 0)
	{
		int doo = 0;
		struct stat fileStat;
		stat(target_Name,&fileStat);
		time_t ruleTime = fileStat.st_mtime;
		for(size_t i = 0; i < vector_size(neighbors); i++)
			{
			char* target = (char*)vector_get(neighbors, i);
		if(access(target , F_OK) != 0)
		{
			doo = 1;
			//free(target);
			break;
		}

		struct stat fileStatT;
		stat(target,&fileStatT);
		time_t compareTime = fileStatT.st_mtime;
		if(difftime(compareTime,ruleTime) >= 0)
		{
			doo = 1;
			//free(target);
			break;
		}
		//free(target);
			}
		if(doo)
		{
			for(size_t i = 0; i < vector_size(current->commands);i++)
			{
			char* commands = (char*)vector_get(current->commands, i);
			if(system(commands) == -1)
			{
				//rule_destroy(current);
				//free(commands);
				current->state = 9;
				vector_destroy(neighbors);
				return 1;
			}
			//free(commands);
			}
			//rule_destroy(current);
			vector_destroy(neighbors);
			current->state = 9;
			return 0;
		}
		else
		{
			//vector_destroy(neighbors);
			current->state = 9;
			return 0;
		}
	}
	else
	{
		//	rule_t* current = 	(rule_t *)graph_get_vertex_value(dependency_graph, target_Name);
			for(size_t i = 0; i < vector_size(current->commands);i++)
			{
			char* commands = (char*)vector_get(current->commands, i);
			if(system(commands)  != 0)
			{
				//free(commands);
				//rule_destroy(current);
				current->state = 9;
				vector_destroy(neighbors);
				return 1;
			}
			//free(commands);
			}
			//rule_destroy(current);
			vector_destroy(neighbors);
			current->state = 9;
			return 0;
	}
	vector_destroy(neighbors);
	current->state = 9;
	return 0;
}

	*/
