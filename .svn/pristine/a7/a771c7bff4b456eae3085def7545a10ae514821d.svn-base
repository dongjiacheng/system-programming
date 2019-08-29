/**
 * CS 241 - Systems Programming
 *
 * Pied Piper - Spring 2017
 */
#include "pied_piper.h"
#include "utils.h"
#include <fcntl.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>


#define TRIES 3

int pied_piper(int input_fd, int output_fd, char **executables) {
  // Your code goes here...
	
 if(input_fd < 0)
 {
  input_fd = 0;
  }
if(output_fd < 0)
{
  output_fd = 1;
}
  char** loc = executables;
	int numberOfcommand = 0;

	while(executables[numberOfcommand])
	{
		numberOfcommand++;
	}
  //fprintf(stderr, "%d\n", numberOfcommand);

int try = TRIES;
int retry = 1;
failure_information printout[numberOfcommand];

while(try && retry)
{
  //fprintf(stderr, "try in front%d\n", try);
  reset_file(input_fd);
  int num = 0;
  pid_t pid[numberOfcommand];
	
  int fh[numberOfcommand-1][2];
  int status;
  executables = loc;
  int sterr[numberOfcommand][2];
	
  //pid_t pid;
  
  for(int i = 0; i < (numberOfcommand-1); i++ )
  {
    pipe2(fh[i],O_CLOEXEC);
   
  }
  for(int i = 0; i < (numberOfcommand); i++ )
  {
     pipe2(sterr[i],O_CLOEXEC);
  }
        //fprintf(stderr, "%d\n",numberOfcommand);


     while (*executables != NULL)
    {
     // fprintf(stderr, "%d\n",num);
      if ((pid[num] = fork()) == -1)
        {
          exit(1);
        }
      

      else if (pid[num] == 0)//this is child,writer
        {
          //sleep(num);
           //fprintf(stderr, "number is %d\n", num);
           //fprintf(stderr, "command is %s\n", *executables);

          if(num == 0)
          {
                dup2(input_fd,0);
          

          }
          else 
          {
            //fprintf(stderr, "%d\n", (num-1)*2);
           // sleep(1);
            if(dup2(fh[num-1][0], 0) == -1)
              print_invalid_input(); //change the input according to the old one 
          }
          //confuse here
          if (*(executables + 1) != NULL)
            {
              //fprintf(stderr, "%d\n", (num)*2 + 1);
            if(dup2(fh[num][1],1) == -1)
                print_invalid_output();
            }
          else
          {
              //fprintf(stderr, "%d\n", dup2(fh[num*2 + 1], output_fd));
              //sleep(1);
            if(dup2(output_fd,1) == -1)//sequence??
              {
                print_invalid_output();
              }
          }

          dup2(sterr[num][1],2);
         
          exec_command(*executables);
          //fprintf(stderr, "failhere\n");
          exit(1);
        }
      else
        {
          num++;
          executables++;
        }
    }

    for(int i = 0; i < (numberOfcommand - 1); i++)
    {
       //fprintf(stderr, "%d\n", i);
        close(fh[i][0]);
        close(fh[i][1]);
      //  close(sterr[i][1]);
    }

    for(int i = 0; i < (numberOfcommand); i++)
    {
        close(sterr[i][1]);
    }


    try--;
    retry = 0;
    for(int i = 0; i < numberOfcommand; i++)
       {

       waitpid(pid[i],&status,0);
       printout[i].status = status;
       //fprintf(stderr, "status%d\n", status[i]);
       //fprintf(stderr, "%d\n", WIFEXITED(status));
       if(WIFEXITED(status) == 0)
         {
            retry = 1;
         }
      else if(WEXITSTATUS(status))
          {
           //fprintf(stderr, "inside\n");

            retry = 1;
          }
          
          if(try == 0)
          {
            //fprintf(stderr, "pid is %d\n", (int)(pid[i]));
            //fprintf(stderr, "status%d\n", status[i]);
            char* temp = calloc(1,1024);
            char buf;
            int bytesread;
          //  int count = 0;
        // read one byte at a time.
            //fprintf(stderr, "the pipe it read%d\n", i);
            while ((bytesread = read(sterr[i][0], &buf, 1)) > 0 ) {
                   
                   strcat(temp,&buf);
               }   

            printout[i].error_message = temp;
            
            
          }
            //else{
            //  *(printout[i].error_message) = ;

            //}
                    close(sterr[i][0]);

        
        }
      //  fprintf(stderr, "retry%d\n", retry);
        
    
       
         //for(int i = 0; i < (numberOfcommand); i++)
         //{
         //}

        if(retry == 0)
        {
          exit(0);
        }

     //   fprintf(stderr, "try%d\n",try );
}

   // fprintf(stderr, "try is %d\n", try);



for(int i = 0; i < (numberOfcommand); i ++)
{
  printout[i].command = loc[i];

  //fprintf(stderr, "%s\n", printout[i].command);
    
    //fprintf(stderr, "%d\n", i);

  //fprintf(stderr, "%s\n", printout[i].error_message);
  //fprintf(stderr, "%d\n", printout[i].status);


//  print_failure_report(&printout[i], numberOfcommand);
}
  print_failure_report(printout, numberOfcommand);

  for(int i = 0; i < (numberOfcommand); i ++)
  {
    free((void*)printout[i].error_message);
  }
  return EXIT_OUT_OF_RETRIES;

}



//signal(SIGPIPE, no_one_listening);

