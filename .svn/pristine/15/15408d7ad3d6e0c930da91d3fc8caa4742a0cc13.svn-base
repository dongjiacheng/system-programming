/**
 * Deadlocked Diners Lab
 * CS 241 - Spring 2017
 */
#include "company.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void *work_interns(void *p) { 
	Company *company = (Company *)p;
 	int failed;
  int tries_left;
  pthread_mutex_t *fork_lft,*fork_rgt,*fork_tmp;
  
  
  while(running)
 	{
    Company_have_board_meeting(company);
    fork_lft = Company_get_left_intern(company);
    fork_rgt = Company_get_right_intern(company);

    tries_left = 2;
    if(fork_rgt == fork_lft) break;
    do
    {
      pthread_mutex_lock(fork_lft);
      failed = (tries_left > 0)? pthread_mutex_trylock(fork_rgt) : pthread_mutex_lock(fork_rgt);
      Company_set_failed_flag(company,failed);

      if(Company_get_failed_flag(company))
      {
      pthread_mutex_unlock(fork_lft);
      fork_tmp = fork_lft;
      fork_lft = fork_rgt;
      fork_rgt = fork_tmp;
      tries_left -= 1;
      }
    }while(Company_get_failed_flag(company)&&running);

    if(!Company_get_failed_flag(company))
    {
      Company_hire_interns(company);

      pthread_mutex_unlock(fork_rgt);
      pthread_mutex_unlock(fork_lft);
    }
		
	}

	return NULL; 
}
