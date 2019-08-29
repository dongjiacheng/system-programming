/**
 * MapReduce
 * CS 241 - Spring 2017
 */

#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h> 
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


//#include "splitter.c"

int main(int argc, char **argv) {
    
    //int input_fd = open(argv[1],O_RDONLY);
   // execlp("./splitter","./splitter");
    //fprintf(stderr, "%d\n", atoi(argv[5]));
    //execlp("./splitter","./splitter",argv[1],argv[5],"1",(char*)NULL);
    //fprintf(stderr, "failed\n" );
    //exit(1);
    if(argc != 6)
    {
        print_usage();
        exit(1);
    }

    FILE *fp = fopen(argv[2], "w+");
    fclose(fp);

    int mapper_number = atoi(argv[5]);

    pid_t pid[mapper_number*2 + 1];

    int fd[mapper_number][2];
    for(int i = 0; i < (mapper_number); i++ )
    {
     pipe2(fd[i],O_CLOEXEC);
    }

    for(int i = 0; i < mapper_number;i++)
    {
       if ((pid[i] = fork()) == -1)
        {
          exit(1);
        }
        else if (pid[i] == 0)
        {
            //this is child
            dup2(fd[i][1],1);
           // fprintf(stderr, "i is %d\n", i);
            char index[15];
            sprintf(index, "%d", i);
            execlp("./splitter","./splitter",argv[1],argv[5],&index,(char*)NULL);
            fprintf(stderr, "inputFile : %s\n", argv[1]);
            fprintf(stderr, "splitter exec failed\n");
            exit(1);
        }
        else
        {
            close(fd[i][1]);
        }
    }

    int fh[2];
    pipe2(fh,O_CLOEXEC);

   // fprintf(stderr, ".................................................................................\n");

    for(int i = 0; i < mapper_number;i++)
    {
       if ((pid[mapper_number + i] = fork()) == -1)
        {
          exit(1);
        }
         else if (pid[mapper_number + i] == 0)
        {
            //this is child
            dup2(fd[i][0],0);
            dup2(fh[1],1);
            execlp(argv[3],argv[3],(char*)NULL);
            fprintf(stderr, "%s exec failed\n", argv[3]);
            exit(1);
        }
        else
        {
            close(fd[i][0]);
        }
    }

    close(fh[1]);

    if ((pid[mapper_number*2] = fork()) == -1)
        {
          exit(1);
        }
        else if(pid[mapper_number*2] == 0)
        {
            dup2(fh[0],0);
            int output_fd = open(argv[2],O_WRONLY);
            dup2(output_fd,1);
            execlp(argv[4],argv[4],(char*)NULL);
            fprintf(stderr, "%s exec failed\n", argv[4]);
            exit(1);
        }
        else
        {
            close(fh[0]);
        }

        int status;
    for(int i = 0; i < (mapper_number*2 + 1); i++)
       {

       waitpid(pid[i],&status,0);
       if(WIFEXITED(status) == 0)
         {
            fprintf(stderr, "pid %d", i);
            fprintf(stderr, "exit error\n");
         }
      else if(WEXITSTATUS(status))
          {
           
           if(i < mapper_number)
            print_nonzero_exit_status("splitter", WEXITSTATUS(status));
           else if(i < mapper_number*2)
            print_nonzero_exit_status(argv[3], WEXITSTATUS(status));
           else if(i == mapper_number*2)
            print_nonzero_exit_status(argv[4], WEXITSTATUS(status));

          }
       }
       //fprintf(stderr, "here?\n");
       print_num_lines(argv[2]);

    // Create an input pipe for each mapper.

    // Create one input pipe for the reducer.

    // Open the output file.

    // Start a splitter process for each mapper.

    // Start all the mapper processes.

    // Start the reducer process.

    // Wait for the reducer to finish.

    // Print nonzero subprocess exit codes.

    // Count the number of lines in the output file.

    return 0;
}
