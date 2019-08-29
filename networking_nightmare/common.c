/**
 * Networking
 * CS 241 - Spring 2017
 */
#include "common.h"
#include <stdio.h>

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <errno.h>

//static const size_t MESSAGE_SIZE_DIGITS = 8;

char *create_message(char *name, char *message) {
    int name_len = strlen(name);
    int msg_len = strlen(message);
    char *msg = calloc(1, msg_len + name_len + 4);
    sprintf(msg, "%s: %s", name, message);

    return msg;
}

ssize_t get_message_size(int socket) {
    size_t size;
    ssize_t read_bytes =
        read_all_from_socket(socket, (char *)&size, sizeof(size_t));
    if (read_bytes == 0 || read_bytes == -1)
        return read_bytes;

    return size;
}

// You may assume size won't be larger than a 4 byte integer
ssize_t write_message_size(size_t size, int socket) {
    // Your code here
    //int32_t shize = (uint32_t)size;
    //char tp[MESSAGE_SIZE_DIGITS];
    //sprintf(tp,"%zu",size);
    ssize_t write_bytes =  
        write_all_to_socket(socket, (char*)&size, sizeof(size_t));
    if (write_bytes == 0 || write_bytes == -1)
        return write_bytes;

    return write_bytes;

   // return 9001;
}

ssize_t read_all_from_socket(int socket, char *buffer, size_t count) {
    
ssize_t totalNumber = 0;
//int len = read(socket, buffer, count);
   // fprintf(stderr, "read count is  %zu\n", count);

int len;
while ((len = read(socket, buffer+totalNumber, count- totalNumber)) > -10)
{
    //fprintf(stderr, "len %d\n", len);
    //if(len > 0)
    //{
      //  totalNumber += len;
       // return totalNumber;
    //}
    if(len == -1)
    {

        if(errno == EINTR)
            continue;
        else
            break;

    }
    else if(len == 0)
    {
        break;
    }
    totalNumber += len;
    if((size_t)totalNumber == count)
        break;

}

//fprintf(stderr, "read total is%zu\n",totalNumber );

return totalNumber;

    // Your Code Here
   // return 9001;
}

ssize_t write_all_to_socket(int socket, const char *buffer, size_t count) {
    // Your Code Here
   ssize_t totalNumber = 0;

   
//int len = read(socket, buffer, count);
int len;
while ((len = write(socket, buffer+totalNumber, count - totalNumber)) > -10)
{   
     //  fprintf(stderr, "===================================wirtelen %d==================\n", len);
       
    //if(len > 0)
    //{
      //  totalNumber+=len;
       // return totalNumber;
    //}
    if(len == -1)
    {
        if(errno == EINTR)
            continue;
        else
        {
            if(errno == EAGAIN || errno == EWOULDBLOCK){
              //  fprintf(stderr, "==============wriet contimue here ============\n" );
                continue;
            }
          //  fprintf(stderr, "errononotset\n");
            return -1;
        }
    }
    else if(len == 0)
    {
        break;
    }
   // fprintf(stderr, "total%zu\n", totalNumber);
    totalNumber += len;
    if(count == (size_t)totalNumber)
        break;


}

//fprintf(stderr, "total is %zu\n", totalNumber);
return totalNumber;

}



ssize_t read_all_from_socket_server(int socket, char *buffer, size_t count) {
    
ssize_t totalNumber = 0;
//int len = read(socket, buffer, count);
   // fprintf(stderr, "read count is  %zu\n", count);

int len;
while ((len = read(socket, buffer+totalNumber, count- totalNumber)) > -10)
{
    //fprintf(stderr, "len %d\n", len);
    //if(len > 0)
    //{
      //  totalNumber += len;
       // return totalNumber;
    //}
    if(len == -1)
    {

        if(errno == EINTR)
            continue;
        else
        {
            if(errno == EAGAIN || errno == EWOULDBLOCK){
              //  fprintf(stderr, "==============wriet contimue here ============\n" );
                continue;
            }
            return -1;
        }

    }
    else if(len == 0)
    {
        break;
    }
    totalNumber += len;
    if((size_t)totalNumber == count)
        break;

}

//fprintf(stderr, "read total is%zu\n",totalNumber );

return totalNumber;

    // Your Code Here
   // return 9001;
}

