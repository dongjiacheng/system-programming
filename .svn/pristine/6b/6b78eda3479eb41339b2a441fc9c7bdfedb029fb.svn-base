/**
 * Scheduler Lab
 * CS 241 - Spring 2017
 */
#include "libpriqueue.h"
#include "libscheduler.h"
#include "vector.h"

static priqueue_t pqueue;
static vector* finish;
//static vector* processing;

static core_t core;
static scheme_t scheme;
static int (*comparison_func)(const void *, const void *);
int R;
int preemption;
signed globle_temp;
int Rs;

void job_destructor(void *elem) { free(elem); }

void *job_default_constructor(void) { return calloc(1, sizeof(job_t)); }

void *job_copy_constructor(void *elem) {
  job_t* old = (job_t*)elem;
  
  job_t *job = malloc(sizeof(job_t));
    job->priority = old->priority;
    job->id = old->id;
    job->running_time = old->running_time;
    job->arrival_time = old->arrival_time;
    job->round = old->round;
    job->remainning_time = old->remainning_time;
    job->laststartTime = old->laststartTime;
    job->active = old->active;
    job->end_time = old->end_time;
    //job-> response_time = old->response_time;
    //job-> turnaround_time = old->turnaround_time;
    //job-> wait_time = old->wait_time;
    job->start_time = old->start_time;
  return job;
}


int comparer_fcfs(const void *a, const void *b) { 
    job_t* jobA = (job_t*)a;
    job_t* jobB = (job_t*)b;
    

    return (int)(jobA->arrival_time - jobB->arrival_time); }

int break_tie(const void *a, const void *b) { return comparer_fcfs(a, b); }

int comparer_ppri(const void *a, const void *b) {
    // Complete as is
    //job_t* jobA = (job_t*)a;
    //job_t* jobB = (job_t*)b;

    return comparer_pri(a, b);
}

int comparer_pri(const void *a, const void *b) { 
    job_t* jobA = (job_t*)a;
    job_t* jobB = (job_t*)b;

if(jobA->priority - jobB->priority == 0)
    {
        return break_tie(a,b);
    }
    else
    return (jobA->priority-jobB->priority); }

int comparer_plrtf(const void *a, const void *b) { 
    job_t* jobA = (job_t*)a;
    job_t* jobB = (job_t*)b;
    if(jobA->remainning_time - jobB->remainning_time == 0)
    {
        return break_tie(a,b);
    }


    return (int)(jobA->remainning_time - jobB->remainning_time); }

int comparer_rr(const void *a, const void *b) { 
    job_t* jobA = (job_t*)a;
    job_t* jobB = (job_t*)b;
    if(jobA->round == jobB->round)
        {
            if(jobA->laststartTime == jobB->laststartTime)
            {
                return jobB->arrival_time - jobA->arrival_time;
            }
            else
            {
                return jobA->laststartTime - jobB->laststartTime;
            }
        }
    else
        return (jobA->round - jobB->round);

  }

int comparer_sjf(const void *a, const void *b) { 
    job_t* jobA = (job_t*)a;
    job_t* jobB = (job_t*)b;
    if(jobA->running_time - jobB->running_time == 0)
    {
        return break_tie(a,b);
    }

    return (int)(jobA->running_time - jobB->running_time); }

void scheduler_start_up(scheme_t s) {
    switch (s) {
    case FCFS:
        comparison_func = comparer_fcfs;
        break;
    case PRI:
        comparison_func = comparer_pri;
        break;
    case PPRI:
        comparison_func = comparer_ppri;
        preemption = 1;
        break;
    case PLRTF:
        comparison_func = comparer_plrtf;
        preemption = 1;
        break;
    case RR:
        comparison_func = comparer_rr;
        Rs = 1;
        break;
    case SJF:
        comparison_func = comparer_sjf;
        break;
    default:
        printf("Did not recognize scheme\n");
        exit(1);
    }
    priqueue_init(&pqueue, comparison_func);
  //  processing =  vector_create(job_copy_constructor,
    //                 job_destructor,
      //                job_default_constructor);
    finish =  vector_create(job_copy_constructor,
                     job_destructor,
                      job_default_constructor);
}

job_t* findactive(){
    job_t* pulledJob = (job_t*)priqueue_poll(&pqueue);
    if(pulledJob == NULL)
        return NULL;
    else if(pulledJob->active)
    {
        return pulledJob;
    }

    job_t* result = findactive();
    priqueue_offer(&pqueue,pulledJob);
    return result;
}




bool scheduler_new_job(int job_number, unsigned time, unsigned running_time,
                       int priority) {
    job_t *actived = findactive();

    if(actived != NULL)
    { 
    //actived->active = ;
    actived->remainning_time = actived->remainning_time -  (time - actived->laststartTime);
    if(Rs)
        actived->laststartTime = time;
    priqueue_offer(&pqueue,actived);
    }
    int size = priqueue_size(&pqueue);
    job_t *job = malloc(sizeof(job_t));
    job->priority = priority;
    job->id = job_number;
    job->running_time = running_time;
    job->arrival_time = time;
    job->round = R;
    job->active = 0;
    job->remainning_time = running_time;
    job->laststartTime = time; 
    job->start_time = -1;
    //job-> response_time = 0;
    //job-> turnaround_time = 0;
    //job-> wait_time = 0;
    job->end_time = -1;
    priqueue_offer(&pqueue, job);
    job_t *following_job = (job_t*) priqueue_peek(&pqueue);
    if(following_job->id == job->id && preemption)
    {
    priqueue_poll(&pqueue);// modification needed
    
    job->start_time = time;
    job->laststartTime = time;
    job->active = 1;
    job_t *actived = findactive();
    if(actived != NULL)
    {
        actived->active = 0;
        if(actived->start_time == time)
            actived->start_time = -1;
        if(actived->laststartTime == time)
        actived->laststartTime = globle_temp;
        priqueue_offer(&pqueue,actived);
    }
    priqueue_offer(&pqueue, job);
    //vector_push_back(processing, job);
    return true;
    }
    else if(size == 0)
    {
    priqueue_poll(&pqueue);// modification needed
    job->start_time = time;
    job->laststartTime = time;
    job->active = 1;
    priqueue_offer(&pqueue, job);
        return true;
    }
    else
    return false;
}


job_t* findAndDelete(int job_number){
    job_t* pulledJob = (job_t*)priqueue_poll(&pqueue);
    if(pulledJob == NULL)
        return NULL;
    else if(pulledJob->id == job_number)
    {
        return pulledJob;
    }

    job_t* result = findAndDelete(job_number);
    priqueue_offer(&pqueue,pulledJob);
    return result;
}

int scheduler_job_finished(int job_number, unsigned time)  
    {
        job_t* pulledJob =  findAndDelete(job_number);

        job_t *following_job = (job_t*) priqueue_poll(&pqueue);
        pulledJob->end_time = time;       
        vector_push_back(finish, pulledJob);
        free(pulledJob);
        if(following_job != NULL)
        {
        int result = following_job->id;
        if(following_job->start_time == -1)
        following_job-> start_time = time; 
        globle_temp = following_job -> laststartTime;
        following_job-> laststartTime = time;
        following_job-> active = 1;
        priqueue_offer(&pqueue, following_job);
        //free(following_job);
        return result;
        }
        else
        {
            return -1;
        }
    }

   

int scheduler_quantum_expired(unsigned time) { 
    job_t* pulledJob = (job_t*)findactive();
    pulledJob -> round = pulledJob -> round + 1;
    pulledJob -> active = 0;
    R = pulledJob->round;
    priqueue_offer(&pqueue, pulledJob);
    job_t *following_job = (job_t*) priqueue_poll(&pqueue);
    following_job->active = 1;
    following_job->laststartTime = time;
    if(following_job->start_time == -1)
        following_job->start_time = time;
    priqueue_offer(&pqueue, following_job);

    return following_job->id; }

float scheduler_average_waiting_time() { 
    float wait_time = 0;
    for(size_t i = 0; i < vector_size(finish); i++)
    {
    job_t* current = (job_t*)vector_get(finish, i);
    wait_time += ((current->end_time - current->arrival_time) - current->running_time);
    //free(current);
    }
    //fprintf(stderr, "%f\n", wait_time);
    //fprintf(stderr, "%zu\n", vector_size(finish));
    return wait_time/vector_size(finish); }

float scheduler_average_turnaround_time() { 
     float turnaround_time = 0;
    for(size_t i = 0; i < vector_size(finish); i++)
    {
    job_t* current = (job_t*)vector_get(finish, i);
    //fprintf(stderr, "id %d arrival_time is %d end_time is %d\n",current->id,current->arrival_time,current->end_time );
    turnaround_time += (current->end_time - current->arrival_time);
    //free(current);
    }
    //fprintf(stderr, "%f\n", turnaround_time);
    //fprintf(stderr, "%zu\n", vector_size(finish));
    return turnaround_time/vector_size(finish); }
     

float scheduler_average_response_time() { 
    float response_time = 0;
    for(size_t i = 0; i < vector_size(finish); i++)
    {

    job_t* current = (job_t*)vector_get(finish, i);
     //fprintf(stderr, "start(%d)arrive(%d)\n", current->start_time,current->arrival_time);
    response_time +=( current->start_time - current->arrival_time ) ;
    //free(current);
    }
    //fprintf(stderr, "%f\n", response_time);
    //fprintf(stderr, "%zu\n", vector_size(finish));
    return response_time/vector_size(finish); 

     }

void scheduler_clean_up() { priqueue_destroy(&pqueue); 
vector_destroy(finish);}

void scheduler_show_queue() {
    //job_t* pulledJob = (job_t*)priqueue_poll(&pqueue);
    //if(pulledJob == NULL)
      //  return ;
    //fprintf(stderr, "job%d(%d)RR:%d ", pulledJob->id,pulledJob->active, pulledJob->round);

   // scheduler_show_queue();
    //priqueue_offer(&pqueue,pulledJob);
        // This function is left entirely to you! Totally optional.
}
